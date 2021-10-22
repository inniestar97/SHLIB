#include "Student.h"
#include "grammarCheck.h"
#include "Book.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#define BASKETMAX 10
#define BORROWMAX 1

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
	this->name = std_info.substr(0, std_info.find('_')); // �л� �̸�
	std_info = std_info.substr(std_info.find('_') + 1, string::npos);
	this->s_id = std_info.substr(0, std_info.find('\n'));

	getline(std_file, std_info);
	std_info = std_info.substr(0, std_info.find('\n'));
	if (std_info == "false") isOverdue = false;
	else this->isOverdue = true;
	getline(std_file, std_info);
	std_info = std_info.substr(0, std_info.find('\n'));
	if (std_info == "false") isBlacklist = false;
	else this->isBlacklist = true;

	getline(std_file, std_info); // ���� \n
	getline(std_file, std_info); // std_info => '���⵵������\n"
	getline(std_file, std_info); // ������ å ������ std_info �� ����

	std_info = std_info.substr(0, std_info.find('\n')); // ������ å ����

	stringstream ss(std_info);
	string token;
	vector<string> word;
	while (getline(ss, token, '_')) {
		word.push_back(token);
	}
	this->borrow = new Book(word.at(0), word.at(2), word.at(3), word.at(4), word.at(5));

	getline(std_file, std_info);
	while (getline(std_file, std_info)) {
		std_info = std_info.substr(0, std_info.find('\n'));

		stringstream ss(std_info);
		string token;
		vector<string> word;
		while (getline(ss, token, '_')) {
			word.push_back(token);
		}
		this->reserveBookList.push_back(new Book(word.at(0), word.at(1), word.at(2), word.at(3), word.at(4)));
	}

	std_file.close();
}

Student::~Student()
{
	// �����Ҵ� �� ��� �κ� �޸� ����
	delete borrow;
	borrow = nullptr;

	for (size_t i = 0; i < searchResult.size(); i++) {
		delete searchResult.at(i);
			searchResult.at(i) = nullptr;
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
	// data file �о�ͼ� booklist�� ���� - �����ڿ��� ����� ���̶� �ʿ� �������մϴ�.
}

void Student::searchBookMenu() // �ڷ�˻� - �����
{
	searchResult.clear(); // ���� �ʱ�ȭ
	int basketListNum;
	cout << "1. ���������� �˻�\n2. ���ڸ����� �˻�\n3. ���θ޴��� ���ư���\n";
	cin >> basketListNum;

	switch (basketListNum) {
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
			int bookbasketListNum;
			bool isExistBasket = false;
			cin >> bookbasketListNum;

			// ��ٱ��Ͽ� ������ ��� ����
			// #define BASKETMAX 10 �߰� ���� -- ���� ���� (�����)
			for (auto book : bookBasketList) {
				if (book == searchResult[bookbasketListNum - 1]) {
					cout << "��ٱ��Ͽ� �̹� ���� å�Դϴ�. �ٽ� �������ּ���." << endl;
					isExistBasket = true;
					break;
				}
			}

			if (!isExistBasket) {
				bookBasketList.push_back(searchResult[bookbasketListNum - 1]);
				cout << "[" << searchResult[bookbasketListNum - 1]->getName() << "]�� ��ٱ��Ͽ� ��ҽ��ϴ�." << endl;
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
		cout << "\t1. ���� ����\n\t2. ���� ���� ����\n\t3. ���� ���� ����\n\t4. ���ư���"; // �ϴ� 1. �ϰ����� -> ���ô���� ���� (���� ���� 1���̸�)
		cout << "\n-----------------------------------------\n";
		cout << "\n�޴�����:";
		cin >> basketListNum;

		if (!cin) { // cin ����ó��
			cout << "1~4�� ������ �Է����ּ���.\n";
			cin.ignore(INT_MAX, '\n');
			cin.clear();

			rewind(stdin);
		}
		else if (basketListNum > 4 || basketListNum < 1) {
			cout << "1~4�� ������ �Է����ּ���.\n";
		}
		else break;
	}

	switch (basketListNum) {
	case 1:
		//borrowBook();   // ��� �ϰ�����! ���� ���� 10���� ��!
		sel_borrowBook(); // ���� ���� 1���̸� ���ô���!
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

// ��� �ϰ�����
void Student::borrowBook() // ��ٱ��� -> �ϰ����� (������ ���� �ٷ�� �ʿ�) - ������
{
	size_t basketListNum = bookBasketList.size(); // ����� ����: int -> size_t ����
	// size_t borrowbooknum = borrowBookList.size(); // ���� �Ǽ��� 1������ �ٿ��� �ּ�ó��

	/*  ----------------------------------
		���� �Ұ��� ���
		1. �̹� ����� ���
		2. ù��° ������ != ��
		3. �ܿ� ����Ƚ�� < ��ٱ��� list size 
	    -----------------------------------  */

	if(bookBasketList.empty()){
		cout << "------------------------------------------------\n";
		cout << "��ٱ��Ͽ� ���� å�� �����ϴ�. ����Ű�� ������ �޴��� ���ư��ϴ�. \n";
		cout << "------------------------------------------------\n";
		// string yn;
		// cin >> yn;
		return;
	} 

	vector<int> cant;
	bool flag3 = false;
	for (int i = 0; i < basketListNum; i++) {
		if (!bookBasketList.at(i)->getBorrowTF()) { // 1. �̹� �����.
			cant.emplace_back(i);
		}
		else if (bookBasketList.at(i)->getReservStudents().size() > 0) {
			if (bookBasketList.at(i)->getReservStudents().at(0) != this) { // 2. ù��° ������ != ��
				cant.emplace_back(i);
			}
		}
	}

	if (BORROWMAX - borrowbooknum < basketListNum) { // 3. �ܿ� ����Ƚ�� < ��ٱ��� list size
		flag3 = true;
	}
	if (flag3) {
		cout << " -- ���� ���� Ƚ���� ��ٱ��Ϻ��� " << basketListNum - (BORROWMAX - borrowbooknum) << "�� �����ϴ�. ��ٱ��ϸ� �����ּ���. -- \n";
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


// ��� ���ô���
void Student::sel_borrowBook() // ��ٱ��� -> ���ô��� (������ ���� �ٷ�� �ʿ�) - ������
{
	size_t basketListNum = bookBasketList.size(); 
	while(true){
		// ��ٱ��ϰ� �� ���
		if(bookBasketList.empty()){
			cout << "------------------------------------------------\n";
			cout << "��ٱ��Ͽ� ���� å�� �����ϴ�. ����Ű�� ������ �޴��� ���ư��ϴ�. \n";
			cout << "------------------------------------------------\n";
			// string yn;
			// cin >> yn;
			return;
		} 

	}

}

void Student::deleteBook() // ��ٱ��� -> ���� ���� ���� (������ ���� �ٷ�� �ʿ�) - ������
{
	while(true){
		// ��ٱ��ϰ� �� ���
		if(bookBasketList.empty()){
			cout << "------------------------------------------------\n";
			cout << "��ٱ��Ͽ� ���� å�� �����ϴ�. ����Ű�� ������ �޴��� ���ư��ϴ�. \n";
			cout << "------------------------------------------------\n";
			// string yn;
			// cin >> yn;
			return;
		} 
		// â �ʱ�ȭ �ʿ�
		bookListPrint(4, true, true, true, true);
		cout << "------------------------------------------------\n";
		cout << "������ å�� ��ȣ�� �Է��ϼ��� (0�� �Է��ϸ� �޴��� ���ư��ϴ�.): ";
		
		int select; // +1 �ؼ� �����ؾ� ��.
		cin >> select;
		if(select == 0){
			cout << "\n�޴��� ���ư��ϴ�.\n";
			cout << "------------------------------------------------\n";
			return;
		}
		cout << "------------------------------------------------\n";
		bookBasketList.erase(bookBasketList.begin()+select-1); // ����
		cout << "�ش� ������ ������ �Ϸ�Ǿ����ϴ�.\n";						
	}
}
void Student::reserveBook() // ��ٱ��� -> ���� ���� ���� (������ ���� �ٷ�� �ʿ�) - ������
{
	while(true){
		// ��ٱ��ϰ� �� ���
		if(bookBasketList.empty()){
			cout << "------------------------------------------------\n";
			cout << "��ٱ��Ͽ� ���� å�� �����ϴ�. ����Ű�� ������ �޴��� ���ư��ϴ�. \n";
			cout << "------------------------------------------------\n";
			// string yn;
			// cin >> yn;
			return;
		} 

		// ������� ���� Ƚ���� ��� ������ ��� break;
		if(reserveBookList.size()>=3) { 
			cout << "------------------------------------------------\n";
			cout << "����Ƚ���� ��� �����Ǿ����ϴ�. ����Ű�� ������ �޴��� ���ư��ϴ�. \n";
			cout << "------------------------------------------------\n";

			// string yn;
			// cin >> yn;
			return;
		}
		// â �ʱ�ȭ �ʿ�
		bookListPrint(4, true, true, true, true);
		cout << "------------------------------------------------\n";
		cout << "������ å�� ��ȣ�� �Է��ϼ��� (0�� �Է��ϸ� �޴��� ���ư��ϴ�.): ";
		
		int select; // +1 �ؼ� �����ؾ� ��.
		cin >> select; // Ȥ�� ������ ��ȣ�� �Է��ϰԵʤ�... �ϴ� �����ϸ� ��ٱ��Ͽ��� ������
		if(select == 0){
			cout << "\n�޴��� ���ư��ϴ�.\n";
			cout << "------------------------------------------------\n";
			return;
		}
		// ����Ұ� : ������ ���డ���ο� (5��) �ʰ� (����� ���� Ƚ�� �ʰ��� ������ �ٷ�) 
		if(bookBasketList.at(select-1)->getReservStudents().size() >= 5){
			cout << "------------------------------------------------\n";
			cout << "�ش� ������ ���� ���� �ο����� �ʰ��Ǿ����ϴ�.\n";
		}else{
			cout << "------------------------------------------------\n";

			reserveBookList.emplace_back(bookBasketList.at(select-1)); //����
			bookBasketList.erase(bookBasketList.begin()+select-1); // ��ٱ��Ͽ��� ����� ���� ���� (Ȥ�� ����ڰ� �������� �����ұ��)

			cout << "�ش� ������ ������ �Ϸ�Ǿ����ϴ�.\n";				
		}
		
	}
}

void Student::myPageMenu()// ���������� �޴� //������
{
	int num; //�޴� ����
	int u1; //1. ������Ȳ������ ����� ���� (�ݳ�/����)
	int booknum; //���� ��ȣ ����

	while (1) {
		cout << "------------------------------------------------\n";
		cout << "1. ���� ��Ȳ\n2. ���� ��Ȳ\n3. ���ư���\n";
		cout << "------------------------------------------------\n";
		cout << "�޴�����: ";
		cin >> num;
		switch (num) {
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
					cin >> booknum;
					returnBook(booknum);
					break;
				}
				else if (u1 == 2) {
					cout << "���� ��ȣ�� �������ּ���: ";
					cin >> booknum;
					extendBook(booknum);
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
			cin >> booknum;
			cancelReserveBook(booknum);
			break;
		case 3:
			cout << "�޴��� ���ư��ϴ�.\n";
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
	int reserveNum = BI.at(booknum).book->getReserveStudents().size();
	// .book �߰��߾��

   //����� ����: �������� ��� �ּ�ó�� - ������ �����Ϸ�
	if (reserveNum == 0)
		reserveNumFlag = false;
	else
		reserveNumFlag = true;

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
	RL.erase(RL.begin() + booknum);
	cout << "�ش� ���� ������ ��ҵǾ����ϴ�.\n";
}

void Student::quit() //���ư���
{
}

// Book��ü(1. bookList, 2. borrowBookList, 3. reserveBookList, 4.bookBasket, 5.serachResult �� listNum���� �Է�), ������, ���ڸ�, ���Ⱑ�ɿ���, �����ο���
void Student::bookListPrint(int listNum, bool nameTF, bool authorTF, bool borrowTF, bool reserveNumTF) { //��������Ʈ ��� - ������(1,3,4,5), �����(2)
	vector<Book*> List;
	vector<BorrowInfo>* BI; // 2. borrowBookList ������ �����
	int listSize = 0;
	switch (listNum) {
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
			if (listNum == 2) {
				cout << "\t" << "[��ü����]" << "\t" << "[�ݳ���¥]" << "\t" << "[���尡�ɿ���]";
			}
			cout << "\t" << (borrowTF ? "[���Ⱑ�ɿ���]" : "") << "\t" << (reserveNumTF ? "[�����ο���]" : "");
			cout << "\n-------------------------------------------\n";
		}
		else {
			if (listNum == 2) { //������Ȳ
				cout << "\n" << i + 1 << ".\t" << (nameTF ? BI->at(i).book->getName() : "") << "\t" << (authorTF ? BI->at(i).book->getAuthor() : "") << "\t" << BI->at(i).book->getTranslator() << "\t" << BI->at(i).book->getPublisher();

				// ---- ����� (�̿ϼ��ϴٰ� ����) ---- ȭ���� Student.cpp ����
				// cout<<"\t"<</*��ü����*/<<"\t"<<returnDate<<"\t"<</*���尡�ɿ���*/; //������Ȳ����

				// ---- �������

				if (borrowTF) { //���Ⱑ�ɿ���
					cout << "\t";

					if (BI->at(i).book->getBorrowTF()) {
						cout << "X";
					}
					else cout << "O";
				}
				if (reserveNumTF) {//�����ο���
					cout << "\t" << BI->at(i).book->getReservStudents().size();
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
				cout << "\t" << (reserveNumTF ? to_string(List.at(i)->getReservStudents().size()) : "");
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

void Student::setS_id(string s_id)
{
	this->s_id = s_id;
}
void Student::setId(string id)
{
	this->id = id;
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

string Student::getS_id() const {
	return s_id;
}

string Student::getId() const
{
	return id;
}

bool Student::getIsOverdue() const
{
	return isOverdue;
}

bool Student::getIsBlacklist() const
{
	return isBlacklist;
}

string Student::getBorrowDate() const
{
	return borrowDate;
}

string Student::getBookName() const
{
	return borrow->getName();
}
bool Student::operator==(Student student) // ������
{
	if (this->s_id == student.s_id) { // �й� ������ ���� �ι�
		return true;
	}
	else return false;
}