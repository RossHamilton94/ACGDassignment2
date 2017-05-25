/*

WebcamTexture.cs
Ross Hamilton, 04/2017
NHE2422 Assignment 2

This file contains the Unity code to display a webcam image, pass it to OpenCV,
and retrieve any important data from OpenCV.

*/

using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System;

public class WebcamTexture : MonoBehaviour
{
    //Set up objects
    WebCamTexture webcamTex;
    List<Vector2> detectionPixels;

    //Set up wait timer to delay OpenCV processing
    float waitTimer = 0.0f;
    float opencvProcessingWait = 5.0f;

    //Set a bool to keep track of whether OpenCV has initialised
    bool cvInitialised = false;

    //Initialises variables - run once at launch
    void Start()
    {
        //Initialise WebcamTexture object and a renderer to display it
        webcamTex = new WebCamTexture();
        Renderer renderer = GetComponent<Renderer>();
        //Pass the renderer the WebcamTexture
        renderer.material.mainTexture = webcamTex;

        //Set the WebcamTexture to continue playing
        webcamTex.Play();

        //Initialise the OpenCV code -> Now done in update on a timer
        //OpenCVInterop.Init(webcamTex.height, webcamTex.width);
    }

    //Contains all per-frame operations, run once per frame
    void Update()
    {
        //If the program has waited for the webcam to initialise...
        if (waitTimer >= opencvProcessingWait)
        {
            //If OpenCV has not yet initialised
            if (!cvInitialised)
            {
                //Initialise the OpenCV code
                OpenCVInterop.Init(webcamTex.height, webcamTex.width);
                //Make sure we know that OpenCV has initialised
                cvInitialised = true;
            }

            //If OpenCV has initialised
            else
            {
                //Process the current frame in OpenCV
                ProcessFrame();
            }
        }

        //Add the time since last frame to the wait timer
        waitTimer = waitTimer + Time.deltaTime;
    }

    //This method passes the current frame to the OpenCV code, pixel by pixel
    void PassFrame()
    {
        //Indicate that we're passing the frame now
        Debug.Log("Passing frame...");

        //Set up counters
        int i = 0;
        int j = 0;

        //Set up a point (holds co-ords) and a Vec3b to hold colour
        Point p = new Point();
        Vec3b v = new Vec3b();

        //Loop through x co-ords
        for (i = 0; i < webcamTex.width; i++)
        {
            //Loop through y co-ords
            for (j = 0; j < webcamTex.height; j++)
            {
                //Get the current pixel colour
                Color col = webcamTex.GetPixel(i, j);

                //Convert the pixel colour data from decimal to 0 - 255
                //      and set it in our Vec3b object
                v.r = (int)(col.r * 255);
                v.g = (int)(col.g * 255);
                v.b = (int)(col.b * 255);

                //Set our x and y co-ordinates to the values of our counters
                p.xPos = i;
                p.yPos = j;

                //Debug output, extremely slow when uncommented but useful for
                //      troubleshooting loop/pixel passing issues
                //Debug.Log("Pixel data: x=" + p.xPos + " y=" + p.yPos + " r=" + v.r + " g=" + v.g + " b=" + v.b);

                //Pass our co-ordinates and colour data to OpenCV
                OpenCVInterop.SetPixel(p, v);
            }
        }

        //Report that the frame has been passed from Unity to OpenCV
        Debug.Log("Frame passed!");
        //Call OpenCV's update method - it has received a new frame so should
        //      update its debug window
        OpenCVInterop.Update();
    }

    //An old attempt at retrieving data from OpenCV
    private void RetrieveDetectionData()
    {
        //Report that we're trying to retrieve frame data
        Debug.Log("Retrieving frame data...");

        //detectionPixels = new List<Vector2>();
        //retrievalDone = false;
        //for (int i = 0; i < webcamTex.width; i++)
        //{
        //    for (int j = 0; i < webcamTex.height; i++)
        //    {
        //        if (OpenCVInterop.GetPixel(i, j) == true)
        //        {
        //            detectionPixels.Add(new Vector2(i, j));
        //        }
        //    }
        //}

        //Set up an array of pixel co-ordinates to fill from OpenCV
        Point[] vec = new Point[(webcamTex.width) * (webcamTex.height)];

        //Debug.Log("OpenCV reports " + OpenCVInterop.ReturnFrame(vec, vec.Length) + " detected blue pixels");

        //Report end of operation and result
        Debug.Log("Vec is " + vec.Length + " pixels long");
        Debug.Log("Frame data retrieved!");
    }

    //An old attempt at retrieving data from OpenCV
    private List<Vector2> RetrievePixelListDEPRECATED()
    {
        //Report that we're trying to retrieve frame data
        Debug.Log("Retrieving pixel list...");
        //Set up a list for pixel retrieval
        List<Vector2> outList = new List<Vector2>();

        //Loop through x co-ords using a counter
        for (int i = 0; i <= webcamTex.width; i++)
        {
            //Loop through y co-ords using a counter
            for (int j = 0; j <= webcamTex.height; j++)
            {
                //Set up a point to hold co-ordinates
                Point p;

                //Set our x and y to the counter values
                p.xPos = i;
                p.yPos = j;

                //Check if the pixel is one that OpenCV has detected our colour at
                if (OpenCVInterop.GetPixelState(p))
                {
                    //Add the pixel to our list if OpenCV reports it as a detected pixel
                    outList.Add(new Vector2(i, j));
                }
            }
        }

        //Report the length of the retrieved list for debug purposes
        Debug.Log("Pixel list retrieved! Length: " + outList.Count);

        //Return the retrieved list of pixels
        return outList;
    }

    //Retrieves a list of pixel co-ordinates at which the colour was detected
    private List<Vector2> RetrievePixelList()
    {
        //Report that we are beginning to fetch the list of detected pixels
        Debug.Log("Retrieving pixel list...");
        //Set up our list of detected pixels
        List<Vector2> outList = new List<Vector2>();
        //Retrieve OpenCV's count of detected pixels
        int numPixels = OpenCVInterop.GetReturnLength();
        //Report OpenCV's count of detected pixels for debug purposes
        Debug.Log("OpenCV reports " + numPixels + " detected pixels");

        //Loop to retrieve all detected pixels
        for (int i = 0; i < numPixels; i++)
        {
            //Set up a point object and set it to the co-ordinates of the
            //      next detected pixel
            Point p = OpenCVInterop.GetDetectedPixels(i);

            //Add the pixel to the list
            outList.Add(new Vector2(p.xPos, p.yPos));
        }

        //Report how many we retrieved
        Debug.Log("Pixel list retrieved! Length: " + outList.Count);

        //Return the list of retrieved pixels
        return outList;
    }

    //Process the current frame in OpenCV
    public bool ProcessFrame()
    {
        //Pass the current frame to OpenCV
        PassFrame();

        //Manipulate the image to detect our target colour
        Detect();

        //Retrieve the data processed by OpenCV
        detectionPixels = RetrievePixelList();

        return true;
    }

    private void Detect()
    {
        Debug.Log("Telling OpenCV to detect...");

        //Call the Detect method in our OpenCV code
        OpenCVInterop.Detect();

        Debug.Log("Told OpenCV to detect!");
    }
}

// Define the functions which can be called from the .dll.
internal static class OpenCVInterop
{
    [DllImport("acgd")]
    internal static extern void Init(int frameHeight, int frameWidth);

    [DllImport("acgd")]
    internal static extern int Detect();

    [DllImport("acgd")]
    internal static extern void Update();

    [DllImport("acgd")]
    internal static extern void SetPixel(Point p, Vec3b v);

    [DllImport("acgd")]
    internal static extern bool GetPixelState(Point p);

    [DllImport("acgd")]
    internal static extern Point GetDetectedPixels(int i);

    [DllImport("acgd")]
    internal static extern int GetReturnLength();

    //[DllImport("acgd")]
    //internal static extern void Release();

    //[DllImport("acgd")]
    //internal static extern bool GetPixel(int x, int y);

    //[DllImport("acgd")]
    //internal static extern int ReturnFrame([MarshalAs(UnmanagedType.LPArray)] Point[] vec, int length);
}

//The Point object represents an X and Y co-ordinate
[StructLayout(LayoutKind.Sequential)]
public struct Point
{
    public int xPos, yPos;
}

//The Vec3b object represents a vector of colour data
[StructLayout(LayoutKind.Sequential)]
public struct Vec3b
{
    public int b, g, r;
}
