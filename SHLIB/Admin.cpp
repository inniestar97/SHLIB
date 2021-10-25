#include "Admin.h"
#include "grammarCheck.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <filesystem>
#include <sstream>
#include <io.h>

using namespace std;

Admin::Admin()
	:current_menu(0)
{
	/*
	 * Admin�α��ν� �����ڸ���Ʈ, ������Ʈ�� ��� ���ϳ����� �ҷ��� 
	 * �ҷ��� ��, �� ������� vector �� push
	 */
	string info;
	ifstream borrowFile, blackFile, bookFile;
	//ifstream overdueFile;
	borrowFile.open("datafile/User/forAdmin/borrowList.txt");
	if (!borrowFile.is_open()) {
		cerr << "datafile/User/forAdmin/borrowList.txt is not Open\n";
		exit(1);
	} else {
		while (getline(borrowFile, info)) {
			string studentID = info.substr(0, info.find('_')); // �л����̵�
			borrowList.push_back(new Student(studentID)); // �л����� -> �����ڸ���Ʈ��
		}
	}
	while (borrowFile.is_open()) borrowFile.close();

	blackFile.open("datafile/User/forAdmin/blackList.txt");
	if (!blackFile.is_open()) {
		cerr << "datafile/User/forAdmin/borrowList.txt is not Open\n";
		exit(1);
	} else {
		while (getline(blackFile, info)) {
			string studentID = info.substr(0, info.find('_'));
			blackList.push_back(new Student(studentID)); // �л����� -> �� ����Ʈ��
		}
	}
	while (blackFile.is_open()) blackFile.close();

	for (auto& file : filesystem::directory_iterator(filesystem::current_path().string() + "\\datafile\\bookDB\\")) {
		string path = file.path().string();

		stringstream ss(path);
		vector<string> last_path;
		string split;
		while (getline(ss, split, '\\')) {
			last_path.push_back(split);
		}

		split = last_path[last_path.size() - 1]; // ���丮 "å.txt"
		last_path.clear();

		string na = split.substr(0, split.find('-'));
		string au = split.substr(split.find('-') + 1, split.find(".txt") - (split.find('-') + 1));

		booklist.push_back(new Book(na, au));
	}
	
	// ��ü�� ��ܿ��� ��ü�� ��� ����
	for (Student* bmem: borrowList){
		if (getDiff_date(bmem->getDueDate(), getCurrent_date()) > 0){ // ���� : stoi(bmem->getDueDate()) - stoi(getCurrent_date()) < 0
			overdueList.push_back(bmem);
			bmem->setIsOverdue(true);
		}
	}
}

Admin::~Admin()
{
	for (size_t i = 0; i < borrowList.size(); i++) {
		delete borrowList.at(i);
		borrowList.at(i) = nullptr;
	}
	borrowList.clear();
	for (size_t i = 0; i < borrowList.size(); i++) {
		delete overdueList.at(i);
		overdueList.at(i) = nullptr;
	}
	overdueList.clear();
	for (size_t i = 0; i < borrowList.size(); i++) {
		delete blackList.at(i);
		blackList.at(i) = nullptr;
	}
	blackList.clear();
}

void Admin::menu()
{
	while (true) {
		int num;
		cout << "<������ ���>\n" << endl;
		cout << "1. ���� �߰�" << endl;
		cout << "2. ���� ����" << endl;
		cout << "3. ȸ�� ����͸�" << endl;
		cout << "4. �α׾ƿ�\n" << endl;
		num = input("\n�޴� ���� : ", 1, 4);
		setCurrent_menu(num);
		switch (num) {
		case 1:
			addBookMenu();
			break;
		case 2:
			deleteBookMenu();
			break;
		case 3:
			monitoring();
			break;
		case 4:	
			string path = "datafile/User/forAdmin/blackList.txt";
			ofstream file(path, ios::out);
			if (!file.is_open()) {
				cerr << "blackList file is not open for change" << endl;
				exit(1);
			}

			for (size_t i = 0; i < blackList.size(); i++) {
				file << blackList[i]->getId() << "_" << blackList[i]->getName() << "_" << blackList[i]->getS_id() << endl;
			}

			while (file.is_open()) file.close();

			return;
		}
	}
}

//�ϼ�
void Admin::addBookMenu() // �����߰�
{
	while (true) {
		cout << "<���� �߰�>\n ������/���ڸ�/����/���ǻ�/����⵵\n\n";
		cout << "1. ���� ���� '/'�����ڷ� �յ� ���� ���� �����Ͽ� �Է��� �ּ���.\n";
		cout << "2. ���ڰ� ���� �� ĭ�� ����ּ���. ex) ������/���ڸ�//���ǻ�/����⵵\n";
		cout << "3. \":q\"�� �Է� �� ������ ����� ���� �޴��� ���ϴ�.\n\n";
		cout << "-----------------------------------------------------------------------\n";
		cout << "���� ���� : ";

		string inp_s;
		cin >> inp_s;
		if (inp_s == ":q") {
			return;
		}

		vector<string> a; // �Էµ� ��������

		//�Է� ������ ���� ��Ģ Ȯ��
		size_t prev = 0, cur;
		cur = inp_s.find('/'); // ������: '/'
		while (cur != string::npos)
		{
			string sub_str = inp_s.substr(prev, cur - prev); // ���ڿ� split
			a.push_back(sub_str);
			prev = cur + 1;
			cur = inp_s.find('/', prev);
		}
		a.push_back(inp_s.substr(prev, cur - prev));// ������ split

		//���� ��Ģ �˻�
		if (!check_book(a[0])) {
			cout << "å������ ���� ���Ŀ� ���� �ʽ��ϴ�";
			continue;
		}
		if (!check_author(a[1])) {
			cout << "���ڸ��� ���� ���Ŀ� ���� �ʽ��ϴ�";
			continue;
		}
		if (!check_translator(a[2])) {
			cout << "���ڰ� ���� ���Ŀ� ���� �ʽ��ϴ�";
			continue;
		}
		if (!check_publisher(a[3])) {
			cout << "���ǻ簡 ���� ���Ŀ� ���� �ʽ��ϴ�";
			continue;
		}
		if (!check_year(a[4])) {
			cout << "����⵵�� ���� ���Ŀ� ���� �ʽ��ϴ�";
			continue;
		}

		// �Է��� ������ �̹� �����ϴ��� Ȯ��
		string book_file_name = "datafile/bookDB/" + a[0] + "-" + a[1] + ".txt";
		if (_access(book_file_name.c_str(), 0) == 0) { // ������ �̹� �����ϴ� ���
			cout << "�̹� �ش� ������ �����մϴ�.\n";
			continue;
		}

		ofstream new_book_file(book_file_name); // ���� ���� ����
		//������ �ش� ������ ���������Ͽ� �߰�
		string write_new_book_file;
		if (a[2] == "") { // ���ڰ� ���ٸ�
			write_new_book_file = "����_" + a[3] + "_" + a[4];
		} else { // ���ڰ� �ִٸ�
			write_new_book_file = a[2] + "_" + a[3] + "_" + a[4];
		}
		new_book_file << write_new_book_file << endl;
		new_book_file << "�����ڸ��\n�����ڸ��\n";
		new_book_file.close();
		
		booklist.push_back(new Book(a[0], a[1])); // å ����Ʈ�� �߰��ϰ� ����
		a.clear();
	}
}

//�ϼ�
void Admin::deleteBookMenu() // ���� ���� 
{
	int n;
	int i = 0;
	bool flag = false;
	while(true){
		cout << "<���� ����>\n";
		cout << "1. ���������� ����\n";
		cout << "2. ���ڸ����� ����\n";
		cout << "3. ���ư���\n";
		n = input("\n���� : ", 1, 3);

		string a_name; // ���ڸ�
		string b_name; // ������

		switch (n) {
		case 1:
			while(true) {
				cout << "�������� �Է��ϼ���(�ڷ� ������ \":q\"�� �Է��ϼ���) :";
				cin >> b_name;
				i = 0;
				if (b_name==":q")
					break;
				if (!check_book(b_name)) {
					cout << "�������� ���� ���Ŀ� ���� �ʽ��ϴ�";
					continue;
				}
				vector<Book*> a;
				flag = false;
				//��ġ�ϴ� å ��� ����
				for (Book* book : booklist) {
					if	(book->getName() == b_name) {
						a.push_back(book);
						flag = true;
					}

				}
				//å�� �ִ°�
				if (flag) {
					string inp;
					int c;
					while(true) {
						i = 0;
						cout<<"������ : "<< b_name <<endl;
						cout<<"    [���ڸ�]\t[����]\t[���ǻ�]\t[����⵵]"<<endl;
						for (Book* book : a){
							i++;
							cout<<i<<". "<<book->getAuthor()<<"\t"<<book->getTranslator()<<"\t"<<book->getPublisher()<<"\t"<<book->getPublishYear()<<endl;
						}
						cout<<"��ȣ �Է�(�ڷ� ������ \":q\"�� ��������): ";
						cin.ignore();
						getline(cin, inp);
						if(inp==":q")
							break;
						// ���ڰ� �ƴϸ�
						bool isdigit_num = true;
						for (size_t i = 0; i < inp.size(); i++) {
							if (isdigit(inp[i]) == 0) {
								cout << "���ڰ� �ƴմϴ�" << endl;
								isdigit_num = false;
								break;
							}
						}	
						if (isdigit_num == false) continue;
						
						c = stoi(inp);
						
						if (c<1 || c>a.size()){
							cout << "�ùٸ� ������ �ƴմϴ�." << endl;
							continue;
						}
						//�������� �̹� �������� -> �ٽ� �Է¹޾ƿ�!
						if (a[c-1]->getBorrowTF()){
							cout<<"���� �ݳ����� ���� å�Դϴ�"<<endl;
						}
						else{
							//���� ����
							booklist.erase(booklist.begin() + c-1);
							//���� ���� ����
							string str = "datafile/bookDB/" + a[c-1]->getName() + "-" + a[c-1]->getAuthor() + ".txt";
							remove(str.c_str());
							cout<<"���� �Ϸ�!"<<endl;
							return;
						}
					}
					
					break;
				}
				else
					cout<<"�ش� ������ �������� �ʽ��ϴ�."<<endl;
			}
			break;
		case 2:
			while(true) {
				cout << "���ڸ��� �Է��ϼ���(�ڷ� ������ \":q\"�� �Է��ϼ���) :";
				cin >> a_name;
				i = 0;
				if (a_name==":q")
					break;
					
				if (!check_author(a_name)) {
					cout << "���ڸ��� ���� ���Ŀ� ���� �ʽ��ϴ�";
					continue;
				}
				vector<Book*> a;
				//��ġ�ϴ� å ��� ����
				for (Book* book : booklist) {
					if	(book->getAuthor() == a_name) {
						a.push_back(book);
						flag = true;
					}
				}
				//å�� �ִ°�
				if (flag) {
					string inp;
					int c;
					while(true) {
						i = 0;
						cout<<"���ڸ� : "<< a_name <<endl;
						cout<<"    [������]	  [����]	    [���ǻ�]     [����⵵]";
						for (Book* book : a) {
							i++;
							cout<<i<<". "<<book->getName()<<" "<<book->getTranslator()<<" "<<book->getPublisher()<<" "<<book->getPublishYear()<<endl;
						}
						getline(cin, inp);
						if(inp==":q")
							break;
						// ���ڰ� �ƴϸ�
						bool isdigit_num = true;
						for (size_t i = 0; i < inp.size(); i++) {
							if (isdigit(inp[i]) == 0) {
								cout << "���ڰ� �ƴմϴ�" << endl;
								isdigit_num = false;
								break;
							}
						}	
						if (isdigit_num == false) continue;
						c=stoi(inp);
						if (c < 1 || c > a.size()) {
							cout << "�ùٸ� ������ �ƴմϴ�." << endl;
							continue;
						}
						//�������� �̹� ��������
						if (a[c-1]->getBorrowTF()) {
							cout<<"���� �ݳ����� ���� å�Դϴ�"<<endl;
						}
						else{
							//���ͼ���
							booklist.erase(booklist.begin() + c-1);
							//���� ���� ����
							string str = "datafile/bookDB/" + a[c-1]->getName() + "-" + a[c-1]->getAuthor() + ".txt";
							remove(str.c_str());
							cout<<"���� �Ϸ�!"<<endl;
							return;
						}
					}	
					break;
				}
				else
					cout<<"�ش� ������ �������� �ʽ��ϴ�."<<endl;
			}
			break;
		case 3:
			return;
		default:
			cout<<"1~3������ ������ �Է��� �ּ���"<<endl;
		}
	}
}


//�ϼ�
void Admin::monitoring() // ȸ�� ����͸�
{
	int n;
	string cnum;
	while(true) {
		cout << "<ȸ�� ����͸�>\n";
		cout << "1. ��ü�� ���.\n";
		cout << "2. ������ ���\n";
		cout << "3. ������Ʈ\n";
		cout << "4. ���ư���\n";

		n = input("\n���� : ", 1, 4);

		int i = 0, c;

		switch(n) {
		case 1:
			cout << "<��ü�� ���>\n";
			cout<< " [�й�] [�̸�] [�������� ����] [������] [�ݳ���] [��ü�ϼ�] " <<endl;
			for(Student* omem : overdueList) {//��ü�ϼ� ����
				i++;
				cout<< i <<". "<< omem->getS_id()<< " " << omem->getName() <<" "<< omem->getBorrowDate() <<" "<< omem->getDueDate()<<" "<< getDiff_date(omem->getDueDate(), getCurrent_date())<<endl;
			}
			
			while(cnum != ":q") {
				cout << "������Ʈ�� �߰��� ȸ�� ��ȣ �Է� (�ڷ� ������ \":q\"�� �Է��ϼ���)\n";
				for (size_t i = 0; i < 3; i++) {
					cout << "." << endl;
				}
				while (true) {
					cout << ">> ";
					//cin >> cnum;
					getline(cin, cnum);
					if (cnum == ":q") break;
					// ���ڰ� �ƴϸ�
					bool isdigit_num = true;
					for (size_t i = 0; i < cnum.size(); i++) {
						if (isdigit(cnum[i]) == 0) {
							cout << "���ڰ� �ƴմϴ�." << endl;
							isdigit_num = false;
							break;
						}
					}
					if (isdigit_num == false) continue;

					// ���ڸ�
					c = stoi(cnum); // ���ڷ� ����
					if (c >= 1 && c <= overdueList.size()) { // ���� �ȿ� �ִٸ�
						break;
					}
					else {
						cout << "�ùٸ� ������ �ƴմϴ�." << endl;
					}
				}

				if (cnum == ":q") break;
				c = stoi(cnum);
				//overdueList[c-1]������Ʈ�� �߰�
				bool isinBlack = false;
				for(Student* bmem : blackList) {
					if (bmem->getS_id() == overdueList[c - 1]->getS_id()){
						isinBlack = true;
						break;
					}
				}
				if (isinBlack) {
					cout<<"�̹� ������Ʈ�� �ִ� ����Դϴ�."<<endl;
				}else {
					blackList.push_back(overdueList[c - 1]);

					overdueList[c - 1]->setIsOverdue(false);
					overdueList[c - 1]->setIsBlacklist(true);
					// å �ڵ� �ݳ�
					overdueList[c - 1]->returnBook();
					cout<<"������Ʈ �߰� �Ϸ�!"<<endl;
					return;
				}

			}
			cnum = "";
			break;
		case 2:
			cout << "<������ ���>\n";
			//borrowlist����			
    		for (int ii = 0; ii < borrowList.size(); ii++) {
				for (int jj = 0; jj < borrowList.size() - ii - 1; jj++) {
					if (borrowList[jj] > borrowList[jj + 1]) {
						Student* temp = borrowList[jj];
						borrowList[jj] = borrowList[jj + 1];	
						borrowList[jj + 1] = borrowList[jj];
					}
				}
			}
			while(cnum!=":q"){
				cout << "[�й�] [�̸�] [�������� ����] [������] [�ݳ�������]" << endl;
				for (Student* bmem : borrowList) {
					i++;
					cout<< i<<". "<<bmem->getS_id()<< " " << bmem->getName() << " " <<bmem->getBookName()<<" "<< bmem->getBorrowDate()<<" "<<bmem->getDueDate()<<endl;
				}

				cout << "(�ڷ� ������ \":q\"�� �Է��ϼ���)\n";
				for (size_t i = 0; i < 3; i++) {
					cout << "." << endl;
				}
				cout << ">> ";
				cin >> cnum;
			}
			cnum = "";
			break;
		case 3:
			cout << "<������Ʈ>\n";
			while(true){
				cout<<" [�й�] [�̸�]"<<endl;

				for (Student* blackmem : blackList) {
					i++;
					cout<<i<<". "<<blackmem->getS_id()<<" "<<blackmem->getName()<<" "<< endl;
				}
				cout << "������Ʈ���� ������ ȸ�� ��ȣ �Է� (�ڷ� ������ \":q\"�� �Է��ϼ���)\n";
				for (size_t i = 0; i < 3; i++) {
					cout << "." << endl;
				}
				while (true) {
					cout << ">> ";
					getline(cin, cnum);
					if (cnum == ":q") {
						break;
					}
					// ���ڰ� �ƴϸ�
					bool isdigit_num = true;
					for (size_t i = 0; i < cnum.size(); i++) {
						if (isdigit(cnum[i]) == 0) {
							cout << "���ڰ� �ƴմϴ�" << endl;
							isdigit_num = false;
							break;
						}
					}
					if (isdigit_num == false) continue;

					// ���ڸ�
					c = stoi(cnum); // ���ڷ� ����
					if (c >= 1 && c <= blackList.size()) { // ���� �ȿ� �ִٸ�
						break;
					}
					else {
						cout << "�ùٸ� ������ �ƴմϴ�." << endl;
					}
				}
				if(cnum==":q")
					break;
				else {
					//������Ʈ���� blackmem[c-1]����
					int c = stoi(cnum);
					blackList.erase(blackList.begin() + c-1);

					//���Ͽ����� ����(blackmem[c-1].getName().txt������ isBlackList����
					blackList[c - 1]->setIsOverdue(false);
					blackList[c - 1]->setIsBlacklist(false);

					//���Ͽ��� �߰�(blackList[c-1].getName().txt)
					string path = "datafile/User/" + blackList[c - 1]->getId() + ".txt";

					ofstream file(path, ios::out);
					if (!file.is_open()) {
						cerr << path + "is not open for eliminate blackList" << endl;
						exit(1);
					}
					file << blackList[c - 1]->getPassword() << "_" << blackList[c - 1]->getName() << "_" << blackList[c - 1]->getS_id() << endl;
					file << boolalpha << blackList[c - 1]->getIsOverdue() << endl;
					file << boolalpha << blackList[c - 1]->getIsBlacklist() << endl;
					file << endl;
					file << "���⵵������" << endl;
					file << "���൵������" << endl;
					for (size_t i = 0; i < blackList[c - 1]->getReserveBookList().size(); i++) {
						Book* book = blackList[c - 1]->getReserveBookList().at(i);
						file << book->getName() << "_" << book->getAuthor() << "_" << book->getTranslator() << "_" << book->getPublisher() << "_" << book->getPublishYear() << endl;
					}
					
					file.close();
				}
			}
			cnum = "";
			break;
		case 4:
			return;
		default:
			cout << "1~4������ ������ �Է��� �ּ���" << endl;
		}
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