#define _GNU_SOURCE
#include "protos.h"
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdio.h>

//
int main(int argc, char **argv)
{
    if(argc<2){
      return printf("Usage : [train ppm] \n"),-1;
    }

    char *path = argv[1];
    struct dirent **list = NULL;
    int nb_files = scandir(path,&list,NULL,alphasort);

    if(nb_files<0){
      perror("scandir");
      return 1;
    }
    ppm_t *train_images;
    //Loading images pre-processed, and training

    for(int i=2;i<nb_files;i++){
      char buf[512];
      snprintf(buf,512,"%s%s",path,list[i]->d_name);
      train_images = ppm_open(buf);
      printf("Resolution: %u Pixels, %u MPixels\n", (train_images->h * train_images->w), (train_images->h * train_images->w) / 1000000);
      trainer(1,train_images,train_images);
    }
    
    if (!train_images )
      return printf("Error: cannot open ppm file (%s) \n", argv[1]), -1;

    
    return 0;
}
