#include "Student.h"
#include "grammarCheck.h"
#include <iostream>
#include <string>

Student::Student()
{
}

Student::~Student()
{
}

void Student::menu() // ����� ��� �޴�
{
	int num;
	cout << "1. �ڷ� �˻�\n2. ��ٱ���\n3. ����������\n4. �α׾ƿ�\n";
	cin >> num;
	// num�� ���� �޴� ȣ��
	setCurrent_menu(num);
}

void Student::initBookList() { 
	// data file �о�ͼ� booklist�� ����
}

void Student::searchBookMenu() // �ڷ�˻� - �����
{
	searchResult.clear(); // ���� �ʱ�ȭ
	int num;
	cout << "1. ���������� �˻�\n2. ���ڸ����� �˻�\n3. ���θ޴��� ���ư���\n";
	cin >> num;

	switch (num) {
	case 1:
	{
		string bookName;
		cout << "������ (�ڷ� ������ :q �� �Է��ϼ���) >> ";
		cin >> bookName;
		if (bookName == ":q") return;

		for (auto book : bookList) {
			// ���� �ӿ� keyword �ִ��� Ȯ��
			if (book.getName().find(keyword) != string::npos) { // �˻� ����
				searchResult.push_back(book);
			}
		}
		break;
	}
	case 2:
	{
		string bookauthor;
		cout << "���ڸ� (�ڷ� ������ :q �� �Է��ϼ���) >> ";
		cin >> bookauthor;
		if (bookauthor == ":q") return;

		for (auto book : bookList) {
			// ���� �ӿ� keyword �ִ��� Ȯ��
			if (book.getAuthor().find(keyword) != string::npos) { // �˻� ����
				searchBooks.push_back(book);
			}
		}
		break;
	}
	case 3:
		return;
	default:
		return;
	}

	if (searchResult.size() == 0) {
		cout << "�˻� ����� �����ϴ�. �ƹ� Ű�� ������ ���� �޴��� ���ư��ϴ�." << endl;
		return;
	}

	while (true) {
		// �˻� å ���: 5��. ����, ����, ����, ���ǻ�, ����, ���Ⱑ�ɿ���, �����ο� ��
		bookListPrint(5, true, true, false, false, true, true);
		//cout << "=======================" << endl << "1. ��ٱ��� ���\n2.���� ������ �̵��ϱ�\n3. ���� ������ �̵��ϱ�\n4. ���ư���" << endl << "=======================" << endl;
		// ���� ������, ���� ������ ������ ����Ʈ �Լ�����.. ���� ������ ��
		cout << "=======================" << endl << "1. ��ٱ��� ���\n2. ���ư���" << endl << "=======================" << endl;
		cout << "�޴��� �����ϼ���: " << endl;
		
		int option;
		cin >> option;

		if (option == 1) {
			cout << "��ٱ��Ͽ� ���� å ��ȣ�� �����ϼ���: ";
			int bookNum;
			bool isExistBasket = false;
			cin >> bookNum;

			// ��ٱ��Ͽ� ������ ��� ����
			for (auto book : bookBasketList) {
				if (book == searchResult[bookNum-1]) {
					cout << "��ٱ��Ͽ� �̹� ���� å�Դϴ�. �ٽ� �������ּ���." << endl;
					isExistBasket = true;
					break;
				}
			}

			if (!isExistBasket) {
				bookBasketList.push_back(searchResult[bookNum - 1]);
				cout << "[" << searchResult[bookNum-1].getName() << "]�� ��ٱ��Ͽ� ��ҽ��ϴ�." << endl;
			}
		} else
			return;
	}
}

void Student::bookBasketMenu()// ��ٱ��� �޴� - ������
{
	int num;
	while(1){
		cout<<"\n��ٱ���\n";
		bookListPrint(4, true, true, true, true);
		cout<<"\n-----------------------------------------\n";
		cout << "\t1. �ϰ� ����\n\t2. ���� ���� ����\n\t3. ���� ���� ����\n\t4. ���ư���";
		cout<<"\n-----------------------------------------\n";
		cout << "\n�޴�����:";
		cin >> num;

		if(!cin){ // cin ����ó��
			cout<<"1~4�� ������ �Է����ּ���.\n";
			cin.ignore(INT_MAX, '\n'); 
			cin.clear();

			rewind(stdin);
		}else if(num>4 || num < 1){
			cout<<"1~4�� ������ �Է����ּ���.\n";
		}else break;
	}

	switch(num){
	case 1:
		borrowBook();
		break;
	case 2:
		deleteBook();
		break;
	case 3:
		reserveBook();
		break;
	case 4:
		quit();
		break;
	}

}


void Student::borrowBook() // ��ٱ��� -> �ϰ����� (������ ���� �ٷ�� �ʿ�)
{
	
}

void Student::deleteBook() // ��ٱ��� -> ���� ���� ���� (������ ���� �ٷ�� �ʿ�)
{

}
void Student::reserveBook() // ��ٱ��� -> ���� ���� ���� (������ ���� �ٷ�� �ʿ�)
{
}



void Student::myPageMenu()// ���������� �޴� //������
{
	int num; //�޴� ����
	int u1; //1. ������Ȳ������ ����� ���� (�ݳ�/����)
	int booknum; //���� ��ȣ ����
 
	while(1){
		cout << "------------------------------------------------\n";
		cout << "1. ���� ��Ȳ\n2. ���� ��Ȳ\n3. ���ư���\n";
		cout << "------------------------------------------------\n";
		cout << "�޴�����: ";
		cin >> num;
		switch (num) {
		case 1:
			//�� ����Ǽ��� ���⵵�� ��� ���
			bookListPrint(2, true, true, true, true);


			//�ݳ��� ����
			while (1) {
				cout << "------------------------------------------------\n";
				cout << "1. �ݳ��ϱ�\n2. �����ϱ�\n";
				cout << "------------------------------------------------\n";
				cout << "�޴�����: ";
				cin >> u1;
				if (u1 == 1) {
					cout << "���� ��ȣ�� �������ּ���: ";
					cin >> booknum;
					returnBook(booknum);
				}
				else if (u1 == 2) {
					cout << "���� ��ȣ�� �������ּ���: ";
					cin >> booknum;
					extendBook(booknum);
				}
				else {
					cout << "�޴��� �ٽ� �������ּ���.\n";
					break
				}
			}
			break;
		case 2: //�̿ϼ�
			//�� ����Ǽ��� ���൵�� ��� ���
			bookListPrint(3, true, true, true, true);

			//���� ���
			cout << "���� ��ȣ�� �������ּ���: ";
			cin >> booknum;
			cancelReserveBook(booknum);
			break;
		case 3:
			quit();
			break;
		default:
			cout << "�޴��� �ٽ� �������ּ���.\n";
			break;
		}
	}

}

void Student::returnBook(int booknum) // ���������� -> å �ݳ� //������
{
	//vector<BorrowInfo> borrowBookList���� �ش� ���� ����
	//borrowBookList.erase(borrowBookList.begin()+booknum);
	cout << "�ش� ������ �ݳ��� �Ϸ�Ǿ����ϴ�.\n";
}

void Student::extendBook(int booknum) // ���������� -> å ���� //������
{ //�̿ϼ�
	if (getIsOverdue()){
		//��ü�� ���
		cout << "�ش� ������ ��ü�� ������, ������ �Ұ����մϴ�.\n";
	}

	//�����ڰ� �����ϴ� ���
	cout << "�ش� ������ �ٸ� ����ڰ� �̹� ������ ������, ������ �Ұ����մϴ�.\n";

	if (!getIsOverdue()){
		//���忡 ���� ���� ��� 
		cout << "�ش� ���� ������ �Ϸ�Ǿ����ϴ�.\n";
	}
	

}

void Student::cancelReserveBook(int booknum) // ���������� -> å ���� ��� //������
{
	//vector<Book> reserveBookList���� �ش� ���� ����
	reserveBookList.erase(reserveBookList.begin()+booknum);
	cout << "�ش� ���� ������ ��ҵǾ����ϴ�.\n";
}

void Student::quit() //���ư���
{
}

// ������ �κ� ���� ��. 
// Book��ü(1. bookList, 2. borrowBookList, 3. reserveBookList, 4.bookBasket, 5.serachResult �� num���� �Է�), ������, ���ڸ�, ���Ⱑ�ɿ���, �����ο���
void Student::bookListPrint(int listNum, bool nameTF, bool authorTF, bool borrowTF, bool reserveNumTF){ //��������Ʈ ��� - ������(1,3,4,5), �����(2)

	vector<Book> *List;
	vector<BorrowInfo> *BI; // 2. borrowBookList ������ �����
	int listSize = 0; 
	switch(listNum){
		case 1: // ��������Ʈ
			List = &bookList;
			listSize = List->size();
			break;
		case 2: // ������Ȳ
			BI = &borrowBookList;
			listSize = BI->size();
			break;
		case 3: // ������Ȳ
			List = &reserveBookList;
			listSize = List->size();
			break;
		case 4: // ��ٱ���
			List = &bookBasketList;
			listSize = List->size();
			break;
		case 5: //�˻����
			List = & searchResult;
			listSize = List->size();
			break;
	}

	for (int i = -1; i < listSize; i++) { // index: -1�� ��ܹ� ���, 0���� å ���
		if (i == -1) {
			cout << "\n" << (nameTF ? "[������]" : "") << "\t" << (authorTF ? "[���ڸ�]" : "") << "\t" << "[����]\t[���ǻ�]";
			if (listNum == 2) {
				cout << "\t" << "[��ü����]" << "\t" << "[�ݳ���¥]" << "\t" << "[���尡�ɿ���]";
			}
			cout << "\t" << (borrowTF ? "[���Ⱑ�ɿ���]" : "") << "\t" << (reserveNumTF ? "[�����ο���]" : "");
		}
		else {
			if (listNum == 2) { //������Ȳ 

				cout << "\n" << i + 1 << ".\t" << (nameTF ? BI->at(i).book.getName() : "") << "\t" << (authorTF ? BI->at(i).book.getAuthor() : "") << "\t" << BI->at(i).book.getTranslator() << "\t" << BI->at(i).book.getPublisher();

				// ---- ����� (�̿ϼ��ϴٰ� ����) ---- ȭ���� Student.cpp ����
				// cout<<"\t"<</*��ü����*/<<"\t"<<returnDate<<"\t"<</*���尡�ɿ���*/; //������Ȳ����

				// ---- �������

				if (borrowTF) { //���Ⱑ�ɿ���
					cout << "\t";
					if (BI->at(i).book.getBorrowerTF()) {
						cout << "X";
					}
					else cout << "O";
				}
				if (reserveNumTF) {//�����ο���
					cout << "\t" << BI->at(i).book.getReservStudentsNum();
				}
			}
			else {
				cout << "\n" << (nameTF ? List->at(i).getName() : "") << "\t" << (authorTF ? List->at(i).getAuthor() : "") << "\t" << List->at(i).getTranslator() << "\t" << List->at(i).getPublisher();
				if (borrowTF) { //���Ⱑ�ɿ���
					cout << "\t";
					if (List->at(i).getBorrowerTF()) {
						cout << "X";
					}
					else cout << "O";
				}
				cout << "\t" << (reserveNumTF ? to_string(List->at(i).getReservStudentsNum()) : "");
			}
		}
	} 

}

}

void Student::setCurrent_menu(int menu)
{
	this->current_menu = menu;
}

void Student::setName(string name)
{
	this->name = name;
}

void Student::setSid(string sid)
{
	this->sid = sid;
}

void Student::setIsOverdue(bool check)
{
	this->isOverdue = check;
}

void Student::setIsBlacklist(bool check)
{
	this->isBlacklist = check;
}

int Student::getCurrent_menu() const
{
	return current_menu;
}

string Student::getName() const
{
	return name;
}

string Student::getsid() const
{
	return sid;
}

bool Student::getIsOverdue() const
{
	return isOverdue;
}

bool Student::getIsBlacklist() const
{
	return isBlacklist;
}
