#pragma once
#pragma warning(disable:4996)

#ifndef CCamCapture_H
#define CCamCapture_H

#include <stdlib.h>
#include <stdio.h>
//#include <windef.h>
#include <Windows.h>
#include "opencv/cv.h"
#include "opencv/highgui.h"

typedef enum {
	STOP = 0,
	PAUSE,
	START
}CaptureState;
typedef void(__cdecl *pfnCaptureCallback)(IplImage* pFrame);

class CCamCapture {
public:
	CCamCapture();
	~CCamCapture();
	BOOL CreateCapture();
	void DestoryCapture();
	void StartCapture();
	void PauseCapture();
	void StopCapture();
	CaptureState GetCaptureState() const;
	IplImage* GetSelectedROI() const;
	CvRect GetTargetRect() const;
	void SetTargetRect(CvRect Rect);
	void SetCaptureCallback(pfnCaptureCallback pfnCaptureCB);
protected:
	static DWORD WINAPI threadGrabImage(LPVOID pparam);
	virtual void doGrabLoop();
	virtual void onGrabLoop_cvInit();
	virtual void onGrabLoop_cvClose();
//	// camera inited flag
	BOOL m_bCamInited;
//	// Event indicating that the thread is about to exit.
	HANDLE m_heventThreadDone;
//	// camera capture state
	CaptureState m_State;
//	// target selection
//	static BOOL m_bTargetObj;
private:
	//member function
	static void onMouseCB(int event, int x, int y, int flag, void* param);
	void onGrabLoop_DrawROI(IplImage* frame);
	//static member
	static IplImage* m_pImage;
	static IplImage* m_pROI;
	static CvRect m_TargetRect;
	static CvPoint m_Origin;
	static BOOL m_drawing;
	static BOOL m_ROIflag;
	//member variable
	CvCapture* m_pCapture;

//	pfnCaptureCallback m_pfnCustomCB;

};
#endif