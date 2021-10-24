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
		cerr << "datafile/bookDB/" + na + "-" + au + ".txt file is not open." << endl;
		exit(1);
	}

	string info;
	file >> info;
	this->translator = info.substr(0, info.find('_'));
	info = info.substr(info.find('_') + 1, string::npos);
	this->publisher = info.substr(0, info.find('_'));
	info = info.substr(info.find('_') + 1, string::npos);
	this->publishYear = info.substr(0, info.find('_'));

	file >> info; // info -> "�����ڸ��"
	
	file >> info;
	if (info != "�����ڸ��") {  // �����ڰ� �ִ°��
		this->borrower = new Student(info.substr(0, info.find('_'))); // borrower -> ���� �л�
		file >> info; // info-> "�����ڸ��"
	} else {
		this->borrower = nullptr;
	}
	getline(file, info); // ���๮�� ����
	while (getline(file, info)) { // ������ ���̵�_�̸�_�й�
		reserveStudents.push_back(new Student(info.substr(0, info.find("_"))));
	}
	file.close();
}

Book::~Book()
{
	if (borrower != nullptr) {
		delete borrower;
		borrower = nullptr;
	}

	for (size_t i = 0; i < reserveStudents.size(); i++) {
		delete reserveStudents.at(i);
		reserveStudents.at(i) = nullptr;
	}
	reserveStudents.clear();
}

void Book::addBorrow(Student* student) { // ����� (�ӽ� ��� �޽���)
	if (borrower == nullptr) { // �����ڰ� ���ٸ�
		borrower = student;

		string bookpath = "datafile/bookDB" + name + "-" + author + ".txt";

		ofstream file(bookpath, ios::out);
		if (!file.is_open()) {
			cerr << "datafile/bookDB/" + name + "-" + author + ".txt is Not Open for addBorrow." << endl;
			exit(1);
		}

		file << translator + "_" + publisher + "_" + publishYear << endl;
		file << "�����ڸ��" << endl;
		file << student->getId() + "_" + student->getName() + "_" + student->getS_id() << endl;
		file << "�����ڸ��" << endl;
		for (size_t i = 0; i < reserveStudents.size(); i++) {
			string reID = reserveStudents[i]->getId();
			string reName = reserveStudents[i]->getName();
			string reSID = reserveStudents[i]->getS_id();
			file << reID + "_" + reName + "_" + reSID << endl;
		}
		file.close();

	} else { // �����ڰ� �ִٸ�
		cout << "���� �Ұ�" << endl;
	}
}

void Book::deleteBorrow() { // ����� (�ӽ� ��� �޽���)
	if (borrower != nullptr) {
		delete borrower;
		borrower = nullptr;

		string bookpath = "datafile/bookDB" + name + "-" + author + ".txt";
		ofstream file(bookpath, ios::out);
		if (!file.is_open()) {
			cerr << "datafile/bookDB/" + name + "-" + author + ".txt is Not Open for deleteBorrower." << endl;
			exit(1);
		}

		file << translator + "_" + publisher + "_" + publishYear << endl;
		file << "�����ڸ��" << endl << "�����ڸ��" << endl;

		for (size_t i = 0; i < reserveStudents.size(); i++) {
			string reID = reserveStudents[i]->getId();
			string reName = reserveStudents[i]->getName();
			string reSID = reserveStudents[i]->getS_id();
			file << reID + "_" + reName + "_" + reSID << endl;
		}
		file.close();

	} else {
		cout << "������ ����" << endl;
	}
}

void Book::addReserve(Student* user) // ����� (�������� �����丵 �� �س��µ� ���� ����� �����ּ���)
{
	// �̹� ������ ��ܿ� ���� ��� ����ó��
	for (auto stdu : reserveStudents) {
		if (stdu == user) {
			cout << "�̹� �������� �����Դϴ�." << endl;
			return;
		}
	}
	reserveStudents.push_back(user);

	string bookpath = "datafile/bookDB" + name + "-" + author + ".txt";
	ofstream file(bookpath, ios::out);
	if (!file.is_open()) {
		cerr << "datafile/bookDB/" + name + "-" + author + ".txt is Not Open for addReserve." << endl;
		exit(1); // �ٵ� exit �����ϴ� �� ���� �������? ���Ŀ� ���� ����? - ����
	}

	file << translator + "_" + publisher + "_" + publishYear << endl;
	file << "�����ڸ��" << endl;
	file << borrower->getId() + "_" + borrower->getName() + "_" + borrower->getS_id() << endl;
	file << "�����ڸ��" << endl;
	for (size_t i = 0; i < reserveStudents.size(); i++) {
		string reID = reserveStudents[i]->getId();
		string reName = reserveStudents[i]->getName();
		string reSID = reserveStudents[i]->getS_id();
		file << reID + "_" + reName + "_" + reSID << endl;
	}
	file.close();
}

void Book::deleteReserve(Student* user) // �����
{
	for (auto std:reserveStudents) {
		if (std->getId() == user->getId()) {
			reserveStudents.erase(find(reserveStudents.begin(), reserveStudents.end(), user));
			break;
		}
	}

	string bookpath = "datafile/bookDB" + name + "-" + author + ".txt";
	ofstream file(bookpath, ios::out);
	if (!file.is_open()) {
		cerr << "datafile/bookDB/" + name + "-" + author + ".txt is Not Open for addReserve." << endl;
		exit(1);
	}

	file << translator + "_" + publisher + "_" + publishYear << endl;
	file << "�����ڸ��" << endl;
	file << borrower->getId() + "_" + borrower->getName() + "_" + borrower->getS_id() << endl;
	file << "�����ڸ��" << endl;
	for (size_t i = 0; i < reserveStudents.size(); i++) {
		string reID = reserveStudents[i]->getId();
		string reName = reserveStudents[i]->getName();
		string reSID = reserveStudents[i]->getS_id();
		file << reID + "_" + reName + "_" + reSID << endl;
	}
	file.close();
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

Student* Book::getBorrower() const
{
	return borrower;
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