#include "Library.h"
#include "Book.h"
#include "Admin.h"
#include "Student.h"

#include <iostream>

using namespace std;

int main(void) {
	//Book::Book(string na, string au, string tr, string publisher, string year) { // �����
	/*Book book("����", "�۰�", "����", "���ǻ�", "2000");
	Student student;

	student.menu();

	*/

	Library lib;

	while (lib.getCurrent_menu() != 3)
	{ // �ý��� ���ᰡ �ƴҶ� ���� ���
		lib.startMenu();
	}

	return 0;
}

/*

Library -> ���۰� ���ÿ� ��ü library �� �����ϴ� class
Book -> å�� ���� ������ ����ִ� class
User -> Admin �� Student�� �θ� Ŭ����
Admin -> �����ڷ� ���ӵ����� �����Ǵ� Ŭ����
Student -> �л����� ���������� �����Ǵ� Ŭ����

*/
