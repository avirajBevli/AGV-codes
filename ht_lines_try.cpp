#include<opencv2/opencv.hpp>
#include <stdio.h>   
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <math.h>

#include <opencv2/core/core.hpp>

#include <opencv2/imgproc.hpp> // drawing shapes

# define M_PI 3.14159265358979323846

using namespace std;
using namespace cv;

vector<Point> p1;
vector<Point> p2;

void make_line(Mat image, int theeta, int dist)
{
	int r,c;
	int flag = 0;
	Point p1, p2;

	if(theeta != 90 && theeta!= 0)
	{
		r=0;
		c = ( (float)dist/(cos(theeta*M_PI/180.0)) )- (float)r*tan(theeta*M_PI/180.0);
		
		if((c<image.cols) && c>=0)
		{
			p1.x = c;
			p1.y = r;
			flag=1;
		}

		r = (image.rows-1);
		c = ( (float)dist/(cos(theeta*M_PI/180.0)) ) - (float)r*tan(theeta*M_PI/180.0);
		
		if(c<image.cols && c>=0)
		{
			if(flag==0)
			{
				p1.x = c;
				p1.y = r;
			}
			
			else
			{
				p2.x = c;
				p2.y = r;
			}
		}

		c = (image.cols-1);
		r = ( (float)dist/(sin(theeta*M_PI/180.0)) ) - ( (float)c/(tan(theeta*M_PI/180.0)) );
		
		if(r<image.rows && r>=0)
		{
			if(flag==0)
			{
				p1.x = c;
				p1.y = r;
			}
			
			else if(flag==1)
			{
				p2.x = c;
				p2.y = r;
			}
		}

		c = 0;
		r = ( (float)dist/(sin(theeta*M_PI/180.0)) ) - ( (float)c/(tan(theeta*M_PI/180.0)) );
		
		if(r<image.rows && r>=0)
		{
			if(flag==0)
			{
				p1.x = c;
				p1.y = r;
			}
			
			else if(flag==1)
			{
				p2.x = c;
				p2.y = r;
			}
		}

	}

	else if(theeta == 0)
	{
		c = ( (float)dist/(cos(theeta*M_PI/180.0)) );
		p1.x = c;
		p1.y = 0;
		p2.x = c;
		p2.y = (image.rows-1);
	}

	else
	{
		r = (float)dist/(sin(theeta*M_PI/180.0));
		p1.x = 0;
		p1.y = r;
		p2.x = (image.cols - 1);
		p2.y = r;
	}

	Scalar colorLine(0,255,0); // Green
	float thicknessLine = 0.5;

	imshow("original image",image);
	line(image, p1, p2, colorLine, thicknessLine);

}
 

Mat image = imread("lane.jpeg", CV_LOAD_IMAGE_COLOR);
Mat blurred_image = Mat::zeros(image.rows, image.cols, CV_8UC1);
Mat detected_edges = Mat::zeros(image.rows, image.cols, CV_8UC1);

int main()
{
	int k_blur = 3; //for median blur	
	int lowThreshold = 130;
	int ratio = 3;
	int kernel_size = 3; //for canny

	medianBlur(image, blurred_image, k_blur);

	namedWindow("original image");
	namedWindow("canny image");

	Canny( blurred_image, detected_edges, lowThreshold, (lowThreshold*ratio), kernel_size );

	imshow("original image", image);
	imshow("canny image", detected_edges);

	//detected_edges mat is the canny edge image



	//**************************************************************************************
	//***********************CODE FOR HOUGH TRANSFORM FOR LINES*****************************
	//**************************************************************************************

	Mat hough_accumulator = Mat::zeros(180,3000,CV_8UC1);//we have taken theeta from 0 to 180, hence , assumed that d can be both positive or negative
									//distance assumed to be maximum 3000 units, because it will work even for 
									//a ((1000/(root2))*(1000/(root2))) image
	int theeta,dist;

	int r,c;

	int threshold_for_line = 70; //if the number of votes by pixels exceeds this number, then this line is eligible to be made

	Point p;

///////////////////CODE FOR VOTING///////////////////
	for(r=0;r<image.rows;r++)
	{
		for(c=0;c<image.cols;c++)
		{
			if(detected_edges.at<uchar>(r,c)==255) //edge pixel 
			{
				for(theeta=0;theeta<180;theeta++)
				{
					//hough_accumulator[theeta][dist]++; after calcualating distance by d = xcos(theeta) + ysin(theeta)
					//But here, c==x; r==-y; hence , d = (c)(cos(theeta)) - (r)(sin(theeta))
					dist = ( (float)(c*cos((float)(theeta*M_PI)/180.0)) + (float)(r*sin((float)(theeta*M_PI)/180.0)) );

					hough_accumulator.at<uchar>(theeta,dist)++ ;

				}
			}
		}
	}
	
	int iterator;

	int theeta_reqd[10000]; //gives us the theeta_value of the detected lines  
	int dist_reqd[10000];   ////gives us the dist_value of the detected lines  
	
	int count= 0; //finally, the value of count will give us the number of lines detected

	namedWindow("hough accumulator");
	imshow("hough accumulator", hough_accumulator);

 //////////////////CODE TO CHECK WHICH LINES ARE VALID LINES////////////////////
	for(theeta=0;theeta<180;theeta++)
	{
		for(dist=0;dist<3000;dist++)
		{
			if(hough_accumulator.at<uchar>(theeta,dist) > threshold_for_line)
			{
				//pointer_HS = &hough_accumulator[theeta][dist];
				theeta_reqd[count] = theeta;
				dist_reqd[count] = dist;

				count++;
			}	
		}
	}



	//hence the deteted line is given by dist_reqd = (c)cos(theeta_reqd) - (r)sin(theeta_reqd) ;
	
	for(iterator=0;iterator<count;iterator++)
	{
		Scalar colorLine(0,255,0); // Green
	    int thicknessLine = 2;

	    //cout<<"hello"<<endl;

		make_line(image, theeta_reqd[iterator], dist_reqd[iterator]);
	}

	imshow("original image", image);
	waitKey(0);
	destroyAllWindows();

}

