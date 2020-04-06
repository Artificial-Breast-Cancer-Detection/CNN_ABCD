#ifndef DEF_NN
#define DEF_NN
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cmocka.h>
#define max(a, b) ((a) > (b)) ? (a) : (b)

#define INDEX(x, y, w) (((x) * (w)) + (y))

typedef unsigned u32;
typedef unsigned char byte;
typedef unsigned long long u64;

typedef struct ppm_s{
  u64 w;
  u64 h;
  u64 t;
  byte *px;
}ppm_t;

typedef struct pair{
  float a[100];
  float b[100][100];
}pair;

float d_sigmoid(float x);

float sigmoidbis(float x);

ppm_t *ppm_open(char *fname);

ppm_t *rgbengrayscale(ppm_t *train_images);

ppm_t *ppm_create(u64 h, u64 w, u64 t);

void ppm_save(char *fname, ppm_t *p);

int convolve(byte *m, u64 mh, u64 mw, int *f, u64 fh, u64 fw);

void ppm_apply_sobel_filter(byte *img_in, byte *img_out, u64 h, u64 w, float threshold);

ppm_t *edge_detected(ppm_t *img_bord);

void ppm_close(ppm_t *p);

void trainer(int nn, ppm_t *pp_train_images);

void testing(float w[][100], int n_w, int n_h, ppm_t *pp_images, float *h);

#endif
