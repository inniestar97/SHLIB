#pragma once
#include "User.h"
#include "Book.h"

using namespace std;

class Student :
    public User
{
private:
    vector<Book*> bookList; // å ����Ʈ (�������� static�� �ӽ÷� ����)
    int current_menu; // ���� �޴� ��ȣ
    string name; // �л��̸�
    string sid; // �л� ���̵�
    bool isOverdue;
    bool isBlacklist;

    struct BorrowInfo { // ���� å ����
        Book* book;
        string returnDate; // ����ȯ �ʿ� (long �ڷ���?)
    }; 

    vector<Book*> searchResult; // å �˻� ��� ����Ʈ
    vector<BorrowInfo> borrowBookList; // ������ å ����Ʈ
    vector<Book*> reserveBookList; // ������ å ����Ʈ
    vector<Book*> bookBasketList; // ��ٱ���

public:
    //Constructor
    Student() = delete; 
    Student(string name, string password);
    //Destructor 
    ~Student();

    void menu(); // ����� ��� �޴�
    void initBookList(); // å ����Ʈ �ҷ�����
    void searchBookMenu();// �ڷ�˻� �޴�
    void bookBasketMenu(); // ��ٱ��� �޴�
    void borrowBook(); // ��ٱ��� -> �ϰ� ����
    void deleteBook(); // ��ٱ��� -> ���� ���� ����
    void reserveBook(); // ��ٱ��� -> ���� ���� ����
    void myPageMenu(); // ���������� �޴�
    void returnBook(int booknum); // ���������� -> å �ݳ�
    void extendBook(int booknum); // ���������� -> å ����
    void cancelReserveBook(int booknum); // ���������� -> å ���� ���

    void quit(); //���ư��� (�� �޴����� �ִ�)

	void bookListPrint(int listNum, bool nameTF, bool authorTF, bool borrowTF, bool reserveNumTF); // ���� ����Ʈ ��� - ������

    void setCurrent_menu(int menu);
    void setName(string name);
    void setSid(string sid);
    void setIsOverdue(bool check);
    void setIsBlacklist(bool check);
    int getCurrent_menu() const;
    string getName() const;
    string getsid() const;
    bool getIsOverdue() const;
    bool getIsBlacklist() const;
};

