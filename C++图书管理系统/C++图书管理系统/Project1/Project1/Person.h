#pragma once
#include<iostream>
#include"Date.h"
using std::string;

class Person {
private:
	int id;               //标识码，便于查找和记录
	int status;           //身份，0表示系统管理员，1表示图书管理员，2表示学生, 3表示教师，4表示游客
	char username[50];    //登录用户名
	char password[33];    //登录密码
	Date signin_date;     //注册时间
	int book_id1;         //借的书1
	int book_id2;         //借的书2
	int book_id3;         //借的书3
public:
	Person* last;         //上一项
	Person* next;         //下一项
	Person(int id, int status, const char* name);
	Person();
	void set_password(const char* password);    //设置密码
	int change_password();                      //修改密码
	int borrow_book(int book);                  //借书，返回0表示成功借书
	int repay_book(int book);                   //还书
	int show_status();                          //获取身份
	char* show_username();                      //获取用户名
	int show_id();                              //获取ID
	char* show_password();                      //获取密码
	Date show_signin_date();                    //获取注册时间
	int show_borrowed_book();                   //获取已借图书ID
	friend Person* login(char* username, char* password);
	friend int add_person();
	friend Person* check_uesrname(char* username);
};

char* put_passwd();                                  //统一输入密码函数，返回MD5加密后的密码
Person* login(char* username, char* password);       //登录，返回用户对象地址
int add_person();                                    //添加用户，返回用户ID
void remove_person(Person* person);                  //删除用户
Person* check_uesrname(char* username);              //查询用户名，返回对象地址，返回nullptr表示没有这个用户名
void save_person();                                  //保存用户信息
