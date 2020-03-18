#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "protos.h"
#include "elmtNumber.h"


int main(int argc, char **argv) {
  size_t count;
  int N_images = elmtNumber(count);

  printf("%d\n",N_images );

  pgm_t *p_in = pgm_load(argv[1]);

    if(!p_in)
      return printf("Error: cannot open pgm file (%s)\n",argv[2] ), -2;


  pgm_t *p_out = pgm_create(p_in->h, p_in->w, p_in->t);

  printf("Resolution : %llu Pixels\n",(p_in->h * p_in->w));

  pgm_apply_sobel_filter(p_in->p, p_out->p, p_in->h, p_in->w, 100);

  pgm_save(argv[2], p_out);

  pgm_t *train_images;

  train_images = pgm_load(argv[2]);

  pgm_t *test_images;

  test_images = pgm_load(argv[3]);

  trainer(1,train_images,test_images);

  if(!train_images & !test_images)
      return printf("Error : cannot open ppm file(%s) (%s)\n", argv[1], argv[2]), -1;

  pgm_close(p_in);
  pgm_close(p_out);

  return 0;
}
