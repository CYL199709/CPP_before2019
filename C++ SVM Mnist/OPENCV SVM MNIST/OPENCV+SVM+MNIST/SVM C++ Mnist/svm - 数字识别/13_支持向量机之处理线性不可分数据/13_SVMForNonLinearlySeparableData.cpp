#include <fstream>  
#include "opencv2/opencv.hpp"  
#include <vector>  

using namespace std;
using namespace cv;

#define SHOW_PROCESS  0 
#define ON_STUDY      0  //�Ƿ�ѵ����־λ

#define TRAIN   1  
#define TEST    0  

#define SAMPLE_NUM        9    //0-9��������       ���Ϊ9 
#define SAMPLE_MAX_NUM    5000 //ѵ���������������  5000
#define TEST_MAX_NUM    800  //��������������������� 800

/*
************************************************************************
*
*�ࣺclass NumTrainData
*���ܣ�����ѵ���������󣬰�����ǩ����������
*������float data[64];int result;
*
*
************************************************************************
*/
class NumTrainData
{
public:
	NumTrainData()
	{
		memset(data, 0, sizeof(data));
		result = -1;
	}
public:
	float data[64];
	int result;
};

vector<NumTrainData> buffer;//�����õ���ѵ��������������
int featureLen = 64;

/*
********************************************************
*
*
*�������ܣ��õ��ļ�λ���ַ�������ȡ�ļ�����ָ����Ŀ��ͼƬ
*
********************************************************
*/
void ReadImage(Mat &image ,unsigned char pathNum, int num, bool flag)
{
	char filename[1000];
	if (flag)//flagΪ1 ��Ϊѵ������·��
	{
		sprintf(filename, "E:/����/2020.04/C++/OPENCV SVM MNIST/OPENCV+SVM+MNIST/MNISTͼƬ��/MNISTͼƬ��/trainimage/pic2/%d/%d.bmp", pathNum, num);//�����������ת��ͼƬ���ƣ������filename�С�
	}
	else   //��֮��Ϊ��������·��
	{
		sprintf(filename, "E:/����/2020.04/C++/OPENCV SVM MNIST/OPENCV+SVM+MNIST/MNISTͼƬ��/MNISTͼƬ��/testimage/pic2/%d/%d.bmp", pathNum, num);//�����������ת��ͼƬ���ƣ������filename�С�
	}
	
	//cout << filename << endl;
	image = imread(filename, 0);
}

/*
******************************************************************************
*��������void GetROI(Mat& src, Mat& dst)
*���ܣ�������ͼ���зָ��������Ч���򣬲�����Ч�������ڴ������������Ĵ�
*��ڲ���:Mat src    ���ֿ��е�ͼ�������С
*���ز���:Mat dest   �����������С��������ͼ�� 
******************************************************************************
*/
void GetROI(Mat& src, Mat& dst)
{
	int left, right, top, bottom;
	left = src.cols;
	right = 0;
	top = src.rows;
	bottom = 0;

	//Get valid area ��������ɨ�裬�õ���Ч�� 
	for (int i = 0; i<src.rows; i++)
	{
		for (int j = 0; j<src.cols; j++)
		{
			if (src.at<uchar>(i, j) > 0)
			{
				if (j<left) left = j;
				if (j>right) right = j;
				if (i<top) top = i;
				if (i>bottom) bottom = i;
			}
		}
	}



	int width = right - left;
	int height = bottom - top;
	int len = (width < height) ? height : width;//�õ�����

	//Create a squre  
	dst = Mat::zeros(len, len, CV_8UC1);//�������������ͼ��ԭ��

	//Copy valid data to squre center  
	Rect dstRect((len - width) / 2, (len - height) / 2, width, height);//����Ч���������������ͼ�����Ĵ�
	Rect srcRect(left, top, width, height);
	Mat dstROI = dst(dstRect);
	Mat srcROI = src(srcRect);
	srcROI.copyTo(dstROI);
}

int ReadTrainData(void)
{
	Mat src;//����ԭͼ
	Mat temp = Mat::zeros(8, 8, CV_8UC1);//��ά�Ⱥ��ͳһ�߶�8*8
	Mat img, dst;//dstΪ������Ч����

	//Create source and show image matrix  //����
	Scalar templateColor(255, 0, 255);
	NumTrainData rtd;//8*8=64���ֽ�ȫ��0��resultΪ-1

	for (int m = 0; m <= SAMPLE_NUM; m++)//���ѭ����0-9������
	{
		for (int n = 1; n <= SAMPLE_MAX_NUM; n++)//�ڲ�ѭ����1-5000����������
		{
				//Read source data  
				ReadImage(src, m, n,TRAIN);
				GetROI(src, dst);//�ָ����ͼ�����

				#if(SHOW_PROCESS) //���Ҫ��ʾ���̣��򽫵������ͼ����ݶ��Ŵ�ʮ������ʾ 
					//Too small to watch  
					img = Mat::zeros(dst.rows * 10, dst.cols * 10, CV_8UC1);
					resize(dst, img, img.size());

					stringstream ss;
					ss << "Number " << m<<","<<n;
					string text = ss.str();
					putText(img, text, Point(10, 50), FONT_HERSHEY_SIMPLEX, 1.0, templateColor);

					imshow("img", img);  
				#endif  

				rtd.result = m;//��ǩֵ
				resize(dst, temp, temp.size());//���������ͼ��ͳһ�ٴε���Ϊ8*8��ͼ��
				//threshold(temp, temp, 10, 1, CV_THRESH_BINARY);  

				//��8*8��ͼ�����Ϊ1*64������洢
				for (int i = 0; i<8; i++)
				{
					for (int j = 0; j<8; j++)
					{
						rtd.data[i * 8 + j] = temp.at<uchar>(i, j);
					}
				}

				buffer.push_back(rtd);//������������ŵ�������

				
	     }
   }
	
	cout <<"��ȡѵ���������" << endl;
	waitKey(0);
	return 0;
}

void newSvmStudy(vector<NumTrainData>& trainData)
{
	int testCount = trainData.size();

	Mat m = Mat::zeros(1, featureLen, CV_32FC1);
	Mat data = Mat::zeros(testCount, featureLen, CV_32FC1);
	Mat res = Mat::zeros(testCount, 1, CV_32SC1);
	cout << testCount;
	for (int i = 0; i< testCount; i++)
	{
		
		NumTrainData td = trainData.at(i);
		
		memcpy(m.data, td.data, featureLen*sizeof(float));
		
		normalize(m, m);
		
		memcpy(data.data + i*featureLen*sizeof(float), m.data, featureLen*sizeof(float));
		
		res.at<float>(i, 0) = td.result;

		cout <<i<< endl;
	}

	/////////////START SVM TRAINNING//////////////////  
	CvSVM svm ;//= CvSVM()
	CvSVMParams param;
	CvTermCriteria criteria;//������ֹ����

	criteria = cvTermCriteria(CV_TERMCRIT_EPS, 1000, FLT_EPSILON);
	param = CvSVMParams(CvSVM::C_SVC, CvSVM::RBF, 10.0, 8.0, 1.0, 10.0, 0.5, 0.1, NULL, criteria);

	svm.train(data, res, Mat(), Mat(), param);
	svm.save("SVM_DATA.xml");

	cout << "ѵ������"<< endl;
	waitKey(0);
}


int newSvmPredict()
{
	CvSVM svm ;//= CvSVM()
	svm.load("SVM_DATA.xml");

	Mat src;
	Mat temp = Mat::zeros(8, 8, CV_8UC1);
	Mat LEN = Mat::zeros(1, featureLen, CV_32FC1);
	Mat img, dst;

	int    right = 0;
	float  rightPercent[10] = {0};//��ȷ��

	Scalar templateColor(255, 0, 0);
	NumTrainData rtd;

	for (int m = 0; m <= SAMPLE_NUM; m++)//���ѭ����0-9������
	{
		for (int n = 1; n <= TEST_MAX_NUM; n++)//�ڲ�ѭ����1-800����������
		{
				ReadImage(src, m, n,TEST);
				GetROI(src, dst);

				//Too small to watch  
				img = Mat::zeros(dst.rows * 30, dst.cols * 30, CV_8UC3);
				resize(dst, img, img.size());

				rtd.result = m;
				resize(dst, temp, temp.size());

				//threshold(temp, temp, 10, 1, CV_THRESH_BINARY);  
				for (int i = 0; i<8; i++)
				{
					for (int j = 0; j<8; j++)
					{
						LEN.at<float>(0, j + i * 8) = temp.at<uchar>(i, j);
					}
				}

				normalize(LEN, LEN);
				char ret = (char)svm.predict(LEN);

				if (ret == m)
				{
					right++;
				}

				#if(SHOW_PROCESS)  
						stringstream ss;
						ss << "Number " << m << "," << n;
						string text = ss.str();
						putText(img, text, Point(10, 50), FONT_HERSHEY_SIMPLEX, 1.0, templateColor);

						imshow("img", img);
						if (waitKey(0) == 27) //ESC to quit  
							break;
				#endif  

		}

		rightPercent[m] = right/800.0;//��ȷ��
		cout << "����" << m << "��ȷ�ʣ�" << rightPercent[m] << endl;
		right = 0;//��������
	}


	cout <<"���Խ���" << endl;
	system("pause");
	waitKey(0);
	return 0;
}

int main(int argc, char *argv[]) 
{
	#if(ON_STUDY)  
		ReadTrainData();
		newSvmStudy(buffer);
	#else  
		newSvmPredict();
	#endif  
		return 0;
}