#include "Library.h"
#include "Student.h"
#include "Admin.h"
#include "grammarCheck.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

Library::Library()
	:current_menu(0)
{
}

//�ϼ�
void Library::startMenu()
{
	int num;
	while(true){
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
	int tt;
	// �α��� ���� �Է�

	ifstream read_ID_file;
	while (true) {
		cout << "���̵� : ";
		cin >> t_id;

		// ���̵� ���� ���� Ȯ�� ->
		//�����ϸ� return ����(����ȭ������ �̵�) continue (�ٽ� �Է�) ����
		if (!check_id(t_id)) {
			cout << "�ùٸ��� ���� ���̵��Դϴ�." << endl;
			cout << "�ٽ� �Է� �Ͻ÷��� 'Y'��, ����ȭ������ ���ư��÷���ƹ�Ű�� �����ּ���." << endl;
			cin >> tt;
			if (tt != 'Y')
				return;
		} 
		
		read_ID_file.open("datafile/User/"+ t_id +".txt");
		if (!read_ID_file.is_open()) { // ���̵� �������� �������
			/*�̹� �����ϴ� ���̵� üũ  x*/
			cout << "�������� �ʴ� ���̵��Դϴ�." << endl;
			cout << "�ٽ� �Է� �Ͻ÷��� 'Y'��, ����ȭ������ ���ư��÷���ƹ�Ű�� �����ּ���." << endl;
			cin >> tt;
			if (tt != 'Y')
				return;

		} else { // ���̵� �����ϴ� ���
			break;
		}

	}

	while (true) {
		cout << "��й�ȣ : ";
		cin >> t_password;

		//�����ϸ� return ����(����ȭ������ �̵�) continue (�ٽ� �Է�) ����
		if (!check_password(t_password)) {
			cout << "�ùٸ��� ���� ��й�ȣ�Դϴ�." << endl;
			cout << "�ٽ� �Է� �Ͻ÷��� 'Y'��, ����ȭ������ ���ư��÷���ƹ�Ű�� �����ּ���." << endl;
			cin >> tt;
			if (tt!='Y'){
				return;
			}
		}

		string std_info; // �л� �н�����
		getline(read_ID_file, std_info);
		string std_password = std_info.substr(0, std_info.find('_'));
		if (std_password != t_password) { // ������� password �� �ٸ���
			cout << "ȸ������ ��й�ȣ�� ��ġ���� �ʽ��ϴ�." << endl;
			cout << "�ٽ� �Է� �Ͻ÷��� 'Y'��, ����ȭ������ ���ư��÷���ƹ�Ű�� �����ּ���." << endl;
			cin >> tt;
			if (tt!='Y'){
				read_ID_file.close();
				return;
			}
		} else { // ��й�ȣ�� ��ġ�ϴ°��
			break;
		}
	}
	read_ID_file.close();

	if (t_id=="admin"){
		user = new Admin();
	}else{
		user = new Student(t_id);
	}

	if (dynamic_cast<Student*>(user) != nullptr) { //�л��̸�
		Student* std = dynamic_cast<Student*>(user);
		while (std->getCurrent_menu() != 4) {
			std->menu();
			// �޴����� �α׾ƿ� (4��) ���ý� while���� ��������
		}
		delete std;
		std = nullptr;
	} else if (dynamic_cast<Admin*>(user) != nullptr) { // ������ ���
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

// ������ ȸ������ - �̹� �����ϴ� �й�?
void Library::makeAccount()
{
	string t_id;
	int tt;
	ifstream read_ID_file;

	while (true) {
		cout << "���̵� : ";
		cin >> t_id;

		// ���̵� ���� ���� Ȯ��
		//�����ϸ� return ����(����ȭ������ �̵�) continue (�ٽ� �Է�) ����
		if (!check_id(t_id)) {
			cout << "�ùٸ��� ���� ���̵��Դϴ�." << endl;
			cout << "�ٽ� �Է� �Ͻ÷��� 'Y'��, ����ȭ������ ���ư��÷���ƹ�Ű�� �����ּ���." << endl;
			cin >> tt;
			if (tt!='Y'){
				return;
			}
		}

		read_ID_file.open("datafile/User/"+t_id+".txt");
		if (read_ID_file.is_open()) { // ���̵� ������ ���
			/*�̹� �����ϴ� ���̵� üũ*/
			cout << "�̹� �����ϴ� ���̵��Դϴ�." << endl;
			cout << "�ٽ� �Է� �Ͻ÷��� 'Y'��, ����ȭ������ ���ư��÷���ƹ�Ű�� �����ּ���." << endl;
			cin >> tt;
			if (tt != 'Y')
				return;

		} else { // ���̵� �������� �ʴ� ���
			break;
		}
	}

	string t_password;
	while (true) {
		cout<<"��й�ȣ : ";
		cin>>t_password;

		if (!check_password(t_password)) {
			cout << "�ùٸ��� ���� ��й�ȣ�Դϴ�." << endl;
			cout << "�ٽ� �Է� �Ͻ÷��� 'Y'��, ����ȭ������ ���ư��÷���ƹ�Ű�� �����ּ���." << endl;
			cin >> tt;
			if (tt!='Y'){
				return;
			}
		}
	}

	string t_name;
	while (true) {
		cout<<"�̸� : ";
		cin>>t_name;

		if (!check_Name(t_name)) {
			cout << "�ùٸ��� ���� �̸��Դϴ�." << endl;
			cout << "�ٽ� �Է� �Ͻ÷��� 'Y'��, ����ȭ������ ���ư��÷���ƹ�Ű�� �����ּ���." << endl;
			cin >> tt;
			if (tt!='Y'){
				return;
			}
		}
	}

	string t_sid;
	while (true) {
		cout<<"�й� : ";
		cin>>t_sid;
		

		if (!check_studentID(t_sid)) {
			cout << "�ùٸ��� ���� �й��Դϴ�." << endl;
			cout << "�ٽ� �Է� �Ͻ÷��� 'Y'��, ����ȭ������ ���ư��÷���ƹ�Ű�� �����ּ���." << endl;
			cin >> tt;
			if (tt!='Y'){
				return;
			}
		}

		//�̹� ���ԵǾ� �ִ� �й����� Ȯ���ؾ��� �𸣰ڴ�
	}

	// ���� ���� ���� �Ϸ� -> �̻��� ���ƿ�
	ofstream new_student_file("datafile/Student/" + t_id + ".txt");
	if (!new_student_file.is_open()) {
		cerr << "datafile/Student/" + t_id + ".txt	file is Not Open" << endl;
	}

	new_student_file << t_password << "_" << t_name << "_" << t_sid << endl;
	new_student_file << "false" << endl; // ��ü���� �ʱ�ȭ false
	new_student_file << "false" << endl << endl; // blackList ���� �ʱ�ȭ false

	new_student_file << "���� ���� ����" << endl;
	new_student_file << "���� ���� ����" << endl;

	new_student_file.close();
}

void Library::setCurrent_menu(int current_menu)
{
	this->current_menu = current_menu;
}

int Library::getCurrent_menu() const
{
	return current_menu;
}
