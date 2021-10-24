#include "Library.h"
#include "Student.h"
#include "Admin.h"
#include "grammarCheck.h"
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <locale>
#include <filesystem>
#include <io.h>
//#include <windows.h>
//#include<rpcndr.h>
//#include<WTypesbase.h>
//#include<wtypes.h>
//#include<ObjIdlbase.h>
//#include<ObjIdl.h>
//#include<OAIdl.h>

using namespace std;

Library::Library()
	:current_menu(0)
{
	// ������ 
	time_t timet = time(nullptr);
	struct tm stm; 
	localtime_s(&stm, &timet);

	char buf[100];
	
	if (strftime(buf, sizeof(buf), "%Y%m%d", &stm)) { //Year Month Day �پ ����
		//current_date = buf;
		setCurrent_date(buf);
	}
} 

//�ϼ�
void Library::startMenu()
{
	int num;
	while (true) {
		cout << "1. �α���" << endl;
		cout << "2. ȸ�� ����" << endl;
		cout << "3. �ý��� ����" << endl;

		cin >> num;
		setCurrent_menu(num);

		switch (num) {
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

//�ϼ�
void Library::login()
{
	User* user;
	string t_id;
	string t_password;
	char tt;
	
	ifstream read_ID_file;
	// �α��� ���� �Է�
	while (true) {
		cout << "���̵� : ";
		cin >> t_id;

		// ���̵� ���� ���� Ȯ�� -  �����ϸ� return ����(����ȭ������ �̵�) continue (�ٽ� �Է�) ����

		if (t_id != "admin" && !check_id(t_id)) {
			cout << "�ùٸ��� ���� ���̵��Դϴ�." << endl;
			cout << "�ٽ� �Է� �Ͻ÷��� 'Y'��, ����ȭ������ ���ư��÷��� �ƹ�Ű�� �����ּ���." << endl;
			cin >> tt;
			if (tt != 'Y' && tt != 'y') {
				//system("cls");
				return;
			}
			else
				continue;
		}

		// ���̵� ���� ���� Ȯ��
		string id_file = "datafile/User/" + t_id + ".txt";
		if (_access(id_file.c_str(), 0) == -1)// ���̵� �������� �������
		{
			cout << "�������� �ʴ� ���̵��Դϴ�." << endl;
			cout << "�ٽ� �Է� �Ͻ÷��� 'Y'��, ����ȭ������ ���ư��÷���ƹ�Ű�� �����ּ���." << endl;
			cin >> tt;
			if (tt != 'Y' && tt != 'y')
				return;
			else
				continue;
		}
		else { // ���̵� �����ϴ� ���
			read_ID_file.open(id_file);
			if (_access(id_file.c_str(), 0) == -1) {
				cerr << "idFile is not open for login" << endl;
				exit(1);
			}
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
			if (tt != 'Y' && tt != 'y') {
				read_ID_file.close();
				return;
			}
			else {
				read_ID_file.close();
				continue;
			}
		}

		string std_info; // �л� �н�����
		getline(read_ID_file, std_info);
		string std_password = std_info.substr(0, std_info.find('_' | '\n'));
		if (std_password != t_password) { // ������� password �� �ٸ���
			cout << "ȸ������ ��й�ȣ�� ��ġ���� �ʽ��ϴ�." << endl;
			cout << "�ٽ� �Է� �Ͻ÷��� 'Y'��, ����ȭ������ ���ư��÷���ƹ�Ű�� �����ּ���." << endl;
			cin >> tt;
			if (tt != 'Y' && tt != 'y') {
				read_ID_file.close();
				return;
			}
			else
				read_ID_file.close();
				continue;
		}
		else { // ��й�ȣ�� ��ġ�ϴ°��
			break;
		}
	}
	read_ID_file.close();

	if (t_id == "admin") {
		user = new Admin();
	}
	else {
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
	}
	else if (dynamic_cast<Admin*>(user) != nullptr) { // ������ ���
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

//ȸ������
void Library::makeAccount()
{
	string t_id;
	char tt;

	while (true) {
		cout << "���̵� : ";
		cin >> t_id;

		// ���̵� ���� ���� Ȯ��
		//�����ϸ� return ����(����ȭ������ �̵�) continue (�ٽ� �Է�) ����
		if (!check_id(t_id)) {
			cout << "�ùٸ��� ���� ���̵��Դϴ�." << endl;
			cout << "�ٽ� �Է� �Ͻ÷��� 'Y'��, ����ȭ������ ���ư��÷���ƹ�Ű�� �����ּ���." << endl;
			cin >> tt;
			if (tt != 'Y' && tt != 'y') {
				return;
			}
		}

		string id_file = "datafile/User/" + t_id + ".txt";
		if (_access(id_file.c_str(), 0) == 0) { // ���̵� ������ ���
			/*�̹� �����ϴ� ���̵� üũ*/
			cout << "�̹� �����ϴ� ���̵��Դϴ�." << endl;
			cout << "�ٽ� �Է� �Ͻ÷��� 'Y'��, ����ȭ������ ���ư��÷���ƹ�Ű�� �����ּ���." << endl;
			cin >> tt;
			if (tt != 'Y' && tt != 'y')
				return;
		}
		else { // ���̵� �������� �ʴ� ���
			break;
		}
	}

	string t_password;
	while (true) {
		cout << "��й�ȣ : ";
		cin >> t_password;

		if (!check_password(t_password)) {
			cout << "�ùٸ��� ���� ��й�ȣ�Դϴ�." << endl;
			cout << "�ٽ� �Է� �Ͻ÷��� 'Y'��, ����ȭ������ ���ư��÷���ƹ�Ű�� �����ּ���." << endl;
			cin >> tt;
			if (tt != 'Y' && tt != 'y') {
				return;
			}
		}
	}

	string t_name;
	while (true) {
		cout << "�̸� : ";
		cin >> t_name;

		if (!check_Name(t_name)) {
			cout << "�ùٸ��� ���� �̸��Դϴ�." << endl;
			cout << "�ٽ� �Է� �Ͻ÷��� 'Y'��, ����ȭ������ ���ư��÷���ƹ�Ű�� �����ּ���." << endl;
			cin >> tt;
			if (tt != 'Y' && tt != 'y') {
				return;
			}
		}
	}

	string t_sid;
	while (true) {
		cout << "�й� : ";
		cin >> t_sid;

		if (!check_studentID(t_sid)) {
			cout << "�ùٸ��� ���� �й��Դϴ�." << endl;
			cout << "�ٽ� �Է� �Ͻ÷��� 'Y'��, ����ȭ������ ���ư��÷���ƹ�Ű�� �����ּ���." << endl;
			cin >> tt;
			if (tt != 'Y' && tt != 'y') {
				return;
			}
		}

		bool flag = false;
		//�̹� ���ԵǾ� �ִ� �й����� Ȯ��->c++17
		for (auto& file : filesystem::directory_iterator(filesystem::current_path().string() + "datafile/User/")) 
		{
			ifstream fs(file.path());    //open the file	
			
			string info;
			fs >> info; // ��й�ȣ_�̸�_�й�
			
			string t1,t2,fsid;
			t1 = info.substr(info.find('_') + 1, string::npos); // �̸�_�й�
			t2 = info.substr(0, info.find('_')); // �̸�
			fsid = info.substr(info.find('_') + 1, string::npos); // �й�
			fs.close();
			if (fsid == t_sid){
				flag= true;
				break;
			}
		}

		if (flag) {
			cout << "�̹� ���ԵǾ� �ִ� �й��Դϴ�." << endl;
			cout << "�ٽ� �Է� �Ͻ÷��� 'Y'��, ����ȭ������ ���ư��÷���ƹ�Ű�� �����ּ���." << endl;
			cin >> tt;
			if (tt != 'Y') {
				return;
			}		
		} else {
			break;
		}
	}

	// ���� ���� ���� �Ϸ�
	ofstream new_student_file("datafile/User/" + t_id + ".txt");

	new_student_file << t_password << "_" << t_name << "_" << t_sid << endl;
	new_student_file << "false" << endl; // ��ü���� �ʱ�ȭ false
	new_student_file << "false" << endl << endl; // blackList ���� �ʱ�ȭ false

	new_student_file << "���⵵������" << endl;
	new_student_file << "���൵������" << endl;

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
