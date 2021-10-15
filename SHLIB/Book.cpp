#include "Book.h"
#include "Student.h"
#include "grammarCheck.h"
#include <vector>
#include <iostream>

/*	string name; // å�̸�
	string author; // å ����
	string translator; // ����
	string publisher; // ���ǻ�
	string publishYear; // ����⵵
	Student borrower; // ������
	vector<Student> reserveStudents; // ������ �����  */

Book::Book(string na, string au, string tr, string publisher, string year) { // �����
	this->name = na;
	this->author = au;
	this->translator = tr;
	this->publisher = publisher;
	this->publishYear = year;
	this->borrower = nullptr;
}

Book::~Book()
{
	delete borrower;
	borrower = nullptr;
}

void Book::addBorrow(Student* student) { // ����� (�ӽ� ��� �޽���)
	if (borrower != nullptr) {
		borrower = new Student();
	} else {
		cout << "���� �Ұ�" << endl;
	}
}

void Book::deleteBorrow() { // ����� (�ӽ� ��� �޽���)
	if (borrower != nullptr) {
		delete borrower;
		borrower = nullptr;
	} else {
		cout << "������ ����" << endl;
	}
}

void Book::addReserve(Student* user) // �����
{
	// �̹� ������ ��ܿ� ���� ��� ����ó��
	for (auto std:reserveStudents) {
		if (std->getId() == user->getId()) {
			cout << "�̹� ������" << endl;
			return;
		}
	}
	reserveStudents.push_back(user);
}

void Book::deleteReserve(Student* user) // �����
{
	for (auto std:reserveStudents) {
		if (std->getId() == user->getId()) {
			reserveStudents.erase(find(reserveStudents.begin(), reserveStudents.end(), user));
			break;
		}
	}
}

void Book::setName(string name)
{
	this->name = name;
}

void Book::setAuthor(string author)
{
	this->author = author;
}

void Book::setTranslator(string translator)
{
	this->translator = translator;
}

void Book::setPublisher(string publisher)
{
	this->publisher = publisher;
}

void Book::setPublishYear(string publishYear)
{
	this->publishYear = publishYear;
}

string Book::getName() const
{
	return name;
}

string Book::getAuthor() const
{
	return author;
}

string Book::getTranslator() const
{
	return translator;
}

string Book::getPublisher() const
{
	return publisher;
}

string Book::getPublishYear() const
{
	return publishYear;
}

bool Book::operator== (Book book) { // �����
	if (this->name == book.name && this->author == book.author && this->translator == book.translator && this->publisher == book.publisher && this->publishYear == book.publishYear) {
		return true;
	}
	else return false;
}