#pragma once
#include <string>
#include <vector>
#include "Student.h"
 
using namespace std;
 
class Book
{
private:
	string name; // å�̸�
	string author; // å ����
	string translator; // ����
	string publisher; // ���ǻ�
	string publishYear; // ����⵵
	Student borrower; // ������
	vector<Student> reserveStudents; // ������ ����� 
public:

	// Constructor
	Book()=delete;
	Book(string na, string au, string tr, string publisher, string year);
	// Destructor
	~Book();

	void addBorrow(Student& student); // ������ �߰�
	void deleteBorrow(); // ������ ����

	void addReserve(Student& user); // ������ �߰�
	void deleteReserve(Student& user); // ������ ����


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

	//void addBorrowStudent(Student& user); // ������� �߰�
	//void addReserveStudents(Student& user); // ������ �߰�

	bool operator== (Book book);
};

