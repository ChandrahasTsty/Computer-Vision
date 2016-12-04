#include <stdio.h>
#include <stdlib.h>
#include "jpeg-9b/jpeglib.h"
#include "jpeg-9b/jpegint.h"
#include "DataTypes.h"
#include <math.h>

void im_write (ImageData i,char * filename, int quality)
{
     unsigned char rdata[i.pixels];
     unsigned char gdata[i.pixels];
     unsigned char bdata[i.pixels];
     int buffer1,buffer2,buffer3;
     long int r,c;
     for(r=0;r<i.height;r++)
     {
       for(c=0;c<i.width;c++)
       {
        buffer1=nearbyintf(255*(*(i.rdata+(r*(i.width))+c)));
        buffer2=nearbyintf(255*(*(i.gdata+(r*(i.width))+c)));
        buffer3=nearbyintf(255*(*(i.bdata+(r*(i.width))+c)));
        
        rdata[(r*(i.width))+c]=(char) buffer1;
        gdata[(r*(i.width))+c]=(char)buffer2;
        bdata[(r*(i.width))+c]=(char) buffer3;
        
       }
     }
     struct jpeg_compress_struct cinfo;
     struct jpeg_error_mgr jerr;
     FILE * outfile;
     JSAMPROW row_pointer[1];
     int row_stride;
     cinfo.err = jpeg_std_error(&jerr);
     jpeg_create_compress(&cinfo);
     if ((outfile = fopen(filename, "wb")) == NULL) 
     {
       fprintf(stderr, "can't open %s\n", filename);
       exit(1);
     }
     jpeg_stdio_dest(&cinfo, outfile);
     cinfo.image_width = i.width;
     cinfo.image_height = i.height;
     cinfo.input_components = 3;
     cinfo.in_color_space = JCS_RGB;
     jpeg_set_defaults(&cinfo);
     jpeg_set_quality(&cinfo, quality, TRUE);
     jpeg_start_compress(&cinfo, TRUE);
     row_stride = i.width * 3;
     r=0;
     unsigned char buffer[3*i.width];
     while (cinfo.next_scanline < cinfo.image_height) 
     {
       for(c=0;c<i.width;c++)
       {
        buffer[3*c]=rdata[(r*(i.width))+c];
        buffer[3*c+1]=gdata[(r*(i.width))+c];
        buffer[3*c+2]=bdata[(r*(i.width))+c];
       }
       r=r+1;
       row_pointer[0]=&buffer[0];
       (void) jpeg_write_scanlines(&cinfo, row_pointer, 1); 
     }
     fclose(outfile);
     jpeg_destroy_compress(&cinfo);
}
