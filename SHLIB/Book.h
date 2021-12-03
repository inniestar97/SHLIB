#pragma once
#include <string>
#include <vector>

using namespace std;

class Student;

class Book
{
private:
	string name; // å�̸�
	string author; // å ����
	string translator;
	string publisher;
	string publishYear; // ����⵵
	string borrower; // ������ ����, ������ ���� ID�� �����ϸ� ����
	// �����ڸ��
	vector<string> newrS; // ���̵�_�̸�_�й�

public:
	// Constructor
	Book() = delete;
	Book(string na, string au);
	// Destructor
	~Book();

	void addBorrow(Student* student); // ������ �߰�
	void deleteBorrow(); // ������ ����

	void addReserve(Student* user); // ������ �߰�
	void deleteReserve(Student* user); // ������ ����

	void setName(string name);
	void setAuthor(string author);
	void setTranslator(string translator);
	void setPublisher(string publisher);
	void setPublishYear(string publishYear);
	string getName() const;
	string getAuthor() const;
	string getTranslator() const;
	string getPublisher() const;
	string getPublishYear() const;
	string getBorrower() const;

	//������ student - booklistprint �κп� �ʿ��� �Լ�
	bool getBorrowTF() const;
	//vector<Student*> getReservStudents();
	int getReserveStudentsSize() const;
	bool isFirstRSisME(Student* me) const;
	
	bool operator== (Book book);
};
