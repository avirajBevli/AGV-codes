//implement trackbar using call back
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace std;
using namespace cv;

Mat src;

void MyCallbackForBrightness(int iValueForBrightness, void *userData)
{
     Mat dst;
     int iValueForContrast = *( static_cast<int*>(userData) );

      //Calculating brightness and contrast value
     int iBrightness = iValueForBrightness - 50;
     double dContrast = iValueForContrast / 50.0;

      //Calculated contrast and brightness value
     cout << "MyCallbackForBrightness : Contrast=" << dContrast << ", Brightness=" << iBrightness << endl;

     //adjust the brightness and contrast
     src.convertTo(dst, -1, dContrast, iBrightness); 

     //show the brightness and contrast adjusted image
     imshow("My Window", dst);
}


void MyCallbackForContrast(int iValueForContrast, void *userData)
{
     Mat dst;
     int iValueForBrightness = *( static_cast<int*>(userData) );

      //Calculating brightness and contrast value
     int iBrightness = iValueForBrightness - 50;
     double dContrast = iValueForContrast / 50.0;

      //Calculated contrast and brightness value
     cout << "MyCallbackForContrast : Contrast=" << dContrast << ", Brightness=" << iBrightness << endl;

      //adjust the brightness and contrast
     src.convertTo(dst, -1, dContrast, iBrightness); 

      //show the brightness and contrast adjusted image
     imshow("My Window", dst);
}



int main(int argc, char** argv)
{
     // Read original image 
    src = imread("sushmita.jpg");

      //if fail to read the image
    if (src.data == false) 
    { 
          cout << "Error loading the image" << endl;
          return -1; 
    }

    // Create a window
    namedWindow("My Window", WINDOW_NORMAL);

    int iValueForBrightness = 50;
    int iValueForContrast = 50;



//the 5th and 6th arguments of createTrackbar() are important ----------- ***********pay attention**********

/*
createTrackbar() has 6 arguments:
int createTrackbar(const string& trackbarname, const string& winname, int* value, int count, TrackbarCallback onChange = 0, void* userdata = 0)
This OpenCV function creates a trackbar and attached that trackbar to a specified window

trackbarname - The name of the trackbar
winname - The name of the window to which the trackbar is attached
value - This integer, pointed by this pointer, holds the value associated with the position of the trackbar
count - The maximum value of the trackbar. The minimum value is always zero.
onChange - This function will be called everytime the position of the trackbar is changed. The prototype of this function should be "FunctionName(int, void*)". The "int" value is the value associate with the position of the trackbar. And "void*" is any pointer value which you pass as the "userdata" (See the next parameter).
userdata - This pointer variable will be passed as the second parameter of the above function
*/




    //Create track bar to change brightness
    createTrackbar("Brightness", "My Window", &iValueForBrightness, 100, MyCallbackForBrightness, &iValueForContrast);

    //Create track bar to change contrast
    createTrackbar("Contrast", "My Window", &iValueForContrast, 100, MyCallbackForContrast, &iValueForBrightness);
  
    imshow("My Window", src);

    // Wait until user press some key
    waitKey(0);


    return 0;
}