/*����һ�µİ�����֮ͬ����
��1������ӡ���ѯ���޸ġ�ɾ����������ܶ�����д��һ��������
��2��ͼ����Ψһ���ж�д��һ�������ĺ�����
��3�����˵�����д��һ��������
��4�������жϹ���д��һ��������*/

#include <iostream>
#include <iomanip>
#include <string> 
using namespace std;
#define MaxNum 100          		//���������С
#define N 14
//����ȫ�ֱ���
int     count1 = 0;               	//������¼��ǰͼ��ĸ���
string  order_num [MaxNum];    	//ͼ����
string  goods_num [MaxNum];   	//ͼ����
double  goods_price[MaxNum];   	//ͼ�鵥��
int     goods_count1[MaxNum];  	//ͼ������
string  name[MaxNum];         	//��������
//��������к���������
int  password();               	    //�����
void menu() ;                     	//���˵�����
void Append();                    	//���ͼ�麯��
int  effective();                	//�ж�ͼ����Ψһ�Ժ���
int  Search_order_num(string);	    //��ͼ���Ų�ѯ���������ҳɹ������������±꣬���򷵻�-1
void Delete_menu();           	    //ɾ������
void Modify();                  	//�޸�ͼ�麯��
void Print_goods(int);          	//���ͼ����Ϣ����,��ʾ������ָ���±��ͼ��
int main()                          //������
{
	if (password())
		menu();
	return 0;
}
void  menu( )//���˵�����
{
	cout<<endl;
	while(1)
	{
		cout<<"*****************************************************"<<endl;
		cout<<"*           ������������ѡ������������ţ�          *"<<endl;
        cout<<"*         1:���ͼ��               2:����ͼ��       *"<<endl;
		cout<<"*         3:�޸�ͼ��               4:ɾ��ͼ��       *"<<endl;
		cout<<"*         5:���ͼ��               0:�˳�           *"<<endl;
		cout<<"*****************************************************"<<endl;
	    int n,i;
	    cin>>n;
	    switch  (n) 
		{
		case 1:
			{
                Append( );
			    break;
			}
		case 2:
			{
				cout<<"������Ҫ���ҵ�ͼ���ţ�";
	            string num;
	            cin>>num;
				i=Search_order_num(num);
				if(i!=-1)
				{
					cout<<"���ҳɹ���"<<endl;
		            cout<<setw(N)<<"ͼ����"<<setw(N)<<"ͼ����";
		            cout<<setw(N)<<"ͼ�鵥��"<<setw(N)<<"ͼ������";
		            cout<<setw(N)<<"��������"<<endl;
		            cout<<setw(N)<<order_num[i]<<setw(N)<<goods_num[i]<<setw(N)<<goods_price[i];
		            cout<<setw(N)<<goods_count1[i]<<setw(N)<<name[i]<<endl;
				}
	            else
	            	cout<<"����ʧ�ܣ�"<<endl; 
			    break;
			 }
		case 3:
			{
                Modify();					  
			    break;
			 }
		case 4:
			{
                Delete_menu();					  
			    break;	              
			}
		case 5:
			{
				if(count1==0)
				{
					cout<<"������ͼ����Ϣ��"<<endl;
				}
				else
				{
					cout<<setw(N)<<"ͼ����"<<setw(N)<<"ͼ����"<<setw(N)<<"ͼ�鵥��";
	                cout<<setw(N)<<"ͼ������" <<setw(N)<<"��������"<<endl;
	                for(i=0;i<count1;i++)
						Print_goods(i);
				}
			    break;
			}
        case 0:
				return;
		default:
			cout<<"�����������������룡"<<endl;
		}
	}
}
int effective()//�ж�ͼ����Ψһ�Ժ���
{
	for(int i=0;i<count1;i++)
		if(order_num[count1]==order_num[i])
			return 0;
	return 1;
}
void Append()//���ͼ�麯��
{  
	cout<<"������ͼ����Ϣ��"<<endl;
	cout<<"ͼ���ţ�";
	cin>>order_num[count1];
	while(1)
	{
		if( effective()==0)
		{
			cout<<"ͼ�����ظ������������룡"<<endl;	
			cin>>order_num [count1];
		}
		else
			break;
	}
    cout<<"ͼ����:"; cin>>goods_num[count1];
	cout<<"ͼ�鵥��:"; cin>>goods_price[count1];
	cout<<"ͼ������:"; cin>>goods_count1[count1];
	cout<<"��������:"; cin>>name[count1];
	cout<<"�����ϣ�"<<endl;
	count1++; 
 }
int Search_order_num(string num)//���Һ���
{ 
	int i;
	for(i=0;i<count1;i++)
		if(order_num[i]==num)
			return i;
	return -1;  
}
void Print_goods(int i)//��ʾͼ����Ϣ
{
		cout<<setw(N)<<order_num[i]<<setw(N)<<goods_num[i]<<setw(N)<<goods_price[i];
		cout<<setw(N)<<goods_count1[i]<<setw(N)<<name[i]<<endl;
}
void Delete_menu()//ɾ������
{
	string ch;
	char xx;
	cout<<"������ɾ����ͼ���ţ�"<<endl;
	cin>>ch;
	int i=0,j;
	while(i<count1)
	{
		if(order_num[i]==ch)
			break;
		i++;
	}
	if(i>=count1)
		cout<<"��ͼ�鲻���ڣ�"<<endl;
	else
	{
		cout<<"�Ѳ�ѯ����ͼ����Ϣ��"<<endl;		
		cout<<setw(N)<<"ͼ����"<<setw(N)<<"ͼ����"<<setw(N)<<"ͼ������";
		cout<<setw(N)<<"ͼ�鵥��" <<setw(N)<<"��������"<<endl;
		Print_goods(i);
		cout<<"��ѡ��(ɾ��Y������N����";
		cin>>xx;
		if(xx=='y'||xx=='Y')
		{
			for(j=i+1;j<count1 ;j++)
			{
				order_num[j-1]=order_num[j];		
				goods_price[j-1]=goods_price[j];
				goods_num[j-1]=goods_num[j];
				name[j-1]=name[j];
				goods_count1[j-1]=goods_count1[j];														
			}
			count1--;
			cout<<"ɾ����ϣ�"<<endl;
		}
		else
			cout<<"��������ɾ��������"<<endl;
	}
}

void Modify( )//�޸ĺ������Ȱ�ͼ���Ž��в��ң����޸�
{
	string num;
	int m;
	cout<<"��������Ҫ�޸ĵ�ͼ���ţ�";
	cin>>num;
	m=Search_order_num(num);
	if(m==-1)
	{
		cout<<"������ͼ�������󣬿��в����ڸ�ͼ����Ϣ��"<<endl;
		return;
	}
	cout<<"��ǰͼ����Ϣ���£�"<<endl;
	cout<<setw(N)<<"ͼ����"<<setw(N)<<"ͼ����"<<setw(N)<<"ͼ������"<<setw(N)<<"ͼ�鵥��";
	cout<<setw(N)<<"��������"<<endl;
	Print_goods(m);
	while(1)
	{
		cout<<"************************************************************"<<endl;
		cout<<"*           ������������ѡ������������ţ�               *"<<endl;
		cout<<"*         1:�޸�ͼ���             2:�޸���������      *"<<endl;
		cout<<"*         3:�޸�ͼ�鵥��           4:�޸�ͼ������        *"<<endl;
		cout<<"*                            0:�˳�                      *"<<endl;
		cout<<"************************************************************"<<endl;
		int n;
		cin>>n;
		switch  (n) 
		{
		case 1:
			{
				cout<<"�������µ�ͼ���ţ�"<<endl;
				cin>>goods_num[m];
				cout<<"�޸���ϣ�"<<endl;
				break;
			}
		case 2:
			{
				cout<<"�������µ�����������"<<endl;
				cin>>name[m];
				cout<<"�޸���ϣ�"<<endl;
				break;
			}
		case 3:
			{
				cout<<"�������µ�ͼ�鵥�ۣ�"<<endl;
				cin>>goods_price[m];
				cout<<"�޸���ϣ�"<<endl;
				break;
			}
		case 4:
			{
				cout<<"�������µ�ͼ��������"<<endl;
				cin>>goods_count1[m];
				cout<<"�޸���ϣ�"<<endl;
				break;
			}
		case 0:
			return;
		default:
			cout<<"�����������������룡"<<endl;
		}
	}
}
int password()//�����
{
    char p[20];
    int n=0;
    cout<<"�������¼���"<<endl;
    while(1)
    {
        cin>>p;
        if (strcmp(p,"abcd")==0)
        {
            cout<<"���������ȷ��"<<endl;
            return 1;
        }
        else
        {
            cout<<"�������������������룡"<<endl;
            n++;
            if(n==3)
            {
                cout<<"������3�Σ�����Ȩ���в�����"<<endl;
                return 0;
            }
        }
    }
}


