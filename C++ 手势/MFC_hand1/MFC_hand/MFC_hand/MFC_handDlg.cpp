
// MFC_handDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_hand.h"
#include "MFC_handDlg.h"
#include "afxdialogex.h"
#include "handreg.h"
#include "Winuser.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//手势类，同时初始化face_cascade_name
Myhand Hand_Dect("E:/openCV249/opencv/sources/data/haarcascades/haarcascade_frontalface_alt.xml");

VideoCapture capture;
CRect Rect_MFC_src;
CRect Rect_MFC_dst;
Mat read_src_image, read_src_image_change;//读入图片的地方使用

int flag_camera_read = 0;//摄像头正在工作标志1（工作），0（不工作）
int flag_file_read = 0;//文件读取正在工作标志1（工作），0（不工作）

POINT p; //鼠标坐标点
RECT rt;//屏幕框

// CMFC_handDlg 对话框

CMFC_handDlg::CMFC_handDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFC_handDlg::IDD, pParent)
	, m_Finger_num(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFC_handDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FINGER_NUM, m_Finger_num);
}

BEGIN_MESSAGE_MAP(CMFC_handDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_STARTCAMERA, &CMFC_handDlg::OnBnClickedStartcamera)
	ON_BN_CLICKED(IDC_STOPCAMERA, &CMFC_handDlg::OnBnClickedStopcamera)
	ON_BN_CLICKED(IDQUIT, &CMFC_handDlg::OnBnClickedQuit)
	ON_BN_CLICKED(IDC_OPENIMAGE, &CMFC_handDlg::OnBnClickedOpenimage)
	ON_BN_CLICKED(IDC_DETECTION, &CMFC_handDlg::OnBnClickedDetection)
	ON_BN_CLICKED(IDC_CLAERIMAGE, &CMFC_handDlg::OnBnClickedClaerimage)
END_MESSAGE_MAP()


// CMFC_handDlg 消息处理程序

BOOL CMFC_handDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	//设置src控件
	namedWindow("src", 1);
	HWND hwnd1 = (HWND)cvGetWindowHandle("src");//获取src窗口的句柄
	HWND hparent1 = ::GetParent(hwnd1);//获取hwnd1的父句柄，函数返回父窗口句柄
	::SetParent(hwnd1, GetDlgItem(IDC_STATIC_IMAGESHOW)->m_hWnd); //改变子窗口的父窗口，即将子窗口hwnd1的父窗口改为IDC_STATIC_IMAGESHOW
	::ShowWindow(hparent1, SW_HIDE);//设定指定窗口的显示状态，将父窗口（包括其之窗口）的显示状态设为SW_HIDE（隐藏窗口并激活其他窗口）

	//设置dst控件
	namedWindow("dst", 1);
	HWND hwnd2 = (HWND)cvGetWindowHandle("dst");
	HWND hparent2 = ::GetParent(hwnd2);
	::SetParent(hwnd2, GetDlgItem(IDC_STATIC_IMAGESHOW2)->m_hWnd);
	::ShowWindow(hparent2, SW_HIDE);

	//获取picture control大小——src
	CWnd *pWnd1 = GetDlgItem(IDC_STATIC_IMAGESHOW);//IDC_STATIC_IMAGESHOW为picture控件ID
	pWnd1->GetClientRect(&Rect_MFC_src);//Rect_MFC_src为控件的大小。
	//获取picture control大小——dst
	CWnd *pWnd2 = GetDlgItem(IDC_STATIC_IMAGESHOW2);//IDC_STATIC_IMAGESHOW2为picture控件ID
	pWnd2->GetClientRect(&Rect_MFC_dst);//Rect_MFC_dst为控件的大小。

	Hand_Dect.face_cascade.load(Hand_Dect.face_cascade_name);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFC_handDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFC_handDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFC_handDlg::OnBnClickedStartcamera()
{
	// TODO:  在此添加控件通知处理程序代码
	
	//加载分类器——人脸检测
	/*Hand_Dect.face_cascade_name = "E://openCV249/opencv/sources/data/haarcascades/haarcascade_frontalface_alt.xml";
	if (!Hand_Dect.face_cascade.load(Hand_Dect.face_cascade_name)){ printf("Error loading\n"); }*/
	
	int before_x = 40;
	int before_y = 40;
	int single_flag = 1;
	int double_flag = 1;
	int right_flag = 1;
	capture.open(0);
	if (!capture.isOpened()) { printf("Error opening video capture\n"); }

	flag_camera_read = 1;
	Mat Gray_src;
	while (capture.read(Hand_Dect.srcimage))
	{	
		try
		{
			Hand_Dect.srcimage.copyTo(Hand_Dect.dstimage);

			cvtColor(Hand_Dect.srcimage, Gray_src, CV_BGR2GRAY);
			//进行图像处理
			////开启人脸检测线程
			//thread t_face_camera(&Myhand::t_detect_face, ref(Hand_Dect));
			Hand_Dect.detect_face(Gray_src);

			m_Finger_num = Hand_Dect.mymain();

			//将m_Finger_num（手指个数）传到MFC界面可编辑文本框上
			UpdateData(FALSE);
			//改变图像大小以适应显示窗口src
			resize(Hand_Dect.srcimage, Hand_Dect.srcimage, Size(Rect_MFC_src.Width(), Rect_MFC_src.Height()), 0, 0, INTER_LANCZOS4);
			//显示图像
			imshow("src", Hand_Dect.srcimage);
			//dst
			resize(Hand_Dect.dstimage, Hand_Dect.dstimage, Size(Rect_MFC_dst.Width(), Rect_MFC_dst.Height()), 0, 0, INTER_LANCZOS4);
			imshow("dst", Hand_Dect.dstimage);
			//t_face_camera.join();

			// MOUSEEVENTF_MIDDLEDOWN、MOUSEEVENTF_MIDDLEUP；中间键命令
			if (m_Finger_num == 1)	//单击左键
			{
				if (Hand_Dect.fingerTips_single.y < 200 && single_flag == 1)
				{
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
					Sleep(5);
					mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
					single_flag = 0;

				}				
			 }
			if (m_Finger_num == 2)	//双击左键
			{
				if (Hand_Dect.fingerTips_single.y < 300 && double_flag == 1)
				{
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
					Sleep(10);
					mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
					Sleep(10);
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
					Sleep(10);
					mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
					double_flag = 0;
				}
			 }
			if (m_Finger_num == 3)	//单击右键
			{
				if (Hand_Dect.fingerTips_single.y < 300 && right_flag == 1)
				{
					mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
					Sleep(10);
					mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
				}
			}
			//移动鼠标
			//GetDlgItem(IDD_MFC_HAND_DIALOG)->GetWindowRect(&rt);
			ShowCursor(true);
			int now_x = Hand_Dect.hand_center.x;
			int now_y = Hand_Dect.hand_center.y;
			int dx = now_x - before_x;
			int dy = now_y - before_y;

			GetCursorPos(&p);//获取鼠标坐标 
			if (m_Finger_num >= 4)
			{
				//GetDlgItem(IDC_STATIC_IMAGESHOW2)->GetWindowRect(&rt);
				mouse_event(MOUSEEVENTF_MOVE, -dx * 3, dy * 2, 0, 0);
				Sleep(2);//控制移动时间间隔
				if (abs(dx) > 5 || abs(dy) > 5)
				{
					single_flag = 1;
					double_flag = 1;
					right_flag = 1;
				}
			}
			before_x = now_x;
			before_y = now_y;

				

			if (waitKey(1) == ' ') { break; }
		}
		catch (Exception &e)
		{
			//flag_camera_read = 1;
		}
	}
	
	Gray_src.release();
}



void CMFC_handDlg::OnBnClickedStopcamera()
{
	// TODO:  在此添加控件通知处理程序代码
	if (flag_camera_read == 1) //摄像头工作时允许进行处理 
	{
		destroyWindow("src");
		destroyWindow("dst");
		capture.release();//关闭摄像头
		m_Finger_num = 0;
		UpdateData(FALSE);

		// 重新创建窗口
		//设置src控件
		namedWindow("src", 1);
		HWND hwnd1 = (HWND)cvGetWindowHandle("src");
		HWND hparent1 = ::GetParent(hwnd1);
		::SetParent(hwnd1, GetDlgItem(IDC_STATIC_IMAGESHOW)->m_hWnd);
		::ShowWindow(hparent1, SW_HIDE);
		//设置dst控件
		namedWindow("dst", 1);
		HWND hwnd2 = (HWND)cvGetWindowHandle("dst");
		HWND hparent2 = ::GetParent(hwnd2);
		::SetParent(hwnd2, GetDlgItem(IDC_STATIC_IMAGESHOW2)->m_hWnd);
		::ShowWindow(hparent2, SW_HIDE);

		flag_camera_read = 0;
	}	
}





void CMFC_handDlg::OnBnClickedOpenimage()
{
	// TODO:  在此添加控件通知处理程序代码
	capture.release();//关闭摄像头

	//添加文件过滤器
	// 由若干字符串对组成的一个字符串序列 , _T来保证字符串与windows的兼容性
	CString CstrFile_adr = _T("");
	// 文件对话框类CFileDialog
	CFileDialog  	dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("Describe Files (*.jpg)|*.jpg|All Files (*.*)|*.*||"), NULL);
	//重命名对话框的标题
	dlgFile.m_ofn.lpstrTitle = _T("Read image");
	//创建有模式对话框
	if (dlgFile.DoModal() == IDOK)
	{
		//获取读入的文件的路径
		CstrFile_adr = dlgFile.GetPathName();
	}
	
	if (CstrFile_adr != _T(""))//确保已经选择了文件——>进行后续操作，否则不进行
	{
		//Cstring转string
		std::string strFile_adr(CW2A(CstrFile_adr.GetString()));

		read_src_image = imread(strFile_adr);
		imshow("src", read_src_image);
		resize(read_src_image, read_src_image_change, Size(Rect_MFC_dst.Width(), Rect_MFC_dst.Height()), 0, 0, INTER_LANCZOS4);
		//显示图像
		imshow("src", read_src_image_change);

		//重新设置dst控件
		destroyWindow("dst");
		namedWindow("dst", 1);
		HWND hwnd2 = (HWND)cvGetWindowHandle("dst");
		HWND hparent2 = ::GetParent(hwnd2);
		::SetParent(hwnd2, GetDlgItem(IDC_STATIC_IMAGESHOW2)->m_hWnd);
		::ShowWindow(hparent2, SW_HIDE);
		flag_file_read = 1;
	}
}


void CMFC_handDlg::OnBnClickedDetection()
{
	Mat Gray;

	// TODO:  在此添加控件通知处理程序代码
	if (flag_file_read == 1) //已读入图像
	{ 
		//进行图像处理
		Hand_Dect.srcimage = read_src_image;

		Hand_Dect.srcimage.copyTo(Hand_Dect.dstimage);
		cvtColor(Hand_Dect.srcimage, Gray, CV_BGR2GRAY);
		 Hand_Dect.detect_face(Gray);

		//开启人脸检测线程
		//thread t_face_file(&Myhand::t_detect_face, ref(Hand_Dect));
		//t_face_file.join();

		m_Finger_num = Hand_Dect.mymain();
		//将m_Finger_num（手指个数）传到MFC界面可编辑文本框上
		UpdateData(FALSE);
		//改变图像大小以适应显示窗口
		resize(Hand_Dect.dstimage, Hand_Dect.dstimage, Size(Rect_MFC_dst.Width(), Rect_MFC_dst.Height()), 0, 0, INTER_LANCZOS4);
		//显示图像
		imshow("dst", Hand_Dect.dstimage);
	}
}


void CMFC_handDlg::OnBnClickedClaerimage()
{
	// TODO:  在此添加控件通知处理程序代码
	if (flag_file_read == 1)//已读入图像
	{		
		destroyWindow("src");
		destroyWindow("dst");
		// 重新创建窗口
		//设置src控件
		namedWindow("src", 1);
		HWND hwnd1 = (HWND)cvGetWindowHandle("src");
		HWND hparent1 = ::GetParent(hwnd1);
		::SetParent(hwnd1, GetDlgItem(IDC_STATIC_IMAGESHOW)->m_hWnd);
		::ShowWindow(hparent1, SW_HIDE);
		//设置dst控件
		namedWindow("dst", 1);
		HWND hwnd2 = (HWND)cvGetWindowHandle("dst");
		HWND hparent2 = ::GetParent(hwnd2);
		::SetParent(hwnd2, GetDlgItem(IDC_STATIC_IMAGESHOW2)->m_hWnd);
		::ShowWindow(hparent2, SW_HIDE);

		m_Finger_num = 0;
		//将m_Finger_num（手指个数）传到MFC界面可编辑文本框上
		UpdateData(FALSE);

		flag_file_read = 0;
	}
}



void CMFC_handDlg::OnBnClickedQuit()
{
	// TODO:  在此添加控件通知处理程序代码
	capture.release();//关闭摄像头
	destroyWindow("src");
	destroyWindow("dst");
	AfxGetMainWnd()->SendMessage(WM_CLOSE);
	this->DestroyWindow();

}





