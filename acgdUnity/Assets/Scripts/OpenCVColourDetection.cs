using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;

public class OpenCVColourDetection : MonoBehaviour {

	// Use this for initialization
	void Start ()
    {
        Debug.Log("Init code: " + OpenCVInterop.Init());
	}
	
	// Update is called once per frame
	void Update ()
    {
        Debug.Log("Detect code: " + OpenCVInterop.Detect());
    }

    void OnApplicationQuit()
    {
        OpenCVInterop.Release();
    }
}

// Define the functions which can be called from the .dll.
internal static class OpenCVInterop
{
    [DllImport("acgd")]
    internal static extern int Init();

    [DllImport("acgd")]
    internal static extern void Release();

    [DllImport("acgd")]
    internal static extern int Detect();
}