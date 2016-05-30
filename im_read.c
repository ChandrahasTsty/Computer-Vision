#include <stdio.h>
#include <stdlib.h>
#include "jpeg-9b/jpeglib.h"
#include "jpeg-9b/jpegint.h"
void im_read(char *filename)
{
struct jpeg_decompress_struct cinfo;
struct jpeg_error_mgr jerr;
int row_stride;
int buffer_height = 1;
int k=0;
JSAMPARRAY buffer = (JSAMPARRAY)malloc(sizeof(JSAMPROW) * buffer_height);
/*JSAMPARRAY rowptr;*/
buffer[0] = (JSAMPROW)malloc(sizeof(JSAMPLE) * row_stride);
cinfo.err=jpeg_std_error(&jerr);
jpeg_create_decompress(&cinfo);
FILE *jpg_file=fopen(filename,"rb");
FILE *log_file=fopen("log.txt","w");
if(jpg_file==NULL)
  fprintf(log_file, "Image file cant be opened");
else 
  fprintf(log_file,"Image file opened successfully");
jpeg_stdio_src(&cinfo,jpg_file);
jpeg_read_header(&cinfo,TRUE);
jpeg_start_decompress(&cinfo);
row_stride = cinfo.output_width * cinfo.output_components;
buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
FILE *data_file=fopen("data.csv","w");
fprintf(data_file,"The height of %s is %d\n",filename,cinfo.output_height);
fprintf(data_file,"The width of %s is %d\n",filename,cinfo.output_width);
fprintf(data_file,"Number of pixels in %s is %d\n",filename,cinfo.output_width*cinfo.output_height);			
while(cinfo.output_scanline<cinfo.output_height)
{
  jpeg_read_scanlines(&cinfo,buffer, 1);
  for(k=0;k<=(cinfo.output_width-1);k++)
  {
  fprintf(data_file,"(%d,%d,%d)\n",*(*(buffer)+(3*k)),*(*(buffer)+(3*k)+1),*(*(buffer)+(3*k)+2));
  }
}
jpeg_finish_decompress(&cinfo);
jpeg_destroy_decompress(&cinfo);
}
int main()
{
 im_read("cones.jpg");
}
