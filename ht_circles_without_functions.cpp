//backchod test
#include<opencv2/opencv.hpp>
#include <stdio.h>   
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <math.h>

#include <opencv2/core/core.hpp>

#include <opencv2/imgproc.hpp> // drawing shapes

//# define M_PI 3.14159265358979323846

using namespace std;
using namespace cv;

//void make_circle(/*Mat hough_accumulator,*/ Point centre , int radiusCircle, int colorCircle, int thicknessCircle);

//Mat image;
//Mat hough_accumulator;

int main()
{
	Mat image = imread("circles.jpg",CV_LOAD_IMAGE_COLOR);
	Mat hough_accumulator = Mat::zeros(image.rows, image.cols, CV_16UC1);

	Mat detected_edges = Mat::zeros(image.rows, image.cols, CV_8UC1);
	//Mat blurred_image = Mat::zeros(image.rows, image.cols, CV_8UC1); 

	//int k_blur = 3; //for median blur	
	int lowThreshold = 150;
	int ratio = 3;

	int kernel_size = 3; //for canny

	//medianBlur(image, blurred_image, k_blur);

	namedWindow("original image",CV_WINDOW_FREERATIO);
	namedWindow("canny image",CV_WINDOW_FREERATIO);
	//namedWindow("blurred image");

	Canny( image, detected_edges, lowThreshold, (lowThreshold*ratio), kernel_size );
	//Canny( blurred_image, detected_edges, lowThreshold, (lowThreshold*ratio), kernel_size );

	imshow("original image", image);
	//imshow("blurred image",blurred_image);
	imshow("canny image", detected_edges);

	//detected_edges mat is the canny edge image



	//**************************************************************************************
	//***********************CODE FOR HOUGH TRANSFORM FOR CIRCLES*****************************
	//**************************************************************************************

	
	int a,b,R; //R is the radius of the circle

	int r,c;

	int threshold_for_circle = 1000; //if the number of votes by pixels exceeds this number, then this line is eligible to be made

	Point centre_circle[10]; //assume maximum number of circles in the picture is 10
	int radius_circle[10]; //radius of the corresponding circles detected
	//int count; //for the number of circles

	int k;
	
	/* for(k=0;k<10;k++)
	{
		radius_circle[k] = 20;
	}
 */

///////////////////CODE FOR VOTING///////////////////
	
	//assume maximum radius of the circle in the image is 400
	//int i = 1; //the pixel intensity of the circle constructed
	int radiusCircle;
	int colorCircle = 2;
	Point centre;

	for(R=95;R<105;R++)
	{
		for(r=0;r<image.rows;r++)
		{
			for(c=0;c<image.cols;c++)
			{
				if(detected_edges.at<uchar>(r,c)!= 0) //edge pixel 
				{
					radiusCircle = R;
					centre.x = c;
					centre.y = r;

					int r1,c11,c12;
					
					for(r1=c-R;r1<=(c+R);r1++)
					{
						/*for(c1=0;c1<hough_accumulator.cols;c1++)
						{
							if( (pow((c1-centre.x),2) + (pow((r1-centre.y),2)) > (pow(radiusCircle,2) - 5) ) && (pow((c1-centre.x),2) + (pow((r1-centre.y),2)) > (pow(radiusCircle,2) - 5 )  ) )
							{
								hough_accumulator.at<uchar>(r1,c1)+=2;
							}
						}*/
						if(pow(radiusCircle,2)-(pow((r1-centre.y),2))>0)
						{
							c11=c+sqrt(pow(radiusCircle,2)-pow((r1-centre.y),2));
							c12=c-sqrt(pow(radiusCircle,2)-pow((r1-centre.y),2));
							
							if(c11>0&&c11<hough_accumulator.cols)
								hough_accumulator.at<uchar>(r1,c11)+=1;

							if(c12>0&&c12<hough_accumulator.cols)
								hough_accumulator.at<uchar>(r1,c12)+=1;
						}
					}
									//increments the pixels on the circumfrence's pixel value by 1 everytime voting happens
				}
			}
		}
	}	
	
	int iterator;

	/*int theeta_reqd[10000]; //gives us the theeta_value of the detected lines  
	int dist_reqd[10000];   ////gives us the dist_value of the detected lines  
	*/
	int count= 0; //finally, the value of count will give us the number of circles detected

	namedWindow("hough accumulator",CV_WINDOW_FREERATIO);
	imshow("hough accumulator", hough_accumulator);

 //////////////////CODE TO CHECK WHICH LINES ARE VALID LINES////////////////////
	for(r=0;r<hough_accumulator.rows;r++)
	{
		for(c=0;c<hough_accumulator.cols;c++)
		{
			if(hough_accumulator.at<uchar>(r,c) > threshold_for_circle)
			{
				//pointer_HS = &hough_accumulator[theeta][dist];
				centre_circle[count].x = c;
				centre_circle[count].y = r;

cout<<"hello"<<endl;
				count++;
			}	
		}
	}



	//hence the deteted line is given by dist_reqd = (c)cos(theeta_reqd) - (r)sin(theeta_reqd) ;
	
	for(iterator=0;iterator<count;iterator++)
	{
		Scalar color_hough(0,255,0); // Green
	    int thickness = 2;

		circle(image, centre_circle[iterator] , 10, color_hough, thickness);
		cout<<"done"<<endl;
	}

	imshow("original image", image);
	waitKey(0);
	destroyAllWindows();

}

