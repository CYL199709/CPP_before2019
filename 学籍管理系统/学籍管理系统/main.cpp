#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<string.h>
#include<iostream>
using namespace std;
class STU{
public:
	
	void AddData(); //��Ϣ����
	void input();     //��Ϣ����
	void del();        //��Ϣɾ��
	void rev();        //��Ϣ�޸�
	void find();       //��Ϣ��ѯ
	void disp();      //��Ϣ��ʾ
private:
	
		int num;        //ѧ��
		char name[20];  //����
		char sex[20];    //�Ա�
		char ban[20];   //�༶	
};


void menu();        //�˵�
STU student[100];
static int n = 0;
int main()         //������
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

void STU::input()       //��Ϣ���
{
	int i=1;
	system("cls");             //����
	cout<<"\n��Ϣ����\n";
	cout << "\n������ѧ������(1-100):";
	int num;
	cin>>num;
	cout<<"\n������ѧ����Ϣ:\n";
	
	
	cout<<"�밴˳������ѧ�ţ��������Ա𣬰༶";
	for(int i=0;i<num;i++)
	{
		cout << endl << i+n << endl;;
		cin >> student[i + n].num >> student[i + n].name >> student[i + n].sex >> student[i + n].ban; 
	}
	n += num;
	cout<<"\n���س����������벢����";
	getch(); //���һ��������ַ�
}

void STU::del()         //��Ϣɾ��
{
	int i,j,k=0,x;
	
	int num;
	char name[20];
	system("cls");            //����
	cout<<"\n��Ϣɾ��\n";
	cout<<"\n��ѡ��[1/2]ɾ��\n 1.ѧ��  2.����\n";
	cin>>x;
	if(x==1)
	{	
		cout<<"\n������Ҫɾ����Ϣ��ѧ�ţ�";
		cin>>num;
		for (i = 0; i < n; i++)
			if(num==student[i].num)         //��ѧ�Ų���
			{	k=1;
				student[i] = student[n];
				break;
			}
			
	}
	else if(x==2)
	{
		cout<<"\n������Ҫɾ����Ϣ��������";
		cin>>name;
		for(i=0;i<n;i++)
			if(strcmp(name,student[i].name)==0)  //����������
			{	k=1;
			student[i] = student[n];
			break;
			}

	}
	if (k == 1) n--;
	else
	{
		cout << ("\n���޴��ˣ�\n");
	}
	cout<<"\n���س�������";
	getch();
}

void STU::rev()         //��Ϣ�޸�
{
	int i,k=0,x;

	int num;
	char name[20];
	system("cls");            //����
	cout<<"\n��Ϣ�޸�\n";
	cout<<"\n��ѡ��[1/2]�޸�\n 1.ѧ��  2.����\n";
	cin>>x;
	if (x==1)
	{
		cout<<"\n������Ҫ�޸���Ϣ��ѧ�ţ�";
		cin>>num;
		for(i=0;i<n;i++)
			if(num==student[i].num)        //��ѧ�Ų���
			{	k=1;
				cout<<"\n�밴˳������Ҫ�޸ĵ�ѧ�ţ��������Ա𣬰༶\n";
				cin >> student[i].num >> student[i].name >> student[i].sex >> student[i].ban;
				break;
			}
	}
	else if(x==2)
	{
		cout<<"\n������Ҫ�޸���Ϣ��������";
		cin>>name;
		for(i=0;i<n;i++)
			if(strcmp(name,student[i].name)==0)        //����������
			{	k=1;
				cout<<"\n�밴˳������Ҫ�޸ĵ�ѧ�ţ��������Ա𣬰༶";
				cin >> student[i].num >> student[i].name >> student[i].sex >> student[i].ban;
				break;
			}
			
	}
	if (k==0)
	{
		cout << "\n���޴��ˣ�\n";
	}
	cout<<"\n���س�������";
	getch();
}

void STU::find()        //��Ϣ��ѯ
{
	int k=0,x;
	int num;
	
	char name[20];
	system("cls");            //����
	cout<<"\n��Ϣ��ѯ\n";
	cout<<"\n��ѡ��[1/2]��ѯ\n 1.ѧ��  2.����\n";
	cin>>x;
	if(x==1)
	{
		cout<<"���������ѧ�ţ�";
		cin>>num;
		for(int i=0;i<n;i++)
			if(num==student[i].num)        //��ѧ�Ų���
			{	k=1;
				cout<<"\n�Ѳ�ѯ�����ǣ�\n";
				cout<<"ѧ��      ����      �Ա�    �༶\n";
				cout << student[i].num << "	" << student[i].name << "	" << student[i].sex << "	" << student[i].ban;
				break;
			}
		if(!k)
			cout<<"\n��Ǹ��û�в�ѯ����\n";
	}
	else if(x==2)
	{
		cout<<"���������������";
		cin>>name;
		for (int i = 0; i<n; i++)
			if(strcmp(name,student[i].name)==0)        //����������
			{	k=1;
				cout<<"\n�Ѳ�ѯ�����ǣ�\n";
				cout<<"ѧ��      ����      �Ա�    �༶\n";
				cout << student[i].num << "	" << student[i].name << "	" << student[i].sex << "	" << student[i].ban;
				break;
			}
		if(!k)
			cout<<"\n��Ǹ��û�в�ѯ����\n";
	}
	cout<<"\n���س�������";
	getch();
}


void STU::AddData()
{
	
	int i,m;
	system("cls");            //����
	cout<<"����Ҫ��ӵ�ѧ������\n";
	cin>>m;
	cout<<"�밴˳������ѧ�ţ��������Ա𣬰༶\n";
	
	for (i = 0;i<m; i++)
	{
		cin >> student[n + i].num >> student[n + i].name >> student[n + i].sex >> student[n + i].ban;
	}

	n += m;
	
	cout<<"\n���س�������";
	getch();
}

void STU::disp()        //��Ϣ��ʾ
{    	
		cout<<"\n��Ϣ��ʾ\n";
		cout<<"\nѧ��      ����      �Ա�    �༶\n";
		for (int i = 0; i < n;i++)
		{
			cout << student[i].num << "	" << student[i].name << "	" << student[i].sex << "	" << student[i].ban << endl;
		}
		
	cout<<"\n���س�������";
	getch();
}

void menu()          //�˵�
{   system("cls"); 
	cout<<"\n\t\t  ************************  \n";
	cout << "\t\t          ��ӭʹ��        \t\n";
	cout << "\t\t      ѧ���ɼ�����ϵͳ    \t\n";
	cout << "\t\t  ************************  \n\n";
	cout << "\t\t****************************\n";
	cout << "\t\t**                        **\n";
	cout << "\t\t**         ���˵�         **\n";
	cout << "\t\t**                        **\n";
	cout << "\t\t****************************\n\n";
	cout << "\t\t   1.ѧ����Ϣ����  2.ѧ����Ϣɾ��  \n\n";
		cout << "\t\t   3.ѧ����Ϣ�޸�  4.ѧ����Ϣ��ѯ  \n\n";
	cout << "\t\t   5.ѧ����Ϣ��ʾ  6.ѧ����Ϣ����  \n\n";
	cout << "\t\t   7.�˳�ϵͳ  \n\n";
	cout << "\t\t  ��ѡ��[1/2/3/4/5/6/7]: \n";
}