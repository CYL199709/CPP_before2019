#include <iostream>
#include <cmath>
#include <ctime>
//�������ά��
#define MaxN 300 
using namespace std;

//��������
double *CreateMatrix_A(int N)
{
	double *pMatrix_ = new double[N*N + 1]; //һά�����Ӷ�ά����,��һ��Ԫ��������ά����ϢN
	cout << "���������Ԫ�ء�����������" << endl;
	for (int i = 1; i <= N*N; i++)
	{
		cin >> pMatrix_[i];
	}
	//cout << "������" << endl << "pMatrix_A" << endl;
	pMatrix_[0] = N;
	return pMatrix_;
}

//������λ��
double *CreateMatrix_one(int N)
{
	double *pMatrix_one = new double[N * N + 1]; //һά�����Ӷ�ά����,��һ��Ԫ��������ά����ϢN
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

//���ȱ任���� Eij��������i��j����
int Change_Eij(double Mat[], int ri, int rj) //ri,rj���Ǵ�1����ʼ��ʾ����
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

//���ȱ任���� Eij(k)  Ei(k) 
int Change_EijK(double Mat[], int ri, double k = 1, int rj = 0)//ri,rj���Ǵ�1����ʼ��ʾ����
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

//���ؾ���ֵ���ֵ���ڵ���
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

	mat[0] = post +1;// ��Ԫ���ڵ���
	if (max == 0)
		return false;  //���󲻿���
	return true; //�������
}


//�������桪�������б任��[Mat1  Mat2]
bool invMatrix(double Mat1[], double Mat2[])
{
	bool inv_Yes;//�Ƿ�����־
	int N = Mat1[0];//����λ��
	int main_row = 0;//��Ԫ������
	double PriElementValue = 0;//��Ԫֵ
	double Nextvalue = 0;//������Ԫʱʹ�� 
	double *tempCol_Mat1 = new double[N];

	for (int s = 0; s < N; s++) //������Ԫ�������Mat1��Ϊ��λ�����Ǿ���
	{	
		for (int i = 0; i < N; i++)//ȡMat1��������
			tempCol_Mat1[i] = Mat1[1 + i * N + s];

		inv_Yes = max_post(tempCol_Mat1, s, N);
		if (inv_Yes == false)//���󲻿���
			return false;

		main_row = tempCol_Mat1[0];
		//����ʱ������Ԫ��
		Change_Eij(Mat1, s + 1, main_row);//A
		Change_Eij(Mat2, s + 1, main_row);//��չ��

		PriElementValue = Mat1[1 + s * (N + 1)];//��Ԫֵ
		Change_EijK(Mat1, s + 1, 1.0/PriElementValue); //�����г�����Ԫֵ�����Խ�Ԫ��Ϊ1
		Change_EijK(Mat2, s + 1, 1.0/PriElementValue);//��չ��

		for (int i = s + 1; i < N; i++)//��Ԫ
		{
			Nextvalue = Mat1[i * N + 1 + s];
			Change_EijK(Mat1, s + 1, -Nextvalue, i + 1);
			Change_EijK(Mat2, s + 1, -Nextvalue, i + 1);
		}
	}

	for (int s = 0; s < N; s++) 
			if (Mat1[1 + s * (N + 1)] == 0)//��Ԫֵ
				return false;//������

	for (int j = 0, s = N; s > 1; --s)//������Ԫ�������Mat1��Ϊ��λ����
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


//��ʾ����
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
	int N;//����ά��
	bool cri_Y;//����
	cout << "����������ά����";
	cin >> N; //�������ά��

	double *pMatrix_A = CreateMatrix_A(N);//��������A
	double *pMatrix_Aug = CreateMatrix_one(N);//������λ��

	cout << "ԭ������"<<endl;
	MatrixShow(pMatrix_A);//��ʾ����A

	//cout << "ԭ��λ��" << endl;
	//MatrixShow(pMatrix_Aug);//��ʾ����I

	cri_Y = invMatrix(pMatrix_A, pMatrix_Aug);

	if (cri_Y == false)
		cout << "���󲻿���" << endl;
	else
	{
		cout << "�����" << endl;
		MatrixShow(pMatrix_Aug);//��ʾ����A
	}

	system("pause");

	delete[] pMatrix_A;
	delete[] pMatrix_Aug;
	
	return 0;
}
