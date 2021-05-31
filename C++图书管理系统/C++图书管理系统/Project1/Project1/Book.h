#pragma once
#include<iostream>
#include"Date.h"
#include"Person.h"
using std::string;
using std::ostream;

class Book {
private:
	char name[100];     //����
	long long ISBN;     //��ISBN��
	int id;             //��ţ����ڲ��ҡ���ʶ
	int state;          //����״̬��0��ʾ�ڹݣ�1��ʾ���
	char reader[50];    //������state=1ʱ�����������
	Date borrow_date;   //�������ڣ�state=1ʱ�����������
	int borrow_time;    //����ʱ�����죩��state=1ʱ�����������
	Date repay_date;    //�������ڣ�state=1ʱ�����������
public:
	Book* last;         //��һ��
	Book* next;         //��һ��
	Book(const char* bookname, long long isbn);
	Book();
	int borrow(Person* reader, int time);              //����
	int repay();                                       //����
	char* show_book_name();                            //��ȡ����
	long long show_book_ISBN();                        //��ȡ��ISBN��
	int show_book_state();                             //��ȡ�����״̬
	char* show_borrow_person();                        //��ȡ������
	int show_book_id();                                //��ȡͼ�����
	Date show_repay_date();                            //��ȡ����ʱ��
	bool is_exceed_limit();                            //�жϽ����Ƿ����ڣ�����true��ʾ�Ѿ�����
	void modify(string s, int a);                      //�޸�ͼ�����ݣ�int���ͣ�
	void modify(string s, char* a);                    //�޸�ͼ�����ݣ�char*���ͣ�
	void modify(string s, Date a);                     //�޸�ͼ�����ݣ�Date���ͣ�
	void modify(string s, long long a);                //�޸�ͼ�����ݣ�long long���ͣ�
};

int search_book(char* name);                                                //��ѯͼ�飬��ʾ��ѯ����������ҵ����������
Book* id_to_book(int id);                                                   //��ͼ��ID��Ӧ��ͼ������ַ
int add_book(const char* name, long long isbn, Person* user = nullptr);     //���ͼ��
void remove_book(Book* book, Person* user);                                 //ɾ��ͼ��
void save_book();                                                           //����ͼ����Ϣ