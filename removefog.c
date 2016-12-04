#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "DataTypes.h"
#include "main.h"
void maxarray(float* start,int size)
   {
    int i;
    for(i=0;i<size;i++)
     if(*(start+i)<0.1)
         *(start+i)=0.1;
    
    }   
int main(int argc,char* argv[])
{
  char* output_file;
  char* input_file;
  if(argc!=3)
  {
   printf("Not enough or More arguments for execution in command-line, Try again by specifiying only input and output jpeg files");
   return 0;
  }
  else 
  {
    input_file=argv[1];
    output_file=argv[2];
  }
  int j,k,y,patch_size,numPixels;
  patch_size=3;
  float aerialperspective= 0.95; 
  ImageData i=im_read(input_file);
  float  red[i.pixels];
  float  green[i.pixels];
  float  blue[i.pixels];
  
  for(j=0;j<i.height;j++)
  {
    for(k=0;k<i.width;k++)
    {
      red[(j*i.width)+k]=(*(i.rdata+(j*i.width)+k));
      green[(j*i.width)+k]=(*(i.gdata+(j*i.width)+k));
      blue[(j*i.width)+k]=(*(i.bdata+(j*i.width)+k));
    }
  }
  float* J=makeDarkChannel(i,patch_size);
  numPixels= ceil(0.001*i.height*i.width);
  float* A=estimateA(i,J,numPixels);
  for(j=0;j<i.height;j++)
  {
    for(k=0;k<i.width;k++)
    {
      (*(i.rdata+(j*i.width)+k))=(*(i.rdata+(j*i.width)+k))/(*A);
      (*(i.gdata+(j*i.width)+k))=(*(i.gdata+(j*i.width)+k))/(*(A+1));
      (*(i.bdata+(j*i.width)+k))=(*(i.bdata+(j*i.width)+k))/(*(A+2));
    }
  }
  J=makeDarkChannel(i,patch_size);
  for(y=0;y<i.pixels;y++)
  {
    *(J+y)=1-aerialperspective*(*(J+y));
    
    if(*(J+y)>1)
      *(J+y)=1;
    else if(*(J+y)<0)
      *(J+y)=0;
  }
  J=refineTransmap(J,i.width,i.height);
 maxarray(J,i.pixels);
  for(j=0;j<i.height;j++)
  {
    for(k=0;k<i.width;k++)
    {
      (*(i.rdata+(j*i.width)+k))=1.3*((((red[(j*i.width)+k])-(*A))/(*(J+((j*i.width)+k))))+(*A));
      (*(i.gdata+(j*i.width)+k))=1.3*((((green[(j*i.width)+k])-(*(A+1)))/(*(J+((j*i.width)+k))))+(*(A+1)));
      (*(i.bdata+(j*i.width)+k))=1.3*((((blue[(j*i.width)+k])-(*(A+2)))/(*(J+((j*i.width)+k))))+(*(A+2)));
      
      if((*(i.rdata+(j*i.width)+k))<0)
        (*(i.rdata+(j*i.width)+k))=0;
       
      else if((*(i.rdata+(j*i.width)+k))>1)
        (*(i.rdata+(j*i.width)+k))=1;
       
      if((*(i.gdata+(j*i.width)+k))<0)
       (*(i.gdata+(j*i.width)+k))=0;
       
      if((*(i.gdata+(j*i.width)+k))>1)
       (*(i.gdata+(j*i.width)+k))=1;
       
      if((*(i.bdata+(j*i.width)+k))<0)
       (*(i.bdata+(j*i.width)+k))=0;
       
      else if((*(i.bdata+(j*i.width)+k))>1)
       (*(i.bdata+(j*i.width)+k))=1;
    }
  }
  im_write(i,output_file,100);
  free(A);
  free(J);
  free(i.rdata);
  free(i.gdata);
  free(i.bdata);
 return 0;
 }

