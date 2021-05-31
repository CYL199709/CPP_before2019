#pragma once
#include<iostream>
using std::ostream;

class Date {
public:
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
	Date();                  //构造函数无参数时默认填充系统当前日期
	Date(int y, int m, int d);
	bool operator<=(Date);
	Date operator+(int);
	int show_hour();
	int show_minute();
	int show_second();
};

ostream& operator<<(ostream& out, Date a);
