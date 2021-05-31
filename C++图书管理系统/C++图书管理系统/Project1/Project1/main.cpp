#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<fstream>
#include<cstring>
#include<windows.h>
#include"Book.h"
#include"Person.h"
using namespace std;

Book* book_head = nullptr;
Person* person_head = nullptr;
Book* last_book = nullptr;
Person* last_person = nullptr;
int book_count = 0;
int person_count = 0;
int book_last_id = 0;
int person_last_id = 0;

int save_logs(const char* bookname, const char* username, int operation)
{
	ofstream log("record.log", ios::app);
	if (log.is_open()) {
		Date now;
		log << "书名：";
		for (int i = 0; bookname[i] != 0; i++) {
			log << bookname[i];
		}
		log << endl;
		log << "用户名：";
		for (int i = 0; username[i] != 0; i++) {
			log << username[i];
		}
		log << endl;
		log << "操作：";
		if (operation == 1) log << "借书";
		else if (operation == 2) log << "还书";
		else if (operation == 3) log << "修改图书信息";
		else if (operation == 4) log << "新增图书";
		else if (operation == 5) log << "删除图书";
		log << endl;
		log << "日期：" << now << "   " << now.show_hour() << ":" << now.show_minute() << ":" << now.show_second() << endl;
		log << "----------------------------------------------" << endl;
		log.close();
		return 0;
	}
	else return -1;
}

int main()
{
read_person:                   //读取或新建Person信息
	ifstream in("person.dat", ios::_Noreplace | ios::binary);
	if (in.is_open()) {
		Person* last = nullptr;
		in.read((char*)&person_count, sizeof(int));
		in.read((char*)&person_last_id, sizeof(int));
		for (int i = 0; i < person_count; i++) {
			Person* p = new Person;
			in.read((char*)p, sizeof(Person));
			p->next = nullptr;
			if (person_head == nullptr) {
				person_head = p;
				p->last = nullptr;
				last = p;
			}
			else {
				p->last = last;
				last->next = p;
				last = p;
			}
		}
		last_person = last;
		in.close();
	}
	else {
		ofstream out("person.dat", ios::binary);
		if (out.is_open()) {
			person_count = 2;
			person_last_id = 2;
			Person* admin = new Person(1, 0, "admin");
			Person* guest = new Person(2, 4, "guest");
			last_person = guest;
			admin->set_password("21232f297a57a5a743894a0e4a801fc3");
			guest->set_password("084e0343a0486ff05530df6c705c8bb4");
			admin->next = guest;
			guest->last = admin;
			out.write((char*)&person_count, sizeof(int));
			out.write((char*)&person_last_id, sizeof(int));
			out.write((char*)admin, sizeof(Person));
			out.write((char*)guest, sizeof(Person));
			out.close();
			person_head = admin;
		}
		else {
			cout << "新建Person存储文件失败！程序即将退出。" << endl;
			system("PAUSE");
			exit(0);
		}
	}
read_book:                             //读取Book信息
	ifstream b("book.dat", ios::_Noreplace | ios::binary);
	if (b.is_open()) {
		Book* last = nullptr;
		b.read((char*)&book_count, sizeof(int));
		b.read((char*)&book_last_id, sizeof(int));
		for (int i = 0; i < book_count; i++) {
			Book* p = new Book;
			b.read((char*)p, sizeof(Book));
			p->next = nullptr;
			if (book_head == nullptr) {
				book_head = p;
				p->last = nullptr;
				last = p;
			}
			else {
				p->last = last;
				last->next = p;
				last = p;
			}
		}
		last_book = last;
		b.close();
	}
begin:                                  //登录界面
	system("cls");
	cout << endl << "------------------------图书管理系统------------------------" << endl;
	cout << "请先登录。默认管理员账户密码均为admin，游客账户密码均为guest。" << endl;
	char* username = new char[50];
	char* password = new char[33];
	Person* user = nullptr;
	char* temp;
putin:
	cout << "用户名：";
	cin >> username;
	cout << "密码：";
	temp = put_passwd();
	strcpy(password, temp);
	delete temp;
	user = login(username, password);
	if (user == nullptr) {
		cout << endl << "用户名或密码错误，请重新输入。" << endl;
		goto putin;
	}
	delete[] username;
	delete[] password;
	char choice;
	while (1) {
		system("cls");
		switch (user->show_status()) {
		case 0:      //系统管理员
			cout << endl << "******************************************************" << endl;
			cout << "登录身份：系统管理员     登录用户：" << user->show_username() << "    用户ID：" << user->show_id() << endl;
			if (user->show_id() == 1 && !strcmp(user->show_password(), "21232f297a57a5a743894a0e4a801fc3")) {
				cout << endl << "***您的系统管理员账户仍是初始密码，建议您尽快修改密码。***" << endl;
			}
			cout << endl << "现在你想做什么？" << endl;
			cout << "1、添加用户" << endl;
			cout << "2、删除用户" << endl;
			cout << "3、修改密码" << endl;
			cout << "4、退出登录" << endl;
			cout << "5、退出程序" << endl;
			cout << endl << "******************************************************" << endl;
			cout << endl << "您的选项：";
			cin >> choice;
			if (choice == '1') {
				try {
					add_person();
				}
				catch (int e) {
					if (e == -1) cout << endl << "尝试次数过多！" << endl;
					system("PAUSE");
				}
			}
			else if (choice == '2') {
				Person* person;
				cout << "请输入要删除的用户名：";
				char* name = new char[50];
				cin >> name;
				if (name == "admin") {
					cout << "默认管理员账户不能删除。" << endl;
					system("PAUSE");
					break;
				}
				if (name == "guest") {
					cout << "游客账户不能删除。" << endl;
					system("PAUSE");
					break;
				}
				person = check_uesrname(name);
				if (person) {
					cout << endl << "该账户信息如下：" << endl;
					cout << "用户名：" << name << endl;
					cout << "ID：" << person->show_id() << endl;
					cout << "用户身份：";
					if (person->show_status() == 0) cout << "系统管理员";
					else if (person->show_status() == 1) cout << "图书管理员";
					else if (person->show_status() == 2) cout << "读者";
					cout << endl;
					cout << "注册时间：" << person->show_signin_date() << endl;
					cout << endl << "确认要删除此用户吗？（y/n）:";
					char confrm;
					cin >> confrm;
					if (confrm == 'y') {
						remove_person(person);
						cout << "成功删除。" << endl;
					}
					else cout << "该用户没有被删除。" << endl;
				}
				else cout << "没有找到该用户。" << endl;
				system("PAUSE");
			}
			else if (choice == '3') {
				user->change_password();
			}
			else if (choice == '4') {
				goto begin;
			}
			else if (choice == '5') {
				exit(0);
			}
			break;
		case 1:       //图书管理员
			cout << endl << "******************************************************" << endl;
			cout << "登录身份：图书管理员     登录用户：" << user->show_username() << "    用户ID：" << user->show_id() << endl;
			cout << endl << "现在你想做什么？" << endl;
			cout << "1、添加图书" << endl;
			cout << "2、删除图书" << endl;
			cout << "3、修改图书信息" << endl;
			cout << "4、修改密码" << endl;
			cout << "5、退出登录" << endl;
			cout << "6、退出程序" << endl;
			cout << endl << "******************************************************" << endl;
			cout << endl << "您的选项：";
			cin >> choice;
			if (choice == '1') {
				char name[100];
				long long isbn;
				cout << "请输入书名：";
				getchar();
				cin.getline(name, 99);
				cout << "请输入书ISBN码：";
				cin >> isbn;
				add_book(name, isbn, user);
				cout << "成功添加图书。" << endl;
				system("PAUSE");
			}
			else if (choice == '2') {
				char* name = new char[100];
				cout << "请输入要查找的书名（区分大小写）：";
				getchar();
				cin.getline(name, 99);
				cout << endl;
				if (search_book(name) == 0) {
					cout << "没有找到书名包含“" << name << "”的图书。" << endl << "请检查输入是否正确，或图书馆尚未收录此书。" << endl;
				}
				else {
					int id;
					cout << endl << "请输入要删除的图书ID：";
					cin >> id;
					Book* p = id_to_book(id);
					if (p) {
						remove_book(p, user);
						cout << "已成功删除。" << endl;
						system("PAUSE");
					}
					else cout << "没有找到该ID对应的图书。" << endl;
				}
			}
			else if (choice == '3') {
				char* name = new char[100];
				cout << "请输入要查找的书名（区分大小写）：";
				getchar();
				cin.getline(name, 99);
				cout << endl;
				if (search_book(name) == 0) {
					cout << "没有找到书名包含“" << name << "”的图书。" << endl << "请检查输入是否正确，或图书馆尚未收录此书。" << endl;
				}
				else {
					int id;
					cout << endl << "请输入要修改的图书ID：";
					cin >> id;
					Book* p = id_to_book(id);
					cout << endl << "您要修改什么信息？" << endl;
					cout << "1、书名" << endl;
					cout << "2、书ISBN码" << endl;
					cout << "3、借阅状态" << endl;
					cout << "4、借书日期" << endl;
					cout << "5、借阅时长" << endl;
					cout << "6、还书日期" << endl;
					cout << endl << "请输入序号：";
					char c;
					cin >> c;
					if (c == '1') {
						cout << "请输入书名：";
						char name[100];
						cin >> name;
						p->modify("name", name);
					}
					if (c == '2') {
						cout << "请输入书ISBN码：";
						long long isbn;
						cin >> isbn;
						p->modify("isbn", isbn);
					}
					if (c == '3') {
						cout << "请输入借阅状态（0表示在馆，1表示借出）：";
						int state;
						cin >> state;
						p->modify("state", state);
					}
					if (c == '4') {
						cout << "请输入借书日期（年，月，日，用空格分隔）：";
						int y, m, d;
						cin >> y >> m >> d;
						Date a(y, m, d);
						p->modify("borrow_date", a);
					}
					if (c == '5') {
						cout << "请输入借阅时长（天）：";
						int t;
						cin >> t;
						p->modify("borrow_time", t);
					}
					if (c == '6') {
						cout << "请输入还书日期（年，月，日，用空格分隔）：";
						int y, m, d;
						cin >> y >> m >> d;
						Date b(y, m, d);
						p->modify("repay_date", b);
					}
					cout << "图书信息已修改。" << endl;
					save_logs(id_to_book(id)->show_book_name(), user->show_username(), 3);
					system("PAUSE");
				}
			}
			else if (choice == '4') {
				user->change_password();
			}
			else if (choice == '5') {
				goto begin;
			}
			else if (choice == '6') {
				exit(0);
			}
			break;
		case 2:       //学生
			cout << endl << "******************************************************" << endl;
			cout << "登录身份：学生     登录用户：" << user->show_username() << "    用户ID：" << user->show_id() << endl;
			cout << endl << "现在你想做什么？" << endl;
			cout << "1、借书" << endl;
			cout << "2、还书" << endl;
			cout << "3、查看已借图书" << endl;
			cout << "4、修改密码" << endl;
			cout << "5、退出登录" << endl;
			cout << "6、退出程序" << endl;
			cout << endl << "******************************************************" << endl;
			cout << endl << "您的选项：";
			cin >> choice;
			if (choice == '1') {
				char* name = new char[100];
				cout << "请输入要查找的书名（区分大小写）：";
				getchar();
				cin.getline(name, 99);
				cout << endl;
				if (search_book(name) == 0) {
					cout << "没有找到书名包含“" << name << "”的图书。" << endl << "请检查输入是否正确，或图书馆尚未收录此书。" << endl;
				}
				else {
					int id;
					cout << endl << "作为学生，您只能同时借阅1本图书。" << endl;
					cout << "请输入要借阅的图书ID：";
					cin >> id;
					Book* p = id_to_book(id);
					if (p) {
						try {
							p->borrow(user, 7);
							user->borrow_book(id);
							save_logs(id_to_book(id)->show_book_name(), user->show_username(), 1);
						}
						catch (int e) {
							if (e == 1) cout << "抱歉，该书已被借走。" << endl;
							if (e == 2) p->repay();
						}
					}
					else cout << "没有找到该ID对应的图书。" << endl;
				}
				system("PAUSE");
			}
			else if (choice == '2') {
				if (user->show_borrowed_book() == 0) {
					cout << "请输入要还书的ID：";
					int id;
					cin >> id;
					if (id_to_book(id)) {
						if (strcmp(id_to_book(id)->show_borrow_person(), user->show_username()) == 0) {
							user->repay_book(id);
							id_to_book(id)->repay();
							if (id_to_book(id)->is_exceed_limit())
								cout << "您已经超过了最晚还书期限，需缴纳20元罚款。" << endl;
							cout << "还书成功。" << endl;
							save_logs(id_to_book(id)->show_book_name(), user->show_username(), 2);
						}
						else cout << "你没有借这本书！" << endl;
					}
					else cout << "未找到该图书ID对应的图书，请检查输入是否正确。" << endl;
				}
				system("PAUSE");
			}
			else if (choice == '3') {
				user->show_borrowed_book();
				system("PAUSE");
			}
			else if (choice == '4') {
				user->change_password();
			}
			else if (choice == 5) {
				goto begin;
			}
			else if (choice == '6') {
				exit(0);
			}
			break;
		case 3:       //教师
			cout << endl << "******************************************************" << endl;
			cout << "登录身份：教师     登录用户：" << user->show_username() << "    用户ID：" << user->show_id() << endl;
			cout << endl << "现在你想做什么？" << endl;
			cout << "1、借书" << endl;
			cout << "2、还书" << endl;
			cout << "3、查看已借图书" << endl;
			cout << "4、修改密码" << endl;
			cout << "5、退出登录" << endl;
			cout << "6、退出程序" << endl;
			cout << endl << "******************************************************" << endl;
			cout << endl << "您的选项：";
			cin >> choice;
			if (choice == '1') {
				char* name = new char[100];
				cout << "请输入要查找的书名（区分大小写）：";
				getchar();
				cin.getline(name, 99);
				cout << endl;
				if (search_book(name) == 0) {
					cout << "没有找到书名包含“" << name << "”的图书。" << endl << "请检查输入是否正确，或图书馆尚未收录此书。" << endl;
				}
				else {
					int id;
					cout << endl << "作为教师，您可以同时借阅3本图书。" << endl;
					cout << "请输入要借阅的图书ID：";
					cin >> id;
					Book* p = id_to_book(id);
					if (p) {
						try {
							p->borrow(user, 7);
							user->borrow_book(id);
							save_logs(id_to_book(id)->show_book_name(), user->show_username(), 1);
						}
						catch (int e) {
							if (e == 1) cout << "抱歉，该书已被借走。" << endl;
							if (e == 2) p->repay();
						}
					}
					else cout << "没有找到该ID对应的图书。" << endl;
				}
				system("PAUSE");
			}
			else if (choice == '2') {
				if (user->show_borrowed_book() == 0) {
					cout << "请输入要还书的ID：";
					int id;
					cin >> id;
					if (id_to_book(id)) {
						if (strcmp(id_to_book(id)->show_borrow_person(), user->show_username()) == 0) {
							user->repay_book(id);
							id_to_book(id)->repay();
							if (id_to_book(id)->is_exceed_limit())
								cout << "您已经超过了最晚还书期限，需缴纳20元罚款。" << endl;
							cout << "还书成功。" << endl;
							save_logs(id_to_book(id)->show_book_name(), user->show_username(), 2);
						}
						else cout << "你没有借这本书！" << endl;
					}
					else cout << "未找到该图书ID对应的图书，请检查输入是否正确。" << endl;
				}
				system("PAUSE");
			}
			else if (choice == '3') {
				user->show_borrowed_book();
				system("PAUSE");
			}
			else if (choice == '4') {
				user->change_password();
			}
			else if (choice == '5') {
				goto begin;
			}
			else if (choice == '6') {
				exit(0);
			}
			break;
		case 4:       //游客
			cout << endl << "******************************************************" << endl;
			cout << "登录身份：游客     登录用户：" << user->show_username() << "    用户ID：" << user->show_id() << endl;
			cout << "以游客身份登录时，您只能查询图书在馆状态。" << endl;
			cout << endl << "现在你想做什么？" << endl;
			cout << "1、查询图书" << endl;
			cout << "2、退出登录" << endl;
			cout << "3、退出程序" << endl;
			cout << endl << "******************************************************" << endl;
			cout << endl << "您的选项：";
			cin >> choice;
			if (choice == '1') {
				char* name = new char[100];
				cout << "请输入要查找的书名（区分大小写）：";
				getchar();
				cin.getline(name, 99);
				cout << endl;
				if (search_book(name) == 0) {
					cout << "没有找到书名包含“" << name << "”的图书。" << endl << "请检查输入是否正确，或图书馆尚未收录此书。" << endl;
				}
				system("PAUSE");
			}
			else if (choice == '2') {
				goto begin;
			}
			else if (choice == '3') {
				exit(0);
			}
			break;
		}
	}
	return 0;
}
