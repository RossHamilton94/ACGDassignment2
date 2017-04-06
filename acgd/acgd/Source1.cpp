#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

Mat frameGrabBgr;


Mat greenFilter(const Mat& src)
{
	Mat greenOnly;
	inRange(src, Scalar(80, 100, 100), Scalar(140, 255, 255), greenOnly);

	return greenOnly;
}

extern "C" int __declspec(dllexport) __stdcall Detect()
{
	Mat frameGrabHsv;
	cvtColor(frameGrabBgr, frameGrabHsv, CV_BGR2HSV);

	Mat filteredFrameGrab;
	filteredFrameGrab = greenFilter(frameGrabHsv);

	return 0;
}

extern "C" void __declspec(dllexport) __stdcall Init(int frameHeight, int frameWidth)
{
	frameGrabBgr = cv::Mat(frameWidth, frameHeight, CV_8UC3);

	/*Size frameSize(frameWidth, frameHeight);
	frameGrabBgr.create(frameSize, CV_16S);*/
}

extern "C" void __declspec(dllexport) __stdcall SetPixel(int x, int y, int r, int g, int b)
{
	x = 1;
	y = 1;

	Vec3b col;// = frameGrabBgr.at<Vec3b>(x, y);

	col[0] = b;
	col[1] = g;
	col[2] = r;

	frameGrabBgr.at<Vec3b>(x,y) = col;
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