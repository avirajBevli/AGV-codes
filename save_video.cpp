#include<opencv2/opencv.hpp>
#include<iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	
	//VideoCapture is the class and cap is the object of that class

	VideoCapture cap(0);
	//opens the defaut video camera

	if(cap.isOpened() == false)
	{
		cout << "cannot open the video camera" <<endl;
		cin.get(); //wait for any key press
        return -1;
	}

	int frame_width = static_cast<int>(cap.get(CAP_PROP_FRAME_WIDTH));  //get the width of frames of the video
 
	int frame_height = static_cast<int>(cap.get(CAP_PROP_FRAME_HEIGHT));

	Size frame_size(frame_width, frame_height);
    int frames_per_second = 10;

    String window_name = "sample_video.mp4";

    VideoWriter oVideoWriter(window_name, VideoWriter::fourcc('M', 'J', 'P', 'G'), frames_per_second, frame_size, true); 

   	/*
	VideoWriter(const String& filename, int fourcc, double fps, Size frameSize, bool isColor = true)
	This is one of the available overloaded constructors of the VideoWriter object. It constructs and initializes the VideoWriter object in order to write video frames to a given file.

	filename - Name of the file to write video frames.

	fourcc - 4-character code of the codec which is used to compress the video. The complete list of codes can be found in this page. But most of the codecs listed in this page might not work in your computer. These are some popular codecs which might work for you.
	VideoWriter::fourcc('P', 'I', 'M', '1') for MPEG-1
	VideoWriter::fourcc('M', 'J', 'P', 'G') for Motion JPEG
	VideoWriter::fourcc('M', 'P', '4', '2') for MPEG-4 variation of Microsoft

	fps - Frames per second of the written video stream.

	frameSize - Size of the video frames written to this video stream

	isColor - Always pass true to this argument
	   	*/

//no need to make the file yourself (to which to write the video frames) ..... it is automatically made 

/*   	
In each iteration of the above while loop, the program performs following tasks.
Read a frame from the camera.
Write the frame to the file.
Display the frame in a window.
The while loop will break if the Esc key is pressed or the program fails to read a frame from the camera. 
*/


   	while (true)
	{
	    Mat frame;
	    bool isSuccess = cap.read(frame); // read a new frame from the video camera 

	    //Breaking the while loop if frames cannot be read from the camera
	    if (isSuccess == false)
	    {
	        cout << "Video camera is disconnected" << endl;
	        cin.get(); //Wait for any key press
	        break;
	    }

	    /*
	    Make changes to the frame as necessary
	    e.g.  
	     1. Change brightness/contrast of the image
	     2. Smooth/Blur image
	     3. Crop the image
	     4. Rotate the image
	     5. Draw shapes on the image
	    */

	    //write the video frame to the file
	    oVideoWriter.write(frame); 

	    //show the frame in the created window
	    imshow(window_name, frame);

	    //Wait for for 10 milliseconds until any key is pressed.  
	    //If the 'Esc' key is pressed, break the while loop.
	    //If any other key is pressed, continue the loop 
	    //If any key is not pressed within 10 milliseconds, continue the loop 
	    if (waitKey(10) == 27)
	    {
	        cout << "Esc key is pressed by the user. Stopping the video" << endl;
	        break;
	    }
	}

  //Flush and close the video file
    oVideoWriter.release();
    
    return 0;

}