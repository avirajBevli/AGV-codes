/*
*****************************************************************************************
BC BIG BLUNDER
IN get_grad_y and get_grad_x , return  ""abs(get_grad_x(blurred_image,i,j))"" is a must, 
initially i returned not the absolute value and output was random because if after sobel operator
if the pixel value obtained was negative , then the pixels were asigned a random value
hence, even in get_grad() function, too much random noise was obtained
*****************************************************************************************
*/

# define M_PI 3.14159265358979323846

//atan() returns value in the range between -pi/2 to pi/2 radians

#include<opencv2/opencv.hpp>
#include <stdio.h>   
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
using namespace std;
using namespace cv;
#include <math.h>
 
int get_grad_x(Mat image, int i, int j);
int get_grad_y(Mat image, int i, int j);

Mat threshold(Mat image, int threshold_initial);

//Mat threshold_set_max(Mat thinned_grad, int threshold_value_final);

//Mat thin(Mat thresholded_gradient, int k_thin);

Mat hysteresis(Mat thinned_grad, Mat gradient_dir, Mat gradient_mag, int threshold_val_lower, int threshold_val_upper);



//***********saale chutiye**************************************************************************
//***********kernels define karne ke lie humesha normal int array define kara kar*******************
//***********dont even think to define kernel in terms of Mat cuz Mat in an opencv datatype*********
//***********hence, dont do this chutiyaap!!!*******************************************************
int sobel_y[3][3] = {-1,-2,-1,0,0,0,1,2,1};

int sobel_x[3][3] = {-1,0,1,-2,0,2,-1,0,1};
//sign convention taken to be consistent with the coordinate sysem used in opencv, and not the coordinate
//system used in maths


Mat image = imread("canny.jpeg", CV_LOAD_IMAGE_GRAYSCALE);
//int no_rows = image.rows;
//int no_cols = image.cols;


Mat thin(Mat thresholded_gradient ,Mat gradient_dir, Mat gradient_mag);


int main()
{
	Mat blurred_image = imread("canny.jpeg", CV_LOAD_IMAGE_GRAYSCALE);

	int k_blur = 5; //k is the "k"of kernel(k*k), not(2k+1 * 2k+1)
	
	//for canny edge hysteresis
	int threshold_initial = 20;
	int threshold_val_lower = 10;
	//int threshold_value_final = 05;
	int threshold_val_upper= 20;

	/*Edge detectors are are prone to noise. A bit of smoothing with a Gaussian blur helps. 
	****************Usually, a 5x5 Gaussian filter with standard deviation = 1.4 is used for this purpose.***************
	*/
	
	GaussianBlur( image, blurred_image, Size( 5, 5 ), 1.4);
	//medianBlur(image , blurred_image , k_blur);

	Mat hysteresis_mat = Mat::zeros(image.rows, image.cols, CV_8UC1); 
	Mat thinned_grad = Mat::zeros(image.rows, image.cols, CV_8UC1);
	Mat thresholded_gradient = Mat::zeros(image.rows, image.cols, CV_8UC1);
	Mat gradient_mag = Mat::zeros(image.rows, image.cols, CV_8UC1);
	//Mat thinned_grad = Mat::zeros(image.rows, image.cols, CV_8UC1);
	Mat gradient_dir = Mat::zeros(image.rows, image.cols, CV_8UC1);
	//will have values between 0 and 180 degrees

	//int gradient_dir[image.rows][image.cols];
//all entries are values of angles in degrees

///////////////////////each pixel here represents the angle " tan ( getgrad(y)/getgrad(x) ) "///////////////////////	

	Mat gradient_x = Mat::zeros(image.rows, image.cols, CV_8UC1);
	Mat gradient_y = Mat::zeros(image.rows, image.cols, CV_8UC1);

	//blurred_image = threshold(blurred_image, threshold_initial);

	int i,j;
	//int k_erosion = 1; //kernel for erosion size=(5)*(5)
 	//int k_thin = 1; //kernel fot thinning is (3)*(3)

	for(i=0;i<image.rows;i++)
	{
		for(j=0;j<image.cols;j++)
		{
			gradient_x.at<uchar>(i,j) = abs(get_grad_x(blurred_image,i,j));
			gradient_y.at<uchar>(i,j) = abs(get_grad_y(blurred_image,i,j));
			
			gradient_mag.at<uchar>(i,j) = sqrt( pow(gradient_x.at<uchar>(i,j),2) + pow(gradient_y.at<uchar>(i,j),2) );
			
			if(gradient_x.at<uchar>(i,j)!=0)
			{
				gradient_dir.at<uchar>(i,j) = (  180 * (float)atan( get_grad_y(blurred_image,i,j) / get_grad_x(blurred_image,i,j) ) )/(M_PI) + 90 ;
			}

			else
			{
				gradient_dir.at<uchar>(i,j) = 180; ;
			}
		}
	}


	//gradient = threshold(gradient,threshold_initial);
	//eroded_gradient = erode(gradient,k_erosion);

	thresholded_gradient = threshold(gradient_mag,threshold_initial);
		
	thinned_grad = thin(thresholded_gradient ,gradient_dir, gradient_mag);

	//thinned_grad = threshold_set_max(thinned_grad,threshold_value_final);

	hysteresis_mat = hysteresis(thinned_grad, gradient_dir, gradient_mag, threshold_val_lower,threshold_val_upper);

	//namedWindow("gradient_image_x",CV_WINDOW_FREERATIO);
	//namedWindow("gradient_image_y",CV_WINDOW_FREERATIO);
	//namedWindow("gradient_image_dir",CV_WINDOW_FREERATIO);

	//imshow("gradient_image_x",gradient_x);
	//imshow("gradient_image_y",gradient_y);
	//imshow("gradient_image_dir",gradient_dir);

	namedWindow("gradient_image",CV_WINDOW_FREERATIO);
	namedWindow("original_image",CV_WINDOW_FREERATIO);
	namedWindow("blurred_image",CV_WINDOW_FREERATIO);
	namedWindow("threshlded_gradient_image",CV_WINDOW_FREERATIO);
	namedWindow("thinned_gradient_image",CV_WINDOW_FREERATIO);
	//namedWindow("thresholded_thin_image",CV_WINDOW_FREERATIO);
	namedWindow("thresholded_thin_hysteresis_image",CV_WINDOW_FREERATIO);
	//namedWindow("eroded_gradient_image");

	imshow("original_image", image);
	imshow("blurred_image",blurred_image);
	imshow("gradient_image",gradient_mag);
	imshow("threshlded_gradient_image",thresholded_gradient);
	imshow("thinned_gradient_image",thinned_grad);
	//imshow("thresholded_thin_image",thinned_grad);
	imshow("thresholded_thin_hysteresis_image",hysteresis_mat);

	waitKey(0);
	destroyAllWindows();
	
	return 0;
}




int get_grad_x(Mat image, int i, int j)
{
	int r,c;
	int sum=0;
	for(r=-1;r<=1;r++)
	{
		for(c=-1;c<=1;c++)
		{
			sum = sum + (sobel_x[r+1][c+1] * image.at<uchar>(i+r,j+c));
		}
	}

	sum = (sum/8);
	return sum;
}



int get_grad_y(Mat image, int i, int j)
{
	int r,c;
	int sum=0;
	for(r=-1;r<=1;r++)
	{
		for(c=-1;c<=1;c++)
		{
			sum = sum + (sobel_y[r+1][c+1] * image.at<uchar>(i+r,j+c));
		}
	}

	sum = (sum/8);
	return sum;
}


Mat threshold(Mat gradient_mag, int threshold_initial)
{
	int i,j;
	Mat temp = Mat::zeros(image.rows, image.cols, CV_8UC1);
	
	for(i=0;i<gradient_mag.rows;i++)
	{
		for(j=0;j<gradient_mag.cols;j++)
		{
			if(gradient_mag.at<uchar>(i,j) > threshold_initial
		)
			{
				temp.at<uchar>(i,j) = gradient_mag.at<uchar>(i,j);
			}

			else
			{
				temp.at<uchar>(i,j)=0;
			}
		}
	}

return temp;

}


Mat thin(Mat thresholded_gradient ,Mat gradient_dir, Mat gradient_mag)
{
	int r,c,grad_mag,grad_dir;

	Mat temp = Mat::zeros(thresholded_gradient.rows, thresholded_gradient.cols, CV_8UC1);

	for(r=1;r<thresholded_gradient.rows-1;r++)
	{
		for(c=1;c<thresholded_gradient.cols-1;c++)
		{
			
			grad_mag = gradient_mag.at<uchar>(r,c);

			grad_dir = gradient_dir.at<uchar>(r,c);
			
			if(thresholded_gradient.at<uchar>(r,c)!=0)
			{
				if(grad_dir <= 112 && grad_dir >= 68)
				{
					if((gradient_mag.at<uchar>(r,c-1) > grad_mag) || (gradient_mag.at<uchar>(r,c+1) > grad_mag))
					{
						temp.at<uchar>(r,c) = 0;
					}

					else
					{
						temp.at<uchar>(r,c) = thresholded_gradient.at<uchar>(r,c);
					}
				}

				else if(grad_dir <= 67 && grad_dir >= 22)
				{
					if((gradient_mag.at<uchar>(r-1,c+1) > grad_mag) || (gradient_mag.at<uchar>(r+1,c-1) > grad_mag))
					{
						temp.at<uchar>(r,c) = 0;
					}

					else
					{
						temp.at<uchar>(r,c) = thresholded_gradient.at<uchar>(r,c);
					}
				}

				else if(grad_dir >= 113 && grad_dir <= 158)
				{
					if((gradient_mag.at<uchar>(r+1,c-1) > grad_mag) || (gradient_mag.at<uchar>(r-1,c+1) > grad_mag))
					{
						temp.at<uchar>(r,c) = 0;
					}

					else
					{
						temp.at<uchar>(r,c) = thresholded_gradient.at<uchar>(r,c);
					}
				}
			
				else //0 to 21 or 159 to 180
				{
					if((gradient_mag.at<uchar>(r-1,c) > grad_mag) || (gradient_mag.at<uchar>(r+1,c) > grad_mag))
					{
						temp.at<uchar>(r,c) = 0;
					}

					else
					{
						temp.at<uchar>(r,c) = thresholded_gradient.at<uchar>(r,c);
					}
				}

			}	
		}
	}

return temp;
}

//BC, I FORGOT TO WRITE "return temp" and was getting seg fault, this bugged me for atleast 3 hours
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!take care of this load in future!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

Mat hysteresis(Mat thinned_grad, Mat gradient_dir, Mat gradient_mag, int threshold_val_lower, int threshold_val_upper)
{
	int r,c,grad_mag,grad_dir,flag;

	Mat temp = Mat::zeros(thinned_grad.rows, thinned_grad.cols, CV_8UC1);

	for(r=1;r<thinned_grad.rows-1;r++)
	{
		for(c=1;c<thinned_grad.cols-1;c++)
		{
			
			grad_mag = gradient_mag.at<uchar>(r,c);

			grad_dir = gradient_dir.at<uchar>(r,c);
			
			if(thinned_grad.at<uchar>(r,c)!=0) //edge pixel
			{
				
				if(thinned_grad.at<uchar>(r,c) > threshold_val_upper)
				{	
					
					if(grad_dir <= 112 && grad_dir >= 68)
					{
							
						if(thinned_grad.at<uchar>(r-1,c)<threshold_val_upper)
						{	
							if((gradient_mag.at<uchar>(r-1,c) > threshold_val_lower))
							{
								temp.at<uchar>(r-1,c) = 255;
							}

							else
							{
								temp.at<uchar>(r-1,c) = 0;
							}
						}

						else
						{
							temp.at<uchar>(r-1,c) = 255;
						}

						if(thinned_grad.at<uchar>(r+1,c)<threshold_val_upper)
						{	
							if((gradient_mag.at<uchar>(r+1,c) > threshold_val_lower))
							{
								temp.at<uchar>(r+1,c) = 255;
							}

							else
							{
								temp.at<uchar>(r+1,c) = 0;
							}
						}

						else
						{
							temp.at<uchar>(r+1,c) = 255;
						}
					
					}

					

					else if(grad_dir <= 67 && grad_dir >= 22)
					{
						if(thinned_grad.at<uchar>(r-1,c-1)<threshold_val_upper)
						{	
							if((gradient_mag.at<uchar>(r-1,c) > threshold_val_lower))
							{
								temp.at<uchar>(r-1,c-1) = 255;
							}

							else
							{
								temp.at<uchar>(r-1,c) = 0;
							}
						}

						else
						{
							temp.at<uchar>(r-1,c-1) = 255;
						}

						if(thinned_grad.at<uchar>(r+1,c+1)<threshold_val_upper)
						{	
							if((gradient_mag.at<uchar>(r+1,c+1) > threshold_val_lower))
							{
								temp.at<uchar>(r+1,c+1) = 255;
							}

							else
							{
								temp.at<uchar>(r+1,c+1) = 0;
							}
						}

						else
						{
							temp.at<uchar>(r+1,c+1) = 255;	
						}
					}

					else if(grad_dir >= 113 && grad_dir <= 158)
					{
						if(thinned_grad.at<uchar>(r-1,c+1)<threshold_val_upper)
						{	
							if((gradient_mag.at<uchar>(r-1,c+1) > threshold_val_lower))
							{
								temp.at<uchar>(r-1,c+1) = 255;
							}

							else
							{
								temp.at<uchar>(r-1,c+1) = 0;
							}
						}

						else
						{
							temp.at<uchar>(r-1,c+1) = 255;
						}


						if(thinned_grad.at<uchar>(r+1,c-1)<threshold_val_upper)
						{	
							if((gradient_mag.at<uchar>(r+1,c-1) > threshold_val_lower))
							{
								temp.at<uchar>(r+1,c-1) = 255;
							}

							else
							{
								temp.at<uchar>(r+1,c-1) = 0;
							}
						}

						else
						{
							temp.at<uchar>(r+1,c-1) = 255;
						}
					}
				
					else //0 to 21 or 159 to 180
					{
						if(thinned_grad.at<uchar>(r,c-1)<threshold_val_upper)
						{	
							if((gradient_mag.at<uchar>(r,c-1) > threshold_val_lower))
							{
								temp.at<uchar>(r,c-1) = 255;
							}

							else
							{
								temp.at<uchar>(r,c-1) = 0;
							}
						}

						else
						{
							temp.at<uchar>(r,c-1) = 255;
						}

						
						if(thinned_grad.at<uchar>(r,c+1)<threshold_val_upper)
						{	
							if((gradient_mag.at<uchar>(r,c+1) > threshold_val_lower))
							{
								temp.at<uchar>(r,c+1) = 255;
							}

							else
							{
								temp.at<uchar>(r,c+1) = 0;
							}
						}

						else
						{
							temp.at<uchar>(r,c-1) = 255;
						}
					}
				}

				

				else if(thinned_grad.at<uchar>(r,c) > threshold_val_lower)
				{
					if(grad_dir <= 112 && grad_dir >= 68)
					{
							
						if(thinned_grad.at<uchar>(r-1,c)>threshold_val_upper)
						{	
							temp.at<uchar>(r,c) = 255;
						}

						if(thinned_grad.at<uchar>(r+1,c)>threshold_val_upper)
						{	
							temp.at<uchar>(r,c) = 255;
						}
					
					}

					

					else if(grad_dir <= 67 && grad_dir >= 22)
					{
						if(thinned_grad.at<uchar>(r-1,c-1)>threshold_val_upper)
						{	
							temp.at<uchar>(r,c) = 255;
						}

						if(thinned_grad.at<uchar>(r+1,c+1)>threshold_val_upper)
						{	
							temp.at<uchar>(r,c) = 255;
						}
					}

					else if(grad_dir >= 113 && grad_dir <= 158)
					{
						if(thinned_grad.at<uchar>(r-1,c+1)>threshold_val_upper)
						{	
							temp.at<uchar>(r,c) = 255;
						}

						if(thinned_grad.at<uchar>(r+1,c-1)>threshold_val_upper)
						{	
							temp.at<uchar>(r,c) = 255;
						}
					}
				
					else //0 to 21 or 159 to 180
					{
						if(thinned_grad.at<uchar>(r,c-1)>threshold_val_upper)
						{	
							temp.at<uchar>(r,c) = 255;
						}

						if(thinned_grad.at<uchar>(r,c+1)>threshold_val_upper)
						{	
							temp.at<uchar>(r,c) = 255;
						}
					}
				}

				else
				{
					temp.at<uchar>(r,c) = 0;
				}


			}	
		}
	}

return temp;
}



/*Mat threshold_set_max(Mat thinned_grad, int threshold_value_final)
{
	int i,j;
	Mat temp = Mat::zeros(image.rows, image.cols, CV_8UC1);
	
	for(i=0;i<thinned_grad.rows;i++)
	{
		for(j=0;j<thinned_grad.cols;j++)
		{
			if(thinned_grad.at<uchar>(i,j) > threshold_value_final)
			{
				temp.at<uchar>(i,j) = 255;
			}

			else
			{
				temp.at<uchar>(i,j)=thinned_grad.at<uchar>(i,j);
			}
		}
	}

return temp;

}*/