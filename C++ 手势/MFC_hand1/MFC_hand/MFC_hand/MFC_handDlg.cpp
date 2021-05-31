
// MFC_handDlg.cpp : ʵ���ļ�
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


//�����࣬ͬʱ��ʼ��face_cascade_name
Myhand Hand_Dect("E:/openCV249/opencv/sources/data/haarcascades/haarcascade_frontalface_alt.xml");

VideoCapture capture;
CRect Rect_MFC_src;
CRect Rect_MFC_dst;
Mat read_src_image, read_src_image_change;//����ͼƬ�ĵط�ʹ��

int flag_camera_read = 0;//����ͷ���ڹ�����־1����������0����������
int flag_file_read = 0;//�ļ���ȡ���ڹ�����־1����������0����������

POINT p; //��������
RECT rt;//��Ļ��

// CMFC_handDlg �Ի���

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


// CMFC_handDlg ��Ϣ�������

BOOL CMFC_handDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	//����src�ؼ�
	namedWindow("src", 1);
	HWND hwnd1 = (HWND)cvGetWindowHandle("src");//��ȡsrc���ڵľ��
	HWND hparent1 = ::GetParent(hwnd1);//��ȡhwnd1�ĸ�������������ظ����ھ��
	::SetParent(hwnd1, GetDlgItem(IDC_STATIC_IMAGESHOW)->m_hWnd); //�ı��Ӵ��ڵĸ����ڣ������Ӵ���hwnd1�ĸ����ڸ�ΪIDC_STATIC_IMAGESHOW
	::ShowWindow(hparent1, SW_HIDE);//�趨ָ�����ڵ���ʾ״̬���������ڣ�������֮���ڣ�����ʾ״̬��ΪSW_HIDE�����ش��ڲ������������ڣ�

	//����dst�ؼ�
	namedWindow("dst", 1);
	HWND hwnd2 = (HWND)cvGetWindowHandle("dst");
	HWND hparent2 = ::GetParent(hwnd2);
	::SetParent(hwnd2, GetDlgItem(IDC_STATIC_IMAGESHOW2)->m_hWnd);
	::ShowWindow(hparent2, SW_HIDE);

	//��ȡpicture control��С����src
	CWnd *pWnd1 = GetDlgItem(IDC_STATIC_IMAGESHOW);//IDC_STATIC_IMAGESHOWΪpicture�ؼ�ID
	pWnd1->GetClientRect(&Rect_MFC_src);//Rect_MFC_srcΪ�ؼ��Ĵ�С��
	//��ȡpicture control��С����dst
	CWnd *pWnd2 = GetDlgItem(IDC_STATIC_IMAGESHOW2);//IDC_STATIC_IMAGESHOW2Ϊpicture�ؼ�ID
	pWnd2->GetClientRect(&Rect_MFC_dst);//Rect_MFC_dstΪ�ؼ��Ĵ�С��

	Hand_Dect.face_cascade.load(Hand_Dect.face_cascade_name);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFC_handDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMFC_handDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFC_handDlg::OnBnClickedStartcamera()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	
	//���ط����������������
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
			//����ͼ����
			////������������߳�
			//thread t_face_camera(&Myhand::t_detect_face, ref(Hand_Dect));
			Hand_Dect.detect_face(Gray_src);

			m_Finger_num = Hand_Dect.mymain();

			//��m_Finger_num����ָ����������MFC����ɱ༭�ı�����
			UpdateData(FALSE);
			//�ı�ͼ���С����Ӧ��ʾ����src
			resize(Hand_Dect.srcimage, Hand_Dect.srcimage, Size(Rect_MFC_src.Width(), Rect_MFC_src.Height()), 0, 0, INTER_LANCZOS4);
			//��ʾͼ��
			imshow("src", Hand_Dect.srcimage);
			//dst
			resize(Hand_Dect.dstimage, Hand_Dect.dstimage, Size(Rect_MFC_dst.Width(), Rect_MFC_dst.Height()), 0, 0, INTER_LANCZOS4);
			imshow("dst", Hand_Dect.dstimage);
			//t_face_camera.join();

			// MOUSEEVENTF_MIDDLEDOWN��MOUSEEVENTF_MIDDLEUP���м������
			if (m_Finger_num == 1)	//�������
			{
				if (Hand_Dect.fingerTips_single.y < 200 && single_flag == 1)
				{
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
					Sleep(5);
					mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
					single_flag = 0;

				}				
			 }
			if (m_Finger_num == 2)	//˫�����
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
			if (m_Finger_num == 3)	//�����Ҽ�
			{
				if (Hand_Dect.fingerTips_single.y < 300 && right_flag == 1)
				{
					mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
					Sleep(10);
					mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
				}
			}
			//�ƶ����
			//GetDlgItem(IDD_MFC_HAND_DIALOG)->GetWindowRect(&rt);
			ShowCursor(true);
			int now_x = Hand_Dect.hand_center.x;
			int now_y = Hand_Dect.hand_center.y;
			int dx = now_x - before_x;
			int dy = now_y - before_y;

			GetCursorPos(&p);//��ȡ������� 
			if (m_Finger_num >= 4)
			{
				//GetDlgItem(IDC_STATIC_IMAGESHOW2)->GetWindowRect(&rt);
				mouse_event(MOUSEEVENTF_MOVE, -dx * 3, dy * 2, 0, 0);
				Sleep(2);//�����ƶ�ʱ����
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (flag_camera_read == 1) //����ͷ����ʱ������д��� 
	{
		destroyWindow("src");
		destroyWindow("dst");
		capture.release();//�ر�����ͷ
		m_Finger_num = 0;
		UpdateData(FALSE);

		// ���´�������
		//����src�ؼ�
		namedWindow("src", 1);
		HWND hwnd1 = (HWND)cvGetWindowHandle("src");
		HWND hparent1 = ::GetParent(hwnd1);
		::SetParent(hwnd1, GetDlgItem(IDC_STATIC_IMAGESHOW)->m_hWnd);
		::ShowWindow(hparent1, SW_HIDE);
		//����dst�ؼ�
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	capture.release();//�ر�����ͷ

	//����ļ�������
	// �������ַ�������ɵ�һ���ַ������� , _T����֤�ַ�����windows�ļ�����
	CString CstrFile_adr = _T("");
	// �ļ��Ի�����CFileDialog
	CFileDialog  	dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("Describe Files (*.jpg)|*.jpg|All Files (*.*)|*.*||"), NULL);
	//�������Ի���ı���
	dlgFile.m_ofn.lpstrTitle = _T("Read image");
	//������ģʽ�Ի���
	if (dlgFile.DoModal() == IDOK)
	{
		//��ȡ������ļ���·��
		CstrFile_adr = dlgFile.GetPathName();
	}
	
	if (CstrFile_adr != _T(""))//ȷ���Ѿ�ѡ�����ļ�����>���к������������򲻽���
	{
		//Cstringתstring
		std::string strFile_adr(CW2A(CstrFile_adr.GetString()));

		read_src_image = imread(strFile_adr);
		imshow("src", read_src_image);
		resize(read_src_image, read_src_image_change, Size(Rect_MFC_dst.Width(), Rect_MFC_dst.Height()), 0, 0, INTER_LANCZOS4);
		//��ʾͼ��
		imshow("src", read_src_image_change);

		//��������dst�ؼ�
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

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (flag_file_read == 1) //�Ѷ���ͼ��
	{ 
		//����ͼ����
		Hand_Dect.srcimage = read_src_image;

		Hand_Dect.srcimage.copyTo(Hand_Dect.dstimage);
		cvtColor(Hand_Dect.srcimage, Gray, CV_BGR2GRAY);
		 Hand_Dect.detect_face(Gray);

		//������������߳�
		//thread t_face_file(&Myhand::t_detect_face, ref(Hand_Dect));
		//t_face_file.join();

		m_Finger_num = Hand_Dect.mymain();
		//��m_Finger_num����ָ����������MFC����ɱ༭�ı�����
		UpdateData(FALSE);
		//�ı�ͼ���С����Ӧ��ʾ����
		resize(Hand_Dect.dstimage, Hand_Dect.dstimage, Size(Rect_MFC_dst.Width(), Rect_MFC_dst.Height()), 0, 0, INTER_LANCZOS4);
		//��ʾͼ��
		imshow("dst", Hand_Dect.dstimage);
	}
}


void CMFC_handDlg::OnBnClickedClaerimage()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (flag_file_read == 1)//�Ѷ���ͼ��
	{		
		destroyWindow("src");
		destroyWindow("dst");
		// ���´�������
		//����src�ؼ�
		namedWindow("src", 1);
		HWND hwnd1 = (HWND)cvGetWindowHandle("src");
		HWND hparent1 = ::GetParent(hwnd1);
		::SetParent(hwnd1, GetDlgItem(IDC_STATIC_IMAGESHOW)->m_hWnd);
		::ShowWindow(hparent1, SW_HIDE);
		//����dst�ؼ�
		namedWindow("dst", 1);
		HWND hwnd2 = (HWND)cvGetWindowHandle("dst");
		HWND hparent2 = ::GetParent(hwnd2);
		::SetParent(hwnd2, GetDlgItem(IDC_STATIC_IMAGESHOW2)->m_hWnd);
		::ShowWindow(hparent2, SW_HIDE);

		m_Finger_num = 0;
		//��m_Finger_num����ָ����������MFC����ɱ༭�ı�����
		UpdateData(FALSE);

		flag_file_read = 0;
	}
}



void CMFC_handDlg::OnBnClickedQuit()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	capture.release();//�ر�����ͷ
	destroyWindow("src");
	destroyWindow("dst");
	AfxGetMainWnd()->SendMessage(WM_CLOSE);
	this->DestroyWindow();

}





