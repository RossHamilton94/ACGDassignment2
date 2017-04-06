using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;

public class OpenCVColourDetection : MonoBehaviour {

	// Use this for initialization
	void Start ()
    {
        //Debug.Log("Init code: " + OpenCVInterop.Init());
	}
	
	// Update is called once per frame
	void Update ()
    {
        //Debug.Log("Detect code: " + OpenCVInterop.Detect());
        //OpenCVInterop.PassFrame();
    }

    void OnApplicationQuit()
    {
        //OpenCVInterop.Release();
    }
}

