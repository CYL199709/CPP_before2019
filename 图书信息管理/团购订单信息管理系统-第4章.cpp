/*����һ�µİ�����֮ͬ����
��1������ӡ���ѯ���޸ġ�ɾ����������ܶ�����д��һ��������
��2���������Ψһ���ж�д��һ�������ĺ�����
��3�����˵�����д��һ��������
��4�������жϹ���д��һ��������*/

#include <iostream>
#include <iomanip>
#include <string.h> 
using namespace std;
#define MaxNum 100          		//���������С
#define N 14
//����ȫ�ֱ���
int     count=0;                 	//������¼��ǰ�����ĸ���
string  order_num [MaxNum];    	//�������
string  goods_num [MaxNum];   	//��Ʒ���
double  goods_price[MaxNum];   	//��Ʒ����
int     goods_count[MaxNum];  	//��Ʒ����
string  name[MaxNum];         	//�ռ�������
//��������к���������
int  password();               	    //�����
void menu() ;                     	//���˵�����
void Append();                    	//��Ӷ�������
int  effective();                	//�ж϶������Ψһ�Ժ���
int  Search_order_num(string);	    //��������Ų�ѯ���������ҳɹ������������±꣬���򷵻�-1
void Delete_menu();           	    //ɾ������
void Modify();                  	//�޸Ķ�������
void Print_goods(int);          	//���������Ϣ����,��ʾ������ָ���±�Ķ���
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
        cout<<"*         1:��Ӷ���               2:���Ҷ���       *"<<endl;
		cout<<"*         3:�޸Ķ���               4:ɾ������       *"<<endl;
		cout<<"*         5:�������               0:�˳�           *"<<endl;
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
				cout<<"������Ҫ���ҵĶ�����ţ�";
	            string num;
	            cin>>num;
				i=Search_order_num(num);
				if(i!=-1)
				{
					cout<<"���ҳɹ���"<<endl;
		            cout<<setw(N)<<"�������"<<setw(N)<<"��Ʒ���";
		            cout<<setw(N)<<"��Ʒ����"<<setw(N)<<"��Ʒ����";
		            cout<<setw(N)<<"�ռ�������"<<endl;
		            cout<<setw(N)<<order_num[i]<<setw(N)<<goods_num[i]<<setw(N)<<goods_price[i];
		            cout<<setw(N)<<goods_count[i]<<setw(N)<<name[i]<<endl;
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
				if(count==0)
				{
					cout<<"�����޶�����Ϣ��"<<endl;
				}
				else
				{
					cout<<setw(N)<<"�������"<<setw(N)<<"��Ʒ���"<<setw(N)<<"��Ʒ����";
	                cout<<setw(N)<<"��Ʒ����" <<setw(N)<<"�ռ�������"<<endl;
	                for(i=0;i<count;i++)
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
int effective()//�ж϶������Ψһ�Ժ���
{
	for(int i=0;i<count;i++)
		if(order_num[count]==order_num[i])
			return 0;
	return 1;
}
void Append()//��Ӷ�������
{  
	cout<<"�����붩����Ϣ��"<<endl;
	cout<<"������ţ�";
	cin>>order_num[count];
	while(1)
	{
		if( effective()==0)
		{
			cout<<"��������ظ������������룡"<<endl;	
			cin>>order_num [count];
		}
		else
			break;
	}
    cout<<"��Ʒ���:"; cin>>goods_num[count];
	cout<<"��Ʒ����:"; cin>>goods_price[count];
	cout<<"��Ʒ����:"; cin>>goods_count[count];
	cout<<"�ռ�������:"; cin>>name[count];
	cout<<"�����ϣ�"<<endl;
	count++; 
 }
int Search_order_num(string num)//���Һ���
{ 
	int i;
	for(i=0;i<count;i++)
		if(order_num[i]==num)
			return i;
	return -1;  
}
void Print_goods(int i)//��ʾ������Ϣ
{
		cout<<setw(N)<<order_num[i]<<setw(N)<<goods_num[i]<<setw(N)<<goods_price[i];
		cout<<setw(N)<<goods_count[i]<<setw(N)<<name[i]<<endl;
}
void Delete_menu()//ɾ������
{
	string ch;
	char xx;
	cout<<"������ɾ���Ķ�����ţ�"<<endl;
	cin>>ch;
	int i=0,j;
	while(i<count)
	{
		if(order_num[i]==ch)
			break;
		i++;
	}
	if(i>=count)
		cout<<"�ö��������ڣ�"<<endl;
	else
	{
		cout<<"�Ѳ�ѯ����������Ϣ��"<<endl;		
		cout<<setw(N)<<"�������"<<setw(N)<<"��Ʒ���"<<setw(N)<<"��Ʒ����";
		cout<<setw(N)<<"��Ʒ����" <<setw(N)<<"�ռ�������"<<endl;
		Print_goods(i);
		cout<<"��ѡ��(ɾ��Y������N����";
		cin>>xx;
		if(xx=='y'||xx=='Y')
		{
			for(j=i+1;j<count ;j++)
			{
				order_num[j-1]=order_num[j];		
				goods_price[j-1]=goods_price[j];
				goods_num[j-1]=goods_num[j];
				name[j-1]=name[j];
				goods_count[j-1]=goods_count[j];														
			}
			count--;
			cout<<"ɾ����ϣ�"<<endl;
		}
		else
			cout<<"��������ɾ��������"<<endl;
	}
}

void Modify( )//�޸ĺ������Ȱ�������Ž��в��ң����޸�
{
	string num;
	int m;
	cout<<"��������Ҫ�޸ĵĶ�����ţ�";
	cin>>num;
	m=Search_order_num(num);
	if(m==-1)
	{
		cout<<"�����붩��������󣬿��в����ڸö�����Ϣ��"<<endl;
		return;
	}
	cout<<"��ǰ������Ϣ���£�"<<endl;
	cout<<setw(N)<<"�������"<<setw(N)<<"��Ʒ���"<<setw(N)<<"��Ʒ����"<<setw(N)<<"��Ʒ����";
	cout<<setw(N)<<"�ռ�������"<<endl;
	Print_goods(m);
	while(1)
	{
		cout<<"************************************************************"<<endl;
		cout<<"*           ������������ѡ������������ţ�               *"<<endl;
		cout<<"*         1:�޸���Ʒ��             2:�޸��ռ�������      *"<<endl;
		cout<<"*         3:�޸���Ʒ����           4:�޸���Ʒ����        *"<<endl;
		cout<<"*                            0:�˳�                      *"<<endl;
		cout<<"************************************************************"<<endl;
		int n;
		cin>>n;
		switch  (n) 
		{
		case 1:
			{
				cout<<"�������µ���Ʒ��ţ�"<<endl;
				cin>>goods_num[m];
				cout<<"�޸���ϣ�"<<endl;
				break;
			}
		case 2:
			{
				cout<<"�������µ��ռ���������"<<endl;
				cin>>name[m];
				cout<<"�޸���ϣ�"<<endl;
				break;
			}
		case 3:
			{
				cout<<"�������µ���Ʒ���ۣ�"<<endl;
				cin>>goods_price[m];
				cout<<"�޸���ϣ�"<<endl;
				break;
			}
		case 4:
			{
				cout<<"�������µ���Ʒ������"<<endl;
				cin>>goods_count[m];
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


