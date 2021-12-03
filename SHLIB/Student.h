#pragma once
#include "User.h"
#include "Book.h"
#include "Library.h"
using namespace std;

typedef struct _BorrowInfo { // ���� å ����
	Book* book;
	string borrowDate; // ���⳯¥
	string dueDate;	// �ݳ�����
} BorrowInfo;

class Student :
	public User
{
private:

	int current_menu; // ���� �޴� ��ȣ
	/*
		�л�����

		����Ŭ���� User.h
	string id; ���̵�
	string password; ��й�ȣ
	------------------------------------------*/	
	string name; // �л��̸�
	string s_id; // �й�
	string limitDate; // ���ѻ��� ���� �� ���� ���� ��¥.
	/* ^���ѻ����ϰ�� limitDate = ��¥  ���ѻ��� �ƴҰ�� limitDate = "false" */
	int limitedStack; // �������� ����
	vector<BorrowInfo> borrowBookList; // ������ å ����Ʈ -> 2������
	vector<Book*> reserveBookList; // ������ å ����Ʈ
	/*----------------------------------------*/

	/* �Ҹ��� �κ� �Ű� �� ��� �մϴ�. -> �� ����Լ����� �ؾ���*/
	vector<Book*> bookList; // å ����Ʈ (�������� static�� �ӽ÷� ����)
	vector<Book*> searchResult; // å �˻� ��� ����Ʈ
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
	// void borrowBook(); // ��ٱ��� -> �ϰ� ���� /*�ϰ����� ���������� �����ص� �볪��?*/
	void sel_borrowBook(); // ��ٱ��� -> ���� ����
	void deleteBook(); // ��ٱ��� -> ���� ���� ����
	void reserveBook(); // ��ٱ��� -> ���� ���� ����
	void myPageMenu(); // ���������� �޴�
	void returnBook(int bi); // ���������� -> å �ݳ�
	void extendBook(int bi); // ���������� -> å ����
	void cancelReserveBook(int booknum); // ���������� -> å ���� ���

	void bookListPrint(vector<Book*> book, bool borrowListTF, bool nameTF, bool authorTF, bool borrowTF, bool reserveNumTF) const; // ���� ����Ʈ ��� - ������

	void setCurrent_menu(int menu);
	void setName(string name);
	void setS_id(string s_id);
	void setLimitDate(string date);

	int getCurrent_menu() const;
	string getName() const;
	string getS_id() const;
	string getLimitDate() const;
	bool operator== (Student student);
};
