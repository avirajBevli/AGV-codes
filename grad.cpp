#include<opencv2/opencv.hpp>
#include <stdio.h>   
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
using namespace std;
using namespace cv;
 
int get_grad_x(Mat image, int i, int j, Mat gradient_x);
int get_grad_y(Mat image, int i, int j, Mat gradient_y);

//Mat blur(Mat img , int blur_kernel_size);
Mat threshold(Mat imag, int val);

//***********saale chutiye**************************************************************************
//***********kernels define karne ke lie humesha normal int array define kara kar*******************
//***********dont even think to define kernel in terms of Mat cuz Mat in an opencv datatype*********
//***********hence, dont do this chutiyaap!!!*******************************************************

int sobel_y[3][3] = {1,2,1,0,0,0,-1,-2,-1};

int sobel_x[3][3] = {1,0,-1,2,0,-2,1,0,-1};


int main()
{
	Mat image = imread("tiger.jpeg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat blurred_image = imread("tiger.jpeg", CV_LOAD_IMAGE_GRAYSCALE);

	int k = 3; //k is the "k"of kernel(k*k), not(2k+1 * 2k+1)

	GaussianBlur(image , blurred_image , Size(k,k), 0, 0);

	Mat gradient = Mat::zeros(image.rows, image.cols, CV_8UC1);
	Mat gradient_x = Mat::zeros(image.rows, image.cols, CV_8UC1);
	Mat gradient_y = Mat::zeros(image.rows, image.cols, CV_8UC1);

	int i,j;

	for(i=1;i<image.rows-1;i++)
	{
		for(j=1;j<image.cols-1;j++)
		{
			gradient_x.at<uchar>(i,j) = abs(get_grad_x(blurred_image,i,j,gradient_x));
			gradient_y.at<uchar>(i,j) = abs(get_grad_y(blurred_image,i,j,gradient_y));
			gradient.at<uchar>(i,j) = sqrt( pow(gradient_x.at<uchar>(i,j),2) + pow(gradient_y.at<uchar>(i,j),2) );
		}
	}

	//gradient = threshold(gradient,100);

	namedWindow("gradient_image");
	namedWindow("original_image");
	namedWindow("blurred_image");

	imshow("original_image", image);
	imshow("blurred_image",blurred_image);
	imshow("gradient_image",gradient);
	
	waitKey(0);
	destroyAllWindows();
	return 0;
}




int get_grad_x(Mat image, int i, int j, Mat gradient_x)
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



int get_grad_y(Mat image, int i, int j, Mat gradient_y)
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

/*
Mat blur(Mat img , int blur_kernel_size)
{
	
	Mat temp(img.rows, img.cols, CV_8UC1, Scalar(0));
	
	for(int i = blur_kernel_size; i < img.rows - blur_kernel_size ; ++i)
	{
		for(int j = blur_kernel_size; j < img.cols - blur_kernel_size; ++j)
		{
			int val = 0;

			for(int k = -blur_kernel_size; k < (blur_kernel_size+1); ++k)
			{
				for(int l = -blur_kernel_size; l < (blur_kernel_size+1); ++l)
				{
					int intenVal = img.at<uchar>(i+k , j+l);
					float qq = pow(0.5, (abs(k)+abs(l)+2));
					intenVal = (int)(qq*intenVal);
					val += intenVal;
				}
			}

			temp.at<uchar>(i, j) = val;

		}
	}
return temp;

}
*/

/*
Mat threshold(Mat image, int val)
{
	int i,j;
	Mat temp = Mat::zeros(image.rows, image.cols, CV_8UC1);
	
	for(i=0;i<image.rows;i++)
	{
		for(j=0;j<image.cols;j++)
		{
			if(image.at<uchar>(i,j)>val)
			{
				temp.at<uchar>(i,j)=255;
			}

			else
			{
				temp.at<uchar>(i,j)=0;
			}
		}
	}

return temp;

}
*/