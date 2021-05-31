#ifndef HAND_H
#define HAND_H

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/objdetect/objdetect.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include<vector>
#include<iostream>
#include<math.h>
#include <thread>
//#include<Windows.h>
//#include<Winuser.h>

using namespace cv;
using namespace std;

class Myhand
{
public:
	Mat srcimage;//原图像
	Mat dstimage;//处理后的图像

	Point  face_center1 = { 200, 200 }, face_center2 = { 300, 300 };//脸部中心
	Rect rt = {300, 300, 50, 50}; //人脸矩形大小
	int face_num = 0;//脸的个数
	
	Point2f hand_center; //手部质心
	Point fingerTips_single;//指尖坐标
	int finger_find_num;
	Point2f mc;//手掌质心坐标



	String face_cascade_name;
	CascadeClassifier face_cascade;
	Myhand(string ca)
	{
		face_cascade_name = ca;
	}

	//查找指尖
	//src :为二值化图（只有手部廓）
	//max_contours :为src中的（最大的）轮廓，即手部轮廓
	//center :为手部轮廓的质心
	//返回指尖个数
	void gethandpoint(Mat src, vector<Point> max_contours, Point center);


	//查找最大的连通域轮廓
	//src :为二值化图
	//dst :为标记指尖位置后的二值化图
	vector<Point> Maxarea_find(Mat src, Mat dst);

	int mymain();

	//人脸检测
	// frame 原彩色图像
	// 返回 faces.size() 人脸检测个数
	void detect_face(Mat frame);

	////创建人脸检测线程所用函数
	//static  void  t_detect_face(Myhand &img);

private:
	//大津法otsu
	// src 单通道图像
	// dst 二值化图
	void cvThresholdOtsu(Mat src, Mat dst);

	//使用Cr通道进行分割，并去除脸部
	//src:为彩色图
	//rt :为脸部区域的矩形左上角与矩形的with和hight
	//face_num :为src图中的人脸个数
	//返回（处理结果图）去除脸部后的二值化图
	Mat cvSkinOtsu_YCrCb(Mat src, Rect rt, int face_num);//yCbCr

	//计算两点与中心点连线的夹角的函数
	//a = (first_p.x, first_p.y)为第一点坐标
	//b = (second_p.x, second_p.y)为第二点坐标
	double getAngle_center(Point first_p, Point second_p);

};

#endif