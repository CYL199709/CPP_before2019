// MFC_handDlg.h : ͷ�ļ�
//

#pragma once
//opencv����MFC����ͷ�ļ�
#include<opencv2/opencv.hpp>
using namespace cv;
//CRect Rect_MFC_src;
//CRect Rect_MFC_dst;

// CMFC_handDlg �Ի���
class CMFC_handDlg : public CDialogEx
{
// ����
public:
	CMFC_handDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MFC_HAND_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStartcamare();
	afx_msg void OnBnClickedStartcamera();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedStopcamera();
	afx_msg void OnStnClickedStaticImageshow();
	afx_msg void OnBnClickedQuit();
	// ��⵽��ָ����
	int m_Finger_num;
	afx_msg void OnBnClickedOpenimage();
	afx_msg void OnBnClickedDetection();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedClaerimage();
};
