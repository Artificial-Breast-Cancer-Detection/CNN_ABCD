#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

//Sigmoid function (squashes any value of x into a value between 0 and 1)
static inline double sigmoid(double x)
{
  return 1.0 / (1.0 + exp(-x));
}

//Sigmoid derivative
static inline double d_sigmoid(double x)
{
  return x * (1 - x);
}

//Test function
void test(double w[][11], int n_w, double *h, int n_h)
{
  double s, _s, I[11], l[11];

  printf("\nEnter test values\n");

  for (int i = 1; i <= n_w; i++)
    {
      printf("I[%d]: ", i);
      scanf("%lf", &I[i]);
    }

  for (int i = 1; i <= n_h; i++)
    {
      s = 0.0;

      for (int j = 1; j <= n_w; j++)
	s += I[j] * w[j][i];

      l[i] = sigmoid(s);
    }

  s = 0.0;

  for (int i = 1; i <= n_h; i++)
    s += (l[i] * h[i]);

  _s = sigmoid(s);

  printf("\nOutput: (%lf) %.0lf \n\nPress enter to continue ...", _s, nearbyint(_s));

  getchar();
  getchar();
}

//
int main(int argc, char **argv)
{
  int n;   //Number of entries
  int n_w;  //Number of neurons in the input layer
  int n_h;  //Number of neurons in the hidden layer
  char mode;
  int retrains = 0;
  double s, err, alpha = 1.0; //Sigmoid, error, learning rate

  //w: input layer weights, h: hidden layer weights
  double I[101][11], O[11], lw[101][11], lw_d[101][11], w[11][11], lh[101], lh_d[101], h[11];

  //
  srand(time(NULL));

  //Columns
  printf("Number of input layer neurons: ");
  scanf("%d", &n_w);

  printf("Number of hidden layer neurons: ");
  scanf("%d", &n_h);

  //Rows
  printf("Number of entries: ");
  scanf("%d", &n);

  //
  for (int i = 1; i <= n; i++)
    {
      printf("Entry %d\n", i);

      for (int j = 1; j <= n_w; j++)
	{
	  printf("\tI[%d][%d]: ", i, j);
	  scanf("%lf", &I[i][j]);
	}

      printf("\n\tO[%d]: ", i);
      scanf("%lf", &O[i]);
    }

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
      for (int j = 1; j <= n; j++)
	{
       	  for (int k = 1; k <= n_h; k++)
	    {
	      s = 0.0;

	      for (int l = 1; l <= n_w; l++)
		s += (I[j][l] * w[l][k]);

	      lw[j][k] = sigmoid(s);
	    }
	}

      err = 0.0;

      //Forming lh_d
      for (int j = 1; j <= n; j++)
	{
	  s = 0.0;

	  for (int k = 1; k <= n_h; k++)
	    s += (lw[j][k] * h[k]);

	  lh[j] = sigmoid(s);
	  err += fabs(lh[j] - O[j]);
	  lh_d[j] = (lh[j] - O[j]) * d_sigmoid(lh[j]);
	}

      //Forming lw_d
      for (int j = 1; j <= n; j++)
	for (int k = 1; k <= n_h; k++)
	  lw_d[j][k] = lh_d[j] * h[k] * d_sigmoid(lw[j][k]);

      //Updating w
      for (int j = 1; j < n_w; j++)
	{
	  for (int k = 1; k <= n_h; k++)
	    {
	      s = 0.0;

	      for (int l = 1; l <= n; l++)
		s += (I[l][j] * lw_d[l][k]);

	      w[j][k] -= (alpha * s);
	    }
	}

      //Updating h
      for (int j = 1; j <= n_h; j++)
	{
	  s = 0.0;

	  for (int k = 1; k <= n; k++)
	    s += (lw[k][j] * lh_d[k]);

	  h[j] -= (alpha * s);
	}

      //
      if (i == 100000)
	{
	  double err_n = err / (double)n;

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

	  printf("Retrain (0), Keep training (1), or test (2): ");
	  mode = getchar();

	  if (mode == '0')
	    goto lbl1;
	  else
	    if (mode == '1')
	      goto lbl2;
	    else
	      if (mode == '2')
		test(w, n_w, h, n_h);
	}
    }

  return 0;
}
