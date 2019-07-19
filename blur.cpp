
#include<opencv2/opencv.hpp>
using namespace cv;
#include <stdio.h>   
#include <iostream>
#include <cmath>
using namespace std;


Mat blur(Mat img , int blur_kernel_size)
{
	int i,j,k,l;
	Mat temp = Mat::zeros(img.rows, img.cols, CV_8UC1);
	//cout<<"printing kernel"<<endl;

	int highest_elem = pow(2,(2*blur_kernel_size)); 

	if(blur_kernel_size > 0)
	{
		for(i = blur_kernel_size; i < img.rows - blur_kernel_size ; i++)
		{
			for(j = blur_kernel_size; j < img.cols - blur_kernel_size; j++)
			{
				int val = 0;
				int intenVal;
				int sum = 0;
				int qq;

				for(k = -blur_kernel_size; k < (blur_kernel_size+1); k++)
				{
					for(l = -blur_kernel_size; l < (blur_kernel_size+1); l++)
					{
						intenVal = img.at<uchar>(i+k , j+l);
						//float qq = pow(0.5, (abs(k)+abs(l)+(2*blur_kernel_size)));
						qq = highest_elem*(abs(k)+abs(l));

						sum = sum + qq; 

						intenVal = qq*intenVal;
						val += intenVal;
						//cout<<qq<<endl;
					}
				}

				temp.at<uchar>(i, j) = (val/sum);

			}
		}
		
	return temp;

	}	

	else
	{
		return img;
	}

}


Mat original;

int main(int argc, char **argv)
{
	
	original = imread("noisy.jpeg",CV_LOAD_IMAGE_GRAYSCALE);
	
	String windowName_original = "original lisa";
	String windowName_modified = "blurred lisa";
	
	namedWindow(windowName_original,CV_WINDOW_FREERATIO);
	namedWindow(windowName_modified,CV_WINDOW_FREERATIO);

	imshow(windowName_original,original);

	int blur_kernel_size = 1; //((2K+1)*(2K+1) is the kernel size chosen)

	createTrackbar("blur kernel size", windowName_original, &blur_kernel_size, 11);


	while (true)
	     {
	          Mat blurimg;
	          blurimg = blur(original , blur_kernel_size);

	          imshow(windowName_modified,blurimg);

	          // Wait until user press some key for 50ms
	          int iKey = waitKey(10);

	          //if user press 'ESC' key
	          if (iKey == 27)
	          {
	               break;
	          }
	     
		 	 // waitKey(0);
		     //destroyAllWindows();
	     }



	return 0;
}
