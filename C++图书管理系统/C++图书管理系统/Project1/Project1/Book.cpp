#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<fstream>
#include"Book.h"
#include"Person.h"
#include"md5.h"
using std::cout;
using std::endl;
using std::ofstream;
using std::ios;
extern Book* book_head;
extern Book* last_book;
extern int book_last_id;
extern int book_count;
extern int save_logs(const char* bookname, const char* username, int operation);

Book::Book(const char* bookname, long long isbn) : ISBN(isbn) {
	strcpy(name, bookname);
	next = nullptr;
	last = nullptr;
	state = 0;
	id = ++book_last_id;
}

Book::Book() {}

int Book::borrow(Person* reader, int time) {
	if (state == 1) throw 1;
	strcpy(this->reader, reader->show_username());
	Date now;
	borrow_date = now;
	borrow_time = time;
	repay_date = now + time;
	state = 1;
	save_book();
	return 0;
}

int Book::repay()
{
	if (state == 0) throw -1;
	state = 0;
	save_book();
	return 0;
}

char* Book::show_book_name()
{
	return name;
}

long long Book::show_book_ISBN()
{
	return ISBN;
}

int Book::show_book_state()
{
	return state;
}

char* Book::show_borrow_person()
{
	return reader;
}

int Book::show_book_id()
{
	return id;
}

Date Book::show_repay_date()
{
	return repay_date;
}

bool Book::is_exceed_limit()
{
	Date now;
	if (repay_date <= now) return false;
	return true;
}

int search_book(char* name)
{
	Book* p = book_head;
	int is_find(0);
	int count = 0;
	int show_tittle(0);
	unsigned max_length = 0;
	char* str;
	char* str2;
	char* res;
	while (p != nullptr) {                     //第一遍查找
		str = p->show_book_name();
		str2 = name;
		res = (char*)memchr(str, str2[0], strlen(str));
		if (res == nullptr) {
			goto next1;
		}
		int n;
		n = memcmp(res, str2, strlen(str2) - 1);
		if (n != 0) {
			if (strlen(res) <= strlen(str2)) {
				goto next1;
			}
			else {
				res = (char*)memchr(res + 1, str2[0], strlen(res));
				if (res == nullptr) {
					goto next1;
				}
			}
		}
		else {
			if (strlen(p->show_book_name()) > max_length) max_length = strlen(p->show_book_name());
			is_find = 1;
		}
	next1:
		p = p->next;
	}
	max_length = max_length / 8;
	p = book_head;
	if (is_find) {
		while (p != nullptr) {                       //第二遍查找
			str = p->show_book_name();
			str2 = name;
			res = (char*)memchr(str, str2[0], strlen(str));
			if (res == nullptr) {
				goto next2;
			}
			int n;
			n = memcmp(res, str2, strlen(str2) - 1);
			if (n != 0) {
				if (strlen(res) <= strlen(str2)) {
					goto next2;
				}
				else {
					res = (char*)memchr(res + 1, str2[0], strlen(res));
					if (res == nullptr) {
						goto next2;
					}
				}
			}
			else {
				if (show_tittle == 0) {
					cout << endl << "找到如下结果：" << endl;
					cout << "序号\t" << "书名";
					for (unsigned i = 0; i < max_length + 1; i++) {
						cout << "\t";
					}
					cout << "书ISBN码\t是否在馆\tID" << endl;
					for (unsigned i = 0; i < max_length + 7; i++) {
						cout << "--------";
					}
					cout << endl;
					show_tittle = 1;
				}
				count++;
				cout << count << "\t";
				cout << p->show_book_name();
				for (unsigned i = 0; i < max_length - (int)strlen(p->show_book_name()) / 8; i++) cout << "\t";
				cout << "\t";
				cout << p->show_book_ISBN() << "\t";
				if (p->show_book_state()) cout << "已借出";
				else cout << "在馆";
				cout << "\t\t" << p->show_book_id() << endl;
			}
		next2:
			p = p->next;
		}
	}
	return is_find;
}
void Book::modify(string s, int a)
{
	if (s == "state") state = a;
	else if (s == "borrow_time") borrow_time = a;
	save_book();
}

void Book::modify(string s, char* a)
{
	if (s == "name") strcpy(name, a);
	save_book();
}

void Book::modify(string s, Date a)
{
	if (s == "borrow_date") borrow_date = a;
	else if (s == "repay_date") repay_date = a;
	save_book();
}

void Book::modify(string s, long long a)
{
	if (s == "isbn") ISBN = a;
	save_book();
}

Book* id_to_book(int id)
{
	Book* p = book_head;
	while (p != nullptr) {
		if (p->show_book_id() == id) return p;
		p = p->next;
	}
	return nullptr;
}

int add_book(const char* name, long long isbn, Person* user)
{
	Book* p = new Book(name, isbn);
	if (p) {
		if (book_head == nullptr) {
			book_head = p;
			last_book = p;
			p->last = nullptr;
		}
		else {
			last_book->next = p;
			p->last = last_book;
			last_book = p;
		}
		book_count++;
		save_book();
		if (user) {
			save_logs(name, user->show_username(), 4);
		}
		else save_logs(name, "系统自动创建", 4);
			return 0;
	}
	else return -1;
}

void remove_book(Book* book, Person* user)
{
	save_logs(book->show_book_name(), user->show_username(), 5);
	if (book->last == nullptr) {
		book_head = book;
		book->next->last = nullptr;
		delete book;
	}
	else if (book->next == nullptr) {
		book->last->next = nullptr;
		last_book = book->last;
		delete book;
	}
	else {
		book->last->next = book->next;
		book->next->last = book->last;
		delete book;
	}
	book_count--;
	save_book();
}

void save_book()
{
	ofstream out("book.dat", ios::out | ios::binary);
	if (out.is_open()) {
		out.write((char*)&book_count, sizeof(int));
		out.write((char*)&book_last_id, sizeof(int));
		Book* p = book_head;
		for (int i = 0; i < book_count; i++) {
			out.write((char*)p, sizeof(Book));
			p = p->next;
		}
		out.close();
	}
	else {
		cout << "Book信息保存错误。" << endl;
		system("PAUSE");
	}
}