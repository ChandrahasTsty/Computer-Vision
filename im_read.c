/* This C code is not compile ready,it can be used for linking of another programs present in the same directory */

/* Include libraries that are needed */

#include <stdio.h>
#include <stdlib.h>
#include "jpeg-9b/jpeglib.h"
#include "jpeg-9b/jpegint.h"
#include "DataTypes.h"
/*Function based on JPEG library to decompress/read pictures in .jpg, this takes file-name(string) as input */ 
ImageData im_read(char *filename)
{
/*Standard Implementation of the JPEG library */
struct jpeg_decompress_struct cinfo;
struct jpeg_error_mgr jerr;
int row_stride;
int buffer_height = 1;
long int h=0;
long int k=0;
float cr1,cr2,cr3;
JSAMPARRAY buffer = (JSAMPARRAY)malloc(sizeof(JSAMPROW) * buffer_height);
/*JSAMPARRAY rowptr;*/
buffer[0] = (JSAMPROW)malloc(sizeof(JSAMPLE) * row_stride);

/* ImageData is the variable we are using as object for our implementation it's definition is defined in the header file "DataTypes.h" it is defined as
 
typedef struct ImageData{
long int height;
long int width;
long int pixels;
float* rdata;
float* gdata;
float* bdata;
}ImageData;
*/

ImageData ret;
/* The following are implemented as per how the jpeg library works, these lines were directly taken from example.c provided with jpeg-9b file */

cinfo.err=jpeg_std_error(&jerr);
jpeg_create_decompress(&cinfo);

FILE *jpg_file=fopen(filename,"rb");/*  file pointer for input file */
/* Following if-else statement is for checking if the image is present or not */ 
if(jpg_file==NULL)
  printf("Image file is not found");
else 
  printf("Image found in the Directory\n");
  /*Again some standard library code */
jpeg_stdio_src(&cinfo,jpg_file);
jpeg_read_header(&cinfo,TRUE);
jpeg_start_decompress(&cinfo);
row_stride = cinfo.output_width * cinfo.output_components;
/* buffer is for saving decompressed values while looping over entire image */
buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
		
/* data_file is the file pointer pointing to memory for decompressed image file */
 		
//FILE *data_file=fopen("data.csv","w");
/* printing height of image in file */
//fprintf(data_file,"The height of %s is %d\n",filename,cinfo.output_height);
/*printing width of image in file */
//fprintf(data_file,"The width of %s is %d\n",filename,cinfo.output_width);
/*printing number of pixels in image */
//fprintf(data_file,"Number of pixels in %s is %d\n",filename,cinfo.output_width*cinfo.output_height);
ret.height=cinfo.output_height;/*saving height in return object */
ret.width=cinfo.output_width;/*saving width in return object */
ret.pixels=(cinfo.output_height*cinfo.output_width);/*saving pixels in return object */
ret.rdata=(float*)calloc(ret.pixels,sizeof(float));/* Dynamic memory allotment for Red data */
ret.gdata=(float*)calloc(ret.pixels,sizeof(float));/* Dynamic memory allotment for Green data */
ret.bdata=(float*)calloc(ret.pixels,sizeof(float));/* Dynamic memory allotment for Blue data */

/* jpeg_read_scanlines is jpeg library function which reads a image by row-wise and saves it in buffer,We will take those values from buffer and store them in return object, print them in our csv file */

while(cinfo.output_scanline<cinfo.output_height)
{
  jpeg_read_scanlines(&cinfo,buffer, 1);
  /*loop for extracting values stored in buffer, intensity values of a row of image */
  for(k=0;k<=(cinfo.output_width-1);k++)
  {
  cr1=*(*(buffer)+(3*k));
  cr2=*(*(buffer)+(3*k)+1);
  cr3=*(*(buffer)+(3*k)+2);
  //fprintf(data_file,"(%f,%f,%f)\n",(cr1/255),(cr2/255),(cr3/255));
  *(ret.rdata+k+(ret.width*h))=(cr1/255);
  *(ret.gdata+k+(ret.width*h))=(cr2/255);
  *(ret.bdata+k+(ret.width*h))=(cr3/255);
  }
  h++;
}
/* Closing steps as per the library */
jpeg_finish_decompress(&cinfo);
jpeg_destroy_decompress(&cinfo);
return ret;
}
