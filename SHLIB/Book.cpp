#include "Book.h"
#include "Student.h"
#include "grammarCheck.h"
#include <vector>
#include <iostream>
#include <fstream>

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

    /*  datafile/bookDB/å�̸�-������.txt ���� ���

	[����]_[���ǻ�]_[���࿬��]
	�����ڸ��
	1.[���̵�]_[�̸�]_[�й�]
	�����ڸ��
	1.[���̵�]_[�̸�]_[�й�]
	2.[���̵�]_[�̸�]_[�й�]
	3.[���̵�]_[�̸�]_[�й�]
	4.
	5.

    */

    string info;
    getline(file, info); // info -> [����]_[���ǻ�]_[���࿬��]
    this->translator = info.substr(0, info.find('_'));
    info = info.substr(info.find('_') + 1, string::npos);
    this->publisher = info.substr(0, info.find('_'));
    this->publishYear = info.substr(info.find('_') + 1, string::npos);

    file >> info; // info -> "�����ڸ��"
    file >> info;
    borrower = info.substr(info.find('.') + 1, string::npos);
    /*  info ->
        (�����ڰ� �ִ°��) 1. [���̵�]_[�̸�]_[�й�]
        (�����ڰ� ���°��) 1.
    */

    file >> info; // info -> "�����ڸ��" 
    for (size_t i = 1; i <= 5; i++) {
        file >> info; // info -> x. [���̵�]_[�̸�]_[�й�]
        info = info.substr(info.find('.') + 1, string::npos);
        if (info.size() != 0) { // �����ڰ� �ִ� ���
            newrS.push_back(info);
        }
    }

    while (!file.is_open()) file.close();
}

Book::~Book() 
{

}

void Book::addBorrow(Student* student) { // ����� (�ӽ� ��� �޽���)
    if (borrower.size() == 0) { // �����ڰ� ���ٸ�
        borrower = student->getId() + "_" + student->getName() + "_" + student->getS_id();

        //---------------------------------------------------------------
        //-------------------book ���� ���� code-------------------------
        //---------------------------------------------------------------
        string bookpath = "datafile/bookDB/" + name + "-" + author + ".txt";
        ofstream file(bookpath, ios::trunc);
        if (!file.is_open()) {
            cerr << "datafile/bookDB/" + name + "-" + author + ".txt is Not Open for addBorrow." << endl;
            exit(1);
        }

        file << translator + "_" + publisher + "_" + publishYear << endl;
        file << "�����ڸ��" << endl;
        file << "1." << borrower << endl;
        file << "�����ڸ��" << endl;
        for (size_t i = 1; i <= 5; i++) {
            file << i << ".";
            if (newrS.size() >= i) {
                file << newrS.at(i);
            }
            file << endl;
        }
        while (!file.is_open()) file.close();
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        //---------------------------------------------------------------
    }
    else { // �����ڰ� �ִٸ�
        cout << "���� �Ұ�" << endl;
    }
}

void Book::deleteBorrow() { // ����� (�ӽ� ��� �޽���)
    if (borrower.size() != 0) { // �����ڰ� �ִٸ�
        borrower.clear(); // ������ ����

        //---------------------------------------------------------------
        //-------------------book ���� ���� code-------------------------
        //---------------------------------------------------------------
        string bookpath = "datafile/bookDB/" + name + "-" + author + ".txt";
        ofstream file(bookpath, ios::trunc);
        if (!file.is_open()) {
            cerr << "������ ���� ���� ���� ���� ����" << endl;
            exit(1);
        }

        file << translator + "_" + publisher + "_" + publishYear << endl;
        file << "�����ڸ��" << endl;
        file << "1." << endl;
        file << "�����ڸ��" << endl;
        for (size_t i = 1; i <= 5; i++) {
            file << i << ".";
            if (newrS.size() >= i) {
                file << newrS.at(i);
            }
            file << endl;
        }
        while (!file.is_open()) file.close();
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        //---------------------------------------------------------------
    }
    else {
        cout << "������ ����" << endl;
    }
}



void Book::addReserve(Student* user) // ����� (�������� �����丵 �� �س��µ� ���� ����� �����ּ���)
{
    for (auto stdu : newrS) {
        // split
        string id = stdu.substr(0, stdu.find("_"));
        if (id == user->getId()) {
            cout << "�̹� �������� �����Դϴ�." << endl;
            return;
        }
    }

    string user_ = user->getId() + "_" + user->getName() + "_" + user->getS_id();
    newrS.push_back(user_); // ������ �߰�

    //---------------------------------------------------------------
    //-------------------book ���� ���� code-------------------------
    //---------------------------------------------------------------
    string bookpath = "datafile/bookDB/" + name + "-" + author + ".txt";
    ofstream file(bookpath, ios::trunc);
    if (!file.is_open()) {
        cerr << "������ �߰� ���� ���� ���� ����" << endl;
        exit(1);
    }

    file << translator + "_" + publisher + "_" + publishYear << endl;
    file << "�����ڸ��" << endl;
    file << "1." << borrower << endl;
    file << "�����ڸ��" << endl;
    for (size_t i = 1; i <= 5; i++) {
        file << i << ".";
        if (newrS.size() >= i) {
            file << newrS.at(i);
        }
        file << endl;
    }
    while (!file.is_open()) file.close();
    //---------------------------------------------------------------
    //---------------------------------------------------------------
    //---------------------------------------------------------------

}

void Book::deleteReserve(Student* user) // ����� : å���Ͽ��� ������ ����
{
    for (auto stdu : newrS) {
        string id = stdu.substr(0, stdu.find("_"));

        if (id == user->getId()) {
            string fi = user->getId() + "_" + user->getName() + "_" + user->getS_id();
            newrS.erase(find(newrS.begin(), newrS.end(), fi));
            break;
        }
    }

    //---------------------------------------------------------------
    //-------------------book ���� ���� code-------------------------
    //---------------------------------------------------------------
    string bookpath = "datafile/bookDB/" + name + "-" + author + ".txt";
    ofstream file(bookpath, ios::trunc);
    if (!file.is_open()) {
        cerr << "������ ���� ���� ���� ���� ����" << endl;
        exit(1);
    }

    file << translator + "_" + publisher + "_" + publishYear << endl;
    file << "�����ڸ��" << endl;
    file << "1." << borrower << endl;
    file << "�����ڸ��" << endl;
    for (size_t i = 1; i <= 5; i++) {
        file << i << ".";
        if (newrS.size() >= i) {
            file << newrS.at(i);
        }
        file << endl;
    }
    while (!file.is_open()) file.close();
    //---------------------------------------------------------------
    //---------------------------------------------------------------
    //---------------------------------------------------------------
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

string Book::getBorrower() const
{
    return borrower;
}

bool Book::getBorrowTF() const // ���� ���� ����� �����ϴ°�?
{
    // ������ : ���� ���� ���� �Ǻ�
    if (borrower.size() == 0)  return false;
    return true;
}

int Book::getReserveStudentsSize() const
{
    return newrS.size();
}

bool Book::isFirstRSisME(Student* me) const // ���ÿ� ������ ����Ȯ���� ���� ���
{   
    string id = newrS[0].substr(0, newrS[0].find("_"));
    if (me->getS_id() == id) return true; 
    return false;
}

bool Book::operator== (Book book) { // �����
    if (this->name == book.name && this->author == book.author && this->translator == book.translator && this->publisher == book.publisher && this->publishYear == book.publishYear) {
        return true;
    }
    else return false;
}