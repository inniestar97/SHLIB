#include "Book.h"
#include "Student.h"
#include "grammarCheck.h"
#include <vector>
#include <iostream>
#include <fstream>

/*	string name; // å�̸�
	string author; // å ����
	string translator; // ����
	string publisher; // ���ǻ�
	string publishYear; // ����⵵
	Student borrower; // ������
	vector<Student> reserveStudents; // ������ �����  */

Book::Book(string na, string au)
	:name(na), author(au)
{
	/*
	 * å �̸�-���ڸ� �� ���ڷ� ������ å ������ �ҷ� å ��ü�� ����
	 * �̸� ���� ���� ���ǻ� ���࿬�� ������ �����ڸ� ����������
	 */

	ifstream file;
	file.open("datafile/bookDB/" + na + "-" + au + ".txt");
	if (!file.is_open()) {
		cerr << "datafile/bookDB/" + na + "-" + au + ".txt file is not exist." << endl;
		exit(1);
	}

	string info;
	file >> info;
	this->translator = info.substr(0, info.find('_'));
	info = info.substr(info.find('_') + 1, string::npos);
	this->publisher = info.substr(0, info.find('_'));
	info = info.substr(info.find('_') + 1, string::npos);
	this->publishYear = info.substr(0, info.find('_'));

	file >> info;
	file >> info; // ������ ���̵�_�̸�_�й�
	this->borrower = new Student(info.substr(0, info.find('_'))); // borrower -> ���� �л�

	file >> info; // info-> "�����ڸ��"
	getline(file, info); // ���๮�� ����
	while (getline(file, info)) { // ������ ���̵�_�̸�_�й�
		reserveStudents.push_back(new Student(info.substr(0, info.find("_"))));
	}

	this->borrower = nullptr;
}

Book::~Book()
{
	delete borrower;
	borrower = nullptr;

	for (size_t i = 0; i < reserveStudents.size(); i++) {
		delete reserveStudents.at(i);
		reserveStudents.at(i) = nullptr;
	}
	reserveStudents.clear();
}

void Book::addBorrow(Student* student) { // ����� (�ӽ� ��� �޽���)
	if (borrower != nullptr) {
		borrower = student;
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


bool Book::getBorrowTF() const
{
	// ������ : ���� ���� ���� �Ǻ�
	if (borrower == nullptr) return false;
	return true;
}

vector<Student*> Book::getReservStudents() const
{
	// ������ : ������ �������� 
	return reserveStudents;
}

bool Book::operator== (Book book) { // �����
	if (this->name == book.name && this->author == book.author && this->translator == book.translator && this->publisher == book.publisher && this->publishYear == book.publishYear) {
		return true;
	}
	else return false;
}