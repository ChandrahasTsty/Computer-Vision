#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "DataTypes.h"
float* gamma_transform(float c, float power,float* array, long int array_size)
{
  float* ret=(float*)calloc(array_size,sizeof(float));
  int k;
  for (k=0;k<array_size;k++)
   *(ret+k)=c*pow(*(array+k),power);
  return ret;
}

float median_filt(float* input)
{
 float buffer[9];
 int k,i,h;
 for(k=0;k<9;k++)
  buffer[k]=0;
 int  enter;
 for(k=0;k<9;k++)
 {
  enter=1;
  for(i=k-1;i>=0;i--)
  {
   if((enter==1)&&(*(input+k)>buffer[i]))
    {
     if(i==k-1)
     {
      buffer[k]=*(input+k);
      enter=0;
     }
     else
     {
     for(h=k-1;h>=(i+1);h--)
      buffer[h+1]=buffer[h];
     buffer[i+1]=*(input+k);
     enter=0;
     }
    }
  }
  if(enter==1)
  {
   for(h=k-1;h>=0;h--)
      buffer[h+1]=buffer[h];
   buffer[0]=*(input+k);
  }
 }
return buffer[4];

}
float* refineTransmap(float* input,long int width, long int height)
{
  float* h=gamma_transform(0.1,1.30,input,width*height);
  long int f,d,u,y;
  int p=0;
  float buffer[9];
  float* med_f=(float*)calloc(width*height,sizeof(float));
  for(f=0;f<height;f++)
  {
    for(d=0;d<width;d++)
    {
      p=0;
      for(u=(f-1);u<=(f+1);u++)
        {
          for(y=(d-1);y<=(d+1);y++)
          {
           if((u<0)||(u>=height)||(y<0)||(y>=width))
            buffer[p]=0;
           else
            buffer[p]=*(h+(u*width)+y);
           p=p+1;
          }
        }
      *(med_f+(f*width)+d)=median_filt(buffer);
    }
  }
 for(u=0;u<(width*height);u++)
 {
   if(((*(input+u))-(*(med_f+u)))>1)
      (*(input+u))=1;

   else if(((*(input+u))-(*(med_f+u)))>0)
     (*(input+u))=(*(input+u))-(*(med_f+u));

   else if(((*(input+u))-(*(med_f+u)))<0)
     (*(input+u))=0;
 }
 free(h);
 free(med_f);
 return input;
}
