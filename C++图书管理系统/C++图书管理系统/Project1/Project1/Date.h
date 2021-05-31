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
	Date();                  //���캯���޲���ʱĬ�����ϵͳ��ǰ����
	Date(int y, int m, int d);
	bool operator<=(Date);
	Date operator+(int);
	int show_hour();
	int show_minute();
	int show_second();
};

ostream& operator<<(ostream& out, Date a);
