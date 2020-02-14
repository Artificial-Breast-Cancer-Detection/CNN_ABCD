#include "protos.h"
#include <string.h>
#include <assert.h>

//
int main(int argc, char **argv)
{
    if(argc<3){
      return printf("Usage : [train pgm] [test pgm]\n"),-1;
    }
    
    pgm_t *train_images;

    //Loading images pre-processed, and training

    train_images = pgm_load(argv[1]);

    //Loading images, preprocessing, and testing
    pgm_t *test_images;
    //for (int i = 0; i < NB_IMAGES; i++) {
    test_images = pgm_load(argv[2]);            //Load test image
    trainer(1,train_images,test_images);


    //        output_test_image = test(nn, test_images[i]);            //Test
    //
    //        if (current_test_image != output_test_image)
    //            bad_test_classifications++;
    //
    //        total_test_classifications++;
        //}
    //
    //Percentage of bad test classifications
    //
    //    pbtc = (bad_test_classifications * 100) / total_test_classifications;*/
    if (!train_images & !test_images)
      return printf("Error: cannot open ppm file (%s) (%s)\n", argv[1], argv[2]), -1;


    return 0;
}
