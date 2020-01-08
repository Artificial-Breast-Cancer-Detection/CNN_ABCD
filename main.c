#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
      //Loading images, preprocessing, and testing

       	   for (i = 0; i < NB_IMAGES; i++)
	   {
       	       test_images[i] = ppm_load(tnames[i]);			//Load test image

	       gray_image        = rgb2grayscale(test_images[i]);  	//Convert to grascale
	       pp_test_images[i] = edge_detect(gray_image);   		//Detect edges and return a pre-processed image
	       
	       output_test_char = test(nn, pp_images[i]);			//Test

	       if (current_test_char != output_test_char)
	       	  bad_test_classifications++;

	       total_test_classifications++;
	   }

	   //Percentage of bad test classifications

	   pbtc = (bad_test_classifications * 100) / total_test_classifications;

    return 0;
}
