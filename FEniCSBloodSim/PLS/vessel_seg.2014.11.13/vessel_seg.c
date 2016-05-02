/* (C) 2014 F. Arguello, D.L. Vilarino, D.B. Heras and A. Nieto 
 *     CITIUS - Dept. of Electronics and Computing
 *     University of Santiago de Compostela 
 *
 * This program is licensed under a Creative Commons 
 * Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * ( http://creativecommons.org/licenses/by-nc-nd/4.0/ )
*/

// Code related to the paper:
//   F. Arguello, D.L. Vilarino, D.B. Heras and A. Nieto 
//   GPU-Based Segmentation of Retinal Blood Vessels
//   Journal of Real-Time Image Processing, 2014
//   DOI: 10.1007/s11554-014-0469-z
// input: image files in format PPM (color) and PGM (gray)
//   image collections, download from:
//   DRIVE: http://www.isi.uu.nl/Research/Databases/DRIVE/
//   STARE: http://www.ces.clemson.edu/~ahoover/stare/
//   MESSIDOR: http://messidor.crihan.fr/download.php
// output: vessel segmentations
// tested: Linux (gcc version 4.6.3)
// compile with: gcc vessel_seg.c -O3 -lm

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define CONN 8
#define SOBEL_UMBRAL 0.5

// Selection of Image collection
#define DRIVE 0          // Download images from drive page
#define STARE 1          // Download images from stare page
#define MESSIDOR 2       // Download images from messidor page
#define COLLECTION DRIVE // Select here: DRIVE, STARE, or MESSIDOR

// Main parameters to be selected
#define THRESHOLD 4.32      // threshold for binarization of images
#define NOISE_SIZE 4        // pixel groups < 4 will be removed
#define SMALL_STRUC_SIZE 50 // small elonged structures will be removed

// Specific parameters DRIVE
#if COLLECTION==DRIVE
#define H 565          // horizontal size
#define V 584          // vertical size
#define STAGES 1       // PLS number of stages
#define TOPHAT 1       // 0=none, 1=circle r=17 2=square 15x15 3=square 128x128
#define FOV_UMBRAL 20  // gray level of pixels outside FOV <= FOV_UMBRAL
#define NIMG 20        // number of images in collection
#define PREFIX "images/drive/%s.ppm" // subdirectory localization of images
char name[NIMG][60], ground[NIMG][60], mask[NIMG][60]; 
void collect_images() 
{  int i;
   for(i=0;i<NIMG;i++) sprintf(name[i],"%02d_test",i+1);      // image names 
   for(i=0;i<NIMG;i++) sprintf(ground[i],"%02d_manual1",i+1); // ground truth
   for(i=0;i<NIMG;i++) sprintf(mask[i],"%02d_test_mask",i+1); // mask for FOV
}

// Specific parameters STARE
#elif COLLECTION==STARE
#define H 700         // horizontal size
#define V 605         // vertical size
#define STAGES 1      // PLS number of stages
#define TOPHAT 1      // 0=none, 1=circle r=17 2=square 15x15 3=square 128x128
#define FOV_UMBRAL 25 // gray level of pixels outside FOV <= FOV_UMBRAL
#define NIMG 20       // number of images in collection
#define PREFIX "images/stare/%s.ppm" // subdirectory localization of images
char name[NIMG][60], ground[NIMG][60], mask[NIMG][60]; 
void collect_images() 
{  int i, num[]={1,2,3,4,5,44,77,81,82,139,162,163,235,236,239,240,255,291,319,324};
   for(i=0;i<NIMG;i++) sprintf(name[i],"im%04d",num[i]);      // image names
   for(i=0;i<NIMG;i++) sprintf(ground[i],"im%04d.ah",num[i]); // ground truth
   for(i=0;i<NIMG;i++) sprintf(mask[i],"im%04d_mask",num[i]); // mask for FOV
}

// Specific parameters MESSIDOR
#elif COLLECTION==MESSIDOR
#define SMALL_STRUC_SIZE 200
#define H 2240        // horizontal size
#define V 1488        // vertical size
#define STAGES 1      // PLS number of stages
#define TOPHAT 3      // 0=none, 1=circle r=17 2=square 15x15 3=square 128x128
#define FOV_UMBRAL 20 // gray level of pixels outside FOV <= FOV_UMBRAL
#define NIMG 20       // number of images in collection
#define PREFIX "images/messidor/%s.ppm" // subdirectory localization of images
char name[NIMG][60], ground[NIMG][60], mask[NIMG][60]; 
void collect_images()
{  int i, num[]={38557,43808,43832,43882,43906,44261,44284,44338,44349,44400,44431,44598,44636,44692,44714,44762,44782,44843,44901,44923};
   for(i=0;i<1;i++) sprintf(name[i],"20051019_%05d_0100_PP",num[i]);  // image names
   for(i=1;i<20;i++) sprintf(name[i],"20051020_%05d_0100_PP",num[i]); // image names
}
#endif

// STADISTICS
double t_comp[NIMG], t_tot[NIMG], sens[NIMG], esp[NIMG], MAA[NIMG]; 

// -----------------------------------------------------------------------------
// IMAGES load/write, accuracies, averages
// -----------------------------------------------------------------------------

void read_image_ppm(const char *nombre, unsigned char img[V*H])
{  FILE* fp; int i, Nh, Nv, flag; unsigned char texto[100], *data, c;
   data=(unsigned char *) malloc(3*H*V*sizeof(char));
   if(data==NULL) { fprintf(stderr,"No enough RAM\n"); exit(-1); }
   fp = fopen (nombre, "rb");
   if(fp==NULL) { fprintf(stderr,"Can not open file %s\n",nombre); exit(1); }
   else printf("Open file %s,",nombre);
   flag=fscanf(fp,"%s",texto);
   if(strcasecmp((const char *)texto,"P6")) { fprintf(stderr,"No PPM file\n"); exit(-1); } 
   c=fgetc(fp); c=fgetc(fp);
   while((c=='#')) { while (c != '\n') c=fgetc(fp); c=fgetc(fp);}  ungetc(c,fp);
   flag=fscanf(fp,"%s",texto); Nh=atoi((const char*)texto); 
   flag=fscanf(fp,"%s",texto); Nv=atoi((const char*)texto);
   printf(" size %dx%d\n",Nh,Nv);
   if((Nh!=H)||(Nv!=V)) { fprintf(stderr,"Incorrect size image\n"); exit(-1); }
   flag=fscanf(fp,"%s",texto); c = fgetc(fp); ;
   flag=fread(data,3*H*V,1,fp); printf(" Read %u bytes\n",(unsigned int) flag*3*H*V);
     if(flag!=1) { fprintf(stderr,"Can not read all data\n"); exit(-1); }
   for(i=0;i<H*V;i++) img[i]=data[3*i+1];
   fclose(fp); free(data);
}

void read_image_pgm(const char *nombre, unsigned char img[V*H])
{  FILE* fp; int flag, Nh, Nv; unsigned char c, texto[100];
   fp = fopen (nombre, "rb");
   if(fp==NULL) { fprintf(stderr,"Can not open file %s\n",nombre); exit(1); }
   else printf("Open file %s,",nombre);
   flag=fscanf(fp,"%s",texto);   
   if(strcasecmp((const char *)texto,"P5")) { fprintf(stderr,"No PGM file\n"); exit(-1); } 
   c=fgetc(fp); c=fgetc(fp); 
   while((c=='#')) { while (c != '\n') c = fgetc(fp); c = fgetc(fp);} ungetc(c,fp);
   flag=fscanf(fp,"%s",texto); Nh=atoi((const char*)texto); 
   flag=fscanf(fp,"%s",texto); Nv=atoi((const char*)texto);
   printf(" size %dx%d\n",Nh,Nv);
   if((Nh!=H)||(Nv!=V)) { fprintf(stderr,"Incorrect size\n"); exit(-1); }
   flag=fscanf(fp,"%s",texto); c = fgetc(fp); ;
   flag=fread(img,H*V,1,fp); printf(" Read %u bytes\n",(unsigned int) flag*H*V);
   fclose(fp);
}

void save_image_pgm(const char fichero[40], unsigned char img[H*V])
{  FILE *fp; int i, flag; unsigned char *tmp;
   tmp=(unsigned char *)malloc(H*V*sizeof(char));
   if(tmp==NULL) { printf("No enough RAM\n"); exit(-1); }
   fp=fopen(fichero,"wb+");
   if(fp==NULL) { fprintf(stderr,"Can not save %s\n",fichero); exit(1); }
   else printf("Saved file %s, size %dx%d\n",fichero,H,V); 
   fprintf(fp,"P5\n#CREADOR University of Santiago de Compostela\n%d %d\n%d\n",H,V,255);
   for(i=0;i<H*V;i++) tmp[i]=255*img[i];
   flag=fwrite(tmp,H*V,1,fp); printf(" Saved %u bytes\n",(unsigned int) flag*H*V);
   fclose(fp); free(tmp);
}

void compute_accuracies(unsigned char *output, unsigned char *truth, int nimg)
{  char fichero[60]; int i, a00=0, a01=0, a10=0, a11=0, total; 
   unsigned char *mask_img;
   mask_img=(unsigned char *)malloc(H*V*sizeof(unsigned char));
   if(COLLECTION==DRIVE) sprintf(fichero,"images/drive/%s.pgm",ground[nimg]);
   else if (COLLECTION==STARE) sprintf(fichero,"images/stare/%s.pgm",ground[nimg]);
   else return;
   read_image_pgm(fichero,truth);
   if(COLLECTION==DRIVE) sprintf(fichero,"images/drive/%s.pgm",mask[nimg]);
   else if (COLLECTION==STARE) sprintf(fichero,"images/stare/%s.pgm",mask[nimg]);
   else return;
   read_image_pgm(fichero,mask_img);
   
   a00=0; a01=0; a10=0; a11=0; total=0;
   for(i=0;i<H*V;i++)
   {  if(mask_img[i]!=255) continue;
      total++;
      if     ((output[i]==1)&&(truth[i]<128)) a00++;
      else if((output[i]==1)&&(truth[i]>127)) a01++;
      else if((output[i]==0)&&(truth[i]<128)) a10++;
      else if((output[i]==0)&&(truth[i]>127)) a11++;
      else fprintf(stderr,"\n\nERROR\n\n"); }
   // sens=tp/(tp+fn), esp=tn/(tn+fp), MAA=(tp+tn)/total
   sens[nimg]=(1.0*a11)/(a11+a01); esp[nimg]=(1.0*a00)/(a00+a10);
   MAA[nimg]=(a00+a11)/(1.0*total);
   printf("Results(%s) Sens=%f, Spec=%f, MAA=%f\n",
       name[nimg],sens[nimg],esp[nimg],MAA[nimg]);
   free(mask_img);
}

void compute_averages()
{  int nimg; double t1=0, t2=0, p1=0, p2=0, p3=0;
   for(nimg=0;nimg<NIMG;nimg++)
   {  t1+=t_comp[nimg]; t2+=t_tot[nimg]; 
      p1+=sens[nimg]; p2+=esp[nimg]; p3+=MAA[nimg]; }
   printf("-----------------------------------------------\n");
   if(COLLECTION==DRIVE) printf("DRIVE"); else if(COLLECTION==STARE) printf("STARE");
   else if(COLLECTION==MESSIDOR) printf("BIG");
   printf(" CPU imag=%d stages=%d\n",NIMG,STAGES);
   printf("  Sens=%f, Spec=%f, MAA=%f\n",p1/NIMG,p2/NIMG,p3/NIMG);
   printf("  t_comp=%f, t_total=%f\n",t1/NIMG,t2/NIMG);
}

//------------------------------------------------------------------
// UNION-FIND
//------------------------------------------------------------------

void init_labels(unsigned char *img, int *lab)
{  int u; unsigned char B=1;
   for(u=0;u<H*V;u++) lab[u]=u;
   for(u=0;u<H*V;u++) if(img[u]==B) lab[u]=H*V;
}

void path_compress(int *lab)
{  int u, c;
   for(u=0;u<H*V;u++) 
     if(lab[u]<H*V) { c=u; while(lab[c]!=c) { c=lab[c]; } lab[u]=c;  }
}

int union_find_kernel(int *lab, int u, int v)
{  int c=H*V, d=H*V;

   if(lab[u]<H*V) { c=u; while(lab[c]!=c) c=lab[c]; }
   if(lab[v]<H*V) { d=v; while(lab[d]!=d) d=lab[d]; }
   if(c<d) lab[d]=c;
   if(d<c) lab[c]=d;
}

void union_lab_main(int *lab, int conn)
{  int u;
   for(u=0;u<H*V;u++)
   {   if(lab[u]<H*V) {
      if(u%H!=0)   if(lab[u-1]<H*V) union_find_kernel(lab,u,u-1);
      if(u>=H)     if(lab[u-H]<H*V) union_find_kernel(lab,u,u-H);
      if(conn==8)
      {  if((u>=H)&&(u%H!=0))      if(lab[u-H-1]<H*V) union_find_kernel(lab,u,u-H-1);
	 if((u>=H)&&(u%H!=H-1))    if(lab[u-H+1]<H*V) union_find_kernel(lab,u,u-H+1);
    }
}}}

void union_find(unsigned char* img, int *lab, int conn)
{
   init_labels(img, lab);
   union_lab_main(lab,conn);
   path_compress(lab);
}

void remove_small_structures(unsigned char *img, int SEP, int OFF1, int OFF2, int conn)
{  int i, j, *lab, c; unsigned char *tmp, F=0, B=1;
   lab=(int *)malloc(H*V*sizeof(int));
   tmp=(unsigned char *)calloc(H*V,sizeof(char));
   if((lab==NULL)||(tmp==NULL)) { printf("no memoria\n"), exit(-1); }

   union_find(img,lab,conn);
   for(i=0;i<V;i++) for(j=0;j<H;j++) if((i%SEP==OFF1)||(j%SEP==OFF1))
   {  c=lab[i*H+j]; if(c<H*V) tmp[c]|=1; } 
   for(i=0;i<V;i++) for(j=0;j<H;j++) if((i%SEP==OFF2)||(j%SEP==OFF2))
   {  c=lab[i*H+j]; if(c<H*V) tmp[c]|=2; } 
   
   for(i=0;i<H*V;i++) { c=lab[i]; if((c<H*V)&&(tmp[c]==3)) img[i]=F; else img[i]=B; }
   free(tmp); free(lab);
}

void remove_small_groups(unsigned char* img, int conn, int limite)
{  int u, k=0; unsigned char F=0, B=1, flag, *out; int *lab, *cuenta;
   lab=(int *)malloc(H*V*sizeof(int));
   cuenta=(int *)calloc(H*V,sizeof(int));
   out=(unsigned char *)malloc(H*V*sizeof(char));
   if((lab==NULL)||(cuenta==NULL)|(out==NULL)) 
   { printf("no memoria remove_small_groups\n"); exit(-1); }
   memcpy(out,img,H*V*sizeof(char));
   for(u=0;u<H*V;u++) lab[u]=u;
   
   union_find(img,lab,conn);
   
   for(u=0;u<H*V;u++) if(img[u]==F) cuenta[lab[u]]++;
   for(u=0;u<H*V;u++) if(img[u]==F) cuenta[u]=cuenta[lab[u]];
  
   for(u=0;u<H*V;u++) if((cuenta[lab[u]]<limite)) out[u]=B;
   memcpy(img,out,H*V*sizeof(char));
   free(lab); free(cuenta); free(out);
}

//----------------------------------------------------------------------
// FILTERS
//----------------------------------------------------------------------

void erosion(unsigned char in[H*V], unsigned char out[H*V], int ns)
{  int i, j, H2=H+2; unsigned char *tmp, min2; 
   tmp=(unsigned char*)malloc((H+2)*(V+2)*sizeof(char));
   if(tmp==NULL) { printf("error\n"); exit(-1); }
   for(i=0;i<V;i++)for(j=0;j<H;j++) tmp[(i+1)*H2+(j+1)]=in[i*H+j];
   for(j=0;j<H;j++) tmp[j+1]=1; 
   for(j=0;j<H;j++) tmp[(V+1)*H2+j+1]=1;
   for(i=0;i<V;i++) tmp[(i+1)*H2]=1;
   for(i=0;i<V;i++) tmp[(i+1)*H2+H+1]=1; 
   tmp[0]=1; tmp[H+1]=1; tmp[(V+1)*H2]=1; tmp[(V+1)*H2+(H+1)]=1; 
 
   if(ns==3) for(i=0;i<V;i++)for(j=0;j<H;j++) 
      out[i*H+j]=tmp[i*H2+j]&tmp[i*H2+(j+1)]&tmp[i*H2+(j+2)]
                &tmp[(i+1)*H2+j]&tmp[(i+1)*H2+(j+1)]&tmp[(i+1)*H2+(j+2)]
                &tmp[(i+2)*H2+j]&tmp[(i+2)*H2+(j+1)]&tmp[(i+2)*H2+(j+2)];
   else if(ns==2) for(i=0;i<V;i++)for(j=0;j<H;j++) 
      out[i*H+j]=tmp[(i+1)*H2+(j+1)]&tmp[(i+1)*H2+(j+2)]
                &tmp[(i+2)*H2+j]&tmp[(i+2)*H2+(j+1)]&tmp[(i+2)*H2+(j+2)];
   else { printf("Erosion no soportada\n"); exit(-1); }
   free(tmp);
}

void erosion_float(float in[H*V], float out[H*V], unsigned char *st, int ns)
{  int k, l, fil, col; float min2;
   for(k=0;k<H*V;k++)
   {  min2=in[k];
      for(l=0;l<ns*ns;l++) 
      {  fil=(k/H)-((ns-1)/2)+(l/ns); col=(k%H)-((ns-1)/2)+(l%ns);
         if((fil>=0)&&(col>=0)&&(fil<V)&&(col<H)) 
	   if(st[l])  min2=min(min2,in[fil*H+col]); }
     out[k]=min2; }
}

void dilation(unsigned char in[H*V], unsigned char out[H*V], int ns)
{  int i, j, H2=H+2; unsigned char *tmp, min2; 
   tmp=(unsigned char*)malloc((H+2)*(V+2)*sizeof(char));
   if(tmp==NULL) { printf("error\n"); exit(-1); }
   for(i=0;i<V;i++)for(j=0;j<H;j++) tmp[(i+1)*H2+(j+1)]=in[i*H+j];
   for(j=0;j<H;j++) tmp[j+1]=0; 
   for(j=0;j<H;j++) tmp[(V+1)*H2+j+1]=0;
   for(i=0;i<V;i++) tmp[(i+1)*H2]=0;
   for(i=0;i<V;i++) tmp[(i+1)*H2+H+1]=0; 
   tmp[0]=in[0]; tmp[H+1]=0; tmp[(V+1)*H2]=0; tmp[(V+1)*H2+(H+1)]=0; 
 
   if(ns==3) for(i=0;i<V;i++)for(j=0;j<H;j++) 
      out[i*H+j]=tmp[i*H2+j]|tmp[i*H2+(j+1)]|tmp[i*H2+(j+2)]
                |tmp[(i+1)*H2+j]|tmp[(i+1)*H2+(j+1)]|tmp[(i+1)*H2+(j+2)]
                |tmp[(i+2)*H2+j]|tmp[(i+2)*H2+(j+1)]|tmp[(i+2)*H2+(j+2)];
   else if(ns==2) for(i=0;i<V;i++)for(j=0;j<H;j++) 
      out[i*H+j]=tmp[i*H2+j]|tmp[i*H2+(j+1)]
                |tmp[(i+1)*H2+j]|tmp[(i+1)*H2+(j+1)];
   else { printf("dilation no soportada\n"); exit(-1); }
   free(tmp);
}

void dilation_char(unsigned char in[H*V], unsigned char out[H*V], unsigned char *st, int ns)
{  int k, l, fil, col; unsigned char max2;
   for(k=0;k<H*V;k++)
   {  max2=in[k];
      for(l=0;l<ns*ns;l++) 
      {  fil=(k/H)-(ns/2)+(l/ns); col=(k%H)-(ns/2)+(l%ns);
         if((fil>=0)&&(col>=0)&&(fil<V)&&(col<H)) 
	   if(st[l])  max2=max(max2,in[fil*H+col]); }
     out[k]=max2; }
}

void erosion_char(unsigned char in[H*V], unsigned char out[H*V], unsigned char *st, int ns)
{  int k, l, fil, col; unsigned char min2;
   for(k=0;k<H*V;k++)
   {  min2=in[k];
      for(l=0;l<ns*ns;l++) 
      {  fil=(k/H)-((ns-1)/2)+(l/ns); col=(k%H)-((ns-1)/2)+(l%ns);
         if((fil>=0)&&(col>=0)&&(fil<V)&&(col<H)) 
	   if(st[l])  min2=min(min2,in[fil*H+col]); }
     out[k]=min2; }
}

void opening_char(unsigned char img[H*V], unsigned char *st, int ns)
{  unsigned char *tmp1=(unsigned char *)malloc(H*V*sizeof(char));
   if(tmp1==NULL) { printf("error tophat\n"); exit(-1);}
   erosion_char(img,tmp1,st,ns);
   dilation_char(tmp1,img,st,ns);
   free(tmp1);
}

void f_edge(unsigned char in[H*V], unsigned char out[H*V])
{  int i, j, H2=H+2; unsigned char *tmp; 
   tmp=(unsigned char*)malloc((H+2)*(V+2)*sizeof(char));
   if(tmp==NULL) { printf("error\n"); exit(-1); }
   for(i=0;i<V;i++)for(j=0;j<H;j++) tmp[(i+1)*H2+(j+1)]=in[i*H+j];
   for(j=0;j<H;j++) tmp[j+1]=1; 
   for(j=0;j<H;j++) tmp[(V+1)*H2+j+1]=1;
   for(i=0;i<V;i++) tmp[(i+1)*H2]=1;
   for(i=0;i<V;i++) tmp[(i+1)*H2+H+1]=1; 
   tmp[0]=1; tmp[H+1]=1; tmp[(V+1)*H2]=1; tmp[(V+1)*H2+(H+1)]=1; 
   // erosion
   for(i=0;i<V;i++)for(j=0;j<H;j++) 
      out[i*H+j]=tmp[(i+1)*H2+(j+1)]&tmp[i*H2+(j+1)]&tmp[(i+2)*H2+(j+1)]
                &tmp[(i+1)*H2+j]&tmp[(i+1)*H2+(j+2)];
   // etapa final   
   for(i=0;i<H*V;i++) out[i] = in[i] & (!out[i]);
}

// 0=sin replicacion, 1=con replicacion de ultimo dato extremo
void imfilter_float(float in[H*V], float *B, int ns, int replica)
{   int i, j, k, l, nn=(ns-1)/2, H2=H+ns; float *tmp;
    tmp=(float *)calloc((H+ns)*(V+ns),sizeof(float));
    if(tmp==NULL) { printf("error\n"); exit(-1); }
    for(i=0;i<V;i++) for(j=0;j<H;j++) tmp[(i+nn)*H2+(j+nn)]=in[i*H+j];
    if(replica==1)
    {     for(i=0;i<nn;i++) for(j=0;j<H;j++) tmp[i*H2+j+nn]=in[j]; 
          for(i=V+nn;i<V+ns;i++) for(j=0;j<H;j++) tmp[i*H2+j+nn]=in[(V-1)*H+j];
          for(i=0;i<V;i++) for(j=0;j<nn;j++) tmp[(i+nn)*H2+j]=in[i*H];
          for(i=0;i<V;i++) for(j=H+nn;j<H+ns;j++) tmp[(i+nn)*H2+j]=in[i*H+H-1]; 
          for(i=0;i<nn;i++) for(j=0;j<nn;j++) tmp[i*H2+j]=in[0]; 
          for(i=0;i<nn;i++) for(j=H+nn;j<H+ns;j++) tmp[i*H2+j]=in[H-1];
          for(i=V+nn;i<V+ns;i++) for(j=0;j<nn;j++) tmp[i*H2+j]=in[(V-1)*H];
          for(i=V+nn;i<V+ns;i++) for(j=H+nn;j<H+ns;j++) tmp[i*H2+j]=in[(V-1)*H+H-1]; }
    memset(in,0,H*V*sizeof(float));
    for(i=0;i<V;i++)for(j=0;j<H;j++)for(k=0;k<ns;k++)for(l=0;l<ns;l++)
       in[i*H+j]+=B[k*ns+l]*tmp[(i+k)*H2+(j+l)];
    free(tmp);
}

void f_shift(const char dir, unsigned char input[H*V], unsigned char result[H*V],
     unsigned char boundary)
{  int i,j;
   if(dir=='S')
   {  for(i=1;i<V;i++)for(j=0;j<H;j++) result[i*H+j]=input[(i-1)*H+j];
      for(j=0;j<H;j++) result[j]=boundary; }
   else if(dir=='W')
   {  for(i=0;i<V;i++) for(j=0;j<H-1;j++) result[i*H+j]=input[i*H+(j+1)];
      for(i=0;i<V;i++) result[i*H+(H-1)]=boundary;  }
   else if(dir=='N')
   {  for(i=0;i<V-1;i++)for(j=0;j<H;j++) result[i*H+j]=input[(i+1)*H+j];
      for(j=0;j<H;j++) result[(V-1)*H+j]=boundary; }
   else if(dir=='E')
   {  for(i=0;i<V;i++) for(j=1;j<H;j++) result[i*H+j]=input[i*H+(j-1)];
      for(i=0;i<V;i++) result[i*H]=boundary; }
   else printf("The specified direction does not exist!");
}

void f_shift_float(const char dir, float input[H*V], float result[H*V],
     unsigned char boundary)
{  int i,j;
   if(dir=='S')
   {  for(i=1;i<V;i++)for(j=0;j<H;j++) result[i*H+j]=input[(i-1)*H+j];
      for(j=0;j<H;j++) result[j]=boundary; }
   else if(dir=='W')
   {  for(i=0;i<V;i++) for(j=0;j<H-1;j++) result[i*H+j]=input[i*H+(j+1)];
      for(i=0;i<V;i++) result[i*H+(H-1)]=boundary;  }
   else if(dir=='N')
   {  for(i=0;i<V-1;i++)for(j=0;j<H;j++) result[i*H+j]=input[(i+1)*H+j];
      for(j=0;j<H;j++) result[(V-1)*H+j]=boundary; }
   else if(dir=='E')
   {  for(i=0;i<V;i++) for(j=1;j<H;j++) result[i*H+j]=input[i*H+(j-1)];
      for(i=0;i<V;i++) result[i*H]=boundary; }
   else printf("The specified direction does not exist!");
}

void blur(unsigned char im[V*H], float imblur[H*V])
{  int i, j, k, row, col; float *tmp;
   tmp=(float *)calloc(H*V,sizeof(float));
   if(tmp==NULL) { printf("no memoria\n"); exit(-1); }
   memset(imblur,0,H*V*sizeof(float));
    
   for(i=0;i<V;i++)for(j=0;j<H;j++) for(k=-8;k<=8;k++)
   {  col=j+k; if(col<0) col=0; else if(col>=H) col=H-1;
      tmp[i*H+j]+=im[i*H+col]; }
   for(i=0;i<V;i++)for(j=0;j<H;j++) for(k=-8;k<=8;k++)
   {  row=i+k; if(row<0) row=0; else if(row>=V) row=V-1;
       imblur[i*H+j]+=tmp[row*H+j]; }
   for(i=0;i<H*V;i++)  imblur[i]=imblur[i]/289.0;
       
   free(tmp);
}

void remove_outside_FOV(unsigned char *f, unsigned char *tmp, int umbral)
{  int i; for(i=0;i<H*V;i++) if(tmp[i]<umbral) f[i]=1; }

void sobel_edge_detect(unsigned char *in, unsigned char *out) 
{  int i, k, l, fil, col, ns=3; int gx, gy, g;
   int sobx[]={-1,0,1,-2,0,2,-1,0,1}, soby[]={1,2,1,0,0,0,-1,-2,-1};
   for(k=0;k<H*V;k++) { gx=0; gy=0; for(l=0;l<ns*ns;l++)
   {  fil=(k/H)-((ns-1)/2)+(l/ns); col=(k%H)-((ns-1)/2)+(l%ns);
      if(fil<0)fil=0; else if(fil>=V) fil=V-1;
      if(col<0)col=0; else if(col>=H) col=H-1;
      gx+=sobx[l]*in[fil*H+col]; gy+=soby[l]*in[fil*H+col]; }
      g=gx*gx+gy*gy;
      if(g<SOBEL_UMBRAL) out[k]=0; else out[k]=1; }
   // etapa final   
   for(i=0;i<H*V;i++) out[i] = in[i] & (!out[i]);
}

void postproc_big_images(unsigned char *output, unsigned char *tmp1)
{  erosion(output,tmp1,3);
   erosion(tmp1,output,3);
   erosion(output,tmp1,3);
   erosion(tmp1,output,3);
   if(H>4000)
   {  erosion(output,tmp1,3);
      dilation(tmp1,output,3); }
   dilation(output,tmp1,3);
   dilation(tmp1,output,3);
   dilation(output,tmp1,3);
   dilation(tmp1,output,3);
}

//-------------------------------------------------------------
// TOPHAT
//-------------------------------------------------------------

void tophat_square(unsigned char img[V*H], unsigned char out[H*V], int ns)
{  int i, j, k, row, col, nc=ns/2, max2, min2; unsigned char *tmp1;
   tmp1=(unsigned char *)malloc(H*V*sizeof(char));
   if(tmp1==NULL) { printf("no memoria\n"); exit(-1); }
    
   for(i=0;i<H*V;i++) tmp1[i]=255-img[i];
   for(i=0;i<V;i++)for(j=0;j<H;j++) { min2=255; for(k=-nc;k<=nc;k++)
   {  col=j+k; if(col<0) col=0; else if(col>=H) col=H-1;
      min2=min(min2,tmp1[i*H+col]); } out[i*H+j]=min2; }
   for(i=0;i<V;i++)for(j=0;j<H;j++) { min2=255; for(k=-nc;k<=nc;k++)
   {  row=i+k; if(row<0) row=0; else if(row>=V) row=V-1;
      min2=min(min2,out[row*H+j]); } tmp1[i*H+j]=min2; }
   for(i=0;i<V;i++)for(j=0;j<H;j++) { max2=0; for(k=-nc;k<=nc;k++)
   {  col=j+k; if(col<0) col=0; else if(col>=H) col=H-1;
      max2=max(max2,tmp1[i*H+col]); } out[i*H+j]=max2; }
   for(i=0;i<V;i++)for(j=0;j<H;j++) { max2=0; for(k=-nc;k<=nc;k++)
   {  row=i+k; if(row<0) row=0; else if(row>=V) row=V-1;
      max2=max(max2,out[row*H+j]); } tmp1[i*H+j]=max2; }
   for(i=0;i<H*V;i++) out[i]=img[i]+tmp1[i];
   free(tmp1);
}

void tophat_disk(unsigned char img[H*V], unsigned char out[H*V])
{  int i; unsigned char *tmp1;
   unsigned char st15[]={0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,
      0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,
      0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,
      0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
      1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
      1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
      1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
      1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
      1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
      1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
      1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
      0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
      0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,
      0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,
      0,0,0,0,1,1,1,1,1,1,1,0,0,0,0};
   tmp1=(unsigned char *)malloc(H*V*sizeof(char));
   if(tmp1==NULL) { printf("error tophat\n"); exit(-1);}
   for(i=0;i<H*V;i++) tmp1[i]=255-img[i];
   opening_char(tmp1,st15,15);
   for(i=0;i<H*V;i++) out[i]=img[i]+tmp1[i];
   free(tmp1);
}

void tophat(unsigned char img[H*V], unsigned char out[H*V])
{  if(TOPHAT==0) memcpy(out,img,H*V*sizeof(char));
   else if(TOPHAT==1) tophat_disk(img,out);
   else if(TOPHAT==2) tophat_square(img,out,15);
   else if(TOPHAT==3) tophat_square(img,out,129);
}

// ------------------------------------------------------------
// PLS
// ------------------------------------------------------------

float s_cpd=+1;   // s_cpd=-1 => disables the collision point detection
float k_ext=1; // weight ext. potential (k_ext > 0 => evol. to 2QCNN black)
float k_int=1; // 0.25, 0.5 weight the internal potential (k_int>0)
float k_inf=-1; // -0.02, -1 weight balloon potential: deflating (>0), inflating (<0)

void f_IPEv3(unsigned char input[H*V], float int_pot[H*V], int iter)
{  int i; unsigned char *tmp1=NULL;
   float T_diff[]={0.10,0.15,0.10, 0.15,0.00,0.15, 0.10, 0.15, 0.10};
   tmp1=(unsigned char*)malloc(H*V*sizeof(char));
   if(tmp1==NULL) { printf("\nno memoria\n"); exit(-1); }
   f_edge(input,tmp1); 
   for(i=0;i<H*V;i++) int_pot[i]=k_int*tmp1[i]; // el escalado estaba al final
   free(tmp1);    
}

void f_CPDv3(const char dir, unsigned char output[H*V], unsigned char mask_CPD[H*V])
{  unsigned char *tmp1=NULL, *tmp2=NULL; 
   int i; unsigned char dir1, dir2, dir3;
   tmp1=(unsigned char *)malloc(H*V*sizeof(char));
   tmp2=(unsigned char *)malloc(H*V*sizeof(char));
   if((tmp1==NULL)||(tmp2==NULL)) { printf("\nno memoria\n"); exit(-1); }
   if(s_cpd==-1) memset(mask_CPD,0,H*V*sizeof(char));
   else {
   switch(dir)
   {  case 'N': dir1='S'; dir2='E'; dir3='W'; break;
      case 'S': dir1='N'; dir2='E'; dir3='W'; break;
      case 'E': dir1='W'; dir2='N'; dir3='S'; break;
      case 'W': dir1='E'; dir2='N'; dir3='S'; break;
      default: printf("The specified direction does not exist!\n"); }
   f_shift(dir1,output,tmp1,0);
   for(i=0;i<H*V;i++) mask_CPD[i]=(1-output[i])&tmp1[i];
   f_shift(dir2,mask_CPD,tmp1,0);
   f_shift(dir3,mask_CPD,tmp2,0);
   for(i=0;i<H*V;i++) mask_CPD[i]=mask_CPD[i]|tmp1[i]|tmp2[i]; }
   free(tmp1); free(tmp2);
}

void f_GFEv3(const char dir, float ext_pot[H*V], float int_pot[H*V], 
    float inf_pot[H*V], unsigned char mask_CPD[H*V], unsigned char output_GFE[H*V])
{  int i; float *aux1=NULL, *aux2=NULL;   
   aux1=(float *)malloc(H*V*sizeof(float));
   aux2=(float *)malloc(H*V*sizeof(float));
   if((aux1==NULL)||(aux2==NULL)) { printf("\nno memoria\n"); exit(-1); }
   for(i=0;i<H*V;i++) aux1[i]=ext_pot[i]+int_pot[i]+inf_pot[i];
   f_shift_float(dir,aux1,aux2,0);
   for(i=0;i<H*V;i++) 
   {  if((mask_CPD[i]==0)&&(aux1[i]-aux2[i]>=0.001)) output_GFE[i]=1; 
	 else output_GFE[i]=0; }
   free(aux1); free(aux2);
}

void f_DPDv3(const char dir, unsigned char output[H*V], unsigned char output_GFE[H*V])
{  int i; unsigned char *tmp1;
   tmp1=(unsigned char *)malloc(H*V*sizeof(char));
   if(tmp1==NULL) { printf("\nno memoria\n"); exit(-1); }
   f_shift(dir,output,tmp1,0);
   for(i=0;i<H*V;i++) output[i] = output[i]|(tmp1[i]&output_GFE[i]) ;
   free(tmp1);
}

void f_ContEvol(unsigned char input[H*V], float ext_pot[H*V], unsigned char output[H*V], int contador)
{  int i, k; float *inf_pot=NULL, *int_pot=NULL;
   unsigned char dir[4]={'N','E','S','W'}; char sentido;
   unsigned char *mask_CPD=NULL, *output_GFE=NULL, *contour=NULL;
   inf_pot=(float *)malloc(H*V*sizeof(float));
   int_pot=(float *)malloc(H*V*sizeof(float));
   mask_CPD=(unsigned char *)malloc(H*V*sizeof(char));
   output_GFE=(unsigned char *)malloc(H*V*sizeof(char));
   contour=(unsigned char *)malloc(H*V*sizeof(char));
   if((inf_pot==NULL)||(int_pot==NULL)||(mask_CPD==NULL)||(output_GFE==NULL)
     ||(contour==NULL))  { printf("\nno memoria\n"); exit(-1); }
   memcpy(output,input,H*V*sizeof(char));

   for(sentido=+1;sentido>=-1;sentido=sentido-2)
   {  f_IPEv3(output,int_pot,5);
      for(k=0;k<4;k++)
      {  for(i=0;i<H*V;i++) inf_pot[i]=sentido*k_inf*output[i];
         f_CPDv3(dir[k],output,mask_CPD); // salida=mask_CPD
         f_GFEv3(dir[k],ext_pot,int_pot,inf_pot,mask_CPD,output_GFE); //output_GFE 
         f_DPDv3(dir[k],output,output_GFE);	// salida=output
      }
      f_edge(output,contour);
      for(i=0;i<H*V;i++) output[i]=(1-output[i])|contour[i];
   }
   free(inf_pot); free(int_pot); free(mask_CPD); free(output_GFE);
   free(contour);
}

int main()
{  int i, k, stop=1, nimg;
   unsigned char *im, *imt, *edge1, *edge2, *tmp1, *input, *output, *truth, fichero[60];
   unsigned char st2[]={1,1, 1,1}, st3[]={1,1,1, 1,1,1, 1,1,1};
   float max2, diff_a[]={0.10,0.15,0.10, 0.15,0.00,0.15, 0.10,0.15,0.10};  
   float *aux1, *aux2, *imd2, *imblur, *imsobel, *ext_pot; 
   struct timeval start, end, ini, fin;
   im=  (unsigned char *)malloc(H*V*sizeof(char));
   imt=(unsigned char *)malloc(H*V*sizeof(unsigned char));
   edge1=(unsigned char *)malloc(H*V*sizeof(unsigned char));
   edge2=(unsigned char *)malloc(H*V*sizeof(unsigned char));
   input=(unsigned char *)malloc(H*V*sizeof(unsigned char));
   output=(unsigned char *)malloc(H*V*sizeof(unsigned char));
   tmp1=(unsigned char *)malloc(H*V*sizeof(unsigned char));
   if((im==NULL)||(imt==NULL)||(edge1==NULL)||(edge2==NULL)||(tmp1==NULL)||(input==NULL)
     ||(output==NULL)) { printf("\nno memoria\n\n"); exit(-1); }
   ext_pot=(float *)malloc(H*V*sizeof(float));
   aux1=(float *)malloc(H*V*sizeof(float));
   aux2=(float *)malloc(H*V*sizeof(float));
   imd2=(float *)malloc(H*V*sizeof(float));
   if((aux1==NULL)||(aux2==NULL)||(imd2==NULL)
     ||(ext_pot==NULL)) { printf("\nno memoria\n\n"); exit(-1); }
   imblur=aux1; imsobel=aux1;

   collect_images(); 
   for(nimg=0;nimg<NIMG;nimg++)
   { // load data
     gettimeofday(&ini,NULL);
     printf("--------------------------------------\n");
     printf("* %s\n",name[nimg]); 
     sprintf(fichero,PREFIX,name[nimg]);
     read_image_ppm(fichero,im);
     
     // 1. Vessel pre-estimation
     gettimeofday(&start,NULL);
     tophat(im,tmp1);
     blur(tmp1,imblur);
     for(i=0;i<H*V;i++) imblur[i]=tmp1[i]-imblur[i];
     for(i=0;i<H*V;i++) if(imblur[i]+THRESHOLD>0) imt[i]=1; else imt[i]=0;
     remove_outside_FOV(imt,im,FOV_UMBRAL);
     remove_small_groups(imt,CONN,NOISE_SIZE);
   
     // 2. Initial contour estimation
     erosion(imt,input,3);
   
     // 3. External potential estimation
     // 3.1 Edge detection + logical operations
     sobel_edge_detect(imt,edge2);
     for(i=0;i<H*V;i++) imd2[i]=1-edge2[i];
     // 3.2 Distance map
     for(k=1;k<=5;k++)
     {  erosion_float(imd2,aux2,st3,3);
        for(i=0;i<H*V;i++) imd2[i] += aux2[i]/k;
     }
     imfilter_float(imd2,diff_a,3,1); // OJO es conv2
     max2=imd2[0]; for(i=0;i<H*V;i++) if(imd2[i]>max2) max2=imd2[i];
     for(i=0;i<H*V;i++) imd2[i]=imd2[i]/max2;
     for(i=0;i<H*V;i++) ext_pot[i]=k_ext*(100-(50*edge2[i]+50*imd2[i]));
 
     // 4. Contour evolution (PLS)
     for(k=0;k<STAGES;k++)
     {  f_ContEvol(input,ext_pot,output,k);
        memcpy(input,output,H*V*sizeof(char));
     }   

     // 5. Removes and postprocessings
     remove_outside_FOV(input,im,FOV_UMBRAL);
     remove_small_structures(input,SMALL_STRUC_SIZE,0,SMALL_STRUC_SIZE/2,8);
     if(H>2000) postproc_big_images(input,tmp1);
     gettimeofday(&end,NULL);
     t_comp[nimg]=(end.tv_sec-start.tv_sec)+(end.tv_usec-start.tv_usec)*1E-6;
 
     // Save data
     sprintf(fichero,"output/vessel_seg_cpu-%s.pgm",name[nimg]);
     save_image_pgm(fichero,input);
     gettimeofday(&fin,NULL);
     t_tot[nimg]=(fin.tv_sec-ini.tv_sec)+(fin.tv_usec-ini.tv_usec)*1E-6;
     truth=tmp1;
     compute_accuracies(input,truth,nimg);
     printf(" t_comp=%f, t_total=%f\n",t_comp[nimg],t_tot[nimg]);
   }
   compute_averages();
   free(imt); free(edge1); free(edge2); free(tmp1); free(input);
   free(output); free(im); free(aux1); free(aux2); free(imd2);
}

