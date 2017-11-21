
// VisualTrackingDlg.h : header file
//

#pragma once

#include "afxwin.h"
#include "afxcmn.h"
#include <opencv2/core.hpp>
//#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
#include <tchar.h>
using namespace cv;
using namespace std;
#include "CCamCapture.h"

// CVisualTrackingDlg dialog
class CVisualTrackingDlg : public CDialogEx
{
// Construction
public:
	CVisualTrackingDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VISUALTRACKING_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	/*typedef enum {
		STOP,
		PAUSE,
		START
	}CaptureState;*/
public:
	int param1_templatematch, param2_templatematch, param3_templatematch;
	int param1_meanshift, param2_meanshift, param3_meanshift;
	int param1_camshift, param2_camshift, param3_camshift;
	int *param1, *param2, *param3;
	CaptureState state;
	CButton m_Button_Cam;
	CButton m_Button_Pause;
	CComboBox m_Combo_Method;
	CSliderCtrl m_Slider_Param1;
	CSliderCtrl m_Slider_Param2;
	CSliderCtrl m_Slider_Param3;
	CEdit m_Edit_Param1;
	CEdit m_Edit_Param2;
	CEdit m_Edit_Param3;
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedPause();
	afx_msg void OnCbnSelchangeCombo();

	afx_msg void OnNMCustomdrawSliderParam1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderParam2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderParam3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditParam1();
	afx_msg void OnEnChangeEditParam2();
	afx_msg void OnEnChangeEditParam3();
public:
	CCamCapture m_CamCapture;
	BOOL m_Creat_Capture;
	BOOL m_Pause;
};
