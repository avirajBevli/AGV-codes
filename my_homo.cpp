#include <bits/stdc++.h>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include "/usr/include/eigen3/Eigen/Dense"
#include "/usr/include/eigen3/Eigen/Core"

using namespace std;
using namespace cv;

vector<Point2f> fv_points; //image will be of the front view
//these are the series of points which we will mark in the front view image 
//for training, i.e to make the homography matrix, we take 8 points

vector<Point2f> tv_points;//these are the points which are supposed to be in the place defined here according to our calculations
//these are the correspoding 8 points , that we will input on our own

Mat img;
//Mat img1;
/*Mat h;*/

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
    if  ( event == EVENT_RBUTTONDOWN )
    {
        cout << "(" << x << ", " << y << ")" << endl;
        fv_points.push_back(Point2f(x,y));

        if (fv_points.size() == 4)
        {
            Mat top(img.rows, img.cols, CV_8UC3, Scalar(0,0,0));
            //Mat top1(img.rows, img.cols, CV_8UC3, Scalar(0,0,0));

            Mat h = findHomography(fv_points, tv_points);
        
            cout << "homography matrix" << endl;
            cout << h << endl;

            // cout << "-------------------------" << endl;
            // cout << "pixelpermetre " << ppcm*100 << endl;

            // cout << "yshift " << botfront_firstview << endl;

            warpPerspective(img, top, h, Size(img.cols, img.rows));
            //warpPerspective(img1, top1, h, Size(img.cols, img.rows));

            namedWindow("top view", 0);
            //namedWindow("top view1", 0);
            imshow("top view", top);
            //imshow("top view1", top1);
            waitKey(0);
        }
    }
    
}

int main(int argc, char** argv)
{
    img = imread("calibration.jpg",1);
    //img1 = imread("test.jpg",1);
    //resize(img, img, Size(img.cols/4, img.rows/4));
    /*Mat front = imread("front.jpg",1);

 	Mat front_top(front.rows, front.cols, CV_8UC3, Scalar(0,0,0));
*/
 	namedWindow("front view", 0);
 	//namedWindow("front view1", 0);
    namedWindow("top view", 0);

    float pp_cm_y = (img.rows/105.5); //pixels per cm

    //int c3, c4, c5, c6, c7, c8;

//input after measuring the distances of the 4 vertices physically
    tv_points.push_back( Point2f(0, 88*pp_cm_y) );
    tv_points.push_back( Point2f(img.cols-1, 88*pp_cm_y) );
    tv_points.push_back( Point2f(img.cols-1, 67*pp_cm_y) );
    tv_points.push_back( Point2f(0, 67*pp_cm_y) );
    /*tv_points.push_back( Point2f(0, 79.5*pp_cm_y) );
    tv_points.push_back( Point2f(img.cols-1, 79.5*pp_cm_y) );
    tv_points.push_back( Point2f(img.cols-1, 73*pp_cm_y) );
    tv_points.push_back( Point2f(0, 73*pp_cm_y) );
    */

    setMouseCallback("front view", CallBackFunc, NULL);
 
 	//imshow("front view1", img1);
    imshow("front view", img);
   
    /*warpPerspective(front, front_top, h, Size(img.rows, img.cols));

    namedWindow("top view", 0);
    imshow("top view", front_top);*/
    waitKey(0);
 
    return 0;
 
}
