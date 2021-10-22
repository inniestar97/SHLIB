#include "Student.h"
#include "grammarCheck.h"
#include "Book.h"
#include <iostream>
#include <string>
#include <fstream>

#define BASKETMAX 10
#define BORROWMAX 10

Student::Student(string id)
	:id(id), current_menu(0)
{
	ifstream std_file;

	while (!std_file.is_open()) { // �л� ���� open
		std_file.open("datafile/User/" + id + ".txt");
	}

	string std_info;
	getline(std_file, std_info);

	std_info = std_info.substr(std_info.find('_') + 1, string::npos);
}

Student::~Student()
{
	// �����Ҵ� �� ��� �κ� �޸� ����
	for (size_t i = 0; i < bookList.size(); i++) {
		delete bookList.at(i);
		bookList.at(i) = nullptr;
	}
	for (size_t i = 0; i < searchResult.size(); i++) {
		delete searchResult.at(i);
		searchResult.at(i) = nullptr;
	}
	for (size_t i = 0; i  < borrowBookList.size(); i++) {
		delete borrowBookList.at(i).book;
		borrowBookList.at(i).book = nullptr;
	}
	for (size_t i = 0; i < reserveBookList.size(); i++) {
		delete reserveBookList.at(i);
		reserveBookList.at(i) = nullptr;
	}
	for (size_t i = 0; i < bookBasketList.size(); i++) {
		delete bookBasketList.at(i);
		bookBasketList.at(i) = nullptr;
	}
}

void Student::menu() // ����� ��� �޴�
{
	int basketListNum;
	cout << "1. �ڷ� �˻�\n2. ��ٱ���\n3. ����������\n4. �α׾ƿ�\n";
	cin >> basketListNum;
	// basketListNum�� ���� �޴� ȣ��
	setCurrent_menu(basketListNum);
}

void Student::initBookList() {
	// data file �о�ͼ� booklist�� ����
}

void Student::searchBookMenu() // �ڷ�˻� - �����
{
	searchResult.clear(); // ���� �ʱ�ȭ
	int basketListNum;
	cout << "1. ���������� �˻�\n2. ���ڸ����� �˻�\n3. ���θ޴��� ���ư���\n";
	cin >> basketListNum;

	switch  basketListNum) {
	case 1:
	{
		string bookName;
		cout << "������ (�ڷ� ������ :q �� �Է��ϼ���) >> ";
		cin >> bookName;
		if (bookName == ":q") return;

		for (auto book : bookList) {
			// ���� �ӿ� keyword �ִ��� Ȯ��
			if (book->getName().find(bookName) != string::npos) { // �˻� ����
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
			if (book->getAuthor().find(bookauthor) != string::npos) { // �˻� ����
				searchResult.push_back(book);
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
		bookListPrint(5, true, true, true, true);
		// cout << "=======================" << endl << "1. ��ٱ��� ���\n2.���� ������ �̵��ϱ�\n3. ���� ������ �̵��ϱ�\n4. ���ư���" << endl << "=======================" << endl;
		// ���� ������, ���� ������ ������ ����Ʈ �Լ�����.. ���� ������ ��
		cout << "=======================" << endl << "1. ��ٱ��� ���\n2. ���ư���" << endl << "=======================" << endl;
		cout << "�޴��� �����ϼ���: " << endl;

		int option;
		cin >> option;

		if (option == 1) {
			cout << "��ٱ��Ͽ� ���� å ��ȣ�� �����ϼ���: ";
			int boo basketListNum;
			bool isExistBasket = false;
			cin >> boo basketListNum;

			// ��ٱ��Ͽ� ������ ��� ����
			// #define BASKETMAX 10 �߰� ���� -- ���� ���� (�����)
			for (auto book : bookBasketList) {
				if (book == searchResult[boo basketListNum - 1]) {
					cout << "��ٱ��Ͽ� �̹� ���� å�Դϴ�. �ٽ� �������ּ���." << endl;
					isExistBasket = true;
					break;
				}
			}

			if (!isExistBasket) {
				bookBasketList.push_back(searchResult[boo basketListNum - 1]);
				cout << "[" << searchResult[boo basketListNum - 1]->getName() << "]�� ��ٱ��Ͽ� ��ҽ��ϴ�." << endl;
			}
		}
		else
			return;
	}
}

void Student::bookBasketMenu()// ��ٱ��� �޴� - ������
{
	int basketListNum;
	while (1) {
		cout << "\n��ٱ���\n";
		bookListPrint(4, true, true, true, true);
		cout << "\n-----------------------------------------\n";
		cout << "\t1. �ϰ� ����\n\t2. ���� ���� ����\n\t3. ���� ���� ����\n\t4. ���ư���";
		cout << "\n-----------------------------------------\n";
		cout << "\n�޴�����:";
		cin >> basketListNum;

		if (!cin) { // cin ����ó��
			cout << "1~4�� ������ �Է����ּ���.\n";
			cin.ignore(INT_MAX, '\n');
			cin.clear();

			rewind(stdin);
		}
		else if  basketListNum > 4 || basketListNum < 1) {
			cout << "1~4�� ������ �Է����ּ���.\n";
		}
		else break;
	}

	switch  basketListNum) {
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

void Student::borrowBook() // ������ ��ٱ��� -> �ϰ����� (������ ���� �ٷ�� �ʿ�)
{
	size_t basketListNum = bookBasketList.size(); // ����� ����: int -> size_t  ����
	// ���� �Ұ��� ���
	// 1. �̹� ����� ���
	// 2. ù��° ������ != ��
	// 3. �ܿ� ����Ƚ�� < ��ٱ��� list sizeg
	vector<int> cant;
	bool flag3 = false;
	for (int i = 0; i < basketListNum; i++) { 
		if (!bookBasketList.at(i)->getBorrowTF()) { // 1. �̹� �����.
			cant.emplace_back(i);
		}else if(bookBasketList.at(i)->getReservStudents.size() > 0){
			if(bookBasketList.at(i)->getReservStudents.at(0) != this){ // 2. ù��° ������ != ��
				cant.emplace_back(i);
			}
		}		
	}

	if(BORROWMAX - borrowBookList.size() < basketListNum){ // 3. �ܿ� ����Ƚ�� < ��ٱ��� list size
		flag3 = true;
	}
	if(flag3){ 
		cout<<" -- ���� ���� Ƚ���� ��ٱ��Ϻ��� "< basketListNum-(BORROWMAX - borrowBookList.size())<<"�� �����ϴ�. ��ٱ��ϸ� �����ּ���. -- \n";
	}
	if (!cant.empty()) { // ���� �Ұ�
		cout << " ------\t ����Ұ� ����Ʈ\t ------\n";
		cout << "([��ٱ��Ϲ�ȣ. ����, ����])\n";
		for (int i = 0; i < cant.size(); i++) {
			cout << to_string(cant.at(i)) << ".\t���� : " << bookBasketList.at(cant.at(i))->getName() 
			<< " , ���� : " << bookBasketList.at(cant.at(i))->getAuthor() << "\n";
		}
		return;
	}
	else { // ���Ⱑ���ϸ� ����
		for (int i = 0; i < basketListNum; i++) {
			bookBasketList.at(i)->addBorrow(this);
			//borrowBookList.emplace_back(bookBasketList[i], "20211015"); // �����: �������� ��� �ּ�ó����
		}
	}
}

void Student::deleteBook() // ������ ��ٱ��� -> ���� ���� ���� (������ ���� �ٷ�� �ʿ�)
{
}
void Student::reserveBook() // ��ٱ��� -> ���� ���� ���� (������ ���� �ٷ�� �ʿ�)
{
}

void Student::myPageMenu()// ���������� �޴� //������
{
	int basketListNum; //�޴� ����
	int u1; //1. ������Ȳ������ ����� ���� (�ݳ�/����)
	int boo basketListNum; //���� ��ȣ ����

	while (1) {
		cout << "------------------------------------------------\n";
		cout << "1. ���� ��Ȳ\n2. ���� ��Ȳ\n3. ���ư���\n";
		cout << "------------------------------------------------\n";
		cout << "�޴�����: ";
		cin >> basketListNum;
		switch  basketListNum) {
		case 1:

			while (1) {
				//�� ����Ǽ��� ���⵵�� ��� ���
				bookListPrint(2, true, true, true, true);
				
				//�ݳ��� ����
				cout << "------------------------------------------------\n";
				cout << "1. �ݳ��ϱ�\n2. �����ϱ�\n";
				cout << "------------------------------------------------\n";
				cout << "�޴�����: ";
				cin >> u1;
				if (u1 == 1) {
					cout << "���� ��ȣ�� �������ּ���: ";
					cin >> boo basketListNum;
					returnBook(boo basketListNum);
					break;
				}
				else if (u1 == 2) {
					cout << "���� ��ȣ�� �������ּ���: ";
					cin >> boo basketListNum;
					extendBook(boo basketListNum);
					break;
				}
				else {
					cout << "�޴��� �ٽ� �������ּ���.\n";
				}
			}
			break;
		case 2: //�̿ϼ�
			//�� ����Ǽ��� ���൵�� ��� ���
			bookListPrint(3, true, true, true, true);

			//���� ���
			cout << "���� ��ȣ�� �������ּ���: ";
			cin >> boo basketListNum;
			cancelReserveBook(boo basketListNum);
			break;
		case 3:
			quit();
			break;
		default:
			cout << "�޴��� �ٽ� �������ּ���.\n";
		}
	}
}

void Student::returnBook(int booknum) // ���������� -> å �ݳ� //������
{
	//vector<BorrowInfo> borrowBookList���� �ش� ���� ����
	vector<BorrowInfo> BI; // ����� ����: BorrowInfo* -> BorrowInfo
	BI = borrowBookList;

	BI.erase(BI.begin() + booknum);
	cout << "�ش� ������ �ݳ��� �Ϸ�Ǿ����ϴ�.\n";
}

void Student::extendBook(int booknum) // ���������� -> å ���� //������
{
	vector<BorrowInfo> BI;
	BI = borrowBookList;
	bool reserveNumFlag = false; //������ ���翩�� ���� (����� ����: ���������� �ӽ÷� false ó��)

	/* ����� ����: �������� ��� �ּ�ó��
	if (BI.at(booknum)->getReservStudent booknum() == 0)
		reserveNumFlag = false;
	else
		reserveNumFlag = true;
		*/

		//�̿ϼ�
	if (!getIsOverdue() && !reserveNumFlag)
		//���忡 ���� ���� ��� - ���� ������ �ؾ� �� - ��¥ �ٷ��� ��
		cout << "�ش� ���� ������ �Ϸ�Ǿ����ϴ�.\n";
	else if (getIsOverdue())
		//��ü�� ���
		cout << "�ش� ������ ��ü�� ������, ������ �Ұ����մϴ�.\n";
	else if (reserveNumFlag)
		//�����ڰ� �����ϴ� ���
		cout << "�ش� ������ �ٸ� ����ڰ� �̹� ������ ������, ������ �Ұ����մϴ�.\n";
}

void Student::cancelReserveBook(int booknum) // ���������� -> å ���� ��� //������
{
	//vector<Book> reserveBookList���� �ش� ���� ����
	vector<Book*> RL;
	RL = reserveBookList;
	RL.erase(RL.begin() + reserveNumFlag);
	cout << "�ش� ���� ������ ��ҵǾ����ϴ�.\n";
}

void Student::quit() //���ư���
{
}

// ������ �κ� �ٽ� ���� ��. 21:00
// Book��ü(1. bookList, 2. borrowBookList, 3. reserveBookList, 4.bookBasket, 5.serachResult �� basketListNum���� �Է�), ������, ���ڸ�, ���Ⱑ�ɿ���, �����ο���
void Student::bookListPrint(int lis basketListNum, bool nameTF, bool authorTF, bool borrowTF, bool reserv basketListNumTF) { //��������Ʈ ��� - ������(1,3,4,5), �����(2)
	// ������ : getReservStudent basketListNum() -> getReservStudents()���� �ٲ㼭 ��������� ��. (10.22 19:45)
	// ����� : �������� ��� �ּ�ó��
	 /*
	vector<Book*> List;
	vector<BorrowInfo>* BI = nullptr; // 2. borrowBookList ������ �����
	size_t listSize = 0;
	switch (lis basketListNum) {
	case 1: // ��������Ʈ
		List = bookList;
		listSize = List.size();
		break;
	case 2: // ������Ȳ
		BI = &borrowBookList;
		listSize = BI->size();
		break;
	case 3: // ������Ȳ
		List = reserveBookList;
		listSize = List.size();
		break;
	case 4: // ��ٱ���
		List = bookBasketList;
		listSize = List.size();
		break;
	case 5: //�˻����
		List = searchResult;
		listSize = List.size();
		break;
	}

	for (int i = -1; i < listSize; i++) { // index: -1�� ��ܹ� ���, 0���� å ���
		if (i == -1) {
			cout << "\n" << (nameTF ? "[������]" : "") << "\t" << (authorTF ? "[���ڸ�]" : "") << "\t" << "[����]\t[���ǻ�]";
			if (lis basketListNum == 2) {
				cout << "\t" << "[��ü����]" << "\t" << "[�ݳ���¥]" << "\t" << "[���尡�ɿ���]";
			}
			cout << "\t" << (borrowTF ? "[���Ⱑ�ɿ���]" : "") << "\t" << (reserv basketListNumTF ? "[�����ο���]" : "");
			cout << "\n-------------------------------------------\n";
		}
		else {
			if (lis basketListNum == 2) { //������Ȳ
				cout << "\n" << i + 1 << ".\t" << (nameTF ? BI->at(i).book->getName() : "") << "\t" << (authorTF ? BI->at(i).book->getAuthor() : "") << "\t" << BI->at(i).book->getTranslator() << "\t" << BI->at(i).book->getPublisher();

				// ---- ����� (�̿ϼ��ϴٰ� ����) ---- ȭ���� Student.cpp ����
				// cout<<"\t"<<"��ü����"<<"\t"<<returnDate<<"\t"<<"���尡�ɿ���"; //������Ȳ����

				// ---- �������

				if (borrowTF) { //���Ⱑ�ɿ���
					cout << "\t";

					if (BI->at(i).book->getBorrowTF()) {
						cout << "X";
					}
					else cout << "O";
				}
				if (reserv basketListNumTF) {//�����ο���
					cout << "\t" << BI->at(i).book->getReservStudent basketListNum();
				}
			}
			else {
				cout << "\n" << (nameTF ? List.at(i)->getName() : "") << "\t" << (authorTF ? List.at(i)->getAuthor() : "") << "\t" << List.at(i)->getTranslator() << "\t" << List.at(i)->getPublisher();
				if (borrowTF) { //���Ⱑ�ɿ���
					cout << "\t";
					if (List.at(i)->getBorrowTF()) {
						cout << "X";
					}
					else cout << "O";
				}
				cout << "\t" << (reserv basketListNumTF ? to_string(List.at(i)->getReservStudent basketListNum()) : "");
			}
		}
	}
	*/
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

bool Student::operator==(Student student) // ������
{
	if (this->sid == student.sid){
		return true;
	}
	else return false;
}