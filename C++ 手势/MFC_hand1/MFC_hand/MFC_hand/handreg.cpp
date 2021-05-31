#include"handreg.h"


int Myhand::mymain()
{
	//	srcimage.copyTo(dstimage);

		//����������⡪������һ���߳��н���
		//int face_num; //��⵽����������
		//face_num = detect_face(dstimage);
		
		//��ֵ�˲���ȥ����������
		medianBlur(dstimage, dstimage, 5);
		//��˹�˲�
		GaussianBlur(dstimage, dstimage, Size(3, 3), 1);

		//YCrCbɫ�ʿռ��ɫ��⣬��ȥ������
		//dstimage = skinYCrCb(dstimage, rt);
		dstimage = cvSkinOtsu_YCrCb(dstimage, rt, face_num);

		
		//��̬ѧ�����ȸ�ʴ��������
		
		//��ʴ����
		//��ȡ�Զ����
		Mat element_erode = getStructuringElement(MORPH_RECT, Size(10, 10));
		/*	����: MORPH_RECT
		������ : MORPH_CROSS
		��Բ�� : MORPH_ELLIPSE*/
		//��ʴ����
		erode(dstimage, dstimage, element_erode);

		//��ȡ�Զ����
		Mat element_dilate = getStructuringElement(MORPH_RECT, Size(15, 15));
		//���Ͳ���`
		dilate(dstimage, dstimage, element_dilate);

		
		vector<Point> max_contours = Maxarea_find(dstimage, dstimage);
		
		//ָ�����
		gethandpoint(dstimage, max_contours, hand_center);

		
		return finger_find_num;
}

//�������
// frame ԭ��ɫͼ��
// ���� faces.size() ����������
void Myhand::detect_face(Mat Gray_image)
{
	int flag = 0;
	try{
		vector<Rect> faces; //��⵽����������
		/*Mat frame;
		Mat Gray_image;
		src.copyTo(frame);*/
		//��ֵ�˲���ȥ����������
		medianBlur(Gray_image, Gray_image, 5);
		//ת��Ϊ�Ҷ�ͼ
		//cvtColor(frame, Gray_image, CV_BGR2GRAY);
		equalizeHist(Gray_image, Gray_image);   //ֱ��ͼ���⻯ 	//-- Detect faces	


		//�沿���
		face_cascade.detectMultiScale(Gray_image, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(60, 60)); 
 
		for (size_t i = 0; i < faces.size(); i++) //faces.size()Ϊ��⵽����������
		{
			// ������������
			face_center1.x = cvRound((faces[i].x + faces[i].width * 0.5)); //��������������ȡ���������ظ�������ӽ�������ֵ
			face_center1.y = cvRound((faces[i].y + faces[i].height * 0.5));
	

			//�����ο���
			face_center2.x = face_center1.x + 95; face_center2.y = face_center1.y + 200;
			face_center1.x -= 95; face_center1.y -= 120;
		
			rt.x = face_center1.x; rt.y = face_center1.y;//�������Ͻǵ�
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

//���otsu
// src ��ͨ��ͼ��
// dst ��ֵ��ͼ
void Myhand::cvThresholdOtsu(Mat src, Mat dst)
{
	int height = src.rows;
	int width = src.cols;

	//����ֱ��ͼ
	float histogram[256] = { 0 };
	for (int i = 0; i<height; i++)
	{
		unsigned char* p = (unsigned char*)src.data + src.step * i;

		for (int j = 0; j < width; j++)
		{
			histogram[*p++]++;
		}
	}

	//ֱ��ͼ��һ��
	int size = height*width;
	for (int i = 0; i < 256; i++)
	{
		histogram[i] = histogram[i] / size;
	}

	//��ƽ���Ҷ�ֵ
	float avgValue = 0;
	for (int i = 0; i < 256; i++)
	{
		avgValue += i * histogram[i];
	}


	//Ѱ�ҿ�ʹ��䷽������thresh
	double thresh;
	float maxVariance = 0;
	float w = 0, u = 0;

	for (int i = 0; i < 256; i++)
	{
		w += histogram[i];  //�ص���ռ����ͼ��ı���
		u += i*histogram[i];//ƽ���Ҷ�ֵ

		float t = avgValue*w - u;  //ͼ������ࣨǰ���뱳������ƽ���Ҷ�ֵ֮��
		float variance = (t*t) /(w*(1 - w));

		if (variance > maxVariance)
		{
			maxVariance = variance;
			thresh = i;
		}
	}
	
	//ͨ����ֵ��thresh����src���ж�ֵ�����dst 
	threshold(src, dst, thresh, 255, CV_THRESH_BINARY);
	//imshow("Otsu", dst);
}
//ʹ��Crͨ�����зָ��ȥ������
//src:Ϊ��ɫͼ
//rt :Ϊ��������ľ������Ͻ�����ε�with��hight
//face_num :Ϊsrcͼ�е���������
//���أ�������ͼ��ȥ��������Ķ�ֵ��ͼ
Mat Myhand::cvSkinOtsu_YCrCb(Mat src, Rect rt, int face_num)//yCbCr
{
	Mat result, ycrcb;
	vector<Mat> temp_channals;
	ycrcb.create(src.rows, src.cols, CV_8UC3);
	result.create(src.rows, src.cols, CV_8UC1);

	cvtColor(src, ycrcb, CV_BGR2YCrCb);

	split(ycrcb, temp_channals);
	// ����Ӧ��ֵotsu
	cvThresholdOtsu(temp_channals.at(1), temp_channals.at(1));
	temp_channals.at(1).copyTo(result);

	//������Ϊ��
	if (face_num >= 1) //��⵽����ʱ�Ž��д���
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
	//imshow("ȥ���沿��Ϣ", result);
	//imwrite("ȥ���沿��Ϣ.jpg", result);
	return result;
}



//�������������ĵ����ߵļнǵĺ���
//a = (first_p.x, first_p.y)Ϊ��һ������
//b = (second_p.x, second_p.y)Ϊ�ڶ�������
double Myhand::getAngle_center(Point first_p, Point second_p)
{
	//����a . b
	float n = ((first_p.x - hand_center.x) * (second_p.x - hand_center.x))
			+ ((first_p.y - hand_center.y) * (second_p.y - hand_center.y));
	//����|a|*|b|
	float m = sqrt(pow((first_p.x - hand_center.x), 2) + pow((first_p.y - hand_center.y), 2))
			* sqrt(pow((second_p.x - hand_center.x), 2) + pow((second_p.y - hand_center.y), 2));
	
	return fabs((acos(n / m) *180) / 3.14);
}

//����ָ��
//src :Ϊ��ֵ��ͼ��ֻ���ֲ�����
//max_contours :Ϊsrc�еģ����ģ����������ֲ�����
//center :Ϊ�ֲ�����������
//����ָ�����(ȫ�ֱ���)
void Myhand::gethandpoint(Mat src, vector<Point> max_contours, Point center)
{
	vector<Point> couPoint = max_contours;
	float temp_distance = 0, depth = 0, mosthigher = 0;

	//count Num
	int  m = 0, k = 0,  notice = 0, spos = 0;
	int count = 0;//ָ�����

	//��һ��ָ�������
	Point first_point;
	Point singer_before;
	double AREA;
	try{
		AREA = contourArea(max_contours);

		if (AREA >20000 && AREA < 69000  )//��ͨ�����
		{
			for (int i = 0; i < couPoint.size(); i++)
			{
				//�������ĵ������ϵ�i����ľ���
				float dis_curptocenter = sqrt(float(pow((couPoint[i].x - center.x), 2) + pow((couPoint[i].y - center.y), 2)));

				if (dis_curptocenter >= temp_distance)
				{
					temp_distance = dis_curptocenter;//�ҵ���һ�εķ�ֵ
				}
				else
				{
					m++;
					if (m == 1)
					{
						notice = i;//��¼�����������ĵ���������ϵ��������
						fingerTips_single = couPoint[i];
						depth = 500000; //�Ѿ���⵽���㣬�������� dis_curptocenterҪ��С
					}


					if (depth >= dis_curptocenter)//�ӷ�ֵ��ʼ������
					{
						depth = dis_curptocenter;
						k++;//��¼ÿ�δӷ�ֵ���½����˼�����
					}
					else//����������Ҳ�п�����ë��
					{
						if (k > 57)//������µĵ��� > 57  ���ٴζ�ָ��ĺ�ѡ������жϡ������ų�ë��
						{
							if (notice < 30)//��ֹ��һ����Զ����ֵ�λ��̫Զ
							{
								notice = 30;
							}

							k = notice - 50;
							if (k < 0) k = 0;
							//����һ���Ļ�ȡ��ȷ��ָ��λ�ã��ٴα�����ѡָ���notice����� 50 ���� ��ѡ�������������ĵ㣨���ٴ�ȷ��ָ��λ�ã�
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

						
							//��õ�ָ������͹�㣩
							count++;

							//�Ƕ�����
							//��¼��һ��͹��
							if (count == 1 )
							{
								first_point = couPoint[spos];
							}

							if (count > 1)
							{	//�жϵڶ���͹���Ƿ�Ϊָ��
								double angle = getAngle_center(first_point, couPoint[spos]);
								//cout << angle << endl;
								if (angle > 120)
								{
									count--;
									fingerTips_single = singer_before;
								}
							}
							//��ԭͼ���ϻ�Բ
							if (Myhand::hand_center.x != src.rows / 2 || Myhand::hand_center.y != src.cols / 2)
							{
								circle(src, fingerTips_single, 15, Scalar(255), 1, 8, 0);
								singer_before = fingerTips_single;
							}	
						}
						temp_distance = 0;
						m = 0; //Ϊ��һ�����ֵ����׼��
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

//����������ͨ��������������ָ���⡪�����ָ��
//src :Ϊ��ֵ��ͼ
//dst :Ϊ���ָ��λ�ú�Ķ�ֵ��ͼ
//�����ҵ������������ֵ��ͼdst
//���������������max_contours
vector<Point> Myhand::Maxarea_find(Mat src, Mat dst)
{
	vector<vector<cv::Point>> contours;                                 // �������Χ������
	findContours(src, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	double mymax = 0;
	double area_now;
	vector<cv::Point> max_contours;//�����������

	//ȡ����������
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
	//��final_cont�����max_contours
	final_cont.push_back(max_contours);

	//�����������
	Mat Out_Contours_Max = Mat::zeros(src.size(), CV_8UC1);//����һ����srcͬ����С�ĵ�ͨ�հ�ͼ�����������������

	if (contourArea(max_contours) > 5000)//��ͨ�����5000ʱ�Ż�����
	{
		//drawContours(Out_Contours_Max, final_cont, -1, Scalar(255), -1);//ȥ�����һ����-1�����������
		drawContours(Out_Contours_Max, final_cont, -1, Scalar(255));
	
	
		//�������������
		Moments mu;
		mu = moments(max_contours, false);

		//�����������������
		//��ֹmu.m00 = 0
		if (mu.m00 == 0)
			mu.m00 = 1;
		mc = Point2f(mu.m10 / mu.m00, mu.m01 / mu.m00);
		if (mc.x <0 || mc.y >src.rows || mc.y < 0 || mc.x > src.cols)
		{
			hand_center = Point(src.rows / 2, src.cols / 2);
		}
		else
			hand_center = mc;
		////ָ�����
		//gethandpoint(Out_Contours_Max, max_contours, hand_center);

	
		//��������
		//Mat Out_circle = Mat::zeros(src.size(), CV_8UC1);//����һ����srcͬ����С�ĵ�ͨ�հ�ͼ��
		circle(Out_Contours_Max, hand_center, 4, Scalar(255), -1, 8, 0);//��������������һ0ʱΪ�߿�����Բ�����
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
