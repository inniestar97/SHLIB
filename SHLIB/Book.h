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
	//const wchar_t* translator; // ����
	//const wchar_t* publisher; // ���ǻ�
	string translator;
	string publisher;
	string publishYear; // ����⵵
	//string borrower; // ������ ����, ������ ���� ID�� �����ϸ� ����
	//vector<string> reserveStudents;

	
	// Student ȣ���ϴ°� �ȵǼ� �ϴ� �ּ�ó��
	Student* borrower; // ������
	vector<Student*> reserveStudents; // ������ �����

public:
	// Constructor
	Book() = delete;
	Book(string na, string au);
	Book(string na, string au, Student* me);
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
