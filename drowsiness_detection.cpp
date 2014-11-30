/********************************************************

Name:   Concentration Reminder - Drowsiness Detection
Author: Lisa Palathingal
Date:   12/08/2014

**********************************************************/

//Header files
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

//Declaring namespaces
using namespace std;
using namespace cv;

// Function headers 
void detectAndDisplay (Mat frame, int index, int history[]);

// Global variables 
String face_cascade_name = "test/haarcascade_frontalface_alt.xml";
String eyes_cascade_name = "test/haarcascade_eye_tree_eyeglasses.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;

//Display window name
string window_name = "Drowsiness Detection";

RNG rng(12345);

/*************** Function: Main ***********************/

int main( int argc, const char** argv )
{
    CvCapture* capture;
    Mat frame;

    //Load the cascades
    if (!face_cascade.load(face_cascade_name))
    { 
        printf("--(!)Error loading\n"); 
        return -1; 
    };
    if (!eyes_cascade.load(eyes_cascade_name))
    { 
        printf("--(!)Error loading\n"); 
        return -1; 
    };

    //Read the video stream
    capture = cvCaptureFromCAM( -1 );

    if (capture)
    {
        int size = 100;
        int index = -1;
        int i;
        int history[size];
        int closed = 0; 

        //Initialize history[] cells to 0
        for(i = 0; i < size; i++)
        {
            history[i] = 0;
        }

        while( true )
        {
            frame = cvQueryFrame( capture );
            index ++;

            //If index of history[] reaches the value 100, compare number of closed counts with 30
            if(index == size)
            { 
                if(closed >= 30)
	        {
	            printf("You are sleeping\n");
		}
                else
                {
                    printf("You are active\n");
                }

                index = 0;
		closed = 0;
                
		for(i = 0; i < size; i++)
                {
                    history[i] = 0;
                }

            }

	    //Apply the classifier to the frame
            if( !frame.empty() )
            { 
                detectAndDisplay( frame, index, history ); 
            }
            else
            { 
                printf(" --(!) No captured frame -- Break!"); 
                break; 
            }

            if(index == 0)
            {
                if(history[index] == 1)
                {
                    closed = closed + 1;
                    printf("\nCount = %d\n", closed);
                }
            }  
 
            else if(index > 0)
            {
                if(history[index] == 1)
                {
                    if( (history[index-1] == 1) || (history[index-1] == 0) )
                    {
                        closed = closed + 1;
                        printf("\nCount = %d\n", closed);
                    }
                }

                else if(history[index] == 0)
                {
                    if(closed < 30)
                    {
                        closed = 0;
                        printf("\nCount = %d\n", closed);
                    }
                }
            }

            int c = waitKey(10);
            if( (char)c == 'c' ) 
            { 
                break;
            }
        } 
    }
    return 0;
}

/*************************************************************************

Function Name: detectAndDisplay
Input: frame, index, history[]
Output: Detected eyes from the frame, updated values for history array[]
Description: detectAndDisplay detects face and eyes from the input frame and draws 
             a circle around detected eyes. If open eyes are detected, value 0 is 
             inserted in the corresponding index of history[]. If closed eyes are 
             detected, value 1 is inserted in the corresponding index of history[].

***************************************************************************/
void detectAndDisplay( Mat frame, int index, int history[] )
{
    std::vector<Rect> faces;
    Mat frame_gray;

    cvtColor( frame, frame_gray, CV_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );

    //Detect face from the frame
    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

    for( size_t i = 0; i < faces.size(); i++ )
    {
        Mat faceROI = frame_gray( faces[i] );
        std::vector<Rect> eyes;

        //Detect eyes from the detected face
        eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );
        
        //Draw circles around detected eyes
        for( size_t j = 0; j < eyes.size(); j++ )
        {
            Point center( faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5 );
            int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
            circle( frame, center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
        }

        /*If size() is greater than 0, open eyes are detected. history[] value is 0. 
          Else, closed eyes are detected. history[] value is updated to 1.*/
        if (eyes.size() > 0)
        {
            history[index] = 0;
        }

        else
        {
            history[index] = 1;  
        }
    }
 
    //Display window name and frame as output
    imshow( window_name, frame );
}
