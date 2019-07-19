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

Mat img = imread("data5.jpeg",1);

int main()
{
    Mat dst, cdst;
    int lowThreshold = 150;
    
    
    Mat hsvy,bwy;
    //Mat hsvw,bww;
	cvtColor(img,hsvy,COLOR_BGR2HSV);
    //cvtColor(img,hsvw,COLOR_BGR2HSV);
    //thresholding for yellow lanes
	inRange(hsvy,Scalar(0,100,100),Scalar(24,255,255),bwy);
    //inRange(hsvw,Scalar(0,0,100),Scalar(255,255,255),bww);

    Mat yellow_mask = Mat::zeros(img.rows, img.cols, CV_8UC1);
    Mat white_mask = Mat::zeros(img.rows, img.cols, CV_8UC1);
    Mat white_or_yellow = Mat::zeros(img.rows, img.cols, CV_8UC1);

    int i,j;

    int count = 0;

    for(i=img.rows-1;i>(img.rows/2);i--)
    {
        for(j=count;j<img.cols-count;j++)
        {
            if(bwy.at<uchar>(i,j)!=0)
            {
                yellow_mask.at<uchar>(i,j) = 255;// = img.at<Vec3b>(i,j);
            }

            if(img.at<Vec3b>(i,j)[0]>170 && img.at<Vec3b>(i,j)[1]>170 && img.at<Vec3b>(i,j)[2]>170)
            //if(bww.at<uchar>(i,j)!=0)
            {
                white_mask.at<uchar>(i,j) = 255;// = img.at<Vec3b>(i,j); 
            }

            if(yellow_mask.at<uchar>(i,j) || white_mask.at<uchar>(i,j))
            {
                white_or_yellow.at<uchar>(i,j) = 255;
            }
        }
        count++;
    }

    namedWindow("original image",WINDOW_FREERATIO);
  	/*namedWindow("white mask",WINDOW_FREERATIO);
    namedWindow("yellow mask",WINDOW_FREERATIO);*/
    namedWindow("yellow and white mask",WINDOW_FREERATIO);

    /*imshow("white mask",white_mask);
    imshow("yellow mask",yellow_mask);*/
    imshow("original image",img);
    imshow("yellow and white mask",white_or_yellow); 

    imshow("original image",img);

    

    Canny(white_or_yellow, dst, lowThreshold, (3*lowThreshold), 3);
    cvtColor(dst, cdst, CV_GRAY2BGR);
    
    int threshold_for_line = 40;
    //vector<Vec2f> lines;
    //a dynamic array of points, 0th index is for rho(dist), 1st index is for theeta(angle)
    //HoughLines(dst, lines, 1, CV_PI/180, threshold_for_line, 0, 0 );

    /*
    dst: Output of the edge detector. It should be a grayscale image (although in fact it is a binary one)
    lines: A vector that will store the parameters (r,\theta) of the detected lines
    rho : The resolution of the parameter r in pixels. We use 1 pixel.
    theta: The resolution of the parameter \theta in radians. We use 1 degree (CV_PI/180)
    threshold: The minimum number of intersections to “detect” a line
    srn and stn: Default parameters to zero. Check OpenCV reference for more info
    */

    
    namedWindow("canny",WINDOW_FREERATIO);
    imshow("canny",dst);
    
    
    //for loop iterates for the number of lines detected, that is lines.size()
    /* for( size_t i = 0; i < lines.size(); i++ )
    {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        line( cdst, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
    }
    */

    vector<Vec4i> lines;
    HoughLinesP(dst, lines, 1, CV_PI/180, threshold_for_line, 50, 50 );
    
    for( size_t i = 0; i < lines.size(); i++ )
    {
        Vec4i l = lines[i];
        line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
        line( img, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
    }

    namedWindow("hough",WINDOW_FREERATIO);
    imshow("hough",cdst);

    namedWindow("detected lanes",WINDOW_FREERATIO);
    imshow("detected lanes", img);

    waitKey(0);
    destroyAllWindows();
}