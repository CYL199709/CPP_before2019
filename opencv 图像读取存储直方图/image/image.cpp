#include<opencv2/opencv.hpp>
using namespace cv;

Mat src;
int histSize = 30;
void histBar(int, void*);
int main()
{

	src = imread("To_read.jpg"); //读取图片
	if (src.empty())
	{
		printf("can not load image \n");
		return -1;
	}
	namedWindow("image", WINDOW_AUTOSIZE); //创建显示窗口
	
	imshow("image", src);  //显示图片
	imwrite("save.jpg", src); //存储图片
	cvtColor(src, src, COLOR_BGR2GRAY);  //灰度转换
	namedWindow("histogram", WINDOW_AUTOSIZE);
	createTrackbar("histSize:", "histogram", &histSize, 500, histBar);  //直方图

	waitKey(0);
	return 0;

}

void histBar(int, void*)
{
	MatND hist;
	float range[] = { 0, 255 };
	const float* ranges = { range };
	//计算直方图
	calcHist(&src, 1, 0, Mat(), hist, 1, &histSize, &ranges, true, false);
	//将直方图bin的数值归一化到0-255
	normalize(hist, hist, 0, 255, NORM_MINMAX, -1, Mat());

	//显示直方图
	int binsW = cvRound((double)500 / histSize);
	Mat histImg = Mat::zeros(500, 500, CV_8UC3);
	RNG rng(123);
	for (int i = 0; i < histSize; i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		rectangle(histImg, Point(i * binsW, 500), Point((i + 1) * binsW, 500 - cvRound(hist.at<float>(i) * 500 / 255.0)), color, -1);
	}
	imshow("histogram", histImg);
}
