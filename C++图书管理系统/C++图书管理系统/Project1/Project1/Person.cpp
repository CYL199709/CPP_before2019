#define _CRT_SECURE_NO_WARNINGS

#include<conio.h>
#include<fstream>
#include<cstring>
#include"Person.h"
#include"Book.h"
#include"MD5.h"
#include"Date.h"
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::ofstream;
using std::ios;
extern Person* person_head;
extern int person_count;
extern int person_last_id;
extern Person* last_person;

Person::Person(int id, int status, const char* name)
{
    this->id = id;
    this->status = status;
    strcpy(username, name);
    next = nullptr;
    last = nullptr;
    Date now;
    signin_date = now;
    book_id1 = 0;
    book_id2 = 0;
    book_id3 = 0;
}

Person::Person() {}

void Person::set_password(const char* password)
{
    strcpy(this->password, password);
    this->password[32] = 0;
}

int Person::change_password()
{
    cout << "����������ǰ�����룺";
    char* now_passwd = new char[33];
    char* new_passwd1 = new char[33];
    char* new_passwd2 = new char[33];
    char* temp = put_passwd();
    strcpy(now_passwd, temp);
    delete temp;
    if (strcmp(now_passwd, show_password()) == 0) {
        int count(0);
    put:
        if (count == 3) {
            cout << "����������࣡" << endl;
            system("PAUSE");
            return 1;
        }
        cout << "���������룺";
        temp = put_passwd();
        strcpy(new_passwd1, temp);
        delete temp;
        cout << "���ٴ��������룺";
        temp = put_passwd();
        strcpy(new_passwd2, temp);
        delete temp;
        if (strcmp(new_passwd1, new_passwd2) != 0) {
            cout << "�������벻һ�£����������롣" << endl;
            count++;
            goto put;
        }
        set_password(new_passwd1);
        save_person();
        cout << "�����޸ĳɹ���" << endl;
    }
    else {
        cout << "�������" << endl;
        system("PAUSE");
        return 1;
    }
    system("PAUSE");
    delete[] now_passwd;
    delete[] new_passwd1;
    delete[] new_passwd2;
    return 0;
}

int Person::borrow_book(int book)
{
    int s = 1;
    if (status == 2) {
        if (book_id1 != 0) {
            cout << endl << "���Ѿ�����һ�����ˡ���Ϊѧ������ֻ��ͬʱ����1��ͼ�顣" << endl << endl;
            throw 2;
        }
        else {
            book_id1 = book;
            cout << "�ɹ�����ͼ��" << id_to_book(book_id1)->show_book_name() << endl;
            cout << "����" << id_to_book(book_id1)->show_repay_date() << "ǰ�黹��" << endl;
            s = 0;
        }
    }
    if (status == 3) {
        if (book_id3 != 0) {
            cout << endl << "���Ѿ�����һ�����ˡ���Ϊ��ʦ���������ͬʱ����3��ͼ�顣" << endl << endl;
            throw 2;
        }
        else {
            if (book_id1 == 0) {
                book_id1 = book;
                cout << "�ɹ�����ͼ��" << id_to_book(book_id1)->show_book_name() << endl;
                cout << "����" << id_to_book(book_id1)->show_repay_date() << "ǰ�黹��" << endl;
                s = 0;
            }
            else if (book_id2 == 0) {
                book_id2 = book;
                cout << "�ɹ�����ͼ��" << id_to_book(book_id2)->show_book_name() << endl;
                cout << "����" << id_to_book(book_id2)->show_repay_date() << "ǰ�黹��" << endl;
                s = 0;
            }
            else {
                book_id3 = book;
                cout << "�ɹ�����ͼ��" << id_to_book(book_id3)->show_book_name() << endl;
                cout << "����" << id_to_book(book_id3)->show_repay_date() << "ǰ�黹��" << endl;
                s = 0;
            }
        }
    }
    save_person();
    return s;
}

int Person::repay_book(int book)
{
    if (status == 2) {
        book_id1 = 0;
    }
    if (status == 3) {
        if (book_id3 == book) {
            book_id3 = 0;
        }
        else if(book_id2 == book) {
            book_id2 = book_id3;
            book_id3 = 0;
        }
        else if (book_id1 == book) {
            book_id1 = book_id2;
            book_id2 = book_id3;
            book_id3 = 0;
        }
    }
    save_person();
    return 0;
}

int Person::show_status()
{
    return status;
}

char* Person::show_username()
{
    return username;
}

int Person::show_id()
{
    return id;
}

char* Person::show_password()
{
    return password;
}

Date Person::show_signin_date()
{
    return signin_date;
}

int Person::show_borrowed_book()
{
    if (book_id1 == 0) {
        cout << "��û�н��κ�ͼ�飡" << endl;
        return 1;
    }
    unsigned max_length = strlen(id_to_book(book_id1)->show_book_name());
    if (book_id2) {
        if (strlen(id_to_book(book_id2)->show_book_name()) > max_length)
            max_length = strlen(id_to_book(book_id2)->show_book_name());
    }
    if (book_id3) {
        if (strlen(id_to_book(book_id3)->show_book_name()) > max_length)
            max_length = strlen(id_to_book(book_id3)->show_book_name());
    }
    max_length /= 8;
    cout << endl << "���Ѿ��赽��ͼ�����£�" << endl;
    cout << "���\t" << "����";
    for (unsigned i = 0; i < max_length + 1; i++) {
        cout << "\t";
    }
    cout << "��ISBN��\tID\t����������" << endl;
    for (unsigned i = 0; i < max_length + 7; i++) {
        cout << "--------";
    }
    cout << endl;
    int count = 0;
    cout << count << "\t";
    cout << id_to_book(book_id1)->show_book_name();
    for (unsigned i = 0; i < max_length - (int)strlen(id_to_book(book_id1)->show_book_name()) / 8; i++) cout << "\t";
    cout << "\t";
    cout << id_to_book(book_id1)->show_book_ISBN() << "\t";
    cout << id_to_book(book_id1)->show_book_id() << "\t";
    cout << id_to_book(book_id1)->show_repay_date() << endl;
    if (book_id2 != 0) {
        count++;
        cout << count << "\t";
        cout << id_to_book(book_id2)->show_book_name();
        for (unsigned i = 0; i < max_length - (int)strlen(id_to_book(book_id2)->show_book_name()) / 8; i++) cout << "\t";
        cout << "\t";
        cout << id_to_book(book_id2)->show_book_ISBN() << "\t";
        cout << id_to_book(book_id2)->show_book_id() << "\t";
        cout << id_to_book(book_id2)->show_repay_date() << endl;
    }
    if (book_id3 != 0) {
        count++;
        cout << count << "\t";
        cout << id_to_book(book_id3)->show_book_name();
        for (unsigned i = 0; i < max_length - (int)strlen(id_to_book(book_id3)->show_book_name()) / 8; i++) cout << "\t";
        cout << "\t";
        cout << id_to_book(book_id3)->show_book_ISBN() << "\t";
        cout << id_to_book(book_id3)->show_book_id() << "\t";
        cout << id_to_book(book_id3)->show_repay_date() << endl;
    }
    return 0;
}

char* put_passwd()
{
    char ch, passwd[33];
    int i = 0;
    while ((ch = _getch()) != '\r')
    {
        if (ch != 8)
        {
            passwd[i] = ch;
            putchar('*');
            i++;
        }
        else
        {
            putchar('\b');
            putchar(' ');
            putchar('\b');
            i--;
        }
    }
    passwd[i] = '\0';
    char* a=new char[33];
    strcpy(a, md5(passwd));
    a[32] = 0;
    cout << endl;
    return a;
}

Person* login(char* name, char* word)
{
    Person* user = person_head;
    do {
        if (strcmp(user->show_username(), name) == 0) {
            if (strcmp(user->show_password(), word) == 0) return user;
        }
        user = user->next;
    } while (user != nullptr);
    return nullptr;
}

int add_person()
{
    char* name = new char[50];
    char* password1 = new char[33];
    char* password2 = new char[33];
    int status;
    int count(0);
putinname:
    cout << "�������û�����";
	cin >> name;
    Person* a = check_uesrname(name);
    if (a != nullptr) {
        cout << "���û����Ѿ����ڡ�" << endl;
        goto putinname;
    }
    cout << "0��ʾϵͳ����Ա��1��ʾͼ�����Ա��2��ʾѧ����3��ʾ��ʦ��" << endl;
    cout << "��������ݣ�" << endl;
    cin >> status;
    char* temp;
putin:
    if (count == 3) throw -1;
    cout << "���������룺";
    temp = put_passwd();
    strcpy(password1, temp);
    delete temp;
    cout << "���ٴ��������룺";
    temp = put_passwd();
    strcpy(password2, temp);
    delete temp;
    if (strcmp(password1, password2) != 0) {
        cout << "�������벻һ�£����������롣" << endl;
        count++;
        goto putin;
    }
    Person* p = new Person(++person_last_id, status, name);
    p->set_password(password1);
    p->last = last_person;
    last_person->next = p;
    last_person = p;
    person_count++;
    save_person();
    cout << "�ɹ�����û���" << endl;
    system("PAUSE");
    delete[] name;
    delete[] password1;
    delete[] password2;
	return 0;
}

void remove_person(Person* person)
{
    if (person->last == nullptr) {
        person_head = person;
        person->next->last = nullptr;
        delete person;
    }
    else if (person->next == nullptr) {
        person->last = nullptr;
        last_person = person->last;
        delete person;
    }
    else {
        person->last->next = person->next;
        person->next->last = person->last;
        delete person;
    }
    person_count--;
    save_person();
}

Person* check_uesrname(char* username)
{
    Person* user = person_head;
    do {
        if (strcmp(user->show_username(), username) == 0) return user;
        user = user->next;
    } while (user != nullptr);
    return nullptr;
}

void save_person()
{
    ofstream out("person.dat", ios::out | ios::binary);
    if (out.is_open()) {
        out.write((char*)&person_count, sizeof(int));
        out.write((char*)&person_last_id, sizeof(int));
        Person* p = person_head;
        for (int i = 0; i < person_count; i++) {
            out.write((char*)p, sizeof(Person));
            p = p->next;
        }
        out.close();
    }
    else {
        cout << "Person��Ϣ�������" << endl;
        system("PAUSE");
    }
}
