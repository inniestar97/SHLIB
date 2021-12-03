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
	for (auto& file : filesystem::directory_iterator(filesystem::current_path().string() + "\\datafile\\User\\")) {
		string path = file.path().string();
		stringstream ss(path);
		vector<string> last_path;
		string split;
		while (getline(ss, split, '\\')) {
			last_path.push_back(split);
		}
		split = last_path[last_path.size() - 1]; // ���丮 "userId.txt"
		last_path.clear();

		if (split == "admin.txt")
			continue;

		split = split.substr(0, split.find(".txt")); // ���̵�
		Student* std = new Student(split);
		if (std->getBorrowListNum() > 0) {
			borrowList.push_back(std);
		} else {
			delete std;
			std = nullptr;
		}
	}

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
	
}

Admin::~Admin()
{
	for (size_t i = 0; i < borrowList.size(); i++) {
		delete borrowList.at(i);
		borrowList.at(i) = nullptr;
	}
	borrowList.clear();

	for (size_t i = 0; i < booklist.size(); i++) {
		delete booklist.at(i);
		booklist.at(i) = nullptr;
	}
	booklist.clear();
}

void Admin::menu()
{
	while (true) {
		int num;
		cout << "<������ ���>\n" << endl;
		cout << "1. ���� �߰�" << endl;
		cout << "2. ���� ����" << endl;
		cout << "3. ������ ���" << endl;
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
			cout<<"�α׾ƿ� �Ǿ����ϴ�."<<endl;
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
		getline(cin, inp_s);
		if (inp_s == ":q") {
			cin.ignore();
			return;
		}

		//inp_s /���� ����
		int slashNum=0;
		for (int i = 0; i < inp_s.length(); i++) //for������ ��ȯ�� size ������ŭ ���ڿ�
		{
			if (inp_s[i] == '/') 
				slashNum += 1;
		}
		if (slashNum != 4){
			cout << "�Է��� ���� ���Ŀ� ���� �ʽ��ϴ�"<<endl;
			continue;
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
			cout << "å������ ���� ���Ŀ� ���� �ʽ��ϴ�"<<endl;
			continue;
		}
		if (!check_author(a[1])) {
			cout << "���ڸ��� ���� ���Ŀ� ���� �ʽ��ϴ�"<<endl;
			continue;
		}
		if (!check_translator(a[2])) {
			cout << "���ڰ� ���� ���Ŀ� ���� �ʽ��ϴ�"<<endl;
			continue;
		}
		if (!check_publisher(a[3])) {
			cout << "���ǻ簡 ���� ���Ŀ� ���� �ʽ��ϴ�"<<endl;
			continue;
		}
		if (!check_year(a[4])) {
			cout << "����⵵�� ���� ���Ŀ� ���� �ʽ��ϴ�"<<endl;
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
		new_book_file << "�����ڸ��\n";
		for (size_t i = 1; i <= 3; i++) {
			new_book_file << i << "." << endl;

		}
		new_book_file << "�����ڸ��\n";
		for (size_t i = 1; i <= 3; i++) {
			new_book_file << i << "." << endl;

		}
		new_book_file.close();
		
		booklist.push_back(new Book(a[0], a[1])); // å ����Ʈ�� �߰��ϰ� ����
		cout<<"���� �߰� �Ϸ�"<<endl;
		a.clear();
		break;
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
				if (b_name==":q"){
					cin.ignore();
					break;
				}
				if (!check_book(b_name)) {
					cout << "�������� ���� ���Ŀ� ���� �ʽ��ϴ�"<<endl;
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
						if(inp==":q"){
	
							break;
						}
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
				if (a_name==":q"){
					cin.ignore();
					break;
				}
				if (!check_author(a_name)) {
					cout << "���ڸ��� ���� ���Ŀ� ���� �ʽ��ϴ�"<<endl;
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
						cout<<"    [������]\t[����]\t[���ǻ�]\t[����⵵]"<<endl;
						for (Book* book : a) {
							i++;
							cout<<i<<". "<<book->getName()<<" "<<book->getTranslator()<<" "<<book->getPublisher()<<" "<<book->getPublishYear()<<endl;
						}
						getline(cin, inp);
						if(inp==":q"){
							cin.ignore();
							break;
						}
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


//���� ���� student���� �ذ��ϸ�
void Admin::monitoring() // ������ ���
{
	string quit;
	
	//borrowlist����			
   	for (int i = 0; i < borrowList.size(); i++) {
		for (int j = 0; j < borrowList.size() - i - 1; j++) {
			if (borrowList[j] >= borrowList[j + 1]) {
				Student* temp = borrowList[j];
				borrowList[j] = borrowList[j + 1];	
				borrowList[j + 1] = borrowList[j];
			}
		}
	}
	cout << "<������ ���>\n";		
	cout << "[�й�] [�̸�]\n";
	cout<< "[�������� ����] [������] [�ݳ�������]\n";
	int i = 0;
	for (Student* bmem : borrowList) {
		i++;
		cout<< i <<". " << bmem->getS_id() << " " << bmem->getName() << "\n" ;
		
		for(int bi=0;bi<bmem->getBorrowListNum();bi++){
			cout << "\t\t\t" << bmem->getBookName(bi) <<" "<< bmem->getBorrowDate(bi) << " " << bmem->getDueDate(bi) << endl;
		}
	}
	cout << "(�ڷ� ������ \":q\"�� �Է��ϼ���)\n";
	while(quit !=":q") {
		cout << ">> ";
		cin >> quit;
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