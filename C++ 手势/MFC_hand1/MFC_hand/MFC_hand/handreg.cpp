#include"handreg.h"


int Myhand::mymain()
{
	//	srcimage.copyTo(dstimage);

		//进行人脸检测——在另一个线程中进行
		//int face_num; //检测到的人脸个数
		//face_num = detect_face(dstimage);
		
		//中值滤波，去除椒盐噪声
		medianBlur(dstimage, dstimage, 5);
		//高斯滤波
		GaussianBlur(dstimage, dstimage, Size(3, 3), 1);

		//YCrCb色彩空间肤色检测，并去除脸部
		//dstimage = skinYCrCb(dstimage, rt);
		dstimage = cvSkinOtsu_YCrCb(dstimage, rt, face_num);

		
		//形态学处理，先腐蚀，后膨胀
		
		//腐蚀操作
		//获取自定义核
		Mat element_erode = getStructuringElement(MORPH_RECT, Size(10, 10));
		/*	矩形: MORPH_RECT
		交叉形 : MORPH_CROSS
		椭圆形 : MORPH_ELLIPSE*/
		//腐蚀操作
		erode(dstimage, dstimage, element_erode);

		//获取自定义核
		Mat element_dilate = getStructuringElement(MORPH_RECT, Size(15, 15));
		//膨胀操作`
		dilate(dstimage, dstimage, element_dilate);

		
		vector<Point> max_contours = Maxarea_find(dstimage, dstimage);
		
		//指尖查找
		gethandpoint(dstimage, max_contours, hand_center);

		
		return finger_find_num;
}

//人脸检测
// frame 原彩色图像
// 返回 faces.size() 人脸检测个数
void Myhand::detect_face(Mat Gray_image)
{
	int flag = 0;
	try{
		vector<Rect> faces; //检测到的人脸个数
		/*Mat frame;
		Mat Gray_image;
		src.copyTo(frame);*/
		//中值滤波，去除椒盐噪声
		medianBlur(Gray_image, Gray_image, 5);
		//转化为灰度图
		//cvtColor(frame, Gray_image, CV_BGR2GRAY);
		equalizeHist(Gray_image, Gray_image);   //直方图均衡化 	//-- Detect faces	


		//面部检测
		face_cascade.detectMultiScale(Gray_image, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(60, 60)); 
 
		for (size_t i = 0; i < faces.size(); i++) //faces.size()为检测到的人脸个数
		{
			// 人脸中心坐标
			face_center1.x = cvRound((faces[i].x + faces[i].width * 0.5)); //浮点数四舍五入取整，即返回跟参数最接近的整数值
			face_center1.y = cvRound((faces[i].y + faces[i].height * 0.5));
	

			//画矩形框用
			face_center2.x = face_center1.x + 95; face_center2.y = face_center1.y + 200;
			face_center1.x -= 95; face_center1.y -= 120;
		
			rt.x = face_center1.x; rt.y = face_center1.y;//矩形左上角点
			rt.width = face_center2.x - face_center1.x; rt.height = face_center2.y - face_center1.y;

			if ((rt.width <= 0) || (rt.height <= 0)||(rt.x<0)||(rt.y<0))
				rt.x = face_center1.x; rt.y = face_center1.y;
		}
	
			face_num =  faces.size();
	}
	catch (Exception &e)
	{
		flag = 0;
	}
}

//大津法otsu
// src 单通道图像
// dst 二值化图
void Myhand::cvThresholdOtsu(Mat src, Mat dst)
{
	int height = src.rows;
	int width = src.cols;

	//计算直方图
	float histogram[256] = { 0 };
	for (int i = 0; i<height; i++)
	{
		unsigned char* p = (unsigned char*)src.data + src.step * i;

		for (int j = 0; j < width; j++)
		{
			histogram[*p++]++;
		}
	}

	//直方图归一化
	int size = height*width;
	for (int i = 0; i < 256; i++)
	{
		histogram[i] = histogram[i] / size;
	}

	//求平均灰度值
	float avgValue = 0;
	for (int i = 0; i < 256; i++)
	{
		avgValue += i * histogram[i];
	}


	//寻找可使类间方差最大的thresh
	double thresh;
	float maxVariance = 0;
	float w = 0, u = 0;

	for (int i = 0; i < 256; i++)
	{
		w += histogram[i];  //素点数占整幅图像的比例
		u += i*histogram[i];//平均灰度值

		float t = avgValue*w - u;  //图像的另类（前景与背景）的平均灰度值之差
		float variance = (t*t) /(w*(1 - w));

		if (variance > maxVariance)
		{
			maxVariance = variance;
			thresh = i;
		}
	}
	
	//通过阈值（thresh）对src进行二值化输出dst 
	threshold(src, dst, thresh, 255, CV_THRESH_BINARY);
	//imshow("Otsu", dst);
}
//使用Cr通道进行分割，并去除脸部
//src:为彩色图
//rt :为脸部区域的矩形左上角与矩形的with和hight
//face_num :为src图中的人脸个数
//返回（处理结果图）去除脸部后的二值化图
Mat Myhand::cvSkinOtsu_YCrCb(Mat src, Rect rt, int face_num)//yCbCr
{
	Mat result, ycrcb;
	vector<Mat> temp_channals;
	ycrcb.create(src.rows, src.cols, CV_8UC3);
	result.create(src.rows, src.cols, CV_8UC1);

	cvtColor(src, ycrcb, CV_BGR2YCrCb);

	split(ycrcb, temp_channals);
	// 自适应阈值otsu
	cvThresholdOtsu(temp_channals.at(1), temp_channals.at(1));
	temp_channals.at(1).copyTo(result);

	//脸部变为黑
	if (face_num >= 1) //检测到人脸时才进行处理
	{
		for (int i = 1; i < temp_channals.at(0).rows - 1; i++)
		{
			uchar* current = result.ptr< uchar>(i);

			for (int j = 1; j < temp_channals.at(0).cols - 1; j++)
			{
				if (i >= rt.y && j >= rt.x && i <= (rt.y + rt.height) && j <= (rt.x + rt.width))
					current[j] = 0;
			}
		}
	}
	ycrcb.release();
	//imshow("去除面部信息", result);
	//imwrite("去除面部信息.jpg", result);
	return result;
}



//计算两点与中心点连线的夹角的函数
//a = (first_p.x, first_p.y)为第一点坐标
//b = (second_p.x, second_p.y)为第二点坐标
double Myhand::getAngle_center(Point first_p, Point second_p)
{
	//计算a . b
	float n = ((first_p.x - hand_center.x) * (second_p.x - hand_center.x))
			+ ((first_p.y - hand_center.y) * (second_p.y - hand_center.y));
	//计算|a|*|b|
	float m = sqrt(pow((first_p.x - hand_center.x), 2) + pow((first_p.y - hand_center.y), 2))
			* sqrt(pow((second_p.x - hand_center.x), 2) + pow((second_p.y - hand_center.y), 2));
	
	return fabs((acos(n / m) *180) / 3.14);
}

//查找指尖
//src :为二值化图（只有手部廓）
//max_contours :为src中的（最大的）轮廓，即手部轮廓
//center :为手部轮廓的质心
//返回指尖个数(全局变量)
void Myhand::gethandpoint(Mat src, vector<Point> max_contours, Point center)
{
	vector<Point> couPoint = max_contours;
	float temp_distance = 0, depth = 0, mosthigher = 0;

	//count Num
	int  m = 0, k = 0,  notice = 0, spos = 0;
	int count = 0;//指尖个数

	//第一个指尖的坐标
	Point first_point;
	Point singer_before;
	double AREA;
	try{
		AREA = contourArea(max_contours);

		if (AREA >20000 && AREA < 69000  )//连通域面积
		{
			for (int i = 0; i < couPoint.size(); i++)
			{
				//计算重心到轮廓上第i个点的距离
				float dis_curptocenter = sqrt(float(pow((couPoint[i].x - center.x), 2) + pow((couPoint[i].y - center.y), 2)));

				if (dis_curptocenter >= temp_distance)
				{
					temp_distance = dis_curptocenter;//找到第一次的峰值
				}
				else
				{
					m++;
					if (m == 1)
					{
						notice = i;//记录距离质心最大的点的在轮廓上的索引编号
						fingerTips_single = couPoint[i];
						depth = 500000; //已经检测到最大点，用来控制 dis_curptocenter要变小
					}


					if (depth >= dis_curptocenter)//从峰值开始向下走
					{
						depth = dis_curptocenter;
						k++;//记录每次从峰值向下进行了几个点
					}
					else//出现上升，也有可能是毛刺
					{
						if (k > 57)//如果向下的点数 > 57  则再次对指尖的候选点进行判断。用来排除毛刺
						{
							if (notice < 30)//防止第一个最远点出现的位置太远
							{
								notice = 30;
							}

							k = notice - 50;
							if (k < 0) k = 0;
							//更进一步的获取精确的指尖位置，再次遍历候选指尖点notice领域的 50 个点 ，选出距离重心最大的点（即再次确认指尖位置）
							for (k; k < notice + 50; k++)
							{
								temp_distance = sqrt(float((couPoint[k].x - center.x) * (couPoint[k].x - center.x) + (couPoint[k].y - center.y) * (couPoint[k].y - center.y)));
								if (temp_distance > mosthigher)
								{
									mosthigher = temp_distance;
									spos = k;
								}
							}

							mosthigher = 0;

							fingerTips_single = couPoint[spos];

						
							//获得的指尖数（凸点）
							count++;

							//角度条件
							//记录第一个凸点
							if (count == 1 )
							{
								first_point = couPoint[spos];
							}

							if (count > 1)
							{	//判断第二个凸点是否为指尖
								double angle = getAngle_center(first_point, couPoint[spos]);
								//cout << angle << endl;
								if (angle > 120)
								{
									count--;
									fingerTips_single = singer_before;
								}
							}
							//在原图像上画圆
							if (Myhand::hand_center.x != src.rows / 2 || Myhand::hand_center.y != src.cols / 2)
							{
								circle(src, fingerTips_single, 15, Scalar(255), 1, 8, 0);
								singer_before = fingerTips_single;
							}	
						}
						temp_distance = 0;
						m = 0; //为下一个最大值点做准备
						k = 0;

					}
				}
			}
			if (Myhand::hand_center.x == src.rows / 2 && Myhand::hand_center.y == src.cols / 2)
				finger_find_num = 0;
			else
				finger_find_num = count;
		}
		else
			finger_find_num = 0;

	}
	catch (Exception &e)
	{
		//first_point = Point(-1, -1);
	}
}

//查找最大的连通域轮廓——调用指尖检测——标记指尖
//src :为二值化图
//dst :为标记指尖位置后的二值化图
//返回找到的最大轮廓二值化图dst
//返回最大轮廓向量max_contours
vector<Point> Myhand::Maxarea_find(Mat src, Mat dst)
{
	vector<vector<cv::Point>> contours;                                 // 利用最大范围查找手
	findContours(src, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	double mymax = 0;
	double area_now;
	vector<cv::Point> max_contours;//最大轮廓矩阵

	//取最大面积区域
	for (unsigned int i = 0; i < contours.size(); i++)
	{
		try{
			area_now = contourArea(contours[i]);
			if (area_now > mymax)
			{
				mymax = area_now;
				max_contours = contours[i];
			}
		}
		catch (Exception &e)
		{
			//
		}
	}

	vector<vector<Point>> final_cont;
	//在final_cont后插入max_contours
	final_cont.push_back(max_contours);

	//绘制最大轮廓
	Mat Out_Contours_Max = Mat::zeros(src.size(), CV_8UC1);//创建一个和src同样大小的单通空白图像，用来保存最大轮廓

	if (contourArea(max_contours) > 5000)//连通域大于5000时才化质心
	{
		//drawContours(Out_Contours_Max, final_cont, -1, Scalar(255), -1);//去掉最后一个“-1”将输出轮廓
		drawContours(Out_Contours_Max, final_cont, -1, Scalar(255));
	
	
		//计算最大轮廓矩
		Moments mu;
		mu = moments(max_contours, false);

		//计算最大轮廓的质心
		//防止mu.m00 = 0
		if (mu.m00 == 0)
			mu.m00 = 1;
		mc = Point2f(mu.m10 / mu.m00, mu.m01 / mu.m00);
		if (mc.x <0 || mc.y >src.rows || mc.y < 0 || mc.x > src.cols)
		{
			hand_center = Point(src.rows / 2, src.cols / 2);
		}
		else
			hand_center = mc;
		////指尖查找
		//gethandpoint(Out_Contours_Max, max_contours, hand_center);

	
		//绘制质心
		//Mat Out_circle = Mat::zeros(src.size(), CV_8UC1);//创建一个和src同样大小的单通空白图像
		circle(Out_Contours_Max, hand_center, 4, Scalar(255), -1, 8, 0);//倒数第三个数大一0时为线宽，否则圆被填充
	}	

	Out_Contours_Max.copyTo(dst);

	Out_Contours_Max.release();
	//Out_circle.release();
	return max_contours;
}


//
// void  Myhand::t_detect_face(Myhand &img)
//{
//	 img.face_num = img.detect_face(img.srcimage);
//}
//
//
//
