// MFC_handDlg.h : 头文件
//

#pragma once
//opencv——MFC关联头文件
#include<opencv2/opencv.hpp>
using namespace cv;
//CRect Rect_MFC_src;
//CRect Rect_MFC_dst;

// CMFC_handDlg 对话框
class CMFC_handDlg : public CDialogEx
{
// 构造
public:
	CMFC_handDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFC_HAND_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
	// 检测到的指尖数
	int m_Finger_num;
	afx_msg void OnBnClickedOpenimage();
	afx_msg void OnBnClickedDetection();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedClaerimage();
};
