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
	string translator; // ����
	string publisher; // ���ǻ�
	string publishYear; // ����⵵
	Student* borrower; // ������
	vector<Student*> reserveStudents; // ������ �����

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
	Student* getBorrower() const;

	//������ student - booklistprint �κп� �ʿ��� �Լ�
	bool getBorrowTF() const;
	vector<Student*> getReservStudents();

	bool operator== (Book book);
};
