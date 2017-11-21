#include "stdafx.h"

#include "CCamCapture.h"
IplImage* CCamCapture::m_pImage;
IplImage* CCamCapture::m_pROI;
CvRect CCamCapture::m_TargetRect;
CvPoint CCamCapture::m_Origin;
BOOL CCamCapture::m_drawing;
BOOL CCamCapture::m_ROIflag;
CCamCapture::CCamCapture()
{
	m_bCamInited = FALSE;
	m_heventThreadDone = NULL;
	m_pCapture = NULL;
	m_pImage = NULL;
	m_pROI = NULL;
	m_TargetRect = cvRect(0, 0, 0, 0);
	m_Origin = cvPoint(0, 0);
	m_drawing = FALSE;
	m_ROIflag = FALSE;
	StopCapture();
	CString test_thread;
	test_thread.Format(_T("test_thread %d\n"), threadGrabImage);
	//OutputDebugString(test_thread);
}

CCamCapture::~CCamCapture()
{
	if (m_heventThreadDone != NULL) {
		CloseHandle(m_heventThreadDone);
	}	
}

BOOL CCamCapture::CreateCapture()
{
	if (m_bCamInited == TRUE) {
		return TRUE;
	}
	else {
		DWORD thread_identifier;
		if (m_heventThreadDone == NULL) {
			m_heventThreadDone = CreateThread(
				NULL,                   // default security attributes
				0,                      // use default stack size  
				threadGrabImage,       // thread function name
				LPVOID(this),          // argument to thread function 
				0,                      // use default creation flags 
				&thread_identifier);   // returns the thread identifier 

			if (m_heventThreadDone == NULL) {
				CString test;			
				test = "FALSE\n";
				//OutputDebugString(test);
				return FALSE;
			}
			else {
				CString test;
				test = "TRUE\n";
				//OutputDebugString(test);
				return TRUE;
			}
		}
		else {
			if (m_heventThreadDone != NULL) {
				CloseHandle(m_heventThreadDone);
			}
		}
	}
}

void CCamCapture::DestoryCapture()
{
	if (m_bCamInited == TRUE) {
		//m_bCamInited = FALSE;
		StopCapture();

		WaitForSingleObject(m_heventThreadDone, INFINITE);
		CString testdestory;
		//testdestory.Format(_T("destory clear %d"), a);

		//OutputDebugString(testdestory);
		m_heventThreadDone = NULL;
		//testdestory.Format(_T("m_heventThreadDone %d"), m_heventThreadDone);

		//OutputDebugString(testdestory);
		
	}
	else {
		;
	}
}

void CCamCapture::StartCapture()
{
	m_State = START;
}

void CCamCapture::PauseCapture()
{
	m_State = PAUSE;
}

void CCamCapture::StopCapture()
{
	m_State = STOP;
}

CaptureState CCamCapture::GetCaptureState() const
{
	return m_State;
}

IplImage * CCamCapture::GetSelectedROI() const
{
	return m_pROI;
}

CvRect CCamCapture::GetTargetRect() const
{
	return m_TargetRect;
}

void CCamCapture::SetTargetRect(CvRect Rect)
{

}

void CCamCapture::SetCaptureCallback(pfnCaptureCallback pfnCaptureCB)
{
}

DWORD CCamCapture::threadGrabImage(LPVOID pparam)
{
	CCamCapture *pCapture = ((CCamCapture*)pparam);
	pCapture->doGrabLoop();
	return 0;
}

void CCamCapture::doGrabLoop()
{
	CString test_thread;
	IplImage* tmp = NULL;
	onGrabLoop_cvInit();
	test_thread = "in loop";
	while (true) {
		if (m_bCamInited == FALSE) {
			break;
		}
		else {
			switch (m_State) {
			case START: 
				test_thread = "Start\n";
				if(tmp)cvReleaseImage(&tmp);
				
				if (cvGrabFrame(m_pCapture)) {
					m_pImage = cvQueryFrame(m_pCapture);
					if (!m_pImage) {
						CvSize ImageSize = cvSize(640, 480);
						m_pImage = cvCreateImage(ImageSize, IPL_DEPTH_8U, 3);
					}
					else {
						onGrabLoop_DrawROI(m_pImage);
						cvShowImage("Webcam", m_pImage);
						if (cvWaitKey(33) >= 0) {
							StopCapture();
							break;
						}
						else {
							continue;
						}
					}										
				}
				else {
					StopCapture();
					break;
				}
				break;
			case PAUSE:
				test_thread = "Pause\n";			
				if (!(m_pImage)) {
					StopCapture();
					break;
				}
				else {
					if (!tmp) {
						m_pImage = cvQueryFrame(m_pCapture);
						tmp = cvCloneImage(m_pImage);
					}
					else {
						cvCopy(m_pImage, tmp, NULL);
					}
					
					onGrabLoop_DrawROI(tmp);
					cvShowImage("Webcam", tmp);
					if (cvWaitKey(33) >= 0) {
						StopCapture();
						break;
					}
					else {
						continue;
					}
				}
				break;
			case STOP:
				test_thread = "Stop\n";
				m_bCamInited = FALSE;
				StopCapture();
				break;
			default:
				test_thread = "Default\n";
				StopCapture();				
				//OutputDebugString(test_thread);
				break;
			}
		}
	}
	test_thread = "close\n";
	//OutputDebugString(test_thread);
	onGrabLoop_cvClose();
	m_heventThreadDone = (CloseHandle(m_heventThreadDone)) ? NULL : m_heventThreadDone;
	test_thread.Format(_T("m_heventThreadDone %d\n"), m_heventThreadDone);
	//OutputDebugString(test_thread);
}

void CCamCapture::onGrabLoop_cvInit()
{
	m_pCapture = cvCaptureFromCAM(0);
	//cvNamedWindow("Webcam", 0);
	if (!m_pCapture) {
		m_bCamInited = FALSE;
		OutputDebugString(CString("Failed cvCaptureFromCAM!!!\n"));
	}
	else {
		m_bCamInited = TRUE;
		StartCapture();
		//PauseCapture();
		//m_pROI = cvCreateImage(cvSize(0, 0), IPL_DEPTH_8U, 3);
		cvNamedWindow("Webcam", 0);
		//cvNamedWindow("ROI", 1);
		cvSetMouseCallback("Webcam", onMouseCB, NULL);
	}
	CString test_cvInit;
	test_cvInit.Format(_T("cv init = %d"), m_bCamInited);
	//OutputDebugString(test_cvInit);
}

void CCamCapture::onGrabLoop_cvClose()
{
	StopCapture();
	m_TargetRect = { 0,0,0,0 };
	if (m_pCapture != NULL) {
		cvReleaseCapture(&m_pCapture);	
	}
	if (m_pImage != NULL) {
		cvDestroyWindow("Webcam");
	}
	if (m_pROI != NULL) {
		cvDestroyWindow("ROI");
		m_ROIflag = FALSE;
	}
	//m_pImage = NULL;
}

void CCamCapture::onMouseCB(int event, int x, int y, int flag, void * param)
{
	CString str_xy, str_event, str_flag, rect;
	
	switch (event) {
	case CV_EVENT_MOUSEMOVE:
		if (m_drawing) {
			if(x >= 0 && x<m_pImage->width) m_TargetRect.width = x - m_TargetRect.x;
			if (y >= 0 && y<m_pImage->height)m_TargetRect.height = y - m_TargetRect.y;
		}
		str_xy.Format(_T("%d, %d\n"), m_TargetRect.x, m_TargetRect.y);
		break;
	case CV_EVENT_LBUTTONDOWN: 
		m_drawing = TRUE;
		m_Origin = cvPoint(x, y);
		m_TargetRect = cvRect(x, y, 0, 0);
		str_xy.Format(_T("%d, %d\n"), m_TargetRect.x, m_TargetRect.y);
		break;
	
	case CV_EVENT_LBUTTONUP: 
		m_ROIflag = TRUE;
		m_drawing = FALSE;
		if (m_TargetRect.width < 0) {
			m_TargetRect.x += m_TargetRect.width;
			m_TargetRect.width *= -1;
		}
		if (m_TargetRect.height < 0) {
			m_TargetRect.y += m_TargetRect.height;
			m_TargetRect.height *= -1;
		}
		if(!m_TargetRect.width) m_ROIflag = FALSE;
		if (!m_TargetRect.height) m_ROIflag = FALSE;
		str_xy.Format(_T("%d, %d\n"), m_TargetRect.x, m_TargetRect.y);
		//str_xy.Format(_T("%d, %d\n"), m_TargetRect.width, m_TargetRect.height);
		break;		
	default:
		break;
	}
	//m_TargetRect = cvRect(140, 305, 115, 165);
	//cvSetImageROI(m_pROI, m_TargetRect);
	str_event.Format(_T("%d\n"), event);
	str_flag.Format(_T("%d\n"), flag);
//	rect.Format(_T("rect %d,.. %d,.. %d,.. %d\n", m_TargetRect.x, m_TargetRect.y, m_TargetRect.width, m_TargetRect.height));
	//swprintf(buffer, _T("%d, %d"), x, y);
	//OutputDebugString(str_xy);
	//OutputDebugString(str_event);
	//OutputDebugString(str_flag);
	//printf("The param is : %d\n", param);
}

void CCamCapture::onGrabLoop_DrawROI(IplImage * frame)
{
	//CString str_xy;
	//str_xy.Format(_T("%d, %d\n"), m_TargetRect.x, m_TargetRect.y);
	//OutputDebugString(str_xy);	
	if (m_ROIflag) {
		m_ROIflag = FALSE;
		if (m_pROI) {
			cvReleaseImage(&m_pROI);
			cvDestroyWindow("ROI");
		}
		cvSetImageROI(frame, m_TargetRect);
		m_pROI = cvCreateImage(cvGetSize(frame), frame->depth, frame->nChannels);	
		cvCopy(frame, m_pROI, NULL);
		cvShowImage("ROI", m_pROI);
		if(m_pROI->width<50 && m_pROI->height<50) cvResizeWindow("ROI", 100, 100);
		cvResetImageROI(frame);
	}
	if (m_TargetRect.width && m_TargetRect.height) {
		cvRectangle(
			frame,
			cvPoint(m_TargetRect.x, m_TargetRect.y),
			cvPoint(m_TargetRect.x + m_TargetRect.width, m_TargetRect.y + m_TargetRect.height),
			CV_RGB(255, 0, 0),
			2,
			CV_AA,
			0
		);
	}
}