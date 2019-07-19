#include<opencv2/opencv.hpp>
using namespace cv;
#include <stdio.h>   
#include <iostream>
using namespace std;

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp> // drawing shapes

int main(int argc, char **argv)
{
	Mat image(500,500, CV_8UC3, Scalar(0,0,0));
 	if( !image.data )
 	{
        std::cout <<  "Could not open or find the image" << std::endl ;
        exit(EXIT_FAILURE);
    }

        //####################(  Draw Line  )##########################
    Point p1(100,100), p2(200,100);
    Scalar colorLine(0,255,0); // Green
    int thicknessLine = 2;
    
    line(image, p1, p2, colorLine, thicknessLine);
    
    //####################(  Draw Circle  )#########################
    // unfilled circle
    Point centerCircle1(250,250);
    int radiusCircle = 30;
    Scalar colorCircle1(0,0,255);
    int thicknessCircle1 = 2;
    
    circle(image, centerCircle1, radiusCircle, colorCircle1, thicknessCircle1);
    
    // filled circle
    Point centerCircle2(400,100);
    Scalar colorCircle2(0,100,0);
    
    circle(image, centerCircle2, radiusCircle, colorCircle2, CV_FILLED);
    
    //####################(  Draw Rectangle  )#######################
    // unfilled 
    Point p3(400,400), p4(450,450);
    Scalar colorRectangle1(0,0,255);
    int thicknessRectangle1 = 3;
    
    rectangle(image, p3, p4, colorRectangle1,thicknessRectangle1);
    
    //   filled
    Point p5(100,400), p6(150,450);
    Scalar colorRectangle2(255,0,255);
     
    rectangle(image, p5, p6, colorRectangle2, CV_FILLED);
    
    
    //#################( Draw Shapes on Image )######################
    namedWindow( "Display window", cv::WINDOW_AUTOSIZE );
    imshow( "Display window", image );                  

    waitKey(0);   
    
    
    return 0;

}