#include "Library.h"
#include "Student.h"
#include "Admin.h"
#include "grammarCheck.h"
#include <iostream>
#include <string>

using namespace std;

Library::Library()
	:current_menu(0)
{
}

//������
void Library::startMenu()
{
	int num;
	while(True){
		cout << "1. �α���" << endl;
		cout << "2. ȸ�� ����" << endl;
		cout << "3. �ý��� ����" << endl;

		cin >> num;
		setCurrent_menu(num);
		switch(num){
		case 1:
			login();
			break;
		case 2:
			makeAccount();
			break;
		case 3:
			return;// ����
		}
	}
}

//������ ������ ������ ���� �ʿ�κ� ����
void Library::login()
{
	User* user;
	string t_id;
	string t_password;
	
	// �α��� ���� �Է�
	while (true){
		cout<<"���̵� : ";
		cin>>t_id;
		
		// ���̵� ���� ���� Ȯ��
		if ()	
		//�����ϸ� return ����(����ȭ������ �̵�) continue (�ٽ� �Է�) ����


		cout<<"��й�ȣ : ";
		cin>>t_password;



		//�����ϸ� break

		//�����ϸ� return ����(����ȭ������ �̵�) continue (�ٽ� �Է�) ����

	}

	/*
	���� ���̵� ���� �� �� �ϳ� �����Ҵ�
	*/

	if (t_id=="admin"){
		user=new Student();
	}else{
		user = new Admin();
	}


	if (dynamic_cast<Student*>(user) != nullptr) { //�л��̸�
		Student* std = dynamic_cast<Student*>(user);

		while (std->getCurrent_menu() != 4) {
			std->menu();

			// �޴����� �α׾ƿ� (4��) ���ý� while���� ��������
		}

		delete std;
		std = nullptr;
	}
	else if (dynamic_cast<Admin*>(user) != nullptr) { // �����ڸ�
		Admin* ad = dynamic_cast<Admin*>(user);

		while (ad->getCurrent_menu() != 4) {
			ad->menu();

			// �޴����� �α׾ƿ� (4��) ���ý� while���� ��������
		}

		delete ad;
		ad = nullptr;
	}

	delete user;
	user = nullptr;
}

void Library::makeAccount()
{
	
}

void Library::setCurrent_menu(int current_menu)
{
	this->current_menu = current_menu;
}

int Library::getCurrent_menu() const
{
	return current_menu;
}
