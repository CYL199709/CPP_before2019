#include <iostream>
#include <cmath>
#include <ctime>
//定义最大维度
#define MaxN 300 
using namespace std;

//创建矩阵
double *CreateMatrix_A(int N)
{
	double *pMatrix_ = new double[N*N + 1]; //一维向量从二维矩阵,第一个元素数矩阵维度信息N
	cout << "请输入矩阵元素——按行输入" << endl;
	for (int i = 1; i <= N*N; i++)
	{
		cin >> pMatrix_[i];
	}
	//cout << "输入完" << endl << "pMatrix_A" << endl;
	pMatrix_[0] = N;
	return pMatrix_;
}

//创建单位阵
double *CreateMatrix_one(int N)
{
	double *pMatrix_one = new double[N * N + 1]; //一维向量从二维矩阵,第一个元素数矩阵维度信息N
	for (int j=0, i = 1; i <= N * N; i++)
	{
		if (i ==  1 + j * (N + 1))
		{
			pMatrix_one[i] = 1;
			j += 1;
		}
		else pMatrix_one[i] = 0;
	}
	pMatrix_one[0] = N;
	return pMatrix_one;
}

//初等变换函数 Eij——交换i、j两行
int Change_Eij(double Mat[], int ri, int rj) //ri,rj都是从1来开始表示行数
{
	double temp = 0;
	int N = Mat[0];
	if (ri == rj || ri < 1 || rj < 1 || ri > N || rj > N)
		return 0;
	else 
		for (int i = 1; i <= N; i++)
		{
			temp = Mat[N * (ri - 1) + i];
			Mat[N * (ri - 1) + i] = Mat[N * (rj - 1) + i];
			Mat[N * (rj - 1) + i] = temp;
		}
		return 0;
}

//初等变换函数 Eij(k)  Ei(k) 
int Change_EijK(double Mat[], int ri, double k = 1, int rj = 0)//ri,rj都是从1来开始表示行数
{
	int N = Mat[0];
	if (rj != 0)   //Eij(k) 
	{
		if (ri < 1 || rj < 1 || ri > N || rj > N || k ==0)
			return 0;
		else 
			for (int i = 1; i <= N; i++)
				Mat[N * (rj - 1) + i] = Mat[N * (rj - 1) + i] + k * Mat[N * (ri - 1) + i];
	}
	else if (rj == 0)  //Ei(k)
		if (ri < 1 || ri > N || k == 0)
			return 0;
		else
			for (int i = 1; i <= N; i++)
				Mat[N * (ri - 1) + i] = k * Mat[N * (ri - 1) + i];
	return 0;
}

//返回绝对值最大值所在的行
bool max_post(double  mat[], int start, int length)
{
	double max = -1;
	int post = 0;
	for (int i = start; i < length; i++)
	{
		if (abs(mat[i]) > max)
		{
			max = abs(mat[i]);
			post = i;
		}
	}

	mat[0] = post +1;// 主元所在的行
	if (max == 0)
		return false;  //矩阵不可逆
	return true; //矩阵可逆
}


//矩阵求逆——初等行变换法[Mat1  Mat2]
bool invMatrix(double Mat1[], double Mat2[])
{
	bool inv_Yes;//是否可逆标志
	int N = Mat1[0];//矩阵位数
	int main_row = 0;//主元所在行
	double PriElementValue = 0;//主元值
	double Nextvalue = 0;//用于消元时使用 
	double *tempCol_Mat1 = new double[N];

	for (int s = 0; s < N; s++) //向下消元——最后Mat1变为单位上三角矩阵
	{	
		for (int i = 0; i < N; i++)//取Mat1的列向量
			tempCol_Mat1[i] = Mat1[1 + i * N + s];

		inv_Yes = max_post(tempCol_Mat1, s, N);
		if (inv_Yes == false)//矩阵不可逆
			return false;

		main_row = tempCol_Mat1[0];
		//可逆时交换主元行
		Change_Eij(Mat1, s + 1, main_row);//A
		Change_Eij(Mat2, s + 1, main_row);//扩展阵

		PriElementValue = Mat1[1 + s * (N + 1)];//主元值
		Change_EijK(Mat1, s + 1, 1.0/PriElementValue); //将改行除以主元值，将对角元化为1
		Change_EijK(Mat2, s + 1, 1.0/PriElementValue);//扩展阵

		for (int i = s + 1; i < N; i++)//消元
		{
			Nextvalue = Mat1[i * N + 1 + s];
			Change_EijK(Mat1, s + 1, -Nextvalue, i + 1);
			Change_EijK(Mat2, s + 1, -Nextvalue, i + 1);
		}
	}

	for (int s = 0; s < N; s++) 
			if (Mat1[1 + s * (N + 1)] == 0)//主元值
				return false;//不可逆

	for (int j = 0, s = N; s > 1; --s)//向上消元——最后Mat1变为单位矩阵
	{
		for (int i =1 ; i <s ; i++)
		{
			Nextvalue = Mat1[N * N - j - i * N * (j + 1)];
			Change_EijK(Mat1, s, -Nextvalue, s - i);
			Change_EijK(Mat2, s, -Nextvalue, s - i);
		}
		j++;
	}

	delete[] tempCol_Mat1;
	return true;
}


//显示矩阵
void MatrixShow(double Mat[])
{
	for (int j = 1, i = 1; i <= pow(Mat[0], 2); i++)
	{
		if (i == j * Mat[0] +1)
		{
			j += 1;
			cout << endl;
		}
		cout << Mat[i] << "   ";
	}
	cout << endl;
}

int main()
{
	int N;//矩阵维度
	bool cri_Y;//可逆
	cout << "请输入矩阵的维数：";
	cin >> N; //输入矩阵维度

	double *pMatrix_A = CreateMatrix_A(N);//创建矩阵A
	double *pMatrix_Aug = CreateMatrix_one(N);//创建单位阵

	cout << "原矩阵阵"<<endl;
	MatrixShow(pMatrix_A);//显示矩阵A

	//cout << "原单位阵" << endl;
	//MatrixShow(pMatrix_Aug);//显示矩阵I

	cri_Y = invMatrix(pMatrix_A, pMatrix_Aug);

	if (cri_Y == false)
		cout << "矩阵不可逆" << endl;
	else
	{
		cout << "逆矩阵" << endl;
		MatrixShow(pMatrix_Aug);//显示矩阵A
	}

	system("pause");

	delete[] pMatrix_A;
	delete[] pMatrix_Aug;
	
	return 0;
}
