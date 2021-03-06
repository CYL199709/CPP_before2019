/*与上一章的案例不同之处：
（1）将添加、查询、修改、删除、浏览功能都单独写成一个函数。
（2）图书编号唯一性判断写成一个单独的函数。
（3）主菜单单独写成一个函数。
（4）口令判断功能写成一个函数。*/

#include <iostream>
#include <iomanip>
#include <string> 
using namespace std;
#define MaxNum 100          		//定义数组大小
#define N 14
//定义全局变量
int     count1 = 0;               	//用来记录当前图书的个数
string  order_num [MaxNum];    	//图书编号
string  goods_num [MaxNum];   	//图书编号
double  goods_price[MaxNum];   	//图书单价
int     goods_count1[MaxNum];  	//图书数量
string  name[MaxNum];         	//作者姓名
//定义的所有函数的声明
int  password();               	    //口令函数
void menu() ;                     	//主菜单函数
void Append();                    	//添加图书函数
int  effective();                	//判断图书编号唯一性函数
int  Search_order_num(string);	    //按图书编号查询函数，查找成功返回数组中下标，否则返回-1
void Delete_menu();           	    //删除函数
void Modify();                  	//修改图书函数
void Print_goods(int);          	//浏览图书信息函数,显示数组中指定下标的图书
int main()                          //主函数
{
	if (password())
		menu();
	return 0;
}
void  menu( )//主菜单函数
{
	cout<<endl;
	while(1)
	{
		cout<<"*****************************************************"<<endl;
		cout<<"*           根据所做操作选择以下数字序号：          *"<<endl;
        cout<<"*         1:添加图书               2:查找图书       *"<<endl;
		cout<<"*         3:修改图书               4:删除图书       *"<<endl;
		cout<<"*         5:浏览图书               0:退出           *"<<endl;
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
				cout<<"请输入要查找的图书编号：";
	            string num;
	            cin>>num;
				i=Search_order_num(num);
				if(i!=-1)
				{
					cout<<"查找成功！"<<endl;
		            cout<<setw(N)<<"图书编号"<<setw(N)<<"图书编号";
		            cout<<setw(N)<<"图书单价"<<setw(N)<<"图书数量";
		            cout<<setw(N)<<"作者姓名"<<endl;
		            cout<<setw(N)<<order_num[i]<<setw(N)<<goods_num[i]<<setw(N)<<goods_price[i];
		            cout<<setw(N)<<goods_count1[i]<<setw(N)<<name[i]<<endl;
				}
	            else
	            	cout<<"查找失败！"<<endl; 
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
					cout<<"表中无图书信息！"<<endl;
				}
				else
				{
					cout<<setw(N)<<"图书编号"<<setw(N)<<"图书编号"<<setw(N)<<"图书单价";
	                cout<<setw(N)<<"图书数量" <<setw(N)<<"作者姓名"<<endl;
	                for(i=0;i<count1;i++)
						Print_goods(i);
				}
			    break;
			}
        case 0:
				return;
		default:
			cout<<"输入有误，请重新输入！"<<endl;
		}
	}
}
int effective()//判断图书编号唯一性函数
{
	for(int i=0;i<count1;i++)
		if(order_num[count1]==order_num[i])
			return 0;
	return 1;
}
void Append()//添加图书函数
{  
	cout<<"请输入图书信息："<<endl;
	cout<<"图书编号：";
	cin>>order_num[count1];
	while(1)
	{
		if( effective()==0)
		{
			cout<<"图书编号重复，请重新输入！"<<endl;	
			cin>>order_num [count1];
		}
		else
			break;
	}
    cout<<"图书编号:"; cin>>goods_num[count1];
	cout<<"图书单价:"; cin>>goods_price[count1];
	cout<<"图书数量:"; cin>>goods_count1[count1];
	cout<<"作者姓名:"; cin>>name[count1];
	cout<<"添加完毕！"<<endl;
	count1++; 
 }
int Search_order_num(string num)//查找函数
{ 
	int i;
	for(i=0;i<count1;i++)
		if(order_num[i]==num)
			return i;
	return -1;  
}
void Print_goods(int i)//显示图书信息
{
		cout<<setw(N)<<order_num[i]<<setw(N)<<goods_num[i]<<setw(N)<<goods_price[i];
		cout<<setw(N)<<goods_count1[i]<<setw(N)<<name[i]<<endl;
}
void Delete_menu()//删除函数
{
	string ch;
	char xx;
	cout<<"请输入删除的图书编号："<<endl;
	cin>>ch;
	int i=0,j;
	while(i<count1)
	{
		if(order_num[i]==ch)
			break;
		i++;
	}
	if(i>=count1)
		cout<<"该图书不存在！"<<endl;
	else
	{
		cout<<"已查询到，图书信息："<<endl;		
		cout<<setw(N)<<"图书编号"<<setw(N)<<"图书编号"<<setw(N)<<"图书名称";
		cout<<setw(N)<<"图书单价" <<setw(N)<<"作者姓名"<<endl;
		Print_goods(i);
		cout<<"请选择(删除Y，放弃N）：";
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
			cout<<"删除完毕！"<<endl;
		}
		else
			cout<<"放弃本次删除操作！"<<endl;
	}
}

void Modify( )//修改函数，先按图书编号进行查找，后修改
{
	string num;
	int m;
	cout<<"请输入所要修改的图书编号：";
	cin>>num;
	m=Search_order_num(num);
	if(m==-1)
	{
		cout<<"所输入图书编号有误，库中不存在该图书信息！"<<endl;
		return;
	}
	cout<<"当前图书信息如下："<<endl;
	cout<<setw(N)<<"图书编号"<<setw(N)<<"图书编号"<<setw(N)<<"图书名称"<<setw(N)<<"图书单价";
	cout<<setw(N)<<"作者姓名"<<endl;
	Print_goods(m);
	while(1)
	{
		cout<<"************************************************************"<<endl;
		cout<<"*           根据所做操作选择以下数字序号：               *"<<endl;
		cout<<"*         1:修改图书号             2:修改作者姓名      *"<<endl;
		cout<<"*         3:修改图书单价           4:修改图书数量        *"<<endl;
		cout<<"*                            0:退出                      *"<<endl;
		cout<<"************************************************************"<<endl;
		int n;
		cin>>n;
		switch  (n) 
		{
		case 1:
			{
				cout<<"请输入新的图书编号："<<endl;
				cin>>goods_num[m];
				cout<<"修改完毕！"<<endl;
				break;
			}
		case 2:
			{
				cout<<"请输入新的作者姓名："<<endl;
				cin>>name[m];
				cout<<"修改完毕！"<<endl;
				break;
			}
		case 3:
			{
				cout<<"请输入新的图书单价："<<endl;
				cin>>goods_price[m];
				cout<<"修改完毕！"<<endl;
				break;
			}
		case 4:
			{
				cout<<"请输入新的图书数量："<<endl;
				cin>>goods_count1[m];
				cout<<"修改完毕！"<<endl;
				break;
			}
		case 0:
			return;
		default:
			cout<<"输入有误，请重新输入！"<<endl;
		}
	}
}
int password()//口令函数
{
    char p[20];
    int n=0;
    cout<<"请输入登录口令："<<endl;
    while(1)
    {
        cin>>p;
        if (strcmp(p,"abcd")==0)
        {
            cout<<"输入口令正确！"<<endl;
            return 1;
        }
        else
        {
            cout<<"输入口令错误，请重新输入！"<<endl;
            n++;
            if(n==3)
            {
                cout<<"已输入3次，您无权进行操作！"<<endl;
                return 0;
            }
        }
    }
}


