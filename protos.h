#ifndef DEF_NN
#define DEF_NN
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define max(a, b) ((a) > (b)) ? (a) : (b)

#define INDEX(x, y, w) (((x) * (w)) + (y))

typedef unsigned u32;
typedef unsigned char byte;
typedef unsigned long long u64;

typedef struct ppm_s{
  int w;
  int h;
  int t;
  byte *px;
}ppm_t;

typedef struct pgm_s{
  u64 w;
  u64 h;
  u64 t;
  byte *p;
}pgm_t;

typedef struct pair{
  float a[100];
  float b[100][100];
}pair;

ppm_t *rgbengrayscale(ppm_t *train_images);

u64 relu(u32 x);

float d_sigmoid(float x);

float sigmoidbis(float x);

pgm_t *pgm_load(char *fname);

void pgm_save(char *fname, pgm_t *p);

pgm_t *pgm_create(u64 h, u64 w, u64 t);

int convolve(byte *m, u64 mh, u64 mw, int *f, u64 fh, u64 fw);

void pgm_apply_sobel_filter(byte *img_in, byte *img_out, u64 h, u64 w, float threshold);

void pgm_close(pgm_t *p);

ppm_t *ppm_open(char *fname);

void ppm_save(char *fname, ppm_t *p);

ppm_t *ppm_create(u64 h, u64 w, u64 t);

void trainer(int nn, pgm_t *pp_train_images, pgm_t *test_image);

void testing(float w[][100], int n_w, int n_h, pgm_t *pp_images, float *h);

void ppm_close(ppm_t *p);

#endif
