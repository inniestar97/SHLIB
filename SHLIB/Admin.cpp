#include "Admin.h"
#include "string.h"
#include "grammarCheck.h"
#include <iostream>

using namespace std;

Admin::Admin()
	:current_menu(0)
{
	
}

Admin::~Admin()
{
	
}

//�ϼ�
void Admin::menu() // ������ �޴� -> Ÿ �Լ����� ȣ��� �׳� return �ϸ� �� �޴��� ���ƿ��� �˴ϴ�.
{
	while (True) {
	int num;
		cout << "<������ ���>\n" << endl;
		cout << "1. ���� �߰�" << endl;
		cout << "2. ���� ����" << endl;
		cout << "3. ȸ�� ����͸�" << endl;
		cout << "4. �α׾ƿ�\n\n" << endl;
		cout << "�޴� ���� : ";
		cin >> num;		
		setCurrent_menu(num);
		switch (num) {
		case 1:
			addBook	Menu();
			break;
		case 2:
			deleteBookMenu();
			break;
		case 3:
			monitoring();
			break;
		case 4:
			return;
		}
	}
}

//��
void Admin::addBookMenu() // �����߰�
{
	cout << "<���� �߰�>\n ������/���ڸ�/����/���ǻ�/����⵵\n\n";
	cout << "1. ���� ���� '/'�����ڷ� �յ� ���� ���� �����Ͽ� �Է��� �ּ���.\n";
	cout << "2. ���ڰ� ���� �� ĭ�� ����ּ���. ex) ������/���ڸ�//���ǻ�/����⵵\n";
	cout << "3. ':q'�� �Է� �� ������ ����� ���� �޴��� ���ϴ�.\n\n";
	cout << "-----------------------------------------------------------------------\n";
	cout << "���� ���� : ";
	
	string inp_s;
	cin >> inp_s;
	if (inp == ":q"){
		return
	}
	
	// �Է��� ������ �̹� �����ϴ��� Ȯ��
	
	//������ �޴��� ���ư�
	cout << "�̹� �ش� ������ �����մϴ�.\n";
	

	//������ �ش� ������ ���������Ͽ� �߰�
	

}

//��
void Admin::deleteBookMenu() // ���� ����
{
	int n;
	cout << "<���� ����>\n";
	cout << "1. ���������� ����\n";
	cout << "2. ���ڸ����� ����\n";
	cout << "3. ���ư���\n\n";
	cout << "���� : ";

	cin >> n;
	switch(n) {
	case 1:
		cout << "�������� �Է��ϼ��� : ";
		break;
	case 2:
		cout << "���ڸ��� �Է��ϼ��� : ";
		break;
	case 3:
		break;
	}

	// �Է��� ������ �����ϴ��� Ȯ��
	//������ ����
	//������ ���ٰ� ����ϰ� �޴��� ���ư�

}

// ��
void Admin::monitoring() // ȸ�� ����͸�
{
	int n, cnum;
	cout << "<ȸ�� ����͸�>\n";
	cout << "1. ��ü�� ���.\n";
	cout << "2. ������ ���\n";
	cout << "3. ������Ʈ\n";
	cout << "4. ���ư���\n";
	cout << "���� : ";

	cin >> n;

	switch(n) {
	case 1:
		cout << "<��ü�� ���>\n";
		for(auto omem : overdueList) {
			
		}
		cout << "������Ʈ�� �߰��� ȸ�� ��ȣ �Է� (�ڷ� ������ ':q'�� �Է��ϼ���)\n";
		cout << ">> ";
		cin >> cnum;

		break;
	case 2:
		cout << "<������ ���>\n";
		for(auto bmem : borrowList) {
			
		}
		cout << "(�ڷ� ������ ':q'�� �Է��ϼ���)\n";
		cout << ">> ";
		cin >> cnum;

		break;
	case 3:
		cout << "<������Ʈ>\n";
		for(auto blackmem : blackList) {
			
		}
		cout << "������Ʈ���� ������ ȸ�� ��ȣ �Է� (�ڷ� ������ ':q'�� �Է��ϼ���)\n";
		cout << ">> ";
		cin >> cnum;

		break;
	case 4:
		break;
	}
}

void Admin::setCurrent_menu(int menu)
{
	this->current_menu = menu;
}

int Admin::getCurrent_menu() const
{
	return current_menu;
}
