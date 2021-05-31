#include "The_result_gui.h"
#include <QtWidgets/QApplication>

using namespace cv::dnn;
using namespace cv;
using namespace std;


//����ͼ��ȥ��
void showMultiImages(const char* title, int nArgs, ...);
void hist_draw(Mat src_hist);
MatND dstHist;
int histSize = 256;
float range[] = { 0,256 };
const float *histRanges = { range };
int count_hist;


//����ͼ��ʶ��
float confThreshold = 0.3; // ���Ŷ���ֵ
float nmsThreshold = 0.45;  // �����������ֵ
int inpWidth = 416;  // ͼƬ��
int inpHeight = 416; // ��
Ptr<TrackerKCF> tracker = TrackerKCF::create();
Rect2d roi;
//���ڴ洢coco�����
vector<string> classes;
//�����Ŷ��Ƴ�
void postprocess(Mat& frame, const vector<Mat>& out);
//���ڻ���Ԥ���
void drawPred(int classId, float conf, int left, int top, int right, int bottom, Mat& frame);
//��������
vector<String> getOutputsNames(const Net& net);
//���ڴ��������ݵĸ�Ŀ¼ �����޸�
string pro_dir = "C:\\Users\\14587\\Desktop\\���\\"; //��Ŀ��Ŀ¼
Mat frame, frame_1, blob;
//����ģ�͵����ú�Ȩ���ļ� �����޸�·��
String modelConfiguration = pro_dir + "yolov3-tiny.cfg";
String modelWeights = pro_dir + "yolov3-tiny.weights";
// ��������
Net net = readNetFromDarknet(modelConfiguration, modelWeights);
vector<String> getOutputsNames(const Net& net);
void drawPred(int classId, float conf, int left, int top, int right, int bottom, Mat& frame);
void postprocess(Mat& frame, const vector<Mat>& outs);
void yolo_find(Mat input);


//������֤��ʶ��
Rect roi_digital;
void processFrame(Mat & binary_1, Rect &rect);
Mat src;
void svm_match(Mat image_2, Rect &rect);
int flag;
stringstream stream;





int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	The_result_gui w;
	w.show();
	return a.exec();
}


//Ŀ��ʶ��
void The_result_gui::find_image() {
	Mat image = imread(pro_dir+"5png.png");
	imshow("ԭͼ", image);
	// ����coco����
	string classesFile = pro_dir + "coco_classes.txt";// ������name;
	ifstream ifs(classesFile.c_str());
	string line;
	while (getline(ifs, line)) classes.push_back(line);
	net.setPreferableBackend(DNN_BACKEND_OPENCV);
	//��gpu��
	//DNN_TARGET_OPENCL ����Ŀ��ΪGPU��
	//DNN_TARGET_CPU����Ϊcpu
	net.setPreferableTarget(DNN_TARGET_OPENCL);
	//resize(frame, frame, Size(416, 416));

	yolo_find(image);
	// ��������
	static const string kWinName = "���";
	namedWindow(kWinName, CV_WINDOW_NORMAL);
	imshow(kWinName, image);

	waitKey(0);

}
//��֤��ʶ��
void The_result_gui::find_digital() {
	src = imread(pro_dir + "66.jpg");

	GaussianBlur(src, src, Size(3, 3), 0, 0, BORDER_DEFAULT);
	//imshow("src", src);
	//read_model();
	//�Ҷȶ�ֵ��
	Mat gray, binary;
	cvtColor(src, gray, COLOR_BGR2GRAY);
	threshold(gray, binary, 160, 255, THRESH_BINARY_INV);

	//��̬ѧ����
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));

	//morphologyEx(binary, binary, MORPH_OPEN, element);
	//namedWindow("binary1", CV_WINDOW_NORMAL);
	erode(binary, binary, element);
	dilate(binary, binary, element);
	imshow("ԭͼ", src);
	//imshow("��̬ѧ�������", binary);
	processFrame(binary, roi_digital);
	imshow("ʶ����", src);

}
//���ڹر�
void The_result_gui::over() {
	destroyAllWindows();
}
//ͼ����ǿȥ��
void The_result_gui::blur_image() {
	const char* title = "����Ա�";

	Mat src_gray;

	Mat src = imread(pro_dir + "6.jpg");
	//�Ҷ�
	cvtColor(src, src_gray, COLOR_BGR2GRAY);
	cv::resize(src, src, Size(640, 480));
	//ֱ��ͼ���⻯ͼ����ǿ
	count_hist=0;
	hist_draw(src);
	//imshow("ԭͼ��", src);
	Mat src_1;
	Mat imageRGB[3];
	split(src, imageRGB);
	for (int i = 0; i < 3; i++) {
		equalizeHist(imageRGB[i], imageRGB[i]);
	}
	merge(imageRGB, 3, src_1);
	//imshow("ֱ��ͼ���⻯ͼ����ǿ", src_1);
	//������˹ͼ����ǿ
	Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, 0, 4, 0, 0, -1, 0);
	Mat Enhance;
	filter2D(src, Enhance, CV_8UC3, kernel);
	//imshow("������˹����ͼ����ǿ", Enhance);
	hist_draw(Enhance);
	showMultiImages(title, 2, src, Enhance);
	waitKey(0);

}


void hist_draw(Mat src_hist) {
	//��ͨ��
	vector<Mat> src_channels;
	split(src_hist, src_channels);
	//��ֱͨ��ͼ
	Mat b_Hist, g_Hist, r_Hist;
	calcHist(&src_channels[0], 1, 0, Mat(), b_Hist, 1, &histSize, &histRanges, true, false);
	calcHist(&src_channels[1], 1, 0, Mat(), g_Hist, 1, &histSize, &histRanges, true, false);
	calcHist(&src_channels[2], 1, 0, Mat(), r_Hist, 1, &histSize, &histRanges, true, false);
	//������������һ��
	int hist_h = 400;
	int hist_w = 512;
	int bin_w = hist_w / 256;
	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
	normalize(b_Hist, b_Hist, 0, hist_h, NORM_MINMAX, -1, Mat());
	normalize(g_Hist, g_Hist, 0, hist_h, NORM_MINMAX, -1, Mat());
	normalize(r_Hist, r_Hist, 0, hist_h, NORM_MINMAX, -1, Mat());
	//��������ֱ��ͼ
	for (int i = 1; i < 256; i++) {
		line(histImage, Point((i - 1)*bin_w, hist_h - cvRound(b_Hist.at<float>(i - 1))), Point((i)*bin_w, hist_h - cvRound(b_Hist.at<float>(i))), Scalar(255, 0, 0), 2, LINE_AA);
		line(histImage, Point((i - 1)*bin_w, hist_h - cvRound(g_Hist.at<float>(i - 1))), Point((i)*bin_w, hist_h - cvRound(g_Hist.at<float>(i))), Scalar(0, 255, 0), 2, LINE_AA);
		line(histImage, Point((i - 1)*bin_w, hist_h - cvRound(r_Hist.at<float>(i - 1))), Point((i)*bin_w, hist_h - cvRound(r_Hist.at<float>(i))), Scalar(0, 0, 255), 2, LINE_AA);
	}
	if (count_hist == 0) {
		imshow("ԭͼ��ֱ��ͼ", histImage);
	}
	else if (count_hist == 1) {
		imshow("ͼ����ǿֱ��ͼ", histImage);
	}
	count_hist++;
}

//ͬ����ʾ
void showMultiImages(const char* title, int nArgs, ...) {
	Mat DispImage;
	int size;
	int ind;
	int x, y;
	int w, h;
	int r, c;
	float scale;
	int max;
	int space;
	if (nArgs <= 0) {
		printf("Number of arguments too small..../n");
		return;
	}
	else if (nArgs > 12) {
		printf("Number of arguments too large..../n");
		return;
	}
	else if (nArgs == 1) {
		r = c = 1;
		size = 300;
	}
	else if (nArgs == 2) {
		r = 2; c = 1;
		size = 300;
	}
	else if (nArgs == 3 || nArgs == 4) {
		r = 2; c = 2;
		size = 300;
	}
	else if (nArgs == 5 || nArgs == 6) {
		r = 3; c = 2;
		size = 200;
	}
	else if (nArgs == 7 || nArgs == 8) {
		r = 4;
		c = 2;
		size = 200;
	}
	else {
		r = 4;
		c = 3;
		size = 150;
	}
	DispImage = Mat(Size(20 * (r)+size * r, 20 * (c)+size * c), CV_8UC3);
	va_list args;
	va_start(args, nArgs);
	space = 20;
	for (ind = 0, x = space, y = space; ind < nArgs; ind++, x += (space + size)) {
		Mat img = va_arg(args, Mat);
		if (img.data == NULL) {
			printf("Invalid arguments");
			return;
		}
		w = img.cols;
		h = img.rows;
		max = (w > h) ? w : h;
		scale = (float)((float)max / size);
		if (ind % r == 0 && x != space) {
			x = space;
			y += space + size;
		}
		Mat roi = Mat(DispImage, Rect(x, y, (int)(w / scale), (int)(h / scale)));
		resize(img, roi, Size((int)(w / scale), (int)(h / scale)));
	}
	imshow(title, DispImage);
	waitKey(10);
	va_end(args);
}




// �Ƴ������ŶȺ���
void postprocess(Mat& frame, const vector<Mat>& outs)
{
	vector<int> classIds;//����ʶ���������
	vector<float> confidences;//�������Ŷ�
	vector<Rect> boxes;//����߿�
	for (size_t i = 0; i < outs.size(); ++i)
	{
		//�����������ɨ�����б߽��
		//���������Ŷ�ѡ��
		//Ŀ������data:x,y,w,hΪ�ٷֱȣ�x,yΪĿ�����ĵ�����
		float* data = (float*)outs[i].data;
		for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols)
		{
			Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
			Point classIdPoint;
			//���Ŷ�
			double confidence;
			//ȡ��������ֵ������
			minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
			if (confidence > confThreshold)
			{
				int centerX = (int)(data[0] * frame.cols);
				int centerY = (int)(data[1] * frame.rows);
				int width = (int)(data[2] * frame.cols);
				int height = (int)(data[3] * frame.rows);
				int left = centerX - width / 2;
				int top = centerY - height / 2;
				classIds.push_back(classIdPoint.x);
				confidences.push_back((float)confidence);
				boxes.push_back(Rect(left, top, width, height));
			}
		}
	}
	//�����Ŷ�
	vector<int> indices;//����û���ص��߿������
	NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices);
	//���������ص��߿�
	for (size_t i = 0; i < indices.size(); ++i)
	{
		int idx = indices[i];
		Rect box = boxes[idx];
		//ֻѡ����
		//if (classIds[idx] != 0)
			//continue;
		drawPred(classIds[idx], confidences[idx], box.x, box.y,
			box.x + box.width, box.y + box.height, frame);
	}
}

//����Ԥ��߽��
void drawPred(int classId, float conf, int left, int top, int right, int bottom, Mat& frame)
{
	//���Ʊ߽��
	rectangle(frame, Point(left, top), Point(right, bottom), Scalar(255, 178, 50), 2);
	//������Ŷ�
	string label = format("%.2f", conf);
	if (!classes.empty())
	{
		CV_Assert(classId < (int)classes.size());
		label = classes[classId] + ":" + label;
	}
	//���Ʊ߽���ϵı�ǩ
	int baseLine;
	Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
	top = max(top, labelSize.height);
	rectangle(frame, Point(left, top - round(1.5*labelSize.height)), Point(left + round(1.5*labelSize.width), top + baseLine), Scalar(0, 255, 255), FILLED);
	putText(frame, label, Point(left, top), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 0), 1);
}

//�������õ�����
vector<String> getOutputsNames(const Net& net)
{
	static vector<String> names;
	if (names.empty())
	{
		//ȡ�������ָ��
		vector<int> outLayers = net.getUnconnectedOutLayers();
		vector<String> layersNames = net.getLayerNames();
		// ȡ�����������
		names.resize(outLayers.size());
		for (size_t i = 0; i < outLayers.size(); ++i)
			names[i] = layersNames[outLayers[i] - 1];
	}
	return names;
}
void yolo_find(Mat input) {
	// ��dnn�м���ͼƬ blob
		//frame����bolbFromImage()��������Ϊ���������������bolb
	blobFromImage(input, blob, 1 / 255.0, cvSize(inpWidth, inpHeight),
		Scalar(0, 0, 0), true, false);
	//�������
	net.setInput(blob);
	// ���ڴ洢���
	vector<Mat> outs;
	net.forward(outs, getOutputsNames(net));
	// �Ƴ������Ŷȵı߿�
	postprocess(input, outs);
	// ��ʾ����ʱ�䲢���
	vector<double> layersTimes;
	double freq = getTickFrequency() / 1000;
	double t = net.getPerfProfile(layersTimes) / freq;
	string label = format("Inference time for a picture : %.2f ms", t);
	//����ı�
	putText(input, label, Point(0, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
}


//��֤��ʶ��
void processFrame(Mat & binary_1, Rect &rect)
{
	string str;
	vector<vector<Point>> contours;
	vector<Vec4i> hireachy;
	findContours(binary_1, contours, hireachy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	if (contours.size() > 0)
	{
		for (size_t t = 0; t < contours.size(); t++) {
			Mat img_2;
			rect = boundingRect(contours[static_cast<int>(t)]);
			Rect rect_3(rect.x - rect.width / 4, rect.y - rect.height / 4, rect.width + rect.width / 2, rect.height + rect.height / 2);
			//���λ���
			img_2 = binary_1(rect_3);
			rectangle(src, Point2i(rect.x - rect.width / 3, rect.y - rect.height / 4), Point2i(rect.x + rect.width + rect.width / 3, rect.y + rect.height + rect.height / 4), Scalar(0, 0, 255), 3, 8, 0);
			svm_match(img_2, rect);
			//svmƥ��
			namedWindow("�ָ���", CV_WINDOW_NORMAL);
			imshow("�ָ���", img_2);
			waitKey(500);
		}
	}
}
void svm_match(Mat image_2, Rect &rect) {
	resize(image_2, image_2, Size(20, 20));
	//cvtColor(image_2, image_2, COLOR_BGR2GRAY);
	//threshold(image_2, image_2, 0, 255, CV_THRESH_OTSU);
	//imshow("ԭͼ��", image_2);
	Mat input;
	image_2 = image_2.reshape(0, 1);//����ͼƬ���л�
	input.push_back(image_2);
	input.convertTo(input, CV_32FC1);//����ͼƬ���ݵ����ͣ���Ҫ����Ȼ����� 
	Ptr<ml::SVM> svm = Algorithm::load<ml::SVM>(pro_dir + "svm.xml");
	float r = svm->predict(input);
	//�������н���Ԥ��
	stream << r;
	string m;
	stream >> m;
	putText(src, m, Point(rect.x, rect.y), CV_FONT_HERSHEY_SIMPLEX, 3, Scalar(0, 0, 255), 6, 8);
	stream.clear();
	m.clear();
}
