#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<ctime>
#include"Date.h"
using std::ostream;

Date::Date(int y, int m, int d) :year(y), month(m), day(d) {}

Date::Date() {
	time_t nowtime;
	struct tm* p;;
	time(&nowtime);
	p = localtime(&nowtime);
	year = p->tm_year + 1900;
	month = p->tm_mon + 1;
	day = p->tm_mday;
	hour = p->tm_hour;
	minute = p->tm_min;
	second = p->tm_sec;
}

int Date::show_hour() {
	return hour;
}

int Date::show_minute() {
	return minute;
}

int Date::show_second() {
	return second;
}

bool Date::operator<=(Date a) {
	bool re = false;
	if (a.year < year) re = true;
	else if (a.year == year) {
		if (a.month < month) re = true;
		else if (a.month == month) {
			if (a.day <= day) re = true;
		}
	}
	return re;
}

Date Date::operator+(int d) {
	Date re(year, month, day);
	int dtm(0);
	re.day += d;
	if ((re.month == 1 || re.month == 3 || re.month == 5 || re.month == 7
		|| re.month == 8 || re.month == 10 || re.month == 12) && re.day > 31) {
		re.day -= 31;
		dtm = 1;
	}
	else if ((re.month == 4 || re.month == 6 || re.month == 9 || re.month == 11)
		&& re.day > 30) {
		re.day -= 30;
		dtm = 1;
	}
	else if (((re.year % 100 != 0 && re.year % 4 == 0) || (re.year % 400 == 0))
		&& re.month == 2 && re.day > 29) {
		re.day -= 29;
		dtm = 1;
	}
	else if (!((re.year % 100 != 0 && re.year % 4 == 0) || (re.year % 400 == 0))
		&& re.month == 2 && re.day > 28) {
		re.day -= 28;
		dtm = 1;
	}
	if (dtm == 1 && re.month < 12) re.month++;
	else if (dtm == 1 && re.month == 12) {
		re.month = 1;
		re.year++;
	}
	return re;
}

ostream& operator<<(ostream& out, Date a)
{
	out << a.year << "-" << a.month << "-" << a.day;
	return out;
}