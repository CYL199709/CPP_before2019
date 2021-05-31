#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<string.h>
#include<iostream>
using namespace std;
class STU{
public:
	
	void AddData(); //信息插入
	void input();     //信息输入
	void del();        //信息删除
	void rev();        //信息修改
	void find();       //信息查询
	void disp();      //信息显示
private:
	
		int num;        //学号
		char name[20];  //姓名
		char sex[20];    //性别
		char ban[20];   //班级	
};


void menu();        //菜单
STU student[100];
static int n = 0;
int main()         //主函数
{
	
	int i,num;
	
	while(1)
	{
		menu();
		cin >> i;
		switch(i)
		{
		case 1: student[n].input(); break;
		case 2: student[n].del(); break;
		case 3: student[n].rev(); break;
		case 4: student[n].find(); break;
		case 5: student[n].disp(); break;
		case 6: student[n].AddData(); break;
		case 7: exit(0);
		}
	}
	return 0;
}

void STU::input()       //信息添加
{
	int i=1;
	system("cls");             //清屏
	cout<<"\n信息输入\n";
	cout << "\n请输入学生人数(1-100):";
	int num;
	cin>>num;
	cout<<"\n请输入学生信息:\n";
	
	
	cout<<"请按顺序输入学号，姓名，性别，班级";
	for(int i=0;i<num;i++)
	{
		cout << endl << i+n << endl;;
		cin >> student[i + n].num >> student[i + n].name >> student[i + n].sex >> student[i + n].ban; 
	}
	n += num;
	cout<<"\n按回车键结束输入并返回";
	getch(); //获得一个输入的字符
}

void STU::del()         //信息删除
{
	int i,j,k=0,x;
	
	int num;
	char name[20];
	system("cls");            //清屏
	cout<<"\n信息删除\n";
	cout<<"\n请选择按[1/2]删除\n 1.学号  2.姓名\n";
	cin>>x;
	if(x==1)
	{	
		cout<<"\n请输入要删除信息的学号：";
		cin>>num;
		for (i = 0; i < n; i++)
			if(num==student[i].num)         //按学号查找
			{	k=1;
				student[i] = student[n];
				break;
			}
			
	}
	else if(x==2)
	{
		cout<<"\n请输入要删除信息的姓名：";
		cin>>name;
		for(i=0;i<n;i++)
			if(strcmp(name,student[i].name)==0)  //按姓名查找
			{	k=1;
			student[i] = student[n];
			break;
			}

	}
	if (k == 1) n--;
	else
	{
		cout << ("\n查无此人！\n");
	}
	cout<<"\n按回车键返回";
	getch();
}

void STU::rev()         //信息修改
{
	int i,k=0,x;

	int num;
	char name[20];
	system("cls");            //清屏
	cout<<"\n信息修改\n";
	cout<<"\n请选择按[1/2]修改\n 1.学号  2.姓名\n";
	cin>>x;
	if (x==1)
	{
		cout<<"\n请输入要修改信息的学号：";
		cin>>num;
		for(i=0;i<n;i++)
			if(num==student[i].num)        //按学号查找
			{	k=1;
				cout<<"\n请按顺序输入要修改的学号，姓名，性别，班级\n";
				cin >> student[i].num >> student[i].name >> student[i].sex >> student[i].ban;
				break;
			}
	}
	else if(x==2)
	{
		cout<<"\n请输入要修改信息的姓名：";
		cin>>name;
		for(i=0;i<n;i++)
			if(strcmp(name,student[i].name)==0)        //按姓名查找
			{	k=1;
				cout<<"\n请按顺序输入要修改的学号，姓名，性别，班级";
				cin >> student[i].num >> student[i].name >> student[i].sex >> student[i].ban;
				break;
			}
			
	}
	if (k==0)
	{
		cout << "\n查无此人！\n";
	}
	cout<<"\n按回车键返回";
	getch();
}

void STU::find()        //信息查询
{
	int k=0,x;
	int num;
	
	char name[20];
	system("cls");            //清屏
	cout<<"\n信息查询\n";
	cout<<"\n请选择按[1/2]查询\n 1.学号  2.姓名\n";
	cin>>x;
	if(x==1)
	{
		cout<<"请输入你的学号：";
		cin>>num;
		for(int i=0;i<n;i++)
			if(num==student[i].num)        //按学号查找
			{	k=1;
				cout<<"\n已查询到，是：\n";
				cout<<"学号      姓名      性别    班级\n";
				cout << student[i].num << "	" << student[i].name << "	" << student[i].sex << "	" << student[i].ban;
				break;
			}
		if(!k)
			cout<<"\n抱歉，没有查询到！\n";
	}
	else if(x==2)
	{
		cout<<"请输入你的姓名：";
		cin>>name;
		for (int i = 0; i<n; i++)
			if(strcmp(name,student[i].name)==0)        //按姓名查找
			{	k=1;
				cout<<"\n已查询到，是：\n";
				cout<<"学号      姓名      性别    班级\n";
				cout << student[i].num << "	" << student[i].name << "	" << student[i].sex << "	" << student[i].ban;
				break;
			}
		if(!k)
			cout<<"\n抱歉，没有查询到！\n";
	}
	cout<<"\n按回车键返回";
	getch();
}


void STU::AddData()
{
	
	int i,m;
	system("cls");            //清屏
	cout<<"输入要添加的学生数量\n";
	cin>>m;
	cout<<"请按顺序输入学号，姓名，性别，班级\n";
	
	for (i = 0;i<m; i++)
	{
		cin >> student[n + i].num >> student[n + i].name >> student[n + i].sex >> student[n + i].ban;
	}

	n += m;
	
	cout<<"\n按回车键返回";
	getch();
}

void STU::disp()        //信息显示
{    	
		cout<<"\n信息显示\n";
		cout<<"\n学号      姓名      性别    班级\n";
		for (int i = 0; i < n;i++)
		{
			cout << student[i].num << "	" << student[i].name << "	" << student[i].sex << "	" << student[i].ban << endl;
		}
		
	cout<<"\n按回车键返回";
	getch();
}

void menu()          //菜单
{   system("cls"); 
	cout<<"\n\t\t  ************************  \n";
	cout << "\t\t          欢迎使用        \t\n";
	cout << "\t\t      学生成绩管理系统    \t\n";
	cout << "\t\t  ************************  \n\n";
	cout << "\t\t****************************\n";
	cout << "\t\t**                        **\n";
	cout << "\t\t**         主菜单         **\n";
	cout << "\t\t**                        **\n";
	cout << "\t\t****************************\n\n";
	cout << "\t\t   1.学生信息输入  2.学生信息删除  \n\n";
		cout << "\t\t   3.学生信息修改  4.学生信息查询  \n\n";
	cout << "\t\t   5.学生信息显示  6.学生信息插入  \n\n";
	cout << "\t\t   7.退出系统  \n\n";
	cout << "\t\t  请选择[1/2/3/4/5/6/7]: \n";
}