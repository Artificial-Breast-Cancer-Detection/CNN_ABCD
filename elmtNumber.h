#ifndef DEF_ELMTNUMBER
#define DEF_ELMTNUMBER
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int elmtNumber(size_t count)
{
  struct dirent *res;
  struct stat sb;
  const char *path = "./INPUT/0";

  if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode)){
      DIR *folder = opendir ( path );

      if (access ( path, F_OK ) != -1 ){
          if ( folder ){
              while ( ( res = readdir ( folder ) ) ){
                  if ( strcmp( res->d_name, "." ) && strcmp( res->d_name, ".." ) ){

                      count++;
                  }
              }

              closedir ( folder );
          }else{
              perror ( "Could not open the directory" );
              exit( EXIT_FAILURE);
          }
      }

  }else{
      printf("The %s it cannot be opened or is not a directory\n", path);
      exit( EXIT_FAILURE);
  }
    return count;
}
#endif
