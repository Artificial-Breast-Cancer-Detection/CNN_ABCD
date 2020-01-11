#include "protos.h"
#include <math.h>
#include <time.h>

//Sigmoid derivative
float d_sigmoid(float x)
{
    return x * (1 - x);
}

//fonction sigmoid
float sigmoidbis(float x)
{
    return 1.0 / (1.0 + exp(-x));
}

//fonction de train qui renvoie les poids du réseau
void trainer(int nn, ppm_t *pp_train_images,ppm_t *test_image){
    
    //Layer inputs : n_w and hidden layer n_h
    int n_w = 2500,n_h = 100;
    //weights
    float lw0[2500][100],w0[100][100],lh[100],h[100],lh_d[100],lw_d[2500][100];
    //sigmoid error and learning rate
    float s ,err ,alpha = 1.0;
    int retrains = 0;
    char mode;

    lbl1:

    //Init
    for (int i = 1; i <= n_w; i++)
        for (int j = 1; j <= n_h; j++)
            //w[i][j] = (2.0 * rand())/ RAND_MAX - 1;
            w0[j][i]=(sqrt(-2.0*log((double)rand()/RAND_MAX)))*(cos(6.28318530718*(double)rand()/RAND_MAX));


    for (int i = 1; i <= n_h; i++)
        //h[i] = (2.0 * rand()) / RAND_MAX - 1;
        h[i]=(sqrt(-2.0*log((double)rand()/RAND_MAX)))*(cos(6.28318530718*(double)rand()/RAND_MAX));

    lbl2:
    //forming lw0
    for(int k=1;k<=nn;k++){
        for (int i = 1; i <= n_h; i++){
            s = 0.0;
            for (int j = 1; j <= n_w; j++){
                s += pp_train_images->px[j] * w0[j][i];

                lw0[k][i] = sigmoidbis(s);
            }
        }
    }
    err = 0.;

    for(int i=0;;i++) {
        //forming lh_d
        for (int j = 1; j <= nn; j++) {
            s = 0.0;

            for (int k = 1; k <= n_h; k++)
                s += (lw0[j][k] * h[k]);

            lh[j] = sigmoidbis(s);
            err += fabs(lh[j] - 1);
            lh_d[j] = (lh[j] - 1) * d_sigmoid(lh[j]);
        }

        //Forming lw_d
        for (int j = 1; j <= nn; j++) {
            for (int k = 1; k <= n_h; k++) {
                lw_d[j][k] = lh_d[j] * h[k] * d_sigmoid(lw0[j][k]);
            }
        }

        //Updating w0
        for (int j = 1; j < n_w; j++) {
            for (int k = 1; k <= n_h; k++) {
                s = 0.0;

                for (int l = 1; l <= nn; l++)
                    s += (pp_train_images->px[l] * lw_d[l][k]);

                w0[j][k] -= (alpha * s);  
            }
        }

        //Updating h
        for (int j = 1; j <= n_h; j++) {
            s = 0.0;

            for (int k = 1; k <= nn; k++)
                s += (lw0[k][j] * lh_d[k]);

            h[j] -= (alpha * s);
        } 

        //
        if (i == 100) {
            double err_n = err / (double) nn;

            i = 0;

            //Roll around untill error is acceptable
            if (err_n > 0.1) {
                retrains++;
                goto lbl1;
            }

            retrains = 0;

            //Mean absolute error
            printf("retrains: %d, err: %lf\n", retrains, err_n);

            //getchar();
            printf("Retrain (0), Keep training (1), or test (2): ");
            mode = getchar();

            if (mode == '0')
                goto lbl1;
            else
            if (mode == '1')
                goto lbl2;
            else
              if (mode == '2')
                testing(w0,n_w,n_h,test_image,h);
        }
    }
    
}

void testing(float w[][100],int n_w,int n_h,ppm_t *pp_images,float *h) {

    float s, _s,l[100];

    for (int i = 1; i <= n_h; i++)
    {
        s = 0.0;

        for (int j = 1; j <= n_w; j++)
            s += pp_images->px[j] * w[j][i];

        l[i] = sigmoidbis(s);
    }

    s = 0.0;

    for (int i = 1; i <= n_h; i++)
        s += (l[i] * h[i]);

    _s = sigmoidbis(s);

    //output : cancer detection
    char *output = "Cancer not detected\n";
    printf("result : %s\n",output);
    printf("\nProbabilite: (%lf) %.0lf \n\nPress enter to continue ...", _s, nearbyint(_s));

    getchar();
    getchar();
    //exit(0);
}

// fonction pour convertir les images en grayscale
ppm_t *rgbengrayscale(ppm_t *train_images){

  for (int i = 0; i < train_images->w * train_images->h * 3; i += 3)
    {
      //red   --> light gray image
      //green --> dark gray image (b&w photograph) 
      //blue  --> darker gray image (old b&w photograph)
      byte gray = train_images->px[i + 2];
      
      train_images->px[i]     = gray; 
      train_images->px[i + 1] = gray;
      train_images->px[i + 2] = gray;
    }
    return train_images;
}

// fonction de rectification linéaire : permet d'écraser les valeurs entre 0 et 1
inline u64 relu(u32 x){
  u64 res = max(0,x);
  return res;
}

//fonction sigmoid
static inline double sigmoid(double x)
{
  return 1.0 / (1.0 + exp(-x));  
}

//fonction pour ouvrir les images sous format pgm
pgm_t *pgm_load(char *fname){
    char c1, c2;
    pgm_t *p = NULL;
    FILE *fd = fopen(fname, "rb");

    if (fd)
    {
        p = malloc(sizeof(pgm_t));

        //P5
        fscanf(fd, "%c%c", &c1, &c2);

        fscanf(fd, "%llu %llu\n", &p->w, &p->h);

        fscanf(fd, "%llu\n", &p->t);

        p->p = malloc(sizeof(byte) * p->h * p->w);

        fread(p->p, sizeof(byte), p->h * p->w, fd);

        fclose(fd);
    }

    return p;
}

//fonction pour sauvegarder les images pgm
void pgm_save(char *fname, pgm_t *p)
{
    FILE *fd = fopen(fname, "wb");

    if (fd)
    {
        fprintf(fd, "P5\n");

        fprintf(fd, "%llu %llu\n", p->w, p->h);

        fprintf(fd, "%llu\n", p->t);

        fwrite(p->p, sizeof(byte), p->h * p->w, fd);

        fclose(fd);
    }
}

//fonction pour créer une image pgm de sortie
pgm_t *pgm_create(u64 h, u64 w, u64 t)
{
    pgm_t *p = malloc(sizeof(pgm_t));

    p->h = h;
    p->w = w;
    p->t = t;

    p->p = malloc(sizeof(byte) * w * h);

    return p;
}

//
void pgm_close(pgm_t *p)
{
  if (p)
    {
      if (p->p)
	free(p->p);

      free(p);
    }
}

//Convolution of two matrices (dotprod/FMA)
int convolve(byte *m, u64 mh, u64 mw, int *f, u64 fh, u64 fw)
{
  int r = 0;
  
  for (u64 i = 0; i < fh; i++)
    for (u64 j = 0; j < fw; j++)
      r += m[INDEX(i, j, mw)] * f[INDEX(i, j, fw)];
  
  return r;
}

void pgm_apply_sobel_filter(byte *img_in, byte *img_out, u64 h, u64 w, float threshold)
{
  int gx, gy;
  int f1[9] = { -1, 0, 1,
    -2, 0, 2,
    -1, 0, 1 }; //3x3 matrix
  int f2[9] = { -1, -2, -1,
    0, 0, 0,
    1, 2, 1 }; //3x3 matrix
  
  for (u64 i = 0; i < h - 3; i++)
    for (u64 j = 0; j < w - 3; j++)
      {
  gx = convolve(&img_in[INDEX(i, j, w)], h, w, f1, 3, 3);
  gy = convolve(&img_in[INDEX(i, j, w)], h, w, f2, 3, 3);
  
  double mag = sqrt((gx * gx) + (gy * gy));
  
  img_out[INDEX(i, j, w)] = (mag > threshold) ? 255 : mag;
      }
}

//
void pgm_apply_prewitt_filter(byte *img_in, byte *img_out, u64 h, u64 w, float threshold)
{
  int gx, gy;
  int f1[9] = { 1, 0, -1, 1, 0, -1,  1,  0, -1 }; //3x3 matrix
  int f2[9] = { 1, 1,  1, 0, 0,  0, -1, -1, -1 }; //3x3 matrix
  
  for (u64 i = 0; i < h - 3; i++)
    for (u64 j = 0; j < w - 3; j++)
      {
	gx = convolve(&img_in[INDEX(i, j, w)], h, w, f1, 3, 3);
	gy = convolve(&img_in[INDEX(i, j, w)], h, w, f2, 3, 3);
	
	double mag = sqrt((gx * gx) + (gy * gy));
	
	img_out[INDEX(i, j, w)] = (mag > threshold) ? 255 : mag;
      }
}

//Rotate the initial matrix by 45° (8 entries)
void pgm_apply_kirsch_filter(byte *img_in, byte *img_out, u64 h, u64 w)
{
  int max_g;
  int g[8] = { 0 };
  int f[8][9] = {
		 {   5,  5,  5,
		    -3,  0, -3,
		    -3, -3, -3 },
  
		 {   5,  5, -3,
		     5,  0, -3,
		    -3, -3, -3 },
  
		 {   5, -3, -3,
		     5,  0, -3,
		     5, -3, -3 },
  
		 {  -3, -3, -3,
		     5,  0, -3,
		     5,  5, -3 },
	       
		 {  -3, -3, -3,
		    -3,  0, -3,
		     5,  5,  5 },

		 {  -3, -3, -3,
		    -3,  0,  5,
		    -3,  5,  5 },
  
		 {  -3, -3, 5,
		    -3,  0, 5,
		    -3, -3, 5 },
  
		 {  -3,  5,  5,
		    -3,  0,  5,
		    -3, -3, -3 } }; 

  for (u64 i = 0; i < h - 3; i++)
    for (u64 j = 0; j < w - 3; j++)
      {
	for (u64 k = 0; k < 8; k++)
	  g[k] = convolve(&img_in[INDEX(i, j, w)], h, w, f[k], 3, 3);

	max_g = g[0];
	
	for (u64 k = 1; k < 8; k++)
	  max_g = max(max_g, g[k]);
	
	img_out[INDEX(i, j, w)] = max_g >> 2;
      }
}

//
ppm_t *ppm_open(char *fname)
{
    char c0, c1, c;
    FILE *fd = fopen(fname, "rb");

    if (fd)
    {
        ppm_t *p = malloc(sizeof(ppm_t));

        fscanf(fd, "%c%c\n", &c0, &c1);

        c = fgetc(fd);

        if (c == '#')
        {
            //Handle comment
            while (c != '\n')
                c = fgetc(fd);
        }
        else
            fseek(fd, -1, SEEK_CUR);

        fscanf(fd, "%d %d\n", &p->w, &p->h);
        fscanf(fd, "%d\n", &p->t);

        p->px = malloc(sizeof(byte) * p->w * p->h * 3);//h lines & w columns & 3 values per pixel(RGB)

	if ((c0 == 'P') & (c1 == '6')) //Binary mode
            {
                fread(p->px, sizeof(byte), p->w * p->h * 3, fd);
            }
            else
	      if ((c0 == 'P') & (c1 == '3')) //ASCII mode
            {
            }

        fclose(fd);

        return p;
    }
    else
        return NULL;
}

//
void ppm_save(char *fname, ppm_t *p)
{
    FILE *fd = fopen(fname, "wb");

    fprintf(fd, "P6\n");
    fprintf(fd, "%d %d\n", p->w, p->h);
    fprintf(fd, "%d\n", 255);

    fwrite(p->px, sizeof(byte), p->w * 3 * p->h, fd);

    fclose(fd);
}

//
void ppm_close(ppm_t *p)
{
    if (p)
    {
        if (p->px)
            free(p->px);

        free(p);
    }
}

//fonction pour créer une image ppm de sortie
ppm_t *ppm_create(u64 h, u64 w, u64 t)
{
    ppm_t *p = malloc(sizeof(ppm_t));

    p->h = h;
    p->w = w;
    p->t = t;

    p->px = malloc(sizeof(byte) * w * h);

    return p;
}

//fonction d'entrainement du réseau de neurones
void train(ppm_t *pp_train_images){
int nb_images = 2500;   //Number of entries
int n_w = 2500;  //Number of neurons in the input layer
int n_h = 100;  //Number of neurons in the hidden layer
char mode;
int retrains = 0;
double s, err, alpha = 1.0; //Sigmoid, error, learning rate

//w : input layer weights, h : hidden layer weights
 double w[2500][100],h[100],lw[2500][100],lw_d[nb_images][100],lh[nb_images],lh_d[nb_images];

 srand(time(NULL));

 lbl1:

 //Init

 for (int i = 1; i <= n_w; i++)
    for (int j = 1; j <= n_h; j++)
      //w[i][j] = (2.0 * rand())/ RAND_MAX - 1;
      w[j][i]=(sqrt(-2.0*log((double)rand()/RAND_MAX)))*(cos(6.28318530718*(double)rand()/RAND_MAX));
  
  
  for (int i = 1; i <= n_h; i++)
    //h[i] = (2.0 * rand()) / RAND_MAX - 1;
    h[i]=(sqrt(-2.0*log((double)rand()/RAND_MAX)))*(cos(6.28318530718*(double)rand()/RAND_MAX));

  lbl2:
  //It!
  for (int i = 1;; i++)
    {

      //Forming lw
      for (int j = 1; j <= nb_images; j++)
	     {
       	  for (int k = 1; k <= n_h; k++)
	         {
	           s = 0.0;
	      
	            for (int l = 1; l <= n_w; l++){
	              s += (pp_train_images->px[j] * w[l][k]);}
        

	         lw[j][k] = sigmoid(s);
           //printf("lw= %f\n",lw[j][k]);
	         }
	     }

      err = 0.0;

      //Forming lh_d
      for (int j = 1; j <= nb_images; j++)
	     {
	       s = 0.0;

	       for (int k = 1; k <= n_h; k++)
	         s += (lw[j][k] * h[k]);

	     lh[j] = sigmoid(s);
	     err += fabs(lh[j] - pp_train_images->px[j]);
	     lh_d[j] = (lh[j] - pp_train_images->px[j]) * sigmoid(lh[j]);
	     }
      //Forming lw_d
      for (int j = 1; j <= nb_images; j++)
	     for (int k = 1; k <= n_h; k++)
	     lw_d[j][k] = lh_d[j] * h[k] * sigmoid(lw[j][k]);

      //Updating w
      for (int j = 1; j < n_w; j++)
	     {
	     for (int k = 1; k <= n_h; k++)
	     {
	      s = 0.0;

	      for (int l = 1; l <= nb_images; l++)
		    s += (pp_train_images->px[l] * lw_d[l][k]);

	      w[j][k] -= (alpha * s);
	     }
	     }

      //Updating h
      for (int j = 1; j <= n_h; j++)
	     {
	       s = 0.0;

	     for (int k = 1; k <= nb_images; k++){
	       s += (lw[k][j] * lh_d[k]);}

	       h[j] -= (alpha * s);
	     }
      
      //
  if (i == 100000)
	 {
	  double err_n = err / (double)nb_images;

	  i = 0;
	  
	  //Roll around untill error is acceptable
	  if (err_n > 0.1)
	    {
	      retrains++;
	      goto lbl1;
	    }

	  retrains = 0;

	  //Mean absolute error
	  printf("retrains: %d, err: %lf\n", retrains, err_n); 
	  
	  getchar();

	  printf("Retrain (0), Keep training (1)");
	  mode = getchar();
    if (mode == '0')
      goto lbl1;
    else
      if (mode == '1')
        goto lbl2;
      else
        return;
	}
	}
  ppm_close(pp_train_images);
}

void test(ppm_t *test_image,int n_w,int n_h){
  printf("Cancer detection");
}


