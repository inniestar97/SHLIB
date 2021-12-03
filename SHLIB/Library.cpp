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

using namespace std;

Library::Library()
	:current_menu(0)
{
	// ������ 
	time_t timet = time(nullptr);
	struct tm stm; 
	localtime_s(&stm, &timet);

	char buf[100];
	
	if (strftime(buf, sizeof(buf), "%Y.%m.%d", &stm)) { //Year Month Day �پ ����
		setCurrent_date(buf);
	}
} 

//�ϼ�
void Library::startMenu()
{
	string xx;
	while (true) {
		cout << "1. �α���" << endl;
		cout << "2. ȸ�� ����" << endl;
		cout << "3. �ý��� ����" << endl;
		getline(cin, xx);
		bool check = true;
		for (size_t i = 0; i < xx.size(); i++) {
			if (isdigit(xx[i]) == 0) {
				cout << "���ڸ� �Է��Ͽ� �ּ���" << endl;
				check = false;
			}
			if (!check) break;
		}
		if (!check) continue;

		
		int num = stoi(xx);
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
		default:
			cout << "�߸��� �����Դϴ�. �ٽ� �Է����ּ���." << endl;
		}
	}
}

//�ϼ�
void Library::login()
{
	User* user;
	string t_id;
	string t_password;
	string tt;
	
	ifstream read_ID_file;
	// �α��� ���� �Է�
	while (true) {
		cout << "���̵� : ";
		getline(cin, t_id);

		// ���̵� ���� ���� Ȯ�� -  �����ϸ� return ����(����ȭ������ �̵�) continue (�ٽ� �Է�) ����
		if (t_id != "admin" && !check_id(t_id)) {
			cout << "�ùٸ��� ���� ���̵��Դϴ�." << endl;
			cout << "�ٽ� �Է� �Ͻ÷��� 'Y(y)'��, ����ȭ������ ���ư��÷��� �ƹ�Ű�� �����ּ���." << endl;
			getline(cin, tt);
			if (tt != "Y" && tt != "y") {
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
			cout << "�ٽ� �Է� �Ͻ÷��� 'Y(y)'��, ����ȭ������ ���ư��÷���ƹ�Ű�� �����ּ���." << endl;
			getline(cin, tt);
			if (tt != "Y" && tt != "y")
				return;
			else
				continue;
		}
		else { // ���̵� �����ϴ� ���
			read_ID_file.open(id_file);
			if (!read_ID_file.is_open()) {
				cerr << "idFile is not open for login" << endl;
				exit(1);
			}
			break;
		}
	}

	string std_info; // �л� �н�����
	getline(read_ID_file, std_info);
	string std_password = std_info.substr(0, std_info.find('_' | '\n'));

	while (true) {
		cout << "��й�ȣ : ";
		getline(cin, t_password);

		//�����ϸ� return ����(����ȭ������ �̵�) continue (�ٽ� �Է�) ����
		if (!check_password(t_password)) {
			cout << "�ùٸ��� ���� ��й�ȣ�Դϴ�." << endl;
			cout << "�ٽ� �Է� �Ͻ÷��� 'Y(y)'��, ����ȭ������ ���ư��÷���ƹ�Ű�� �����ּ���." << endl;
			getline(cin, tt);
			if (tt != "Y" && tt != "y") {
				read_ID_file.close();
				return;
			}
			else {
				continue;
			}
		}

		
		if (std_password != t_password) { // ������� password �� �ٸ���
			cout << "ȸ������ ��й�ȣ�� ��ġ���� �ʽ��ϴ�." << endl;
			cout << "�ٽ� �Է� �Ͻ÷��� 'Y(y)'��, ����ȭ������ ���ư��÷���ƹ�Ű�� �����ּ���." << endl;
			getline(cin, tt);
			if (tt != "Y" && tt != "y") { 
				read_ID_file.close();
				return;
			}
			else
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

	user = nullptr;
}

//ȸ������
void Library::makeAccount()
{
	string t_id;
	string tt;

	while (true) {
		cout << "���̵� : ";
		getline(cin, t_id);

		// ���̵� ���� ���� Ȯ��
		//�����ϸ� return ����(����ȭ������ �̵�) continue (�ٽ� �Է�) ����
		if (!check_id(t_id)) {
			cout << "�ùٸ��� ���� ���̵��Դϴ�." << endl;
			cout << "�ٽ� �Է� �Ͻ÷��� 'Y(y)'��, ����ȭ������ ���ư��÷���ƹ�Ű�� �����ּ���." << endl;
			getline(cin, tt);
			if (tt != "Y" && tt != "y") {
				return;
			}
			else continue;

		}

		string id_file = "datafile/User/" + t_id + ".txt";
		if (_access(id_file.c_str(), 0) == 0) { // ���̵� ������ ���
			/*�̹� �����ϴ� ���̵� üũ*/
			cout << "�̹� �����ϴ� ���̵��Դϴ�." << endl;
			cout << "�ٽ� �Է� �Ͻ÷��� 'Y(y)'��, ����ȭ������ ���ư��÷���ƹ�Ű�� �����ּ���." << endl;
			getline(cin, tt);
			if (tt != "Y" && tt != "y")
				return;
			else continue;

		}
		else { // ���̵� �������� �ʴ� ���
			break;
		}
	}

	string t_password;
	bool pass_check = false;
	while (true) {
		cout << "��й�ȣ : ";

		getline(cin, t_password);

		if (!check_password(t_password)) {
			cout << "�ùٸ��� ���� ��й�ȣ�Դϴ�." << endl;
			cout << "�ٽ� �Է� �Ͻ÷��� 'Y(y)'��, ����ȭ������ ���ư��÷���ƹ�Ű�� �����ּ���." << endl;
			getline(cin, tt);
			if (tt != "Y" && tt != "y") {
				return;
			} else {
				continue;
			}
		}
		else {
			string check_password;
			cout << "��й�ȣ Ȯ�� : ";
			getline(cin, check_password);
			if (t_password != check_password) {	
				cout << "��й�ȣ Ȯ���� Ʋ���ϴ�." << endl;
				cout << "�ٽ� �Է� �Ͻ÷��� 'Y(y)'��, ����ȭ������ ���ư��÷���ƹ�Ű�� �����ּ���." << endl;
				getline(cin, tt);
				if (tt != "Y" && tt != "y") {
					return;
				} else {
					continue;
				}
			}
			else {
				break;
			}
		}
	}

	string t_name;
	while (true) {
		cout << "�̸� : ";
		getline(cin, t_name);

		if (!check_Name(t_name)) {
			cout << "�ùٸ��� ���� �̸��Դϴ�." << endl;
			cout << "�ٽ� �Է� �Ͻ÷��� 'Y(y)'��, ����ȭ������ ���ư��÷���ƹ�Ű�� �����ּ���." << endl;
			getline(cin, tt);
			if (tt != "Y" && tt != "y") {
				return;
			}
			else continue;

		}
		else break;
	}

	string t_sid;
	while (true) {
		cout << "�й� : ";
		getline(cin, t_sid);

		if (!check_studentID(t_sid)) {
			cout << "�ùٸ��� ���� �й��Դϴ�." << endl;
			cout << "�ٽ� �Է� �Ͻ÷��� 'Y(y)'��, ����ȭ������ ���ư��÷���ƹ�Ű�� �����ּ���." << endl;
			getline(cin, tt);
			if (tt != "Y" && tt != "y") {
				return;
			}
			else continue;

		}

		bool flag = false;
		//�̹� ���ԵǾ� �ִ� �й����� Ȯ��->c++17
		for (auto& file : filesystem::directory_iterator(filesystem::current_path().string() + "/datafile/User/")) 
		{
			ifstream fs(file.path());    //open the file	
			
			string info;
			fs >> info; // ��й�ȣ_�̸�_�й�
			
			string t1, t2, fsid;
			t1 = info.substr(info.find('_') + 1, string::npos); // �̸�_�й�
			t2 = t1.substr(0, t1.find('_')); // �̸�
			fsid = t1.substr(t1.find('_') + 1, string::npos); // �й�
			fs.close();
			if (fsid == t_sid){
				flag = true;
				break;
			}
		}

		if (flag) {
			cout << "�̹� ���ԵǾ� �ִ� �й��Դϴ�." << endl;
			cout << "�ٽ� �Է� �Ͻ÷��� 'Y(y)'��, ����ȭ������ ���ư��÷���ƹ�Ű�� �����ּ���." << endl;
			getline(cin, tt);
			if (tt != "Y" && tt != "y") {
				return;
			}	
			else continue;
	
		} else {
			break;
		}
	}

	// ���� ���� ���� �Ϸ�
	ofstream new_student_file("datafile/User/" + t_id + ".txt");

	if (!new_student_file) {
		cout << "���� open ����" << endl;
		return;
	}

	new_student_file << t_password << "_" << t_name << "_" << t_sid << endl;
	new_student_file << "false" << endl; // ���ѻ��� �ʱ�ȭ false
	new_student_file << 0 << endl << endl; // ���ѻ��� ���� �ʱ�ȭ

	new_student_file << "���⵵������" << endl;
	for (size_t i = 1; i <= 3; i++) {
		new_student_file << i << "." << endl;;
	}
	new_student_file << "���൵������" << endl;
	for (size_t i = 1; i <= 3; i++) {
		new_student_file << i << "." << endl;;
	}

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
