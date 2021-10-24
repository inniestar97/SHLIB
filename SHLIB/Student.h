#pragma once
#include "User.h"
#include "Book.h"
#include "Library.h"
using namespace std;

class Student :
	public User
{
private:
	vector<Book*> bookList; // å ����Ʈ (�������� static�� �ӽ÷� ����)
	int current_menu; // ���� �޴� ��ȣ
	string name; // �л��̸�
	string s_id; // �й�
	bool isOverdue; // ��ü����
	bool isBlacklist; // ������Ʈ ����
	bool canExtend;
	//int borrowBookNum; // ����Ǽ� (1�� ���������� 1�Ǹ� ����)
	Book* borrow; // ���� å�� �� �Ǹ� ���� , ������ å
	string borrowDate; // ���� ��¥
	string dueDate;	// �ݳ�����
	
	/* -> 2������
	struct BorrowInfo { // ���� å ����
		Book* book;
		long borrowDate;
	};
	*/

	vector<Book*> searchResult; // å �˻� ��� ����Ʈ
	//vector<BorrowInfo> borrowBookList; // ������ å ����Ʈ -> 2������
	vector<Book*> reserveBookList; // ������ å ����Ʈ
	vector<Book*> bookBasketList; // ��ٱ���

public:
	//Constructor
	Student() = delete;
	Student(string id);
	//Destructor
	~Student();

	void menu(); // ����� ��� �޴�
	void initBookList(); // å ����Ʈ �ҷ�����
	void searchBookMenu();// �ڷ�˻� �޴�
	void bookBasketMenu(); // ��ٱ��� �޴�
	void borrowBook(); // ��ٱ��� -> �ϰ� ����
	void sel_borrowBook(); // ��ٱ��� -> ���� ����
	void deleteBook(); // ��ٱ��� -> ���� ���� ����
	void reserveBook(); // ��ٱ��� -> ���� ���� ����
	void myPageMenu(); // ���������� �޴�
	void returnBook(); // ���������� -> å �ݳ�
	void extendBook(); // ���������� -> å ����
	void cancelReserveBook(int booknum); // ���������� -> å ���� ���

	


	void bookListPrint(vector<Book*> book, bool borrowListTF, bool nameTF, bool authorTF, bool borrowTF, bool reserveNumTF) const; // ���� ����Ʈ ��� - ������

	void setCurrent_menu(int menu);
	void setName(string name);
	void setS_id(string s_id);
	void setIsOverdue(bool check);
	void setIsBlacklist(bool check);

	int getCurrent_menu() const;
	string getName() const;
	string getS_id() const;
	bool getIsOverdue() const;
	bool getIsBlacklist() const;
	string getBorrowDate() const;
	string getDueDate() const;
	string getBookName() const;
	vector<Book*> getReserveBookList();
	bool operator== (Student student);
};
