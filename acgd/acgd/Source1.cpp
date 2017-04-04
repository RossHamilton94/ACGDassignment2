#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

//Capture feed
VideoCapture capture;
bool frame = 0;

double capWidth;
double capHeight;

//IplImage* GetThresholdedImage(IplImage* img)
//{
//	//Converting the webcam image into a HSV image
//	IplImage* imgHSV = cvCreateImage(cvGetSize(img), 8, 3);
//	cvCvtColor(img, imgHSV, CV_BGR2HSV);
//
//	//Thresholded image to be returned
//	IplImage* imgThresholded = cvCreateImage(cvGetSize(img), 8, 1);
//
//	//Threshold the image
//	cvInRangeS(imgHSV, cvScalar(20, 100, 100), cvScalar(30, 255, 255), imgThresholded);
//
//	//Release temp image and return
//	cvReleaseImage(&imgHSV);
//	return imgThresholded;
//}

Mat redFilter(const Mat& src)
{
	assert(src.type() == CV_8UC3);

	Mat redOnly;
	inRange(src, Scalar(0, 0, 0), Scalar(0, 0, 255), redOnly);

	return redOnly;
}

extern "C" int __declspec(dllexport) __stdcall Detect()
{
	////Hold a frame from the camera
	//frame = 0;
	//frame = capture.grab();

	////If a frame was not stored, throw an error
	//if (!frame)
	//{
	//	return -1;
	//}
	
	//IplImage* thresholdedFrame = GetThresholdedImage(capture.);
	////Calculate the moments to estimate the position of the object
	//CvMoments *moments = (CvMoments*)malloc(sizeof(CvMoments));
	//cvMoments(thresholdedFrame, moments, 1);
	////The actual moment values
	//double moment10 = cvGetSpatialMoment(moments, 1, 0);
	//double moment01 = cvGetSpatialMoment(moments, 0, 1);
	//double area = cvGetCentralMoment(moments, 0, 0);
	////Hold ball positions, last and current
	//static int posX = 0;
	//static int posY = 0;
	//int lastX = posX;
	//int lastY = posY;
	//posX = moment10 / area;
	//posY = moment01 / area;
	////Print current position
	////printf("Position (%d,%d)", posX, posY);
	//cvReleaseImage(&thresholdedFrame);
	//delete moments;
	//Convert target colours to white, all others to black

	Mat frameGrab;
	capture.retrieve(frameGrab);

	Mat filteredFrameGrab;
	filteredFrameGrab = redFilter(frameGrab);

	return 0;
}

extern "C" int __declspec(dllexport) __stdcall Init()
{
	//Initialise camera feed
	capture.open(0);

	capWidth = capture.get(CV_CAP_PROP_FRAME_WIDTH);
	capHeight = capture.get(CV_CAP_PROP_FRAME_HEIGHT);

	//If device not found, throw an error
	if (!capture.isOpened())
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

extern "C" void __declspec(dllexport) __stdcall Release()
{
	//Release camera
	capture.release();
}

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