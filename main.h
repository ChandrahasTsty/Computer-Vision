ImageData im_read(char *filename);
float* estimateA(ImageData i, float* J,int numPixels);
float* makeDarkChannel(ImageData i, int patch_size);
float* refineTransmap(float* input,long int width, long int height);
void im_write (ImageData i,char * filename, int quality);
