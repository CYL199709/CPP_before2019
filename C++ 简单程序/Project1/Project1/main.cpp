#include <iostream>
#include <cstring>
#include <cstdlib>
using namespace std;
class Complex {
private:
	double r, i;
public:
	void Print() {
		cout << r << "+" << i << "i" << endl;
	}
	// �ڴ˴�������Ĵ���
		Complex(const char a[])
		{
			         r = a[0] - '0';
			        i = a[2] - '0';
			
		}
	    Complex(){}//���� 
};
int main() {
	Complex a;
	a = "3+4i"; a.Print();
	a = "5+6i"; a.Print();
	system("pause");
	return 0;
}