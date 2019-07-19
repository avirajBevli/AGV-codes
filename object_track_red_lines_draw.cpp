//in this program, intially set the trackbar sliders such that 
//only the object u want to track is visible in white 
//and rest everything is in black

//then compile the program again after updating the values of iLowH, iHighH, iLowS,.....
//now, u will be able to see the red lines formed when u move the object from one point to another/*
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

 int main( int argc, char** argv )
{
    VideoCapture cap(0); //capture the video from webcam

    if ( !cap.isOpened() )  // if not success, exit program
    {
         cout << "Cannot open the web cam" << endl;
         return -1;
    }

    namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

    //these values i have calculated for a pink bottle that i was testing the code on
    //for other objects, change these values first 
    int iLowH = 127;
    int iHighH = 179;

    int iLowS = 33; 
    int iHighS = 255;

    int iLowV = 54;
    int iHighV = 208;

  //Create trackbars in "Control" window
    createTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
    createTrackbar("HighH", "Control", &iHighH, 179);

    createTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
    createTrackbar("HighS", "Control", &iHighS, 255);

    createTrackbar("LowV", "Control", &iLowV, 255);//Value (0 - 255)
    createTrackbar("HighV", "Control", &iHighV, 255);

    int iLastX = -1; 
    int iLastY = -1;

  //Capture a temporary image from the camera
    Mat imgTmp;
    cap.read(imgTmp); 

  //Create a black image with the size as the camera output
    Mat imgLines = Mat::zeros( imgTmp.size(), CV_8UC3 );;
 

    while (true)
    {
        Mat imgOriginal;

        bool bSuccess = cap.read(imgOriginal); // read a new frame from video

        if (!bSuccess) //if not success, break loop
        {
             cout << "Cannot read a frame from video stream" << endl;
             break;
        }

        Mat imgHSV;

        cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
 
        Mat imgThresholded;

        inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
      
  //morphological opening (removes small objects from the foreground)

        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

   //morphological closing (removes small holes from the foreground)
        dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

   //Calculate the moments of the thresholded image
        Moments oMoments = moments(imgThresholded);

        double dM01 = oMoments.m01;
        double dM10 = oMoments.m10;
        double dArea = oMoments.m00; //sum of all bright pixels' pixel values i.e (255*(no of bright pixels))

   // if the area <= 10000, I consider that the there are no object in the image and it's because of the noise, the area is not zero 
        if (dArea > 10000)
        {
         //calculate the position of the ball
           int posX = dM10 / dArea; //x coordinate of the COM of the ball
           int posY = dM01 / dArea; //y coordinate of the COM of the ball
                
               if (iLastX >= 0 && iLastY >= 0 && posX >= 0 && posY >= 0)
               {
                //Draw a red line from the previous point to the current point
                  line(imgLines, Point(posX, posY), Point(iLastX, iLastY), Scalar(0,0,255), 2);
               }

           iLastX = posX;
           iLastY = posY;
        }

        imshow("Thresholded Image", imgThresholded); //show the thresholded image

        imgOriginal = imgOriginal + imgLines;
        //**********we can directly add two images(blending two images) in opencv c++*********

        imshow("Original", imgOriginal); //show the original image

        if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
        {
            cout << "esc key is pressed by user" << endl;
            break; 
        }
    }

    return 0;
}


/*
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

 int main( int argc, char** argv )
{
    VideoCapture cap(0); //capture the video from webcam

    if ( !cap.isOpened() )  // if not success, exit program
    {
         cout << "Cannot open the web cam" << endl;
         return -1;
    }

    namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

    //these values i have calculated for a pink bottle that i was testing the code on
    //for other objects, change these values first 
    int iLowH = 127;
    int iHighH = 179;

    int iLowS = 33; 
    int iHighS = 255;

    int iLowV = 54;
    int iHighV = 208;

  //Create trackbars in "Control" window
    createTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
    createTrackbar("HighH", "Control", &iHighH, 179);

    createTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
    createTrackbar("HighS", "Control", &iHighS, 255);

    createTrackbar("LowV", "Control", &iLowV, 255);//Value (0 - 255)
    createTrackbar("HighV", "Control", &iHighV, 255);

    int iLastX = -1; 
    int iLastY = -1;

  //Capture a temporary image from the camera
    Mat imgTmp;
    cap.read(imgTmp); 

  //Create a black image with the size as the camera output
    Mat imgLines = Mat::zeros( imgTmp.size(), CV_8UC3 );;
 

    while (true)
    {
        Mat imgOriginal;

        bool bSuccess = cap.read(imgOriginal); // read a new frame from video

        if (!bSuccess) //if not success, break loop
        {
             cout << "Cannot read a frame from video stream" << endl;
             break;
        }

        Mat imgHSV;

        cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
 
        Mat imgThresholded;

        inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
      
  //morphological opening (removes small objects from the foreground)

        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

   //morphological closing (removes small holes from the foreground)
        dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

   //Calculate the moments of the thresholded image
        Moments oMoments = moments(imgThresholded);

        double dM01 = oMoments.m01;
        double dM10 = oMoments.m10;
        double dArea = oMoments.m00; //sum of all bright pixels' pixel values i.e (255*(no of bright pixels))

   // if the area <= 10000, I consider that the there are no object in the image and it's because of the noise, the area is not zero 
        if (dArea > 10000)
        {
         //calculate the position of the ball
           int posX = dM10 / dArea; //x coordinate of the COM of the ball
           int posY = dM01 / dArea; //y coordinate of the COM of the ball
                
           int i,j;     
           for(i=-1;i<=1;i++)
           {
              for(j=-1;j<=1;j++)
              {
                  imgLines.at<Vec3b>(posX+i,posY+j)[1] = 255;
              }
           }      
        }

        imshow("Thresholded Image", imgThresholded); //show the thresholded image

        imgOriginal = imgOriginal + imgLines;
        //**********we can directly add two images(blending two images) in opencv c++*********

        imshow("Original", imgOriginal); //show the original image

        if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
        {
            cout << "esc key is pressed by user" << endl;
            break; 
        }
    }

    return 0;
}
*/