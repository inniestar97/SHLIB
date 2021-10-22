#include "Admin.h"
#include "grammarCheck.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

Admin::Admin()
	:current_menu(0)
{
	
}

Admin::~Admin()
{	
}

//�ϼ�
void Admin::menu()
{
	while (true) {
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
			addBookMenu();
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

	while (true) { 
		cout << "<���� �߰�>\n ������/���ڸ�/����/���ǻ�/����⵵\n\n";
		cout << "1. ���� ���� '/'�����ڷ� �յ� ���� ���� �����Ͽ� �Է��� �ּ���.\n";
		cout << "2. ���ڰ� ���� �� ĭ�� ����ּ���. ex) ������/���ڸ�//���ǻ�/����⵵\n";
		cout << "3. ':q'�� �Է� �� ������ ����� ���� �޴��� ���ϴ�.\n\n";
		cout << "-----------------------------------------------------------------------\n";
		cout << "���� ���� : ";

		string inp_s;
		cin >> inp_s;
		if (inp_s == ":q") { 
			return;
		}
		vector<string> a;

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
		
		if (!check_book(a[0])){
			cout << "å������ ���� ���Ŀ� ���� �ʽ��ϴ�";
			continue;
		}
		if(!check_author(a[1])){
			cout << "���ڸ��� ���� ���Ŀ� ���� �ʽ��ϴ�";
			continue;
		}
		if(!check_translator(a[2])){
			cout << "���ڰ� ���� ���Ŀ� ���� �ʽ��ϴ�";
			continue;
		}
		if(!check_publisher(a[3])) {
			cout << "���ǻ簡 ���� ���Ŀ� ���� �ʽ��ϴ�";
			continue;
		}
		if(!check_year(a[4])){
			cout << "����⵵�� ���� ���Ŀ� ���� �ʽ��ϴ�";
			continue;
		}
		break;
	}


	// �Է��� ������ �̹� �����ϴ��� Ȯ��
	
	cout << "�̹� �ش� ������ �����մϴ�.\n";
	

	//������ �ش� ������ ���������Ͽ� �߰�
	
	cout<<"�߰� �Ϸ�"<<endl;

	return;
}

//��
void Admin::deleteBookMenu() // ���� ����
{
	int n;
	while(true){
		cout << "<���� ����>\n";
		cout << "1. ���������� ����\n";
		cout << "2. ���ڸ����� ����\n";
		cout << "3. ���ư���\n\n";
		cout << "���� : ";
		cin >> n;
		
		string a_name;
		string b_name;

		switch(n) {
		case 1:
			cout << "�������� �Է��ϼ��� : ";
			cin >> b_name;

			// �ִ°�->����

			// ���°�-> ���� ��� -> ��
			
			break;
		case 2:
			cout << "���ڸ��� �Է��ϼ��� : ";
			cin >> a_name;

			//�ִ°� -> ����

			//���°� -> ���� ��� ��

			break;
		case 3:
			return;
		default:
			cout<<"1~3������ ������ �Է��� �ּ���"<<endl;
		}
	}
}

// ��
void Admin::monitoring() // ȸ�� ����͸�
{
	int n;
	string cnum;
	while(true){
		cout << "<ȸ�� ����͸�>\n";
		cout << "1. ��ü�� ���.\n";
		cout << "2. ������ ���\n";
		cout << "3. ������Ʈ\n";
		cout << "4. ���ư���\n";
		cout << "���� : ";

		cin >> n;
		int i = 0;
		
		switch(n) {
		case 1:
			cout << "<��ü�� ���>\n";
			for(auto omem : overdueList) {
				i++;
				cout<< i<<". "<<omem.getName()<<endl;
			}
			
			while(cnum != ":q") {
				cout << "������Ʈ�� �߰��� ȸ�� ��ȣ �Է� (�ڷ� ������ ':q'�� �Է��ϼ���)\n";
				cout << ">> ";
				cin >> cnum;	
				
				//overdueList[i-1]������Ʈ�� �߰�  -> �̹� ������Ʈ�� �����ϸ�? - ��ȹ���� ����
				blackList.push_back(overdueList[i-1]);
				
				//���Ͽ����� ����(overdueList[i-1].getName().txt, admin.txt ���)

			}
			break;
		case 2:
			cout << "<������ ���>\n";

			while(cnum!=":q"){
				cout<< "[�й�] [�̸�] [�������� ����] [������] [�ݳ�������]"<<endl;
				vector<>
				for(auto bmem : borrowList) {
					// �������� ������ ��������� ? - ��ȹ���� ���� - ���ĵ� �ؾ��� �׳� ���� ��
					
					for(int k=0<k)
					cout<< bmem.getsid() <<" "<<bmem.getName() <<" "<< endl;
				}

				cout << "(�ڷ� ������ ':q'�� �Է��ϼ���)\n";
				cout << ">> ";
				cin >> cnum;
			}
			break;
		case 3:
			cout << "<������Ʈ>\n";
			while(true){
				cout<<" [�й�] [�̸�] [������Ʈ ������]"<<endl;//������Ʈ �������� ���� �𸣰ٳ׿�.

				for(auto blackmem : blackList) {
					//��� ���� ���ľ��� - ������Ʈ ������?
					cout<< blackmem.getsid()<<" "<<blackmem.getName()<<" "<< endl;
				}
				cout << "������Ʈ���� ������ ȸ�� ��ȣ �Է� (�ڷ� ������ ':q'�� �Է��ϼ���)\n";
				cout << ">> ";
				cin >> cnum;
				if(cnum==":q")
					break;
				else{
					//������Ʈ���� blackmem[i-1]����
					blackList.erase(blackList + i-1);
					
					//���Ͽ����� ����(blackmem[i-1].getName().txt, admin.txt���)

				}
			}
			break;
		case 4:
			return;
		default:
			cout<<"1~4������ ������ �Է��� �ּ���"<<endl;
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