/*

Source1.cpp
Ross Hamilton, 04/2017
NHE2422 Assignment 2

This file contains the OpenCV code to be called by Unity, which will build,
process and return a frame passed to it.

*/

#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

//Set up our Mat and vector objects
Mat frameGrabBgr;
Mat filteredFrameGrab;
Mat binaryFrameGrab;
vector<Point> nonZeroCoordinates;
Mat frameGrabBGRRotated;
Mat frameGrabHsv;
Mat colFrameGrab;
Mat greyFrameGrab;

//A method (currently unused) which filters a Mat to one colour
Mat colFilter(const Mat& src)
{
	Mat greenOnly;
	inRange(src, Scalar(110, 50, 50), Scalar(130, 255, 255), greenOnly);

	return greenOnly;
}

//This method filters the frame and detects pixels of the target colour
extern "C" int __declspec(dllexport) __stdcall Detect()
{
	//rotate as the image passed is upside-down
	rotate(frameGrabBgr, frameGrabBGRRotated, ROTATE_180);

	//Convert the frame to use HSV, making it work with other OpenCV operations
	cvtColor(frameGrabBGRRotated, frameGrabHsv, CV_BGR2HSV);

	//colFrameGrab = greenFilter(frameGrabHsv);

	//Find all pixels of the specified colour
	inRange(frameGrabHsv, Scalar(110, 50, 50), Scalar(130, 255, 255), colFrameGrab);

	/*cvtColor(greenFrameGrab, filteredFrameGrab, CV_HSV2BGR);
	cvtColor(filteredFrameGrab, greyFrameGrab, CV_BGR2GRAY);
	threshold(greyFrameGrab, binaryFrameGrab, 100, 255, CV_THRESH_BINARY);
	detDone = true;*/

	//Find all co-ordinates that are not now black
	findNonZero(colFrameGrab, nonZeroCoordinates);

	return 0;
}

//Initialise all Mats and windows using the height and width of the first
//		frame passed from Unity
extern "C" void __declspec(dllexport) __stdcall Init(int frameHeight, int frameWidth)
{
	frameGrabBgr = cv::Mat(frameHeight, frameWidth, CV_8UC3);
	frameGrabBGRRotated = cv::Mat(frameHeight, frameWidth, CV_8UC3);
	frameGrabHsv = cv::Mat(frameHeight, frameWidth, CV_8UC3);
	greenFrameGrab = cv::Mat(frameHeight, frameWidth, CV_8UC3);
	greyFrameGrab = cv::Mat(frameHeight, frameWidth, CV_8UC3);

	namedWindow("frames", 1);

	/*frameGrabBgr.rows = frameHeight;
	frameGrabBgr.cols = frameWidth*/

	/*Size frameSize(frameWidth, frameHeight);
	frameGrabBgr.create(frameSize, CV_16S);*/
}

//Update any open windows to display more current information
extern "C" void __declspec(dllexport) __stdcall Update()
{
	//Show the frame passed by Unity in the "frames" window
	imshow("frames", frameGrabBgr);
}

//Set a pixel in the frame grab mat to a certain colour
extern "C" void __declspec(dllexport) __stdcall SetPixel(int x, int y, int b, int g, int r)
{
	//if (x < frameGrabBgr.cols && y < frameGrabBgr.rows)
	//{
	//	Vec3b col = frameGrabBgr.at<Vec3b>(y, x);

	//	col[0] = b;
	//	col[1] = g;
	//	col[2] = r;

	//	frameGrabBgr.at<Vec3b>(y, x) = col;
	//}

	//If within range of the Mat...
	if (x < frameGrabBgr.cols && y < frameGrabBgr.rows)
	{
		//Create a colour Vec of the correct format
		Vec3i col = frameGrabBgr.at<Vec3i>(y, x);

		//Update the colour information for col
		col[0] = b;
		col[1] = g;
		col[2] = r;

		//Set the pixel at x,y to the colour passed
		frameGrabBgr.at<Vec3i>(y, x) = col;
	}
}

//An old method to pass out a bool to Unity - very slow as it relies on
//		std::find for each pixel
extern "C" bool __declspec(dllexport) __stdcall GetPixelState(int x, int y)
{
	//if (std::find(vec.begin(), vec.end(), value) != vec.end())

	if (std::find(nonZeroCoordinates.begin(), nonZeroCoordinates.end(), Point(x, y)) != nonZeroCoordinates.end())
	{
		return true;
	}
	else return false;
}

//Method to return the co-ordinates of a pixel listed in nonZeroCoordinates
extern "C" Vec2i __declspec(dllexport) __stdcall GetDetectedPixels(int i)
{
	//Set up an integer vector to pass the co-ordinates back
	Vec2i returnPix;

	//Set the x and y for this vector to those listed in nonZeroCoordinates
	returnPix.row(nonZeroCoordinates.at(i).y);
	returnPix.col(nonZeroCoordinates.at(i).x);

	//Return the vector
	return returnPix;
}

//Find the number of detected pixels
extern "C" int __declspec(dllexport) __stdcall GetReturnLength()
{
	return nonZeroCoordinates.size();
}

/* BELOW THIS POINT IS OLD CODE WHICH IS UNTESTED AND UNUSED */

//extern "C" bool __declspec(dllexport) __stdcall GetPixel(int x, int y)
//{
//	//x = 1;
//	//y = 1;
//
//	//if (x <= filteredFrameGrab.cols - 1 && y <= filteredFrameGrab.rows - 1)
//	//{
//	//	if (filteredFrameGrab.at<Vec3b>(x, y)[0] == 255)
//	//	{
//	//		return true;
//	//	}
//	//}
//	//else return false;
//
//	if (x <= filteredFrameGrab.cols - 1 && y <= filteredFrameGrab.rows - 1)
//	{
//		if (filteredFrameGrab.at<Vec3b>(x, y)[0] != 0)
//		{
//			return true;
//		}
//	}
//	else return false;
//}
//struct Vector2 { int x; int y; };
//extern "C" int __declspec(dllexport) __stdcall ReturnFrame(Point* pixelsWhereDetected, int length)
//{
//	vector<Point> nonZeroCoordinates;
//	findNonZero(binaryFrameGrab, nonZeroCoordinates);
//
//	//for (int i = 0; i <= nonZeroCoordinates.cols - 1; i++)
//	//{
//	//	for (int j = 0; j <= nonZeroCoordinates.rows - 1; j++)
//	//	{
//	//		pixelsWhereDetected[d].x = i;
//	//		pixelsWhereDetected[d].y = j;
//
//	//		d++;
//	//	}
//	//}
//
//	int count = 0;
//
//	for (int i = 1; i < nonZeroCoordinates.size() - 1; i++)
//	{
//		pixelsWhereDetected[i].x = nonZeroCoordinates[i].x;
//		pixelsWhereDetected[i].y = nonZeroCoordinates[i].y;
//
//		count++;
//	}
//
//	return count;
//}
// Declare structure to be used to pass data from C++ to Mono.
//struct Circle
//{
//	Circle(int x, int y, int radius) : X(x), Y(y), Radius(radius) {}
//	int X, Y, Radius;
//};
//
//CascadeClassifier _faceCascade;
//String _windowName = "Unity OpenCV Interop Sample";
//VideoCapture _capture;
//int _scale = 1;
//
//extern "C" int __declspec(dllexport) __stdcall  Init(int& outCameraWidth, int& outCameraHeight)
//{
//	// Load LBP face cascade.
//	if (!_faceCascade.load("haarcascade_frontalface_alt.xml"))
//		return -1;
//
//	// Open the stream.
//	_capture.open(-1);
//	if (!_capture.isOpened())
//		return -2;
//
//	outCameraWidth = _capture.get(CAP_PROP_FRAME_WIDTH);
//	outCameraHeight = _capture.get(CAP_PROP_FRAME_HEIGHT);
//
//	return 0;
//}
//
//extern "C" void __declspec(dllexport) __stdcall  Close()
//{
//	_capture.release();
//}
//
//extern "C" void __declspec(dllexport) __stdcall SetScale(int scale)
//{
//	_scale = scale;
//}
//
//extern "C" void __declspec(dllexport) __stdcall Detect(Circle* outFaces, int maxOutFacesCount, int& outDetectedFacesCount)
//{
//	Mat frame;
//	_capture >> frame;
//	if (frame.empty())
//		return;
//
//	std::vector<Rect> faces;
//	// Convert the frame to grayscale for cascade detection.
//	Mat grayscaleFrame;
//	cvtColor(frame, grayscaleFrame, COLOR_BGR2GRAY);
//	Mat resizedGray;
//	// Scale down for better performance.
//	resize(grayscaleFrame, resizedGray, Size(frame.cols / _scale, frame.rows / _scale));
//	equalizeHist(resizedGray, resizedGray);
//
//	// Detect faces.
//	_faceCascade.detectMultiScale(resizedGray, faces);
//
//	// Draw faces.
//	for (size_t i = 0; i < faces.size(); i++)
//	{
//		Point center(_scale * (faces[i].x + faces[i].width / 2), _scale * (faces[i].y + faces[i].height / 2));
//		ellipse(frame, center, Size(_scale * faces[i].width / 2, _scale * faces[i].height / 2), 0, 0, 360, Scalar(0, 0, 255), 4, 8, 0);
//
//		// Send to application.
//		outFaces[i] = Circle(faces[i].x, faces[i].y, faces[i].width / 2);
//		outDetectedFacesCount++;
//
//		if (outDetectedFacesCount == maxOutFacesCount)
//			break;
//	}
//
//	// Display debug output.
//	imshow(_windowName, frame);
//}