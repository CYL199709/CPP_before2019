#pragma once
#include<iostream>
#include"Date.h"
using std::string;

class Person {
private:
	int id;               //��ʶ�룬���ڲ��Һͼ�¼
	int status;           //��ݣ�0��ʾϵͳ����Ա��1��ʾͼ�����Ա��2��ʾѧ��, 3��ʾ��ʦ��4��ʾ�ο�
	char username[50];    //��¼�û���
	char password[33];    //��¼����
	Date signin_date;     //ע��ʱ��
	int book_id1;         //�����1
	int book_id2;         //�����2
	int book_id3;         //�����3
public:
	Person* last;         //��һ��
	Person* next;         //��һ��
	Person(int id, int status, const char* name);
	Person();
	void set_password(const char* password);    //��������
	int change_password();                      //�޸�����
	int borrow_book(int book);                  //���飬����0��ʾ�ɹ�����
	int repay_book(int book);                   //����
	int show_status();                          //��ȡ���
	char* show_username();                      //��ȡ�û���
	int show_id();                              //��ȡID
	char* show_password();                      //��ȡ����
	Date show_signin_date();                    //��ȡע��ʱ��
	int show_borrowed_book();                   //��ȡ�ѽ�ͼ��ID
	friend Person* login(char* username, char* password);
	friend int add_person();
	friend Person* check_uesrname(char* username);
};

char* put_passwd();                                  //ͳһ�������뺯��������MD5���ܺ������
Person* login(char* username, char* password);       //��¼�������û������ַ
int add_person();                                    //����û��������û�ID
void remove_person(Person* person);                  //ɾ���û�
Person* check_uesrname(char* username);              //��ѯ�û��������ض����ַ������nullptr��ʾû������û���
void save_person();                                  //�����û���Ϣ
