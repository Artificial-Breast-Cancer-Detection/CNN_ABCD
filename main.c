#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "protos.h"

int main(int argc, char **argv) {

    if(argc<2){
      return printf("Usage : [load pgm] [train pgm] [test pgm]\n" );
    }
      ppm_t *p_in = ppm_open(argv[1]);
        if(!p_in)
          return printf("Error: cannot open ppm file (%s)\n",argv[1] ), -1;

      ppm_t *gray_image = rgbengrayscale(p_in);

      ppm_t *ppm_train_images = edge_detected(gray_image);

      trainer(1,ppm_train_images);

      ppm_close(p_in);
      ppm_close(gray_image);
      ppm_close(ppm_train_images);

  return 0;
}
