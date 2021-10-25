#include "Book.h"
#include "Student.h"
#include "grammarCheck.h"
#include <vector>
#include <iostream>
#include <fstream>


/*   string name; // å�̸�
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
    getline(file, info);
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
    }
    else { // ���� ���°ܿ�
        this->borrower = nullptr;
    }

    getline(file, info); // ���๮�� ����

    while (getline(file, info)) { // ������ ���̵�_�̸�_�й�
        //reserveStudents.push_back(new Student(info.substr(0, info.find("_"))));
        newrS.emplace_back(info); // ������ string���� ����

    }
    file.close();

    cout << na << " å ��� �Ϸ� (�ӽ� �޽���)" << endl;
}


Book::Book(string na, string au, Student* me)
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
    getline(file, info);
    this->translator = info.substr(0, info.find('_'));
    info = info.substr(info.find('_') + 1, string::npos);
    this->publisher = info.substr(0, info.find('_'));
    info = info.substr(info.find('_') + 1, string::npos);
    this->publishYear = info.substr(0, info.find('_'));

    file >> info; // info -> "�����ڸ��"

    file >> info;

    if (info != "�����ڸ��") {  // �����ڰ� �ִ°��
        this->borrower = me; // borrower -> ���� �л�
        file >> info; // info-> "�����ڸ��"
    }
    else {
        this->borrower = nullptr;
    }
    getline(file, info); // ���๮�� ����
    while (getline(file, info)) { // ������ ���̵�_�̸�_�й�
        //reserveStudents.push_back(new Student(info.substr(0, info.find("_"))));
        newrS.emplace_back(info); // ������ string���� ����
    }
    file.close();

    cout << na << " å ��� �Ϸ� (�ӽ� �޽���)" << endl;
}

Book::~Book()
{
    if (borrower != nullptr) {
        borrower = nullptr;
    }

    /*for (size_t i = 0; i < reserveStudents.size(); i++) {
        delete reserveStudents.at(i);
        reserveStudents.at(i) = nullptr;
    }
    reserveStudents.clear();*/
}

void Book::addBorrow(Student* student) { // ����� (�ӽ� ��� �޽���)
    if (borrower == nullptr) { // �����ڰ� ���ٸ�
        borrower = student;

        string bookpath = "datafile/bookDB/" + name + "-" + author + ".txt";

        ofstream file(bookpath, ios::out);
        if (!file.is_open()) {
            cerr << "datafile/bookDB/" + name + "-" + author + ".txt is Not Open for addBorrow." << endl;
            exit(1);
        }

        file << translator + "_" + publisher + "_" + publishYear << endl;
        file << "�����ڸ��" << endl;
        file << student->getId() + "_" + student->getName() + "_" + student->getS_id() << endl;
        file << "�����ڸ��" << endl;
        /*for (size_t i = 0; i < reserveStudents.size(); i++) {
            string reID = reserveStudents[i]->getId();
            string reName = reserveStudents[i]->getName();
            string reSID = reserveStudents[i]->getS_id();
            file << reID + "_" + reName + "_" + reSID << endl;
        }*/
        for (size_t i = 0; i < newrS.size(); i++) { // ������ string���� ����
            file << newrS[i] << endl;
        }
        file.close();

        string brList = "datafile/User/forAdmin/borrowList.txt";
        ofstream file_2(brList, ios::ate);
        if (!file_2.is_open()) {
            cerr << "datafile/User/forAdmin/borrowList.txt file is not open for addBorrow in Book Class" << endl;
            exit(1); // ������ ����
        }

        file_2 << student->getId() << "_" << student->getName() << "_" << student->getS_id() << "-";
        file_2 << this->name << "_" << this->author << "_" << this->translator << "_" << this->publisher << "_" << this->publishYear << endl; 
        file_2.close();
    }
    else { // �����ڰ� �ִٸ�
        cout << "���� �Ұ�" << endl;
    }
}

void Book::deleteBorrow() { // ����� (�ӽ� ��� �޽���)
    if (borrower != nullptr) {
        string later = borrower->getId();
        borrower = nullptr;

        string bookpath = "datafile/bookDB/" + name + "-" + author + ".txt";
        ofstream file(bookpath, ios::trunc);
        if (!file.is_open()) {
            cerr << "datafile/bookDB/" + name + "-" + author + ".txt is Not Open for deleteBorrower." << endl;
            exit(1);
        }

        file << translator + "_" + publisher + "_" + publishYear << endl;
        file << "�����ڸ��" << endl << "�����ڸ��" << endl;

        /*for (size_t i = 0; i < reserveStudents.size(); i++) {
            string reID = reserveStudents[i]->getId();
            string reName = reserveStudents[i]->getName();
            string reSID = reserveStudents[i]->getS_id();
            file << reID + "_" + reName + "_" + reSID << endl;
        }*/
        for (size_t i = 0; i < newrS.size(); i++) { // ������ string���� ����
            file << newrS[i] << endl;
        }
        file.close();

        string deList = "datafile/User/forAdmin/borrowList.txt";
        string new_deList = "datafile/User/forAdmin/borrowList'.txt";
        ifstream file_1(deList);
        ofstream file_2(new_deList);
        if (!file_2.is_open() && !file_1.is_open()) {
            cerr << "datafile/User/forAdmin/borrowList.txt is not open for deleteBorrow in BookClass" << endl;
            exit(1);
        }
        string s;
        while (getline(file_1, s)) {
            if (s.substr(0, s.find("_")) == later) {
                continue;
            }

            file_2 << s;
        }

        file_1.close();
        file_2.close();
        
        remove(deList.c_str());
        rename(new_deList.c_str(), deList.c_str());

    }
    else {
        cout << "������ ����" << endl;
    }
}



void Book::addReserve(Student* user) // ����� (�������� �����丵 �� �س��µ� ���� ����� �����ּ���)
{
    // �̹� ������ ��ܿ� ���� ��� ����ó��
    /*for (auto stdu : reserveStudents) {
        if (stdu == user) {
            cout << "�̹� �������� �����Դϴ�." << endl;
            return;
        }
    }
    reserveStudents.push_back(user);*/

    // ���� �ڵ� ������ string���� ����
    for (auto stdu : newrS) {
        // split
        string id = stdu.substr(0, stdu.find("_"));
        if (id == user->getId()) {
            cout << "�̹� �������� �����Դϴ�." << endl;
            return;
        }
    }
    string user_ = user->getId() + "_" + user->getName() + "_" + user->getS_id();
    newrS.emplace_back(user_);

    string bookpath = "datafile/bookDB/" + name + "-" + author + ".txt";
    ofstream file(bookpath, ios::out);
    if (!file.is_open()) {
        cerr << "datafile/bookDB/" + name + "-" + author + ".txt is Not Open for addReserve." << endl;
        exit(1); // �ٵ� exit �����ϴ� �� ���� �������? ���Ŀ� ���� ����? - ����
    }

    file << translator + "_" + publisher + "_" + publishYear << endl;
    file << "�����ڸ��" << endl;
    if (borrower != nullptr) {
        file << borrower->getId() + "_" + borrower->getName() + "_" + borrower->getS_id() << endl;
    }
    file << "�����ڸ��" << endl;

    /*for (size_t i = 0; i < reserveStudents.size(); i++) {
        string reID = reserveStudents[i]->getId();
        string reName = reserveStudents[i]->getName();
        string reSID = reserveStudents[i]->getS_id();
        file << reID + "_" + reName + "_" + reSID << endl;
    }*/
    // ������ string���� ����
    for (size_t i = 0; i < newrS.size(); i++) {
        file << newrS[i] << endl;
    }

    file.close();

}

void Book::deleteReserve(Student* user) // ����� : å���Ͽ��� ������ ����
{
    /*for (auto std : reserveStudents) {
        if (std->getId() == user->getId()) {
            reserveStudents.erase(find(reserveStudents.begin(), reserveStudents.end(), user));
            break;
        }
    }*/

    // ������ string���� ����
    for (auto stdu : newrS) {
        string id = stdu.substr(0, stdu.find("_"));

        if (id == user->getId()) {
            string fi = user->getId() + "_" + user->getName() + "_" + user->getS_id();
            newrS.erase(find(newrS.begin(), newrS.end(), fi));
            break;
        }
    }

    string bookpath = "datafile/bookDB/" + name + "-" + author + ".txt";
    ofstream file(bookpath, ios::out);
    if (!file.is_open()) {
        cerr << "datafile/bookDB/" + name + "-" + author + ".txt is Not Open for addReserve." << endl;
        exit(1);
    }

    file << translator + "_" + publisher + "_" + publishYear << endl;
    file << "�����ڸ��" << endl;
    if (borrower != nullptr) {
        file << borrower->getId() + "_" + borrower->getName() + "_" + borrower->getS_id() << endl;
    }
    file << "�����ڸ��" << endl;
    /*for (size_t i = 0; i < reserveStudents.size(); i++) {
         string reID = reserveStudents[i]->getId();
         string reName = reserveStudents[i]->getName();
         string reSID = reserveStudents[i]->getS_id();
         file << reID + "_" + reName + "_" + reSID << endl;
     }*/

     // ������ string���� ����
    for (size_t i = 0; i < newrS.size(); i++) {
        file << newrS[i] << endl;
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

bool Book::getBorrowTF() const // ���� ���� ����� �����ϴ°�?
{
    // ������ : ���� ���� ���� �Ǻ�
    if (borrower == nullptr) return false;
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