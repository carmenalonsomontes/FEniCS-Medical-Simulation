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
// tested: Linux (CUDA version 4.2)
// compile with: nvcc -arch=sm_20 vessel_seg.cu -O3 -lm

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define BASE unsigned char
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

void read_image_ppm(const char *nombre, BASE img[V*H])
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

void read_image_ppm_cache(const char *nombre)
{  FILE* fp; int Nh, Nv, flag; unsigned char texto[100], *data, c;
   data=(unsigned char *) malloc(3*H*V*sizeof(char));
   if(data==NULL) { perror("No enough RAM\n"); exit(-1); }
   fp = fopen (nombre, "rb");
   if(fp==NULL) { fprintf(stderr,"Can not open file %s\n",nombre); exit(1); }
   flag=fscanf(fp,"%s",texto);   
   if(strcasecmp((const char *)texto,"P6")) { fprintf(stderr,"No PPM file\n"); exit(-1); } 
   c=fgetc(fp); c=fgetc(fp);
   while((c=='#')) { while (c != '\n') c = fgetc(fp); c = fgetc(fp); } ungetc(c,fp);
   flag=fscanf(fp,"%s",texto); Nh=atoi((const char*)texto); 
   flag=fscanf(fp,"%s",texto); Nv=atoi((const char*)texto);
   if((Nh!=H)||(Nv!=V)) { printf("Incorrect image size\n"); exit(-1); }
   flag=fscanf(fp,"%s",texto); c = fgetc(fp); ;
   flag=fread(data,3*H*V,1,fp); 
     if(flag!=1) { perror("Can not read all data"); exit(-1); }
   fclose(fp); free(data);
}

void read_image_pgm(const char *nombre, BASE img[V*H])
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

void save_image_pgm(const char fichero[40], BASE img[H*V])
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

void compute_accuracies(BASE *output, BASE *truth, int nimg)
{  char fichero[60]; int i, a00=0, a01=0, a10=0, a11=0, total; 
   BASE *mask_img;
   mask_img=(BASE *)malloc(H*V*sizeof(BASE));
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
   printf(" GPU imag=%d stages=%d\n",NIMG,STAGES);
   printf("  Sens=%f, Spec=%f, MAA=%f\n",p1/NIMG,p2/NIMG,p3/NIMG);
   printf("  t_comp=%f, t_total=%f\n",t1/NIMG,t2/NIMG);
}
   
// ---------------------------------------------------------------------
// GPU THINGS
// ---------------------------------------------------------------------  

#define BLOCK 16 // 8 o 16, las reducciones requieren pot.2 y el filtro7 >=6
#define BLOCK2 (BLOCK+2) // para alguna funcion de etiquetado
#define BLOCKa 2   // for big images
#define BLOCKb 128
dim3 dimBlock(BLOCK,BLOCK);
dim3 dimGrid(ceilf(1.0*V/BLOCK),ceilf(1.0*H/BLOCK));
dim3 dimBlock1(BLOCK*BLOCK);
dim3 dimGrid1(ceilf(1.0*H*V/(BLOCK*BLOCK)));
// para reduccion
dim3 dimBlock2(BLOCK*BLOCK);
dim3 dimGrid2(ceilf(1.0*H*V/(BLOCK*BLOCK*BLOCK*BLOCK)));
dim3 dimBlock3(1);
dim3 dimGrid3(1);
// para el reborde
dim3 dimBlock4(BLOCK,BLOCK);
dim3 dimGrid4(ceilf(1.0*V/(BLOCK-2)),ceilf(1.0*H/(BLOCK-2)));
// para la ventana
dim3 dimGrid51(ceilf(1.0*V/BLOCK),1);
dim3 dimGrid52(1,ceilf(1.0*H/BLOCK));
// para la imagen grande
dim3 dimGrid51a(ceilf(1.0*V/BLOCKa),1);
dim3 dimGrid52a(1,ceilf(1.0*H/BLOCKa));
dim3 dimBlocka(BLOCKa,BLOCKb);
dim3 dimBlockb(BLOCKb,BLOCKa);

void cuda_init_2_0()
{  int num_devices, device, found=0;
   int max_multiprocessors = 0, max_device = 0;
   cudaGetDeviceCount(&num_devices);
   for (device = 0; device < num_devices; device++) {
              cudaDeviceProp properties;
              cudaGetDeviceProperties(&properties, device);
 	      printf("Card %d: %s,",device,properties.name);
	      printf(" Capability=%d.%d, ",properties.major,properties.minor);
              if (max_multiprocessors < properties.multiProcessorCount
	           && properties.major >= 2 ) {
		      found=1;
                      max_multiprocessors = properties.multiProcessorCount;
                      max_device = device; } }
   if(found) { cudaSetDevice(max_device); 
               printf("Using card num. %d\n",max_device); }
   else { fprintf(stderr,"No cards with capability >= 2.0 found\n"); exit(0); }
}

void cuda_error(const char *msg)
{   cudaError_t err = cudaGetLastError();
    if( cudaSuccess != err) 
    {   fprintf(stderr, "Cuda error: %s: %s.\n", msg, cudaGetErrorString( err) );
        exit(EXIT_FAILURE); } }

__global__ void max3a(float *gi, float *go)
{ __shared__ float sdata[BLOCK*BLOCK];
  unsigned int tid=threadIdx.x;
  unsigned int i=blockIdx.x*blockDim.x+threadIdx.x;
  if(i<H*V) sdata[tid]=gi[i]; else sdata[tid]=0;
  __syncthreads();
  unsigned int s;
  for(s=blockDim.x/2;s>0;s>>=1)
  {  if(tid < s) sdata[tid]=max(sdata[tid],sdata[tid+s]); 
     __syncthreads(); }
  if(tid==0) go[blockIdx.x]=sdata[0];
}

__global__ void max3b(float *gi)
{ __shared__ float sdata[BLOCK*BLOCK];
  unsigned int tid=threadIdx.x;
  unsigned int i=blockIdx.x*blockDim.x+threadIdx.x;
  if(i<ceilf(1.0*H*V/(BLOCK*BLOCK))) sdata[tid]=gi[i]; else sdata[tid]=0;
  __syncthreads();
  unsigned int s;
  for(s=blockDim.x/2;s>0;s>>=1)
  {  if(tid < s) sdata[tid] = max(sdata[tid],sdata[tid+s]); 
     __syncthreads(); }
  if(tid==0) gi[blockIdx.x*BLOCK*BLOCK]=sdata[0];
}

__global__ void max3c(float *gi)
{  float tmp=0; int i, des=BLOCK*BLOCK*BLOCK*BLOCK;
   tmp=gi[0];
   for(i=1;i<ceilf(1.0*H*V/des);i++) tmp=max(tmp,gi[i*BLOCK*BLOCK]);
   gi[0]=tmp;
}

// ----------------------------------------------------------------
// UNION/FIND
// ----------------------------------------------------------------

__global__ void path_compress_gpu(int *lab)
{  unsigned int i = threadIdx.x + BLOCK*BLOCK*blockIdx.x;
   int c;
   if(i<H*V) if(lab[i]<H*V) { c=i; while(lab[c]!=c) c=lab[c]; lab[i]=c; } 
}

__device__ void union_find_kernel_gpu(int *lab, int u, int v)
{  int c, d;
   c=u; while(lab[c]!=c) c=lab[c];
   d=v; while(lab[d]!=d) d=lab[d];
   if(c<d) atomicMin(&lab[d],c);
   if(d<c) atomicMin(&lab[c],d);
}

__global__ void union_find_global_gpu(int *lab, int conn)
{  int i = threadIdx.x + BLOCK*blockIdx.x;
   int j = threadIdx.y + BLOCK*blockIdx.y;

   if((i<V)&&(j<H))
   { if(lab[i*H+j]<H*V)
     { if(i>0) if(lab[(i-1)*H+j]<H*V) union_find_kernel_gpu(lab,i*H+j,(i-1)*H+j); 
       if(i<V-1) if(lab[(i+1)*H+j]<H*V) union_find_kernel_gpu(lab,i*H+j,(i+1)*H+j); 
       if(j>0) if(lab[i*H+(j-1)]<H*V) union_find_kernel_gpu(lab,i*H+j,i*H+(j-1)); 
       if(j<H-1) if(lab[i*H+(j+1)]<H*V) union_find_kernel_gpu(lab,i*H+j,i*H+(j+1)); 
       if(conn==8) {
        if((i>0)&&(j>0))if(lab[(i-1)*H+(j-1)]<H*V) union_find_kernel_gpu(lab,i*H+j,(i-1)*H+(j-1)); 
        if((i>0)&&(j<H-1))if(lab[(i-1)*H+(j+1)]<H*V) union_find_kernel_gpu(lab,i*H+j,(i-1)*H+(j+1)); 
        if((i<V-1)&&(j>0))if(lab[(i+1)*H+(j-1)]<H*V) union_find_kernel_gpu(lab,i*H+j,(i+1)*H+(j-1));
        if((i>V-1)&&(j<H-1))if(lab[(i+1)*H+(j+1)]<H*V) union_find_kernel_gpu(lab,i*H+j,(i+1)*H+(j+1)); }}}    
}

__global__ void union_find_shared_gpu(int *label, unsigned char *img, int conn)
{  unsigned int i2=threadIdx.x, j2=threadIdx.y, i=i2+1, j=j2+1, c, x, y;
   unsigned int tid=i*BLOCK2+j, tid2=i2*BLOCK+j2;
   int ig = threadIdx.x + BLOCK*blockIdx.x;
   int jg = threadIdx.y + BLOCK*blockIdx.y;
   __shared__ int lab[BLOCK2*BLOCK2]; 
   BASE F=0;
   
   if((ig<V)&&(jg<H)) { if(img[ig*H+jg]==F) lab[tid]=tid; else lab[tid]=H*V; }
   else lab[tid]=H*V;
   
   if(tid2<BLOCK2) { lab[tid2]=H*V; lab[tid2+BLOCK2*(BLOCK2-1)]=H*V;
                     lab[tid2*BLOCK2]=H*V; lab[tid2*BLOCK2+(BLOCK2-1)]=H*V; }
   __syncthreads();
   
   if(lab[tid]<H*V)
   {  if(lab[(i-1)*BLOCK2+j]<H*V) union_find_kernel_gpu(lab,tid,(i-1)*BLOCK2+j);
      if(lab[(i+1)*BLOCK2+j]<H*V) union_find_kernel_gpu(lab,tid,(i+1)*BLOCK2+j); 
      if(lab[i*BLOCK2+(j-1)]<H*V) union_find_kernel_gpu(lab,tid,i*BLOCK2+(j-1)); 
      if(lab[i*BLOCK2+(j+1)]<H*V) union_find_kernel_gpu(lab,tid,i*BLOCK2+(j+1)); 
      if(conn==8) {
        if(lab[(i-1)*BLOCK2+(j-1)]<H*V) union_find_kernel_gpu(lab,tid,(i-1)*BLOCK2+(j-1)); 
        if(lab[(i-1)*BLOCK2+(j+1)]<H*V) union_find_kernel_gpu(lab,tid,(i-1)*BLOCK2+(j+1)); 
        if(lab[(i+1)*BLOCK2+(j-1)]<H*V) union_find_kernel_gpu(lab,tid,(i+1)*BLOCK2+(j-1));
        if(lab[(i+1)*BLOCK2+(j+1)]<H*V) union_find_kernel_gpu(lab,tid,(i+1)*BLOCK2+(j+1)); } 
      __syncthreads();
 
      c=tid; while(lab[c]!=c) c=lab[c]; lab[tid]=c;  
      __syncthreads();

      x=lab[tid]/BLOCK2-1; y=(lab[tid]%BLOCK2)-1; 
      lab[tid]=(x+BLOCK*blockIdx.x)*H+(y+BLOCK*blockIdx.y); }
   __syncthreads();
      
   if((ig<V)&&(jg<H)) label[ig*H+jg]=lab[tid];
}

__global__ void update_test_gpu(int *lab, int *old, int *flag)
{  unsigned int i = threadIdx.x + BLOCK*BLOCK*blockIdx.x;
   if(i<H*V) if(lab[i]!=old[i]) atomicOr(flag,1);  
}

__global__ void histogram_gpu(BASE *img, int *label, int *cuenta)
{ unsigned int i = threadIdx.x + BLOCK*BLOCK*blockIdx.x;
  BASE B=1;
  if(i<H*V) if(img[i]!=B) atomicAdd(&cuenta[label[i]],1);
}

__global__ void extend_histogram_gpu(BASE *img, int *label, int *cuenta)
{ unsigned int i = threadIdx.x + BLOCK*BLOCK*blockIdx.x;
  BASE B=1;
  if(i<H*V) if(img[i]!=B) cuenta[i]=cuenta[label[i]];
}

__global__ void remove_small_groups_gpu(BASE *img, int *cuenta, int size)
{ BASE F=0, B=1;
  unsigned int i = threadIdx.x + BLOCK*BLOCK*blockIdx.x;
  if(i<H*V) if((img[i]==F)&&(cuenta[i]<size)) img[i]=B;
}

void remove_noise(BASE *img, int *flag, int *label, int *cuenta, int conn, int size)
{  int stop=1;

   union_find_shared_gpu<<<dimGrid,dimBlock>>>(label,img,conn);
   union_find_global_gpu<<<dimGrid,dimBlock>>>(label,conn);
   path_compress_gpu<<<dimGrid1,dimBlock1>>>(label);
   while(stop)
   {  cudaMemset(flag,0,sizeof(int));
      cudaMemcpy(cuenta,label,H*V*sizeof(int),cudaMemcpyDeviceToDevice);
      union_find_global_gpu<<<dimGrid,dimBlock>>>(label,conn);
      path_compress_gpu<<<dimGrid1,dimBlock1>>>(label);
      update_test_gpu<<<dimGrid1,dimBlock1>>>(label,cuenta,flag);
      cudaMemcpy(&stop,flag,1*sizeof(int),cudaMemcpyDeviceToHost);
   }
   cudaMemset(cuenta,0,H*V*sizeof(int));
   histogram_gpu<<<dimGrid1,dimBlock1>>>(img,label,cuenta);
   extend_histogram_gpu<<<dimGrid1,dimBlock1>>>(img,label,cuenta);
   remove_small_groups_gpu<<<dimGrid1,dimBlock1>>>(img,cuenta,size);
}

__global__ void grid_cross_detection_gpu(int *label, int *aux, int SEP, int OFF1, int OFF2)
{  unsigned int i = threadIdx.x + BLOCK*blockIdx.x;
   unsigned int j = threadIdx.y + BLOCK*blockIdx.y;
   unsigned int tid=i*H+j; int c;

   if((i<V)&&(j<H)&&((i%SEP==OFF1)||(j%SEP==OFF1)))
   {  c=label[tid]; if(c<H*V) atomicOr(&aux[c],1); } 
   if((i<V)&&(j<H)&&((i%SEP==OFF2)||(j%SEP==OFF2)))
   {  c=label[tid]; if(c<H*V) atomicOr(&aux[c],2); } 
}

__global__ void extend_cross_gpu(int *label, int *aux, BASE *img)
{ BASE F=0, B=1; int c;
  unsigned int i = threadIdx.x + BLOCK*BLOCK*blockIdx.x;
  if(i<H*V) {  c=label[i]; if((c<H*V)&&(aux[c]==3)) img[i]=F; else img[i]=B; }
}

void remove_small_structures(BASE *img, int *flag, int *label, int *aux, int conn, int fill)
{  int stop=1;

   union_find_shared_gpu<<<dimGrid,dimBlock>>>(label,img,conn);
   union_find_global_gpu<<<dimGrid,dimBlock>>>(label,conn);
   path_compress_gpu<<<dimGrid1,dimBlock1>>>(label);
   while(stop)
   {  cudaMemset(flag,0,sizeof(int));
      cudaMemcpy(aux,label,H*V*sizeof(int),cudaMemcpyDeviceToDevice);
      union_find_global_gpu<<<dimGrid,dimBlock>>>(label,conn);
      path_compress_gpu<<<dimGrid1,dimBlock1>>>(label);
      update_test_gpu<<<dimGrid1,dimBlock1>>>(label,aux,flag);
      cudaMemcpy(&stop,flag,1*sizeof(int),cudaMemcpyDeviceToHost);
   }

   cudaMemset(aux,0,H*V*sizeof(int));
   grid_cross_detection_gpu<<<dimGrid,dimBlock>>>(label,aux,fill,0,fill/2);
   extend_cross_gpu<<<dimGrid1,dimBlock1>>>(label,aux,img);
}

// -------------------------------------------------------------
// FILTERS
// --------------------------------------------------------------

__global__ void erosion_shared_gpu(BASE in[H*V], BASE out[H*V])
{   int ig = threadIdx.x + (BLOCK-2)*blockIdx.x-1;
    int jg = threadIdx.y + (BLOCK-2)*blockIdx.y-1;
    unsigned int i=threadIdx.x, j=threadIdx.y;
    __shared__ BASE ins[BLOCK*BLOCK];
    BASE min2;

    if((ig>=0)&&(ig<V)&&(jg>=0)&&(jg<H)) ins[i*BLOCK+j]=in[ig*H+jg]; else ins[i*BLOCK+j]=255;
    __syncthreads(); 

    if((i>0)&&(i<BLOCK-1)&&(j>0)&&(j<BLOCK-1)&&(ig<V)&&(jg<H))  { 
        min2=ins[i*BLOCK+j]&ins[(i-1)*BLOCK+(j-1)]&ins[(i-1)*BLOCK+j]
	    &ins[(i-1)*BLOCK+(j+1)]&ins[i*BLOCK+(j-1)]&ins[i*BLOCK+(j+1)]
	    &ins[(i+1)*BLOCK+(j-1)]&ins[(i+1)*BLOCK+j]&ins[(i+1)*BLOCK+(j+1)];
	out[ig*H+jg]=min2; }
}

__global__ void erosion_float_shared_gpu(float in[H*V], float out[H*V])
{   int ig = threadIdx.x + (BLOCK-2)*blockIdx.x-1;
    int jg = threadIdx.y + (BLOCK-2)*blockIdx.y-1;
    unsigned int i=threadIdx.x, j=threadIdx.y;
    __shared__ float ins[BLOCK*BLOCK];
    float min2;

    if((ig>=0)&&(ig<V)&&(jg>=0)&&(jg<H)) ins[i*BLOCK+j]=in[ig*H+jg]; else ins[i*BLOCK+j]=99999999;
    __syncthreads(); 

    if((i>0)&&(i<BLOCK-1)&&(j>0)&&(j<BLOCK-1)&&(ig<V)&&(jg<H))  { 
        min2=ins[i*BLOCK+j];
	min2=min(min2,ins[(i-1)*BLOCK+(j-1)]);
	min2=min(min2,ins[(i-1)*BLOCK+j]);
	min2=min(min2,ins[(i-1)*BLOCK+(j+1)]);
	min2=min(min2,ins[i*BLOCK+(j-1)] );
	min2=min(min2,ins[i*BLOCK+(j+1)]);
	min2=min(min2,ins[(i+1)*BLOCK+(j-1)]);
	min2=min(min2,ins[(i+1)*BLOCK+j]);
	min2=min(min2,ins[(i+1)*BLOCK+(j+1)]);
	out[ig*H+jg]=min2; }
}

__global__ void dilate_shared_gpu(BASE in[H*V], BASE out[H*V])
{   int ig = threadIdx.x + (BLOCK-2)*blockIdx.x-1;
    int jg = threadIdx.y + (BLOCK-2)*blockIdx.y-1;
    unsigned int i=threadIdx.x, j=threadIdx.y;
    __shared__ BASE ins[BLOCK*BLOCK];
    BASE max2;

    if((ig>=0)&&(ig<V)&&(jg>=0)&&(jg<H)) ins[i*BLOCK+j]=in[ig*H+jg]; else ins[i*BLOCK+j]=0;
    __syncthreads(); 

    if((i>0)&&(i<BLOCK-1)&&(j>0)&&(j<BLOCK-1)&&(ig<V)&&(jg<H))  { 
        max2=ins[i*BLOCK+j]|ins[(i-1)*BLOCK+(j-1)]|ins[(i-1)*BLOCK+j]
	    |ins[(i-1)*BLOCK+(j+1)]|ins[i*BLOCK+(j-1)]|ins[i*BLOCK+(j+1)]
	    |ins[(i+1)*BLOCK+(j-1)]|ins[(i+1)*BLOCK+j]|ins[(i+1)*BLOCK+(j+1)];
	out[ig*H+jg]=max2; }
}

__global__ void f_edge_shared_gpu(BASE *A, BASE *out)
{  int ig = threadIdx.x + (BLOCK-2)*blockIdx.x-1;
   int jg = threadIdx.y + (BLOCK-2)*blockIdx.y-1;
   unsigned int i=threadIdx.x, j=threadIdx.y;
   BASE tmp, sum;
   __shared__ BASE in[BLOCK*BLOCK];

   if((ig>=0)&&(ig<V)&&(jg>=0)&&(jg<H)) tmp=A[ig*H+jg]; else tmp=1;
   in[i*BLOCK+j]=tmp;
    __syncthreads(); 
   
   if((j>0)&&(j<BLOCK-1)&&(i>0)&&(i<BLOCK-1))
   {  sum=tmp&in[(i-1)*BLOCK+j]&in[(i+1)*BLOCK+j]&in[i*BLOCK+(j-1)]&in[i*BLOCK+(j+1)]; 
      if(sum) tmp=0; }
   if((jg<H)&&(ig<V)&&(j>0)&&(j<BLOCK-1)&&(i>0)&&(i<BLOCK-1)) 
      out[ig*H+jg]=tmp;
}

// es mas rapida en memoria global que en shared
__global__ void f_shift_global_gpu(const char dir, BASE *input, BASE *result,
     BASE boundary)
{  unsigned int i = threadIdx.x + BLOCK*blockIdx.x;
   unsigned int j = threadIdx.y + BLOCK*blockIdx.y;
   if((i<V)&&(j<H)) {
     if((dir=='S')&&(i>0)) result[i*H+j]=input[(i-1)*H+j];
     else if((dir=='W')&&(j<H-1)) result[i*H+j]=input[i*H+(j+1)];
     else if((dir=='N')&&(i<V-1)) result[i*H+j]=input[(i+1)*H+j];
     else if((dir=='E')&&(j>0))   result[i*H+j]=input[i*H+(j-1)];
     else result[i*H+j]=boundary; }
}
// vale tanto para replicar o no, para replica=1 
__global__ void filter_float_shared_gpu(float A[H*V], float aux[H*V], int replica)
{   int ig = threadIdx.x + (BLOCK-2)*blockIdx.x-1;
    int jg = threadIdx.y + (BLOCK-2)*blockIdx.y-1;
    unsigned int i=threadIdx.x, j=threadIdx.y;
    float B[]={0.10,0.15,0.10, 0.15,0.00,0.15, 0.10, 0.15, 0.10};
    float tmp;
    __shared__ float in[BLOCK*BLOCK];

    if((ig>=0)&&(ig<V)&&(jg>=0)&&(jg<H)) in[i*BLOCK+j]=A[ig*H+jg]; else in[i*BLOCK+j]=0;
    __syncthreads(); 

    if(replica==1)
    {  if((ig<0)&&(jg<0)) in[i*BLOCK+j]=in[(i+1)*BLOCK+(j+1)];
       else if((ig<0)&&(jg==H)) in[i*BLOCK+j]=in[(i+1)*BLOCK+(j-1)];
       else if((ig==V)&&(jg<0)) in[i*BLOCK+j]=in[(i-1)*BLOCK+(j+1)];
       else if((ig==V)&&(jg==H)) in[i*BLOCK+j]=in[(i-1)*BLOCK+(j-1)];
       else if(ig<0) in[i*BLOCK+j]=in[(i+1)*BLOCK+j];
       else if(ig==V) in[i*BLOCK+j]=in[(i-1)*BLOCK+j];
       else if(jg<0) in[i*BLOCK+j]=in[i*BLOCK+(j+1)];
       else if(jg==H) in[i*BLOCK+j]=in[i*BLOCK+(j-1)];
       __syncthreads(); }

    if((i>0)&&(i<BLOCK-1)&&(j>0)&&(j<BLOCK-1)&&(ig<V)&&(jg<H))  { 
       tmp=B[0]*in[(i-1)*BLOCK+(j-1)]+B[1]*in[(i-1)*BLOCK+j]+B[2]*in[(i-1)*BLOCK+(j+1)]
          +B[3]*in[i*BLOCK+(j-1)]    +B[4]*in[i*BLOCK+j]    +B[5]*in[i*BLOCK+(j+1)]
	  +B[6]*in[(i+1)*BLOCK+(j-1)]+B[7]*in[(i+1)*BLOCK+j]+B[8]*in[(i+1)*BLOCK+(j+1)]; 
       aux[ig*H+jg]=tmp; }
}

__global__ void remove_outside_FOV_gpu(BASE *f, BASE *tmp, int umbral)
{ unsigned int i = threadIdx.x + BLOCK*BLOCK*blockIdx.x;
  if(i<H*V) if(tmp[i]<umbral) f[i]=1; //25
}

__global__ void sobel_edge_detect_lineal_gpu(BASE *in, BASE *out) 
{  unsigned int i = threadIdx.x + BLOCK*BLOCK*blockIdx.x;
   int l, fil, col, ns=3; int gx, gy, g;
   int sobx[]={-1,0,1,-2,0,2,-1,0,1}, soby[]={1,2,1,0,0,0,-1,-2,-1};

   if(i<H*V) {
   gx=0; gy=0; for(l=0;l<ns*ns;l++)
   {  fil=(i/H)-((ns-1)/2)+(l/ns); col=(i%H)-((ns-1)/2)+(l%ns);
      if(fil<0)fil=0; else if(fil>=V) fil=V-1;
      if(col<0)col=0; else if(col>=H) col=H-1;
      gx+=sobx[l]*in[fil*H+col]; gy+=soby[l]*in[fil*H+col]; }
      g=gx*gx+gy*gy;
      if(g<SOBEL_UMBRAL) out[i]=0; else out[i]=1;
   // stagesa final   
   out[i] = in[i] & (!out[i]); }
}

__global__ void sobel_edge_detect_shared_gpu(BASE *A, BASE *out)
{   int ig = threadIdx.x + (BLOCK-2)*blockIdx.x-1;
    int jg = threadIdx.y + (BLOCK-2)*blockIdx.y-1;
    unsigned int i=threadIdx.x, j=threadIdx.y;
    int gx, gy, g; BASE tmp1,tmp2;
    int BX[]={-1,0,1,-2,0,2,-1,0,1}, BY[]={1,2,1,0,0,0,-1,-2,-1};
    __shared__ int in[BLOCK*BLOCK];
     
    if((ig>=0)&&(ig<V)&&(jg>=0)&&(jg<H)) tmp1=A[ig*H+jg]; else tmp1=0;
    in[i*BLOCK+j]=tmp1;
    __syncthreads(); 
    if((ig<0)&&(jg<0)) in[i*BLOCK+j]=in[(i+1)*BLOCK+(j+1)];
    else if((ig<0)&&(jg==H)) in[i*BLOCK+j]=in[(i+1)*BLOCK+(j-1)];
    else if((ig==V)&&(jg<0)) in[i*BLOCK+j]=in[(i-1)*BLOCK+(j+1)];
    else if((ig==V)&&(jg==H)) in[i*BLOCK+j]=in[(i-1)*BLOCK+(j-1)];
    else if(ig<0) in[i*BLOCK+j]=in[(i+1)*BLOCK+j];
    else if(ig==V) in[i*BLOCK+j]=in[(i-1)*BLOCK+j];
    else if(jg<0) in[i*BLOCK+j]=in[i*BLOCK+(j+1)];
    else if(jg==H) in[i*BLOCK+j]=in[i*BLOCK+(j-1)];    
    __syncthreads(); 
   
    if((i>0)&&(i<BLOCK-1)&&(j>0)&&(j<BLOCK-1)&&(ig<V)&&(jg<H))  { 
       gx=BX[0]*in[(i-1)*BLOCK+(j-1)]+BX[1]*in[(i-1)*BLOCK+j]+BX[2]*in[(i-1)*BLOCK+(j+1)]
         +BX[3]*in[i*BLOCK+(j-1)]    +BX[4]*in[i*BLOCK+j]    +BX[5]*in[i*BLOCK+(j+1)]
	 +BX[6]*in[(i+1)*BLOCK+(j-1)]+BX[7]*in[(i+1)*BLOCK+j]+BX[8]*in[(i+1)*BLOCK+(j+1)];
       gy=BY[0]*in[(i-1)*BLOCK+(j-1)]+BY[1]*in[(i-1)*BLOCK+j]+BY[2]*in[(i-1)*BLOCK+(j+1)]
         +BY[3]*in[i*BLOCK+(j-1)]    +BY[4]*in[i*BLOCK+j]    +BY[5]*in[i*BLOCK+(j+1)]
	 +BY[6]*in[(i+1)*BLOCK+(j-1)]+BY[7]*in[(i+1)*BLOCK+j]+BY[8]*in[(i+1)*BLOCK+(j+1)];
       g=gx*gx+gy*gy;	 
       __syncthreads(); 
       if(g<SOBEL_UMBRAL) tmp2=0; else tmp2=1; 
       // esto no tiene que ver con la deteccion de edges
       out[ig*H+jg]= tmp1 & (!tmp2); }
}

__global__ void erosion_st15_filas_win_gpu(BASE A[H*V], BASE out[H*V])
{   int ig = threadIdx.x + BLOCK*blockIdx.x;
    int jg = threadIdx.y + BLOCK*blockIdx.y-BLOCK/2;
    unsigned int i=threadIdx.x, j=threadIdx.y;
    __shared__ BASE in[BLOCK][2*BLOCK];
    int k, l; BASE min2;

    in[i][j+BLOCK]=((ig<V)&&(jg>=0)&&(jg<H))?A[ig*H+jg]:255;
    __syncthreads();

    for(k=0;k<ceilf(1.0*H/BLOCK);k++)
    {  jg=jg+BLOCK;
       in[i][j]=in[i][j+BLOCK];
       in[i][j+BLOCK]=((ig<V)&&(jg<H))?A[ig*H+jg]:255;
       __syncthreads();
       if((ig<V)&&(jg-BLOCK/2<H)) { min2=in[i][j+BLOCK/2];
          for(l=-7;l<=7;l++) min2=min(min2,in[i][j+BLOCK/2+l]);
       out[ig*H+jg-BLOCK/2]= (BASE) min2; } 
       __syncthreads(); }
}

__global__ void erosion_st15_cols_win_gpu(BASE A[H*V], BASE out[H*V])
{   int ig = threadIdx.x + BLOCK*blockIdx.x-BLOCK/2;
    int jg = threadIdx.y + BLOCK*blockIdx.y;
    unsigned int i=threadIdx.x, j=threadIdx.y;
    __shared__ BASE in[2*BLOCK][BLOCK];
    int k, l; BASE min2;

    in[i+BLOCK][j]=((ig>=0)&&(ig<V)&&(jg<H))?A[ig*H+jg]:255;
    __syncthreads();

    for(k=0;k<ceilf(1.0*V/BLOCK);k++)
    {  ig=ig+BLOCK;
       in[i][j]=in[i+BLOCK][j];
       in[i+BLOCK][j]=((ig<V)&&(jg<H))?A[ig*H+jg]:255;
       __syncthreads();
       if((ig-BLOCK/2<V)&&(jg<H)) { min2=in[i+BLOCK/2][j];
          for(l=-7;l<=7;l++) min2=min(min2,in[i+BLOCK/2+l][j]);
       out[(ig-BLOCK/2)*H+jg]= (BASE) min2; } 
       __syncthreads(); }
}


__global__ void filter_st17_filas_win_gpu(BASE A[H*V], float out[H*V])
{   int ig = threadIdx.x + BLOCK*blockIdx.x;
    int jg = threadIdx.y + BLOCK*blockIdx.y-BLOCK/2;
    unsigned int i=threadIdx.x, j=threadIdx.y;
    __shared__ BASE in[BLOCK][2*BLOCK];
    int k, l, a, b; float tmp;

    a=ig; if(a>=V) a=V-1; b=jg; if(b<0) b=0; else if(b>=H) b=H-1; 
    in[i][j+BLOCK]=A[a*H+b];
    __syncthreads();

    for(k=0;k<ceilf(1.0*H/BLOCK);k++)
    {  jg=jg+BLOCK; b=jg; if(b>=H) b=H-1;
       in[i][j]=in[i][j+BLOCK];
       in[i][j+BLOCK]=A[a*H+b];
       __syncthreads();
       if((ig<V)&&(jg-BLOCK/2<H)) 
       {  tmp=0; 
          for(l=-8;l<=8;l++) tmp+=in[i][j+BLOCK/2+l];
          out[ig*H+jg-BLOCK/2]= (float) tmp; } 
       __syncthreads(); }
}

__global__ void filter_st17_cols_win_gpu(float A[H*V], float out[H*V])
{   int ig = threadIdx.x + BLOCK*blockIdx.x-BLOCK/2;
    int jg = threadIdx.y + BLOCK*blockIdx.y;
    unsigned int i=threadIdx.x, j=threadIdx.y;
    __shared__ float in[2*BLOCK][BLOCK];
    int k, l, a, b; float tmp=0;

    b=jg; if(b>=H) b=H-1; a=ig; if(a<0) a=0; else if(a>=V) a=V-1; 
    in[i+BLOCK][j]=A[a*H+b];
    __syncthreads();

    for(k=0;k<ceilf(1.0*V/BLOCK);k++)
    {  ig=ig+BLOCK; a=ig; if(a>=V) a=V-1; 
       in[i][j]=in[i+BLOCK][j];
       in[i+BLOCK][j]=A[a*H+b];
       __syncthreads();
       if((ig-BLOCK/2<V)&&(jg<H)) 
       {  tmp=0;
          for(l=-8;l<=8;l++) tmp+=in[i+BLOCK/2+l][j];
          out[(ig-BLOCK/2)*H+jg]= tmp/289.0; } 
       __syncthreads(); }
}

__global__ void kernel_resta(BASE *im, float *imblur)
{ unsigned int i = threadIdx.x + BLOCK*BLOCK*blockIdx.x;
  if(i<H*V) { imblur[i]=im[i]-imblur[i]; } // 
}

__global__ void kernel_thresholding(float *imblur, BASE *tmp1, float factor)
{ unsigned int i = threadIdx.x + BLOCK*BLOCK*blockIdx.x;
  if(i<H*V) { tmp1[i]=(imblur[i]+factor>0); }
}

void diffusion_and_binarization(BASE *tmp1, BASE *imt, float *imblur, float *aux1, float factor1)
{  filter_st17_filas_win_gpu<<<dimGrid51,dimBlock>>>(tmp1,aux1);
   filter_st17_cols_win_gpu<<<dimGrid52,dimBlock>>>(aux1,imblur);
   kernel_resta<<<dimGrid1,dimBlock1>>>(tmp1,imblur);  
   kernel_thresholding<<<dimGrid1,dimBlock1>>>(imblur,imt,factor1); 
}

void postproc_big_images(BASE *output, BASE *tmp1)
{  erosion_shared_gpu<<<dimGrid4,dimBlock4>>>(output,tmp1);
   erosion_shared_gpu<<<dimGrid4,dimBlock4>>>(tmp1,output);
   erosion_shared_gpu<<<dimGrid4,dimBlock4>>>(output,tmp1);
   erosion_shared_gpu<<<dimGrid4,dimBlock4>>>(tmp1,output);
   if(H>4000)
   {  erosion_shared_gpu<<<dimGrid4,dimBlock4>>>(output,tmp1);
      dilate_shared_gpu<<<dimGrid4,dimBlock4>>>(tmp1,output); }
   dilate_shared_gpu<<<dimGrid4,dimBlock4>>>(output,tmp1);
   dilate_shared_gpu<<<dimGrid4,dimBlock4>>>(tmp1,output);
   dilate_shared_gpu<<<dimGrid4,dimBlock4>>>(output,tmp1);
   dilate_shared_gpu<<<dimGrid4,dimBlock4>>>(tmp1,output);
}

//-------------------------------------------------------------
// TOP-HAT
//-------------------------------------------------------------

__global__ void dilate_st15_filas_win_gpu(BASE A[H*V], BASE out[H*V])
{   int ig = threadIdx.x + BLOCK*blockIdx.x;
    int jg = threadIdx.y + BLOCK*blockIdx.y-BLOCK/2;
    unsigned int i=threadIdx.x, j=threadIdx.y;
    __shared__ BASE in[BLOCK][2*BLOCK];
    int k, l; BASE max2;

    in[i][j+BLOCK]=((ig<V)&&(jg>=0)&&(jg<H))?A[ig*H+jg]:0;
    __syncthreads();

    for(k=0;k<ceilf(1.0*H/BLOCK);k++)
    {  jg=jg+BLOCK;
       in[i][j]=in[i][j+BLOCK];
       in[i][j+BLOCK]=((ig<V)&&(jg<H))?A[ig*H+jg]:0;
       __syncthreads();
       if((ig<V)&&(jg-BLOCK/2<H)) { max2=in[i][j+BLOCK/2];
          for(l=-7;l<=7;l++) max2=max(max2,in[i][j+BLOCK/2+l]);
       out[ig*H+jg-BLOCK/2]= (BASE) max2; } 
       __syncthreads(); }
}

__global__ void dilate_st15_cols_win_gpu(BASE A[H*V], BASE out[H*V])
{   int ig = threadIdx.x + BLOCK*blockIdx.x-BLOCK/2;
    int jg = threadIdx.y + BLOCK*blockIdx.y;
    unsigned int i=threadIdx.x, j=threadIdx.y;
    __shared__ BASE in[2*BLOCK][BLOCK];
    int k, l; BASE max2;

    in[i+BLOCK][j]=((ig>=0)&&(ig<V)&&(jg<H))?A[ig*H+jg]:0;
    __syncthreads();

    for(k=0;k<ceilf(1.0*V/BLOCK);k++)
    {  ig=ig+BLOCK;
       in[i][j]=in[i+BLOCK][j];
       in[i+BLOCK][j]=((ig<V)&&(jg<H))?A[ig*H+jg]:0;
       __syncthreads();
       if((ig-BLOCK/2<V)&&(jg<H)) { max2=in[i+BLOCK/2][j];
          for(l=-7;l<=7;l++) max2=max(max2,in[i+BLOCK/2+l][j]);
       out[(ig-BLOCK/2)*H+jg]= (BASE) max2; } 
       __syncthreads(); }
}

__global__ void erosion_st64_filas_win_gpu(BASE A[H*V], BASE out[H*V])
{   int ig = threadIdx.x + BLOCKa*blockIdx.x;
    int jg = threadIdx.y + BLOCKb*blockIdx.y-BLOCKb/2;
    unsigned int i=threadIdx.x, j=threadIdx.y;
    __shared__ BASE in[BLOCKa][2*BLOCKb];
    int k, l; BASE min2;

    in[i][j+BLOCKb]=((ig<V)&&(jg>=0)&&(jg<H))?A[ig*H+jg]:255;
    __syncthreads();

    for(k=0;k<ceilf(1.0*H/BLOCKb);k++)
    {  jg=jg+BLOCKb;
       in[i][j]=in[i][j+BLOCKb];
       in[i][j+BLOCKb]=((ig<V)&&(jg<H))?A[ig*H+jg]:255;
       __syncthreads();
       if((ig<V)&&(jg-BLOCKb/2<H)) { min2=in[i][j+BLOCKb/2];
          for(l=-64;l<=64;l++) min2=min(min2,in[i][j+BLOCKb/2+l]);
       out[ig*H+jg-BLOCKb/2]= (BASE) min2; } 
       __syncthreads(); }
}

__global__ void erosion_st64_cols_win_gpu(BASE A[H*V], BASE out[H*V])
{   int ig = threadIdx.x + BLOCKb*blockIdx.x-BLOCKb/2;
    int jg = threadIdx.y + BLOCKa*blockIdx.y;
    unsigned int i=threadIdx.x, j=threadIdx.y;
    __shared__ BASE in[2*BLOCKb][BLOCKa];
    int k, l; BASE min2;

    in[i+BLOCKb][j]=((ig>=0)&&(ig<V)&&(jg<H))?A[ig*H+jg]:255;
    __syncthreads();

    for(k=0;k<ceilf(1.0*V/BLOCKb);k++)
    {  ig=ig+BLOCKb;
       in[i][j]=in[i+BLOCKb][j];
       in[i+BLOCKb][j]=((ig<V)&&(jg<H))?A[ig*H+jg]:255;
       __syncthreads();
       if((ig-BLOCKb/2<V)&&(jg<H)) { min2=in[i+BLOCKb/2][j];
          for(l=-64;l<=64;l++) min2=min(min2,in[i+BLOCKb/2+l][j]);
       out[(ig-BLOCKb/2)*H+jg]= (BASE) min2; } 
       __syncthreads(); }
}

__global__ void dilate_st64_filas_win_gpu(BASE A[H*V], BASE out[H*V])
{   int ig = threadIdx.x + BLOCKa*blockIdx.x;
    int jg = threadIdx.y + BLOCKb*blockIdx.y-BLOCKb/2;
    unsigned int i=threadIdx.x, j=threadIdx.y;
    __shared__ BASE in[BLOCKa][2*BLOCKb];
    int k, l; BASE max2;

    in[i][j+BLOCKb]=((ig<V)&&(jg>=0)&&(jg<H))?A[ig*H+jg]:0;
    __syncthreads();

    for(k=0;k<ceilf(1.0*H/BLOCKb);k++)
    {  jg=jg+BLOCKb;
       in[i][j]=in[i][j+BLOCKb];
       in[i][j+BLOCKb]=((ig<V)&&(jg<H))?A[ig*H+jg]:0;
       __syncthreads();
       if((ig<V)&&(jg-BLOCKb/2<H)) { max2=in[i][j+BLOCKb/2];
          for(l=-64;l<=64;l++) max2=max(max2,in[i][j+BLOCKb/2+l]);
       out[ig*H+jg-BLOCKb/2]= (BASE) max2; } 
       __syncthreads(); }
}

__global__ void dilate_st64_cols_win_gpu(BASE A[H*V], BASE out[H*V])
{   int ig = threadIdx.x + BLOCKb*blockIdx.x-BLOCKb/2;
    int jg = threadIdx.y + BLOCKa*blockIdx.y;
    unsigned int i=threadIdx.x, j=threadIdx.y;
    __shared__ BASE in[2*BLOCKb][BLOCKa];
    int k, l; BASE max2;

    in[i+BLOCKb][j]=((ig>=0)&&(ig<V)&&(jg<H))?A[ig*H+jg]:0;
    __syncthreads();

    for(k=0;k<ceilf(1.0*V/BLOCKb);k++)
    {  ig=ig+BLOCKb;
       in[i][j]=in[i+BLOCKb][j];
       in[i+BLOCKb][j]=((ig<V)&&(jg<H))?A[ig*H+jg]:0;
       __syncthreads();
       if((ig-BLOCKb/2<V)&&(jg<H)) { max2=in[i+BLOCKb/2][j];
          for(l=-64;l<=64;l++) max2=max(max2,in[i+BLOCKb/2+l][j]);
       out[(ig-BLOCKb/2)*H+jg]= (BASE) max2; } 
       __syncthreads(); }
}

__global__ void erosion_disk_st15_gpu(BASE A[H*V], BASE out[H*V])
{   int ig = threadIdx.x + BLOCK*blockIdx.x;
    int jg = threadIdx.y + BLOCK*blockIdx.y;
    unsigned int i=threadIdx.x, j=threadIdx.y; int a,b;
    __shared__ BASE in[2*BLOCK][2*BLOCK];
    BASE min2;

    a=ig-BLOCK/2;if(a<0)a=0;else if(a>=V) a=V-1;
    b=jg-BLOCK/2;if(b<0)b=0;else if(b>=H) b=H-1;
    in[i][j]=A[a*H+b];
    a=ig-BLOCK/2;if(a<0)a=0;else if(a>=V) a=V-1;
    b=jg+BLOCK/2;if(b>=H)b=H-1;
    in[i][j+BLOCK]=A[a*H+b];
    a=ig+BLOCK/2;if(a>=V)a=V-1;
    b=jg-BLOCK/2;if(b<0)b=0;else if(b>=H)b=H-1;
    in[i+BLOCK][j]=A[a*H+b];
    a=ig+BLOCK/2;if(a>=V)a=V-1;
    b=jg+BLOCK/2;if(b>=H)b=H-1;
    in[i+BLOCK][j+BLOCK]=A[a*H+b];
    __syncthreads();

    if((ig<V)&&(jg<H)) 
    {  a=i+BLOCK/2; b=j+BLOCK/2;
       min2=in[a][b];
       min2=min(min2,in[a-7][b-3]);
       min2=min(min2,in[a-7][b-2]);
       min2=min(min2,in[a-7][b-1]);
       min2=min(min2,in[a-7][b+0]);
       min2=min(min2,in[a-7][b+1]);
       min2=min(min2,in[a-7][b+2]);
       min2=min(min2,in[a-7][b+3]);
       min2=min(min2,in[a-6][b-4]);
       min2=min(min2,in[a-6][b-3]);
       min2=min(min2,in[a-6][b-2]);
       min2=min(min2,in[a-6][b-1]);
       min2=min(min2,in[a-6][b+0]);
       min2=min(min2,in[a-6][b+1]);
       min2=min(min2,in[a-6][b+2]);
       min2=min(min2,in[a-6][b+3]);
       min2=min(min2,in[a-6][b+4]);
       min2=min(min2,in[a-5][b-5]);
       min2=min(min2,in[a-5][b-4]);
       min2=min(min2,in[a-5][b-3]);
       min2=min(min2,in[a-5][b-2]);
       min2=min(min2,in[a-5][b-1]);
       min2=min(min2,in[a-5][b+0]);
       min2=min(min2,in[a-5][b+1]);
       min2=min(min2,in[a-5][b+2]);
       min2=min(min2,in[a-5][b+3]);
       min2=min(min2,in[a-5][b+4]);
       min2=min(min2,in[a-5][b+5]);
       min2=min(min2,in[a-4][b-6]);
       min2=min(min2,in[a-4][b-5]);
       min2=min(min2,in[a-4][b-4]);
       min2=min(min2,in[a-4][b-3]);
       min2=min(min2,in[a-4][b-2]);
       min2=min(min2,in[a-4][b-1]);
       min2=min(min2,in[a-4][b+0]);
       min2=min(min2,in[a-4][b+1]);
       min2=min(min2,in[a-4][b+2]);
       min2=min(min2,in[a-4][b+3]);
       min2=min(min2,in[a-4][b+4]);
       min2=min(min2,in[a-4][b+5]);
       min2=min(min2,in[a-4][b+6]);
       min2=min(min2,in[a-3][b-7]);
       min2=min(min2,in[a-3][b-6]);
       min2=min(min2,in[a-3][b-5]);
       min2=min(min2,in[a-3][b-4]);
       min2=min(min2,in[a-3][b-3]);
       min2=min(min2,in[a-3][b-2]);
       min2=min(min2,in[a-3][b-1]);
       min2=min(min2,in[a-3][b+0]);
       min2=min(min2,in[a-3][b+1]);
       min2=min(min2,in[a-3][b+2]);
       min2=min(min2,in[a-3][b+3]);
       min2=min(min2,in[a-3][b+4]);
       min2=min(min2,in[a-3][b+5]);
       min2=min(min2,in[a-3][b+6]);
       min2=min(min2,in[a-3][b+7]);
       min2=min(min2,in[a-2][b-7]);
       min2=min(min2,in[a-2][b-6]);
       min2=min(min2,in[a-2][b-5]);
       min2=min(min2,in[a-2][b-4]);
       min2=min(min2,in[a-2][b-3]);
       min2=min(min2,in[a-2][b-2]);
       min2=min(min2,in[a-2][b-1]);
       min2=min(min2,in[a-2][b+0]);
       min2=min(min2,in[a-2][b+1]);
       min2=min(min2,in[a-2][b+2]);
       min2=min(min2,in[a-2][b+3]);
       min2=min(min2,in[a-2][b+4]);
       min2=min(min2,in[a-2][b+5]);
       min2=min(min2,in[a-2][b+6]);
       min2=min(min2,in[a-2][b+7]);
       min2=min(min2,in[a-1][b-7]);
       min2=min(min2,in[a-1][b-6]);
       min2=min(min2,in[a-1][b-5]);
       min2=min(min2,in[a-1][b-4]);
       min2=min(min2,in[a-1][b-3]);
       min2=min(min2,in[a-1][b-2]);
       min2=min(min2,in[a-1][b-1]);
       min2=min(min2,in[a-1][b+0]);
       min2=min(min2,in[a-1][b+1]);
       min2=min(min2,in[a-1][b+2]);
       min2=min(min2,in[a-1][b+3]);
       min2=min(min2,in[a-1][b+4]);
       min2=min(min2,in[a-1][b+5]);
       min2=min(min2,in[a-1][b+6]);
       min2=min(min2,in[a-1][b+7]);
       min2=min(min2,in[a+0][b-7]);
       min2=min(min2,in[a+0][b-6]);
       min2=min(min2,in[a+0][b-5]);
       min2=min(min2,in[a+0][b-4]);
       min2=min(min2,in[a+0][b-3]);
       min2=min(min2,in[a+0][b-2]);
       min2=min(min2,in[a+0][b-1]);
       min2=min(min2,in[a+0][b+1]);
       min2=min(min2,in[a+0][b+2]);
       min2=min(min2,in[a+0][b+3]);
       min2=min(min2,in[a+0][b+4]);
       min2=min(min2,in[a+0][b+5]);
       min2=min(min2,in[a+0][b+6]);
       min2=min(min2,in[a+0][b+7]);
       min2=min(min2,in[a+1][b-7]);
       min2=min(min2,in[a+1][b-6]);
       min2=min(min2,in[a+1][b-5]);
       min2=min(min2,in[a+1][b-4]);
       min2=min(min2,in[a+1][b-3]);
       min2=min(min2,in[a+1][b-2]);
       min2=min(min2,in[a+1][b-1]);
       min2=min(min2,in[a+1][b+0]);
       min2=min(min2,in[a+1][b+1]);
       min2=min(min2,in[a+1][b+2]);
       min2=min(min2,in[a+1][b+3]);
       min2=min(min2,in[a+1][b+4]);
       min2=min(min2,in[a+1][b+5]);
       min2=min(min2,in[a+1][b+6]);
       min2=min(min2,in[a+1][b+7]);
       min2=min(min2,in[a+2][b-7]);
       min2=min(min2,in[a+2][b-6]);
       min2=min(min2,in[a+2][b-5]);
       min2=min(min2,in[a+2][b-4]);
       min2=min(min2,in[a+2][b-3]);
       min2=min(min2,in[a+2][b-2]);
       min2=min(min2,in[a+2][b-1]);
       min2=min(min2,in[a+2][b+0]);
       min2=min(min2,in[a+2][b+1]);
       min2=min(min2,in[a+2][b+2]);
       min2=min(min2,in[a+2][b+3]);
       min2=min(min2,in[a+2][b+4]);
       min2=min(min2,in[a+2][b+5]);
       min2=min(min2,in[a+2][b+6]);
       min2=min(min2,in[a+2][b+7]);
       min2=min(min2,in[a+3][b-7]);
       min2=min(min2,in[a+3][b-6]);
       min2=min(min2,in[a+3][b-5]);
       min2=min(min2,in[a+3][b-4]);
       min2=min(min2,in[a+3][b-3]);
       min2=min(min2,in[a+3][b-2]);
       min2=min(min2,in[a+3][b-1]);
       min2=min(min2,in[a+3][b+0]);
       min2=min(min2,in[a+3][b+1]);
       min2=min(min2,in[a+3][b+2]);
       min2=min(min2,in[a+3][b+3]);
       min2=min(min2,in[a+3][b+4]);
       min2=min(min2,in[a+3][b+5]);
       min2=min(min2,in[a+3][b+6]);
       min2=min(min2,in[a+3][b+7]);
       min2=min(min2,in[a+4][b-6]);
       min2=min(min2,in[a+4][b-5]);
       min2=min(min2,in[a+4][b-4]);
       min2=min(min2,in[a+4][b-3]);
       min2=min(min2,in[a+4][b-2]);
       min2=min(min2,in[a+4][b-1]);
       min2=min(min2,in[a+4][b+0]);
       min2=min(min2,in[a+4][b+1]);
       min2=min(min2,in[a+4][b+2]);
       min2=min(min2,in[a+4][b+3]);
       min2=min(min2,in[a+4][b+4]);
       min2=min(min2,in[a+4][b+5]);
       min2=min(min2,in[a+4][b+6]);
       min2=min(min2,in[a+5][b-5]);
       min2=min(min2,in[a+5][b-4]);
       min2=min(min2,in[a+5][b-3]);
       min2=min(min2,in[a+5][b-2]);
       min2=min(min2,in[a+5][b-1]);
       min2=min(min2,in[a+5][b+0]);
       min2=min(min2,in[a+5][b+1]);
       min2=min(min2,in[a+5][b+2]);
       min2=min(min2,in[a+5][b+3]);
       min2=min(min2,in[a+5][b+4]);
       min2=min(min2,in[a+5][b+5]);
       min2=min(min2,in[a+6][b-4]);
       min2=min(min2,in[a+6][b-3]);
       min2=min(min2,in[a+6][b-2]);
       min2=min(min2,in[a+6][b-1]);
       min2=min(min2,in[a+6][b+0]);
       min2=min(min2,in[a+6][b+1]);
       min2=min(min2,in[a+6][b+2]);
       min2=min(min2,in[a+6][b+3]);
       min2=min(min2,in[a+6][b+4]);
       min2=min(min2,in[a+7][b-3]);
       min2=min(min2,in[a+7][b-2]);
       min2=min(min2,in[a+7][b-1]);
       min2=min(min2,in[a+7][b+0]);
       min2=min(min2,in[a+7][b+1]);
       min2=min(min2,in[a+7][b+2]);
       min2=min(min2,in[a+7][b+3]);    
       out[ig*H+jg]=min2; }
}

__global__ void dilate_disk_st15_gpu(BASE A[H*V], BASE out[H*V])
{   int ig = threadIdx.x + BLOCK*blockIdx.x;
    int jg = threadIdx.y + BLOCK*blockIdx.y;
    unsigned int i=threadIdx.x, j=threadIdx.y; int a,b; 
    __shared__ BASE in[2*BLOCK][2*BLOCK];
    BASE max2;

    a=ig-BLOCK/2;if(a<0)a=0;else if(a>=V) a=V-1;
    b=jg-BLOCK/2;if(b<0)b=0;else if(b>=H) b=H-1;
    in[i][j]=A[a*H+b];
    a=ig-BLOCK/2;if(a<0)a=0;else if(a>=V) a=V-1;
    b=jg+BLOCK/2;if(b>=H)b=H-1;
    in[i][j+BLOCK]=A[a*H+b];
    a=ig+BLOCK/2;if(a>=V)a=V-1;
    b=jg-BLOCK/2;if(b<0)b=0;else if(b>=H)b=H-1;
    in[i+BLOCK][j]=A[a*H+b];
    a=ig+BLOCK/2;if(a>=V)a=V-1;
    b=jg+BLOCK/2;if(b>=H)b=H-1;
    in[i+BLOCK][j+BLOCK]=A[a*H+b];
    __syncthreads();

    if((ig<V)&&(jg<H)) 
    {  a=i+BLOCK/2; b=j+BLOCK/2;
       max2=in[a][b];
       max2=max(max2,in[a-7][b-3]);
       max2=max(max2,in[a-7][b-2]);
       max2=max(max2,in[a-7][b-1]);
       max2=max(max2,in[a-7][b+0]);
       max2=max(max2,in[a-7][b+1]);
       max2=max(max2,in[a-7][b+2]);
       max2=max(max2,in[a-7][b+3]);
       max2=max(max2,in[a-6][b-4]);
       max2=max(max2,in[a-6][b-3]);
       max2=max(max2,in[a-6][b-2]);
       max2=max(max2,in[a-6][b-1]);
       max2=max(max2,in[a-6][b+0]);
       max2=max(max2,in[a-6][b+1]);
       max2=max(max2,in[a-6][b+2]);
       max2=max(max2,in[a-6][b+3]);
       max2=max(max2,in[a-6][b+4]);
       max2=max(max2,in[a-5][b-5]);
       max2=max(max2,in[a-5][b-4]);
       max2=max(max2,in[a-5][b-3]);
       max2=max(max2,in[a-5][b-2]);
       max2=max(max2,in[a-5][b-1]);
       max2=max(max2,in[a-5][b+0]);
       max2=max(max2,in[a-5][b+1]);
       max2=max(max2,in[a-5][b+2]);
       max2=max(max2,in[a-5][b+3]);
       max2=max(max2,in[a-5][b+4]);
       max2=max(max2,in[a-5][b+5]);
       max2=max(max2,in[a-4][b-6]);
       max2=max(max2,in[a-4][b-5]);
       max2=max(max2,in[a-4][b-4]);
       max2=max(max2,in[a-4][b-3]);
       max2=max(max2,in[a-4][b-2]);
       max2=max(max2,in[a-4][b-1]);
       max2=max(max2,in[a-4][b+0]);
       max2=max(max2,in[a-4][b+1]);
       max2=max(max2,in[a-4][b+2]);
       max2=max(max2,in[a-4][b+3]);
       max2=max(max2,in[a-4][b+4]);
       max2=max(max2,in[a-4][b+5]);
       max2=max(max2,in[a-4][b+6]);
       max2=max(max2,in[a-3][b-7]);
       max2=max(max2,in[a-3][b-6]);
       max2=max(max2,in[a-3][b-5]);
       max2=max(max2,in[a-3][b-4]);
       max2=max(max2,in[a-3][b-3]);
       max2=max(max2,in[a-3][b-2]);
       max2=max(max2,in[a-3][b-1]);
       max2=max(max2,in[a-3][b+0]);
       max2=max(max2,in[a-3][b+1]);
       max2=max(max2,in[a-3][b+2]);
       max2=max(max2,in[a-3][b+3]);
       max2=max(max2,in[a-3][b+4]);
       max2=max(max2,in[a-3][b+5]);
       max2=max(max2,in[a-3][b+6]);
       max2=max(max2,in[a-3][b+7]);
       max2=max(max2,in[a-2][b-7]);
       max2=max(max2,in[a-2][b-6]);
       max2=max(max2,in[a-2][b-5]);
       max2=max(max2,in[a-2][b-4]);
       max2=max(max2,in[a-2][b-3]);
       max2=max(max2,in[a-2][b-2]);
       max2=max(max2,in[a-2][b-1]);
       max2=max(max2,in[a-2][b+0]);
       max2=max(max2,in[a-2][b+1]);
       max2=max(max2,in[a-2][b+2]);
       max2=max(max2,in[a-2][b+3]);
       max2=max(max2,in[a-2][b+4]);
       max2=max(max2,in[a-2][b+5]);
       max2=max(max2,in[a-2][b+6]);
       max2=max(max2,in[a-2][b+7]);
       max2=max(max2,in[a-1][b-7]);
       max2=max(max2,in[a-1][b-6]);
       max2=max(max2,in[a-1][b-5]);
       max2=max(max2,in[a-1][b-4]);
       max2=max(max2,in[a-1][b-3]);
       max2=max(max2,in[a-1][b-2]);
       max2=max(max2,in[a-1][b-1]);
       max2=max(max2,in[a-1][b+0]);
       max2=max(max2,in[a-1][b+1]);
       max2=max(max2,in[a-1][b+2]);
       max2=max(max2,in[a-1][b+3]);
       max2=max(max2,in[a-1][b+4]);
       max2=max(max2,in[a-1][b+5]);
       max2=max(max2,in[a-1][b+6]);
       max2=max(max2,in[a-1][b+7]);
       max2=max(max2,in[a+0][b-7]);
       max2=max(max2,in[a+0][b-6]);
       max2=max(max2,in[a+0][b-5]);
       max2=max(max2,in[a+0][b-4]);
       max2=max(max2,in[a+0][b-3]);
       max2=max(max2,in[a+0][b-2]);
       max2=max(max2,in[a+0][b-1]);
       max2=max(max2,in[a+0][b+1]);
       max2=max(max2,in[a+0][b+2]);
       max2=max(max2,in[a+0][b+3]);
       max2=max(max2,in[a+0][b+4]);
       max2=max(max2,in[a+0][b+5]);
       max2=max(max2,in[a+0][b+6]);
       max2=max(max2,in[a+0][b+7]);
       max2=max(max2,in[a+1][b-7]);
       max2=max(max2,in[a+1][b-6]);
       max2=max(max2,in[a+1][b-5]);
       max2=max(max2,in[a+1][b-4]);
       max2=max(max2,in[a+1][b-3]);
       max2=max(max2,in[a+1][b-2]);
       max2=max(max2,in[a+1][b-1]);
       max2=max(max2,in[a+1][b+0]);
       max2=max(max2,in[a+1][b+1]);
       max2=max(max2,in[a+1][b+2]);
       max2=max(max2,in[a+1][b+3]);
       max2=max(max2,in[a+1][b+4]);
       max2=max(max2,in[a+1][b+5]);
       max2=max(max2,in[a+1][b+6]);
       max2=max(max2,in[a+1][b+7]);
       max2=max(max2,in[a+2][b-7]);
       max2=max(max2,in[a+2][b-6]);
       max2=max(max2,in[a+2][b-5]);
       max2=max(max2,in[a+2][b-4]);
       max2=max(max2,in[a+2][b-3]);
       max2=max(max2,in[a+2][b-2]);
       max2=max(max2,in[a+2][b-1]);
       max2=max(max2,in[a+2][b+0]);
       max2=max(max2,in[a+2][b+1]);
       max2=max(max2,in[a+2][b+2]);
       max2=max(max2,in[a+2][b+3]);
       max2=max(max2,in[a+2][b+4]);
       max2=max(max2,in[a+2][b+5]);
       max2=max(max2,in[a+2][b+6]);
       max2=max(max2,in[a+2][b+7]);
       max2=max(max2,in[a+3][b-7]);
       max2=max(max2,in[a+3][b-6]);
       max2=max(max2,in[a+3][b-5]);
       max2=max(max2,in[a+3][b-4]);
       max2=max(max2,in[a+3][b-3]);
       max2=max(max2,in[a+3][b-2]);
       max2=max(max2,in[a+3][b-1]);
       max2=max(max2,in[a+3][b+0]);
       max2=max(max2,in[a+3][b+1]);
       max2=max(max2,in[a+3][b+2]);
       max2=max(max2,in[a+3][b+3]);
       max2=max(max2,in[a+3][b+4]);
       max2=max(max2,in[a+3][b+5]);
       max2=max(max2,in[a+3][b+6]);
       max2=max(max2,in[a+3][b+7]);
       max2=max(max2,in[a+4][b-6]);
       max2=max(max2,in[a+4][b-5]);
       max2=max(max2,in[a+4][b-4]);
       max2=max(max2,in[a+4][b-3]);
       max2=max(max2,in[a+4][b-2]);
       max2=max(max2,in[a+4][b-1]);
       max2=max(max2,in[a+4][b+0]);
       max2=max(max2,in[a+4][b+1]);
       max2=max(max2,in[a+4][b+2]);
       max2=max(max2,in[a+4][b+3]);
       max2=max(max2,in[a+4][b+4]);
       max2=max(max2,in[a+4][b+5]);
       max2=max(max2,in[a+4][b+6]);
       max2=max(max2,in[a+5][b-5]);
       max2=max(max2,in[a+5][b-4]);
       max2=max(max2,in[a+5][b-3]);
       max2=max(max2,in[a+5][b-2]);
       max2=max(max2,in[a+5][b-1]);
       max2=max(max2,in[a+5][b+0]);
       max2=max(max2,in[a+5][b+1]);
       max2=max(max2,in[a+5][b+2]);
       max2=max(max2,in[a+5][b+3]);
       max2=max(max2,in[a+5][b+4]);
       max2=max(max2,in[a+5][b+5]);
       max2=max(max2,in[a+6][b-4]);
       max2=max(max2,in[a+6][b-3]);
       max2=max(max2,in[a+6][b-2]);
       max2=max(max2,in[a+6][b-1]);
       max2=max(max2,in[a+6][b+0]);
       max2=max(max2,in[a+6][b+1]);
       max2=max(max2,in[a+6][b+2]);
       max2=max(max2,in[a+6][b+3]);
       max2=max(max2,in[a+6][b+4]);
       max2=max(max2,in[a+7][b-3]);
       max2=max(max2,in[a+7][b-2]);
       max2=max(max2,in[a+7][b-1]);
       max2=max(max2,in[a+7][b+0]);
       max2=max(max2,in[a+7][b+1]);
       max2=max(max2,in[a+7][b+2]);
       max2=max(max2,in[a+7][b+3]);    
       out[ig*H+jg]=max2; }
}

__global__ void kernel_copy_inv_char(BASE *in, BASE *out)
{ unsigned int i = threadIdx.x + BLOCK*BLOCK*blockIdx.x;
  if(i<H*V) out[i]=255-in[i];
}

__global__ void kernel_suma_float(BASE *in, BASE *in2, BASE *out)
{ unsigned int i = threadIdx.x + BLOCK*BLOCK*blockIdx.x;
  if(i<H*V) out[i]=(BASE)(in[i]+in2[i]);
}

// entrada=im, salida=tmp1
void tophat(BASE *im, BASE *tmp1, BASE *tmp2)
{  if(TOPHAT==0) 
   {  cudaMemcpy(tmp1,im,H*V*sizeof(BASE),cudaMemcpyDeviceToDevice); return;  }
   kernel_copy_inv_char<<<dimGrid1,dimBlock1>>>(im,tmp2);  
   if(TOPHAT==1)
   {  erosion_disk_st15_gpu<<<dimGrid,dimBlock>>>(tmp2,tmp1);
      dilate_disk_st15_gpu<<<dimGrid,dimBlock>>>(tmp1,tmp2); }
   else if(TOPHAT==2)
   {  erosion_st15_filas_win_gpu<<<dimGrid51,dimBlock>>>(tmp2,tmp1);
      erosion_st15_cols_win_gpu<<<dimGrid52,dimBlock>>>(tmp1,tmp2);
      dilate_st15_filas_win_gpu<<<dimGrid51,dimBlock>>>(tmp2,tmp1);
      dilate_st15_cols_win_gpu<<<dimGrid52,dimBlock>>>(tmp1,tmp2);  }
   else if(TOPHAT==3)
   {  erosion_st64_filas_win_gpu<<<dimGrid51a,dimBlocka>>>(tmp2,tmp1);
      erosion_st64_cols_win_gpu<<<dimGrid52a,dimBlockb>>>(tmp1,tmp2);
      dilate_st64_filas_win_gpu<<<dimGrid51a,dimBlocka>>>(tmp2,tmp1);
      dilate_st64_cols_win_gpu<<<dimGrid52a,dimBlockb>>>(tmp1,tmp2);  }
   kernel_suma_float<<<dimGrid1,dimBlock1>>>(im,tmp2,tmp1);  
}

// ------------------------------------------------------------------------
// PLS
// ------------------------------------------------------------------------
float s_cpd=+1; // s_cpd=-1 => disables the collision point detection
float k_ext=1;  // weight ext. potential (k_ext > 0 => evol. to 2QCNN black)
float k_int=1;  // weight the internal potential (k_int>0)
float k_inf=-1; // weight balloon potential: deflating (>0), inflating (<0)

__global__ void kernel1(float *int_pot, BASE *tmp1, float k)
{ unsigned int i = threadIdx.x + BLOCK*BLOCK*blockIdx.x;
  if(i<H*V) int_pot[i]=k*tmp1[i]; 
}

__global__ void kernel2(BASE *output, BASE *tmp1, BASE *output_GFE)
{ unsigned int i = threadIdx.x + BLOCK*BLOCK*blockIdx.x;
  if(i<H*V) output[i] = output[i]|(tmp1[i]&output_GFE[i]);
}

__global__ void kernel3(BASE *output, BASE *contour)
{ unsigned int i = threadIdx.x + BLOCK*BLOCK*blockIdx.x;
  if(i<H*V) output[i]=(1-output[i])|contour[i];
}

__global__ void kernel_inv_bin(BASE *in, float *out)
{ unsigned int i = threadIdx.x + BLOCK*BLOCK*blockIdx.x;
  if(i<H*V) out[i]=1-in[i];
}

__global__ void kernel_add_pot_inv(BASE *edge2, float max2, float *imd2, float kext, float *ext_pot)
{ unsigned int i = threadIdx.x + BLOCK*BLOCK*blockIdx.x;
  if(i<H*V) ext_pot[i]=kext*(100-(50*edge2[i]+50*(imd2[i]/max2)));
}

__global__ void kernel_distance_map(float *in, float *out, int k)
{ unsigned int i = threadIdx.x + BLOCK*BLOCK*blockIdx.x;
  if(i<H*V) out[i] += in[i]/k;
}

__global__ void f_CPDv3_gpu(const char dir1, BASE *output, BASE *mask_CPD, BASE boundary)
{  unsigned int i=threadIdx.x, j=threadIdx.y;
   int ig = threadIdx.x + (BLOCK-2)*blockIdx.x-1;
   int jg = threadIdx.y + (BLOCK-2)*blockIdx.y-1;
   __shared__ BASE tmp1[BLOCK*BLOCK], tmp2[BLOCK*BLOCK], tmp3[BLOCK*BLOCK];

   if((ig>=0)&&(ig<V)&&(jg>=0)&&(jg<H)) tmp2[i*BLOCK+j]=output[ig*H+jg];
   else tmp2[i*BLOCK+j]=boundary;
   __syncthreads();
  
   if(dir1=='N')
   {  if(i>0)tmp1[i*BLOCK+j]=tmp2[(i-1)*BLOCK+j]; else tmp1[i*BLOCK+j]=boundary;
      tmp3[i*BLOCK+j]=(1-tmp2[i*BLOCK+j])&tmp1[i*BLOCK+j];
      __syncthreads();
      if(j>0) tmp1[i*BLOCK+j]=tmp3[i*BLOCK+(j-1)]; else tmp1[i*BLOCK+j]=boundary;
      __syncthreads();
      if(j<BLOCK-1) tmp2[i*BLOCK+j]=tmp3[i*BLOCK+(j+1)]; else tmp2[i*BLOCK+j]=boundary; 
     __syncthreads(); }
   else if(dir1=='S')
   {  if(i<BLOCK-1) tmp1[i*BLOCK+j]=tmp2[(i+1)*BLOCK+j]; else tmp1[i*BLOCK+j]=boundary;
      tmp3[i*BLOCK+j]=(1-tmp2[i*BLOCK+j])&tmp1[i*BLOCK+j];
      __syncthreads();
      if(j>0) tmp1[i*BLOCK+j]=tmp3[i*BLOCK+(j-1)]; else tmp1[i*BLOCK+j]=boundary;
      __syncthreads();
      if(j<BLOCK-1) tmp2[i*BLOCK+j]=tmp3[i*BLOCK+(j+1)]; else tmp2[i*BLOCK+j]=boundary; 
      __syncthreads(); }
   else if(dir1=='E')
   {  if(j<BLOCK-1) tmp1[i*BLOCK+j]=tmp2[i*BLOCK+(j+1)]; else tmp1[i*BLOCK+j]=boundary;
      tmp3[i*BLOCK+j]=(1-tmp2[i*BLOCK+j])&tmp1[i*BLOCK+j];
      __syncthreads();
      if(i<BLOCK-1) tmp1[i*BLOCK+j]=tmp3[(i+1)*BLOCK+j]; else tmp1[i*BLOCK+j]=boundary;
      __syncthreads();
      if(i>0) tmp2[i*BLOCK+j]=tmp3[(i-1)*BLOCK+j]; else tmp2[i*BLOCK+j]=boundary;
      __syncthreads(); }
   else if(dir1=='W')
  {   if(j>0) tmp1[i*BLOCK+j]=tmp2[i*BLOCK+(j-1)]; else tmp1[i*BLOCK+j]=boundary;
      tmp3[i*BLOCK+j]=(1-tmp2[i*BLOCK+j])&tmp1[i*BLOCK+j];
      __syncthreads();
      if(i<BLOCK-1) tmp1[i*BLOCK+j]=tmp3[(i+1)*BLOCK+j]; else tmp1[i*BLOCK+j]=boundary;
      __syncthreads();
      if(i>0) tmp2[i*BLOCK+j]=tmp3[(i-1)*BLOCK+j]; else tmp2[i*BLOCK+j]=boundary;
      __syncthreads(); }

   if((i>0)&&(i<BLOCK-1)&&(j>0)&&(j<BLOCK-1)&&(ig<V)&&(jg<H)) 
      mask_CPD[ig*H+jg]=tmp3[i*BLOCK+j]|tmp1[i*BLOCK+j]|tmp2[i*BLOCK+j];
}

__global__ void f_GFEv3_gpu(const char dir, float *ext_pot, float *int_pot, float kinf, BASE *inf_pot, BASE *mask_CPD, BASE *output_GFE, float boundary)
{  unsigned int i=threadIdx.x, j=threadIdx.y;
   int ig = threadIdx.x + (BLOCK-2)*blockIdx.x-1;
   int jg = threadIdx.y + (BLOCK-2)*blockIdx.y-1;
   __shared__ float aux1[BLOCK*BLOCK], aux2[BLOCK*BLOCK];
   
   if((ig>=0)&&(ig<V)&&(jg>=0)&&(jg<H)) 
        aux1[i*BLOCK+j]=ext_pot[ig*H+jg]+int_pot[ig*H+jg]+kinf*inf_pot[ig*H+jg]; 
   else aux1[i*BLOCK+j]=boundary;
   __syncthreads();
  
   if     ((dir=='S')&&(i>0))       aux2[i*BLOCK+j]=aux1[(i-1)*BLOCK+j];
   else if((dir=='W')&&(j<BLOCK-1)) aux2[i*BLOCK+j]=aux1[i*BLOCK+(j+1)];
   else if((dir=='N')&&(i<BLOCK-1)) aux2[i*BLOCK+j]=aux1[(i+1)*BLOCK+j];
   else if((dir=='E')&&(j>0))       aux2[i*BLOCK+j]=aux1[i*BLOCK+(j-1)];
   else aux2[i*BLOCK+j]=boundary; 

   if((i>0)&&(i<BLOCK-1)&&(j>0)&&(j<BLOCK-1)&&(ig<V)&&(jg<H)) 
   {  if((mask_CPD[ig*H+jg]==0)&&(aux1[i*BLOCK+j]-aux2[i*BLOCK+j]>=0.001)) output_GFE[ig*H+jg]=1; 
      else output_GFE[ig*H+jg]=0;  }
}

void pls(BASE *input, BASE *output, BASE *tmp1, BASE *mask_CPD, BASE *output_GFE, BASE *contour, float *ext_pot, float *int_pot, float *aux1, float *aux2, int iter)
{  int k, contador; BASE dir[4]={'N','E','S','W'}; char sentido;

   for(contador=0;contador<iter;contador++) 
   {  cudaMemcpy(input,output,H*V*sizeof(BASE),cudaMemcpyDeviceToDevice);
      
      for(sentido=+1;sentido>=-1;sentido=sentido-2)
      { f_edge_shared_gpu<<<dimGrid4,dimBlock4>>>(output,tmp1); 
        kernel1<<<dimGrid1,dimBlock1>>>(int_pot,tmp1,k_int);

        for(k=0;k<4;k++)
        {  if(s_cpd==-1) cudaMemset(mask_CPD,0,H*V*sizeof(BASE));
           else {
             f_CPDv3_gpu<<<dimGrid4,dimBlock4>>>(dir[k], output, mask_CPD, 0); }
	     f_GFEv3_gpu<<<dimGrid4,dimBlock4>>>(dir[k],ext_pot,int_pot,sentido*k_inf,output,
	                                       mask_CPD,output_GFE,0);
           f_shift_global_gpu<<<dimGrid,dimBlock>>>(dir[k],output,tmp1,0);
	   kernel2<<<dimGrid1,dimBlock1>>>(output,tmp1,output_GFE);
        }
        f_edge_shared_gpu<<<dimGrid4,dimBlock4>>>(output,contour);
        kernel3<<<dimGrid1,dimBlock1>>>(output,contour);
      }
   }
}

int main()
{  pid_t f; int nimg; char fichero[60];
   collect_images(); 

   // LOAD IMAGES TO DISK CACHE OF LINUX
   f=fork();
   if(f==0) { for(nimg=0;nimg<NIMG;nimg++)
   {  sprintf(fichero,PREFIX,name[nimg]);
      read_image_ppm_cache(fichero); }
   } 
   else {  
   // COMPUTE SEGMENTATIONS
   BASE *img_cpu, *tmp_cpu;
   int k, nimg; float max2; struct timeval start, end, ini, fin;

   img_cpu=(BASE *)malloc(H*V*sizeof(BASE));
   tmp_cpu=(BASE *)malloc(H*V*sizeof(BASE));
   if((img_cpu==NULL)||(img_cpu==NULL)) { printf("no memoria\n"); exit(-1); }
  
   cuda_init_2_0();
   float *ext_pot, *int_pot, *aux1, *aux2;  // inf_pot es kinf*output 
   BASE *im, *input, *output, *mask_CPD, *output_GFE, *contour, *tmp1, *tmp2;
   cudaMalloc((void**)&im,H*V*sizeof(BASE));
   cudaMalloc((void**)&input,H*V*sizeof(BASE));
   cudaMalloc((void**)&output,H*V*sizeof(BASE));
   cudaMalloc((void**)&mask_CPD,H*V*sizeof(BASE));
   cudaMalloc((void**)&output_GFE,H*V*sizeof(BASE));
   cudaMalloc((void**)&contour,H*V*sizeof(BASE));
   cudaMalloc((void**)&tmp1,H*V*sizeof(BASE));
   cudaMalloc((void**)&tmp2,H*V*sizeof(BASE));
   cudaMalloc((void**)&ext_pot,H*V*sizeof(float));
   cudaMalloc((void**)&int_pot,H*V*sizeof(float));
   cudaMalloc((void**)&aux1,H*V*sizeof(float));
   cudaMalloc((void**)&aux2,H*V*sizeof(float));
   // reaprovechamos las variables para ahorrar memoria
   BASE *imt, *edge2; float *imd2;
   imt=mask_CPD; edge2=contour; imd2=int_pot;
 
   for(nimg=0;nimg<NIMG;nimg++)
   { // load data
     gettimeofday(&ini,NULL);
     printf("--------------------------------------\n");
     printf("* %s\n",name[nimg]); 
     sprintf(fichero,PREFIX,name[nimg]);
     read_image_ppm(fichero,img_cpu);
     cudaMemcpy(im,img_cpu,H*V*sizeof(BASE),cudaMemcpyHostToDevice);
     cudaThreadSynchronize(); gettimeofday(&start,NULL);

     // 1. Vessel pre-estimation
     tophat(im,tmp1,tmp2);
     diffusion_and_binarization(tmp1,imt,aux1,aux2,THRESHOLD);
     remove_outside_FOV_gpu<<<dimGrid1,dimBlock1>>>(imt,im,FOV_UMBRAL);
     remove_noise(imt,(int*)ext_pot,(int*)aux1,(int*)aux2,CONN,NOISE_SIZE);

     // 2. Initial contour estimation
     erosion_shared_gpu<<<dimGrid4,dimBlock4>>>(imt,output);
 
     // 3. External potential estimation
     // 3.1 Edge detection + logical operations
     sobel_edge_detect_shared_gpu<<<dimGrid4,dimBlock4>>>(imt,edge2);
     kernel_inv_bin<<<dimGrid1,dimBlock1>>>(edge2,imd2);
     // 3.2 Distance map
     for(k=1;k<=5;k++)
     {  erosion_float_shared_gpu<<<dimGrid4,dimBlock4>>>(imd2,aux2);
        kernel_distance_map<<<dimGrid1,dimBlock1>>>(aux2,imd2,k); }
     cudaMemcpy(aux1,imd2,H*V*sizeof(float),cudaMemcpyDeviceToDevice);
     filter_float_shared_gpu<<<dimGrid4,dimBlock4>>>(aux1,imd2,1);
     // max2=6.0;
     max3a<<<dimGrid1,dimBlock1>>>(imd2,aux1);
     max3b<<<dimGrid2,dimBlock2>>>(aux1);
     max3c<<<dimGrid3,dimBlock3>>>(aux1);
     cudaMemcpy(&max2,aux1,sizeof(float),cudaMemcpyDeviceToHost);
     kernel_add_pot_inv<<<dimGrid1,dimBlock1>>>(edge2,max2,imd2,k_ext,ext_pot);
 
     // 4. Contour evolution (PLS)
     pls(input,output,tmp1,mask_CPD,output_GFE,contour,ext_pot,int_pot,aux1,aux2,STAGES);
     
     // 5. Removes and postprocessings
     remove_outside_FOV_gpu<<<dimGrid1,dimBlock1>>>(output,im,FOV_UMBRAL); 
     remove_small_structures(output,(int*)ext_pot,(int*)aux1,(int*)aux2,CONN,SMALL_STRUC_SIZE);
     if(H>2000) postproc_big_images(output,tmp1);
     cudaThreadSynchronize(); gettimeofday(&end,NULL);
     cudaMemcpy(img_cpu,output,H*V*sizeof(BASE),cudaMemcpyDeviceToHost);

     // Save data
     cudaMemcpy(im,img_cpu,H*V*sizeof(BASE),cudaMemcpyHostToDevice);
     cudaThreadSynchronize(); 
     sprintf(fichero,"output/vessel_seg_gpu-%s.pgm",name[nimg]);
     save_image_pgm(fichero,img_cpu);
     cudaThreadSynchronize(); gettimeofday(&fin,NULL);
     t_tot[nimg]=(fin.tv_sec-ini.tv_sec)+(fin.tv_usec-ini.tv_usec)*1E-6;
     t_comp[nimg]=(end.tv_sec-start.tv_sec)+(end.tv_usec-start.tv_usec)*1E-6;
     compute_accuracies(img_cpu,tmp_cpu,nimg);
     printf(" t_comp=%f, t_total=%f\n",t_comp[nimg],t_tot[nimg]);
   }
   compute_averages();
   cudaFree(im); cudaFree(ext_pot); cudaFree(input); cudaFree(output);
   cudaFree(int_pot); cudaFree(mask_CPD); cudaFree(output_GFE);
   cudaFree(contour); cudaFree(tmp1); cudaFree(tmp2); cudaFree(aux1); cudaFree(aux2);
   free(img_cpu); free(tmp_cpu);
   cuda_error("Error in execution in GPU!!!"); 
   printf("Execution without exceptions in GPU\n");
   }
}

