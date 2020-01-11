#include "protos.h"
#include <string.h>
#include <assert.h>

//
int main(int argc, char **argv)
{
    if(argc<3){
      return printf("Usage : [train ppm] [test ppm]\n"),-1;
    }
    ppm_t *train_images;
    //Loading images pre-processed, and training
    
    train_images = ppm_open(argv[1]);
    
    //Loading images, preprocessing, and testing
    ppm_t *test_images;
    
    test_images = ppm_open(argv[2]);     
    trainer(1,train_images,test_images);
    
    
    if (!train_images & !test_images)
      return printf("Error: cannot open ppm file (%s) (%s)\n", argv[1], argv[2]), -1;

    
    return 0;
}
