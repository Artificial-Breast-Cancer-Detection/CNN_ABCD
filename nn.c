#include "protos.h"
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmocka.h>

//Fonction d_sigmoid
inline float sigmoidbis(float x)
{
  return 1.0/(1.0 + exp(-x));
}

//sigmoid derivative
float d_sigmoid(float x)
{
  return x*(1 - x);
}

/*Fonction pour convertir les imange en grayscale*/
ppm_t *rgbengrayscale( ppm_t *train_images){

  for(int i=0; i < train_images->w * train_images->h * 3; i +=3)
    {

      byte gray = train_images->px[i+2];

      train_images->px[i]     = gray;
      train_images->px[i + 1] = gray;
      train_images->px[i + 2] = gray;
  }
  return train_images;
}

/*Convolution de 2 matrices*/
int convolve(byte *m, u64 mh, u64 mw, int *f, u64 fh, u64 fw){
  int r = 0;

  for(u64 i = 0; i < fh; i++)
    for(u64 j =0; j < fw; j++)
      r += m[INDEX(i, j, mw)] * f[INDEX(i, j, fw)];

  return r;
}
/*Fonction pour enregistre l'image en ppm*/
ppm_t *ppm_open(char *fname){

  char c0, c1, c;
  FILE *fd = fopen(fname, "rb");

  if(fd){
    ppm_t *p = malloc(sizeof(ppm_t));

    fscanf(fd,"%c%c\n", &c0, &c1);

    c = fgetc(fd);

    if(c == '#'){

      while(c != '\n')
          c = fgetc(fd);
    }
    else
        fseek(fd, -1, SEEK_CUR);

    fscanf(fd, "%lld %lld\n", &p->w, &p->h);
    fscanf(fd, "%lld\n", &p->t);

    p->px = malloc(sizeof(byte) * p->w * p->h * 3);

    if((c0 == 'p') & (c1 == '6'))
          {
            fread(p->px, sizeof(byte), p->w * p->h *3, fd);
          }
    else
        if((c0 == 'P') & (c1 == '3'))
      {
      }

      fclose(fd);

      return p;
  }
  else
      return NULL;
}

/*Fonction sauvegarder les images en ppm*/
void ppm_save(char *fname, ppm_t *p){

  FILE * fd = fopen(fname, "wb");

  fprintf(fd, "P6\n");
  fprintf(fd, "%lld %lld\n", p->w, p->h);
  fprintf(fd, "%d\n", 255);

  fwrite(p->px, sizeof(byte), p->w * 3 * p->h, fd);

  fclose(fd);
}

//
void ppm_close(ppm_t *p){

  if(p){

    if(p->px)
      free(p->px);

    free(p);
  }
}
/*Fonction pour créer une image ppm des sortie*/
ppm_t *ppm_create(u64 h, u64 w, u64 t){

  ppm_t *p = malloc(sizeof(ppm_t));

  p->h = h;
  p->w = w;
  p->t = t;

  p->px = malloc(sizeof(byte) * w *h);

  return p;
}

/*fonction pour l'application du filtre de sobell*/
void ppm_apply_sobel_filter(byte *img_in, byte *img_out, u64 h, u64 w, float threshold){

  int gx, gy;
  int f1[9] = { -1, 0, 1,
                -2, 0, 2,
                -1, 0, 1};
  int f2[9] = { -1, -2, -1,
                 0,  0,  0,
                 1,  2,  1};

  for(u64 i = 0; i < h - 3; i++)
    for(u64 j = 0; j < w - 3; j++){

      gx = convolve(&img_in[INDEX(i, j, w)], h, w, f1, 3, 3);
      gy = convolve(&img_in[INDEX(i, j, w)], h, w, f2, 3, 3);

      double mag = sqrt((gx * gx) + (gy * gy));

      img_out[INDEX(i, j, w)] = (mag > threshold) ? 255 : mag;
    }
}

/*foction pour detecter le bord*/

ppm_t *edge_detected(ppm_t *img_bord){

  ppm_t *p_out = ppm_create(img_bord->h, img_bord->w, img_bord->t);

  printf("Resolution : %llu Pixels\n",(img_bord->h * img_bord->w));

  ppm_apply_sobel_filter(img_bord->px, p_out->px, img_bord->h, img_bord->w, 100);

  ppm_close(p_out);

  return p_out;
}
/*Fonction pour faire le test*/
void testing(float w[][100], int n_w, int n_h, ppm_t *pp_images, float *h){

  float s, _s,l[100];

  for(int i = 1; i <= n_h; i++){

    s = 0.0;

    for(int j = 1; j <= n_w; j++){
        s += pp_images->px[j] * w[j][i];
        l[i] = sigmoidbis(s);
      }
  }

  s = 0.0;

  for(int i = 1; i <= n_h; i++)
      s+= (l[i] * h[i]);

    _s = sigmoidbis(s);

    /*Sortie: Detection de cancer*/
    char *output1 = "Cancer detected\n";
    char *output2 = "Cancer not detected\n";

    if((_s) < 0.7){
      printf("Result : %s\n", output1);
    }
    else{
      printf("Result : %s\n", output2);
    }

    printf("\n Probabilite : (%lf)  : %lf\n\n Press enter to continue ...\n",_s,nearbyint(_s));

    getchar();

    exit(0);
}

/*Fonction de train qui renvoie les poids du reseau*/
void trainer(int nn, ppm_t *pp_train_images){

  int n_w = 2500, n_h = 100;

  float lw0[2500][100],w0[100][100],lh[100],h[100],lh_d[100], lw_d[2500][100];

  float s, err, alpha = 1.0;
  int retrains = 0;
  char mode;

  lbl1 :

  for(int i=1; i <= n_w; i++)
      for(int j = 1; j <= n_h;j++)
        w0[j][i] = (sqrt(-2.0*log((double)rand()/RAND_MAX))) * (cos(6.28318530718 * (double)rand()/RAND_MAX));

  for(int i = 1; i <= n_h; i++)
    h[i] = (sqrt(-2.0*log((double)rand()/RAND_MAX))) * (cos(6.28318530718 * (double)rand()/RAND_MAX));

  lbl2 :

    for(int k = 1; k<= nn; k++){
        for(int i = 1; i <= n_h; i++){
          s = 0.0;
            for(int j = 1; j <= n_w; j++){
              s += pp_train_images->px[j] * w0[j][i];
            }
              lw0[k][i] = sigmoidbis(s);
        }
    }

    err = 0.;

    for(int i=0;; i++){
      //forming lh_d
      for(int j = 1; j <= nn; j++){
        s = 0.0;

        for(int k = 1; k<= n_h; k++){
          s += (lw0[j][k] * h[k]);
        }

        lh[j] = sigmoidbis(s);
        err += fabs(lh[j] -1);
        lh_d[j] = (lh[j] -1) * d_sigmoid(lh[j]);

      }
      //forming lw_d
      for(int j = 1; j <= nn; j++){
        for(int k = 1; k<= n_h; k++){
          lw_d[j][k] = lh_d[j] * h[k] *d_sigmoid(lw0[j][k]);
        }
      }
      //updating w0
      for(int j = 1; j < n_w; j++){
        for(int k = 1; k <= n_h; k++){
          s=0.0;

          for(int l = 1; l <= nn; l++){
            s += (pp_train_images->px[l] * lw_d[l][k]);
          }
            w0[j][k] -= (alpha *s);
        }
      }

      //updating h
      for(int j = 1; j <= n_h; j++){
        s=0.0;

        for(int k =1; k <= nn; k++){
          s += (lw0[k][j] * lh_d[k]);
        }
          h[j] -= (alpha * s);

      }

      //
      if(i == 100){
        double err_n =err/ (double)nn;

        i=0;

        if(err_n > 0.1){
          retrains++;
          goto lbl1;
        }
         retrains = 0;

         printf("Retrains : %d, erreur : %lf\n",retrains, err_n );

         printf("Retrain (0), Keep training (1), Exit(2) \n" );
         mode = getchar();

         if (mode == '0')
            goto lbl1;
         else
         if (mode == '1')
            goto lbl2;
         else
         if (mode == '2')
               exit(0);

    }
  }
}
