#pragma once
#include<iostream>
#include"Date.h"
#include"Person.h"
using std::string;
using std::ostream;

class Book {
private:
	char name[100];     //书名
	long long ISBN;     //书ISBN码
	int id;             //序号，便于查找、标识
	int state;          //借阅状态，0表示在馆，1表示借出
	char reader[50];    //借主，state=1时此项才有意义
	Date borrow_date;   //借书日期，state=1时此项才有意义
	int borrow_time;    //借书时长（天），state=1时此项才有意义
	Date repay_date;    //还书日期，state=1时此项才有意义
public:
	Book* last;         //上一项
	Book* next;         //下一项
	Book(const char* bookname, long long isbn);
	Book();
	int borrow(Person* reader, int time);              //借书
	int repay();                                       //还书
	char* show_book_name();                            //获取书名
	long long show_book_ISBN();                        //获取书ISBN码
	int show_book_state();                             //获取书借阅状态
	char* show_borrow_person();                        //获取借书人
	int show_book_id();                                //获取图书序号
	Date show_repay_date();                            //获取还书时间
	bool is_exceed_limit();                            //判断借阅是否逾期，返回true表示已经逾期
	void modify(string s, int a);                      //修改图书数据（int类型）
	void modify(string s, char* a);                    //修改图书数据（char*类型）
	void modify(string s, Date a);                     //修改图书数据（Date类型）
	void modify(string s, long long a);                //修改图书数据（long long类型）
};

int search_book(char* name);                                                //查询图书，显示查询结果，返回找到的书的数量
Book* id_to_book(int id);                                                   //从图书ID对应到图书对象地址
int add_book(const char* name, long long isbn, Person* user = nullptr);     //添加图书
void remove_book(Book* book, Person* user);                                 //删除图书
void save_book();                                                           //保存图书信息