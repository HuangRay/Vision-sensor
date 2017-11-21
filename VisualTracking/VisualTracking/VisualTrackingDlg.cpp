
// VisualTrackingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VisualTracking.h"
#include "VisualTrackingDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CVisualTrackingDlg dialog



CVisualTrackingDlg::CVisualTrackingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VISUALTRACKING_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVisualTrackingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, ID_START, m_Button_Cam);
	DDX_Control(pDX, ID_PAUSE, m_Button_Pause);
	DDX_Control(pDX, ID_COMBO, m_Combo_Method);
	DDX_Control(pDX, ID_SLIDER_PARAM1, m_Slider_Param1);
	DDX_Control(pDX, ID_SLIDER_PARAM2, m_Slider_Param2);
	DDX_Control(pDX, ID_SLIDER_PARAM3, m_Slider_Param3);
	DDX_Control(pDX, ID_EDIT_PARAM1, m_Edit_Param1);
	DDX_Control(pDX, ID_EDIT_PARAM2, m_Edit_Param2);
	DDX_Control(pDX, ID_EDIT_PARAM3, m_Edit_Param3);
}

BEGIN_MESSAGE_MAP(CVisualTrackingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_START, &CVisualTrackingDlg::OnBnClickedStart)
	ON_BN_CLICKED(ID_PAUSE, &CVisualTrackingDlg::OnBnClickedPause)
	ON_CBN_SELCHANGE(ID_COMBO, &CVisualTrackingDlg::OnCbnSelchangeCombo)
	//ON_EN_CHANGE(IDC_EDIT1, &CVisualTrackingDlg::OnEnChangeEdit1)
	ON_NOTIFY(NM_CUSTOMDRAW, ID_SLIDER_PARAM1, &CVisualTrackingDlg::OnNMCustomdrawSliderParam1)
	ON_NOTIFY(NM_CUSTOMDRAW, ID_SLIDER_PARAM2, &CVisualTrackingDlg::OnNMCustomdrawSliderParam2)
	ON_NOTIFY(NM_CUSTOMDRAW, ID_SLIDER_PARAM3, &CVisualTrackingDlg::OnNMCustomdrawSliderParam3)
	ON_EN_CHANGE(ID_EDIT_PARAM1, &CVisualTrackingDlg::OnEnChangeEditParam1)
	ON_EN_CHANGE(ID_EDIT_PARAM2, &CVisualTrackingDlg::OnEnChangeEditParam2)
	ON_EN_CHANGE(ID_EDIT_PARAM3, &CVisualTrackingDlg::OnEnChangeEditParam3)
END_MESSAGE_MAP()


// CVisualTrackingDlg message handlers

BOOL CVisualTrackingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	
	CString slider_value;
	state = STOP;
	param1_templatematch = 0;
	param2_templatematch = 0;
	param3_templatematch = 0;
	param1_meanshift = 0;
	param2_meanshift = 0;
	param3_meanshift = 0;
	param1_camshift = 0;
	param2_camshift = 0;
	param3_camshift = 0;
	param1 = &param1_templatematch;
	param2 = &param2_templatematch;
	param3 = &param3_templatematch;
	m_Combo_Method.InsertString(0, L"TemplateMatch");
	m_Combo_Method.InsertString(1, L"MeanShift");
	m_Combo_Method.InsertString(2, L"CamShift");
	m_Combo_Method.SetCurSel(0);
	m_Slider_Param1.SetRangeMin(0, false);
	m_Slider_Param1.SetRangeMax(100, false);
	//slider_value.Format(_T("%d"), 1);
	slider_value.Format(_T("%d"), m_Slider_Param1.GetPos());
	m_Edit_Param1.SetWindowTextW(slider_value);
	m_Slider_Param2.SetRangeMin(0, false);
	m_Slider_Param2.SetRangeMax(100, false);
	slider_value.Format(_T("%d"), m_Slider_Param2.GetPos());
	m_Edit_Param2.SetWindowTextW(slider_value);
	m_Slider_Param3.SetRangeMin(0, false);
	m_Slider_Param3.SetRangeMax(100, false);
	slider_value.Format(_T("%d"), m_Slider_Param3.GetPos());
	m_Edit_Param3.SetWindowTextW(slider_value);

	m_Creat_Capture = FALSE;
	m_Pause = FALSE;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVisualTrackingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVisualTrackingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVisualTrackingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CVisualTrackingDlg::OnBnClickedStart()
{
	
	// TODO: Add your control notification handler code here
	CString start_caption, str;
	if (!m_Creat_Capture) {
		m_Creat_Capture = m_CamCapture.CreateCapture();
		if (m_Creat_Capture) {
			str = "Open Camera successful";
			//::MessageBox(NULL, str, L"Message", 0);
			
		}
		else {
			str = "Open Camera Failed";
			::MessageBox(NULL, str, L"Message", 0);
		}	
	}
	else {
		m_CamCapture.StopCapture();
		m_Creat_Capture = FALSE;
	}
	Sleep(1000);
	state = m_CamCapture.GetCaptureState();
	switch (state) {
	case STOP:
		//state = START; 
		m_Pause = FALSE;
		start_caption = "Start Capture";
		m_Button_Pause.SetWindowTextW(CString("Pause"));
		break;
	case PAUSE:
		//state = STOP;
		//m_CamCapture.DestoryCapture();
		start_caption = "Start Capture";
		
		break;
	case START:
		//state = STOP;
		//m_CamCapture.DestoryCapture();
		start_caption = "Stop Capture";
		break;
	default:
		break;
	}
	m_Button_Cam.SetWindowTextW(start_caption);

	//test

		//String imageName("./test.jpg"); // by default

		//Mat image;
		//image = imread(imageName, IMREAD_COLOR); // Read the file
		//if (image.empty())                      // Check for invalid input
		//{
		//	cout << "Could not open or find the image" << std::endl;
		//}
		//namedWindow("Display window", WINDOW_AUTOSIZE); // Create a window for display.
		//imshow("Display window", image);                // Show our image inside it.
		//waitKey(0); // Wait for a keystroke in the window

}


void CVisualTrackingDlg::OnBnClickedPause()
{
	// TODO: Add your control notification handler code here
	CString pause_caption, str;
	if (!m_Pause) {
		//m_Creat_Capture = m_CamCapture.CreateCapture();
		if (!m_Creat_Capture) {
			str = "Please open the camera";
			::MessageBox(NULL, str, L"Message", 0);
		}
		else {
			m_Pause = TRUE;
			m_CamCapture.PauseCapture();
		}		
	}
	else {
		m_Pause = FALSE;
		m_CamCapture.StartCapture();
	}
	switch (m_CamCapture.GetCaptureState()) {
	case STOP:
		//state = STOP;
		pause_caption = "Pause";
		break;
	case PAUSE:
		//state = START;
		pause_caption = "Continue";
		break;
	case START:
		//state = PAUSE;
		pause_caption = "Pause";
		break;
	default:
		break;
	}
	m_Button_Pause.SetWindowTextW(pause_caption);
}


void CVisualTrackingDlg::OnCbnSelchangeCombo()
{
	// TODO: Add your control notification handler code here
	switch (m_Combo_Method.GetCurSel()) {
	case 0:
		param1 = &param1_templatematch;
		param2 = &param2_templatematch;
		param3 = &param3_templatematch;
		m_Slider_Param1.SetPos(*param1);
		m_Slider_Param2.SetPos(*param2);
		m_Slider_Param3.SetPos(*param3);
		m_Slider_Param1.EnableWindow(true);
		m_Edit_Param1.EnableWindow(true);
		m_Slider_Param2.EnableWindow(true);
		m_Edit_Param2.EnableWindow(true);
		m_Slider_Param3.EnableWindow(true);
		m_Edit_Param3.EnableWindow(true);
		break;
	case 1:
		param1 = &param1_meanshift;
		param2 = &param2_meanshift;
		param3 = &param3_meanshift;
		m_Slider_Param1.SetPos(*param1);
		m_Slider_Param2.SetPos(*param2);
		m_Slider_Param3.SetPos(*param3);
		m_Slider_Param1.EnableWindow(true);
		m_Edit_Param1.EnableWindow(true);
		m_Slider_Param2.EnableWindow(true);
		m_Edit_Param2.EnableWindow(true);
		m_Slider_Param3.EnableWindow(false);
		m_Edit_Param3.EnableWindow(false);
		break;
	case 2:
		param1 = &param1_camshift;
		param2 = &param2_camshift;
		param3 = &param3_camshift;
		m_Slider_Param1.SetPos(*param1);
		m_Slider_Param2.SetPos(*param2);
		m_Slider_Param3.SetPos(*param3);
		m_Slider_Param1.EnableWindow(true);
		m_Edit_Param1.EnableWindow(true);
		m_Slider_Param2.EnableWindow(false);
		m_Edit_Param2.EnableWindow(false);
		m_Slider_Param3.EnableWindow(false);
		m_Edit_Param3.EnableWindow(false);
		break;
	default:
		break;
	}
}



void CVisualTrackingDlg::OnNMCustomdrawSliderParam1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	CString slider_value;
	*param1 = m_Slider_Param1.GetPos();
	slider_value.Format(_T("%d"), *param1);
	m_Edit_Param1.SetWindowTextW(slider_value);
	*pResult = 0;
}


void CVisualTrackingDlg::OnNMCustomdrawSliderParam2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	CString slider_value;
	*param2 = m_Slider_Param2.GetPos();
	slider_value.Format(_T("%d"), *param2);
	m_Edit_Param2.SetWindowTextW(slider_value);
	*pResult = 0;
}


void CVisualTrackingDlg::OnNMCustomdrawSliderParam3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	CString slider_value;
	*param3 = m_Slider_Param3.GetPos();
	slider_value.Format(_T("%d"), *param3);
	m_Edit_Param3.SetWindowTextW(slider_value);
	*pResult = 0;
}


void CVisualTrackingDlg::OnEnChangeEditParam1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString edit_text;
	m_Edit_Param1.GetWindowTextW(edit_text);
	m_Slider_Param1.SetPos(_ttoi(edit_text));
}


void CVisualTrackingDlg::OnEnChangeEditParam2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString edit_text;
	m_Edit_Param2.GetWindowTextW(edit_text);
	m_Slider_Param2.SetPos(_ttoi(edit_text));
}


void CVisualTrackingDlg::OnEnChangeEditParam3()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString edit_text;
	m_Edit_Param3.GetWindowTextW(edit_text);
	m_Slider_Param3.SetPos(_ttoi(edit_text));
}
