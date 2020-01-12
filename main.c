#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "protos.h"

int main(int argc, char **argv)
    {

    if(argc<3)
      {
      return printf("Usage : [train ppm] [test ppm]\n"),-1;
      }

    ppm_t *train_images;
    train_images = ppm_open(argv[1]); 
    ppm_t *test_images;
    test_images = ppm_open(argv[2]);     
    trainer(1,train_images,test_images);
    
    if (!train_images & !test_images)
      {
      return printf("Error: cannot open ppm file (%s) (%s)\n", argv[1],     argv[2]), -1;
      }
    }



