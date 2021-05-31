

#include <iostream>
#include<fstream>
#include <cstdlib>
using namespace std;

typedef struct _info_
{
	int saveBlackNum;
	int * saveBlack;

}Info;
static Info g_info;


Info * getInfo()
{
	return &g_info;
}
int LinkMySelfBlack(unsigned char * destData, int x, int y, int width, int height)
{

	Info * t_info = getInfo();
	int flag = 0;
	/*if (x - 1 >= 0 && x - 1 < width && y - 1 >= 0 && y - 1 < height)
	{
		if (0 == destData[(y - 1) * width + (x - 1)])
		{
			t_info->saveBlack[t_info->saveBlackNum * 2 + 0] = x - 1;
			t_info->saveBlack[t_info->saveBlackNum * 2 + 1] = y - 1;
			t_info->saveBlackNum++;

			destData[(y - 1) * width + (x - 1)] = 255;

		}

	}*/

	if (x >= 0 && x < width && y - 1 >= 0 && y - 1 < height)
	{
		if (0 == destData[(y - 1) * width + (x)])
		{
			t_info->saveBlack[t_info->saveBlackNum * 2 + 0] = x;
			t_info->saveBlack[t_info->saveBlackNum * 2 + 1] = y - 1;
			t_info->saveBlackNum++;
			flag = 1;
			destData[(y - 1) * width + x] = 255;

		}
	}

	/*if (x + 1 >= 0 && x + 1 < width && y - 1 >= 0 && y - 1 < height)
	{
		if (0 == destData[(y - 1) * width + (x + 1)])
		{
			t_info->saveBlack[t_info->saveBlackNum * 2 + 0] = x + 1;
			t_info->saveBlack[t_info->saveBlackNum * 2 + 1] = y - 1;
			t_info->saveBlackNum++;

			destData[(y - 1) * width + (x + 1)] = 255;

		}
	}*/

	/*if (x - 1 >= 0 && x - 1 < width && y + 1 >= 0 && y + 1 < height)
	{
		if (0 == destData[(y + 1) * width + (x - 1)])
		{
			t_info->saveBlack[t_info->saveBlackNum * 2 + 0] = x - 1;
			t_info->saveBlack[t_info->saveBlackNum * 2 + 1] = y + 1;
			t_info->saveBlackNum++;

			destData[(y + 1) * width + (x - 1)] = 255;

		}
	}*/

	if (x >= 0 && x < width && y + 1 >= 0 && y + 1 < height)
	{
		if (0 == destData[(y + 1) * width + (x)])
		{
			t_info->saveBlack[t_info->saveBlackNum * 2 + 0] = x;
			t_info->saveBlack[t_info->saveBlackNum * 2 + 1] = y + 1;
			t_info->saveBlackNum++;
			flag = 1;
			destData[(y + 1) * width + (x)] = 255;

		}
	}

	/*if (x + 1 >= 0 && x + 1 < width && y + 1 >= 0 && y + 1 < height)
	{
		if (0 == destData[(y + 1) * width + (x + 1)])
		{
			t_info->saveBlack[t_info->saveBlackNum * 2 + 0] = x + 1;
			t_info->saveBlack[t_info->saveBlackNum * 2 + 1] = y + 1;
			t_info->saveBlackNum++;

			destData[(y + 1) * width + (x + 1)] = 255;

		}
	}*/

	if (x - 1 >= 0 && x - 1 < width && y >= 0 && y < height)
	{
		if (0 == destData[(y)* width + (x - 1)])
		{
			t_info->saveBlack[t_info->saveBlackNum * 2 + 0] = x - 1;
			t_info->saveBlack[t_info->saveBlackNum * 2 + 1] = y;
			t_info->saveBlackNum++;
			flag = 1;
			destData[(y)* width + (x - 1)] = 255;

		}
	}

	if (x + 1 >= 0 && x + 1 < width && y >= 0 && y < height)
	{
		if (0 == destData[(y)* width + (x + 1)])
		{
			t_info->saveBlack[t_info->saveBlackNum * 2 + 0] = x + 1;
			t_info->saveBlack[t_info->saveBlackNum * 2 + 1] = y;
			t_info->saveBlackNum++;
			flag = 1;
			destData[(y)* width + (x + 1)] = 255;

		}
	}


	return flag;

}
int getAear(unsigned char * image, int width, int height)
{
	int flag = 0;
	int i = 0;
	int j = 0;
	int temp = 0;
	int tempi = 0;
	ofstream rout("result.txt");
	Info * t_info = getInfo();
	t_info->saveBlack = (int *)calloc(sizeof(int), width * height * 2);
	int index = 0;
	for (j = 0; j < width; j++)
	{
		for (i = 0; i < height; i++)
		{
			
			temp = image[i * width + j];
			if (0 == temp)
			{
				tempi = 0;
				t_info->saveBlackNum = 0;
				t_info->saveBlack[t_info->saveBlackNum * 2 + 0] = j;
				t_info->saveBlack[t_info->saveBlackNum * 2 + 1] = i;
				t_info->saveBlackNum++;
				
				image[i * width + j] = 255;
				
				flag = LinkMySelfBlack(image, j, i, width, height);
				

				while (tempi < t_info->saveBlackNum)
				{				
					flag = LinkMySelfBlack(image, t_info->saveBlack[tempi * 2 + 0], t_info->saveBlack[tempi * 2 + 1], width, height);
					//if (flag == 1)
					{
						rout << '(' << t_info->saveBlack[tempi * 2 + 1] << ',';
						rout << t_info->saveBlack[tempi * 2 + 0] << ')';
					}
						
					tempi++;

				}

				printf("第[%d]个黑色的区域面积 = %d \n", index, t_info->saveBlackNum);
				rout << "\n第[" << index << "]个黑色的区域面积 =" << t_info->saveBlackNum << " \n";
				
				index++;
			}

			
		}
	}
	rout.close();
	return 0;
}


const int M = 20;
const int N = 20;
int main()
{
	unsigned char A[M][N];//图像数组
	ofstream out("data.txt");
	for (int i = 0; i < M; i++)
		for (int j = 0; j < N; j++)
		{
			A[i][j] = unsigned char(255 * rand() % 2);
		}

	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
			out << int(A[i][j]) << ' ';
		out << endl;
	}
	out.close();

	getAear(*A, M, N);


	system("pause");

	return 0;
}

