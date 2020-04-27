#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include "protos.h"

int main(int argc, char **argv) {

char *path;
ppm_t *p_in;
ppm_t *gray_image;
ppm_t *ppm_train_images;
int nb_files;
char buf[512];
struct dirent **list =NULL;

    if(argc<2){
      return printf("Usage : [load pgm] [train pgm] [test pgm]\n" );
    }

    path = argv[1];
    nb_files =scandir(path, &list, NULL, alphasort );

    if(nb_files<0 )
    {
      perror("scandir");
    }

    for (int i = 2; i < nb_files; i++) {

      snprintf(buf, 512, "%s%s", path, list[i]->d_name);
      p_in = ppm_open(buf);
      gray_image = rgbengrayscale(p_in);
      ppm_train_images = edge_detected(gray_image);
      trainer(1,ppm_train_images);
    }


      ppm_close(p_in);
      ppm_close(gray_image);
      ppm_close(ppm_train_images);

  return 0;
}
