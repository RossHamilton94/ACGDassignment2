using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;

public class WebcamTexture : MonoBehaviour {

    WebCamTexture webcamTex;
    Color32[] pixelData;

    // Use this for initialization
    void Start () {

        webcamTex = new WebCamTexture();
        Renderer renderer = GetComponent<Renderer>();
        renderer.material.mainTexture = webcamTex;
        webcamTex.Play();
        pixelData = new Color32[webcamTex.width * webcamTex.height];

        OpenCVInterop.Init(webcamTex.height, webcamTex.width);

        PassFrame();
    }

    // Update is called once per frame
    void Update ()
    {
        //PassFrame();
	}

    void PassFrame()
    {
        int i = 1;
        int j = 1;

        Point p = new Point();
        Vec3b v = new Vec3b();

        while (i < webcamTex.width - 1)
        {
            while (j < webcamTex.height - 1)
            {
                Color col = webcamTex.GetPixel(i, j);
                v.r = (int)col.r;
                v.g = (int)col.g;
                v.b = (int)col.b;

                p.xPos = i;
                p.yPos = j;
                OpenCVInterop.SetPixel(p, v);

                j++;
            }

            i++;
        }
    }
}

// Define the functions which can be called from the .dll.
internal static class OpenCVInterop
{
    [DllImport("acgd")]
    internal static extern int Init(int frameHeight, int frameWidth);

    //[DllImport("acgd")]
    //internal static extern void Release();

    //[DllImport("acgd")]
    //internal static extern int Detect();

    [DllImport("acgd")]
    internal static extern void SetPixel(Point p, Vec3b v);
}

[StructLayout(LayoutKind.Sequential)]
public struct Point
{
    public int xPos, yPos;
}

[StructLayout(LayoutKind.Sequential)]
public struct Vec3b
{
    public int b, g, r;
}

