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
     * 책 이름-저자명 을 인자로 생성시 책 정보를 불러 책 객체로 저장
     * 이름 저자 역자 출판사 발행연도 대출자 예약자를 가지고있음
     */

    ifstream file;
    file.open("datafile/bookDB/" + na + "-" + au + ".txt");
    if (!file.is_open()) {
        cerr << "datafile/bookDB/" + na + "-" + au + ".txt file is not open." << endl;
        exit(1);
    }

    /*  datafile/bookDB/책이름-지은이.txt 파일 양식

	[역자]_[출판사]_[발행연도]
	대출자명단
	1.[아이디]_[이름]_[학번]
	예약자명단
	1.[아이디]_[이름]_[학번]
	2.[아이디]_[이름]_[학번]
	3.[아이디]_[이름]_[학번]
	4.
	5.

    */

    string info;
    getline(file, info); // info -> [역자]_[출판사]_[발행연도]
    this->translator = info.substr(0, info.find('_'));
    info = info.substr(info.find('_') + 1, string::npos);
    this->publisher = info.substr(0, info.find('_'));
    this->publishYear = info.substr(info.find('_') + 1, string::npos);

    file >> info; // info -> "대출자명단"
    file >> info;
    borrower = info.substr(info.find('.') + 1, string::npos);
    /*  info ->
        (대출자가 있는경우) 1. [아이디]_[이름]_[학번]
        (대출자가 없는경우) 1.
    */

    file >> info; // info -> "예약자명단" 
    for (size_t i = 1; i <= 5; i++) {
        file >> info; // info -> x. [아이디]_[이름]_[학번]
        info = info.substr(info.find('.') + 1, string::npos);
        if (info.size() != 0) { // 예약자가 있는 경우
            newrS.push_back(info);
        }
    }

    while (!file.is_open()) file.close();
}

Book::~Book() 
{

}

void Book::addBorrow(Student* student) { // 윤재원 (임시 출력 메시지)
    if (borrower.size() == 0) { // 대출자가 없다면
        borrower = student->getId() + "_" + student->getName() + "_" + student->getS_id();

        //---------------------------------------------------------------
        //-------------------book 파일 수정 code-------------------------
        //---------------------------------------------------------------
        string bookpath = "datafile/bookDB/" + name + "-" + author + ".txt";
        ofstream file(bookpath, ios::trunc);
        if (!file.is_open()) {
            cerr << "datafile/bookDB/" + name + "-" + author + ".txt is Not Open for addBorrow." << endl;
            exit(1);
        }

        file << translator + "_" + publisher + "_" + publishYear << endl;
        file << "대출자명단" << endl;
        file << "1." << borrower << endl;
        file << "예약자명단" << endl;
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
    else { // 대출자가 있다면
        cout << "대출 불가" << endl;
    }
}

void Book::deleteBorrow() { // 윤재원 (임시 출력 메시지)
    if (borrower.size() != 0) { // 대출자가 있다면
        borrower.clear(); // 대출자 삭제

        //---------------------------------------------------------------
        //-------------------book 파일 수정 code-------------------------
        //---------------------------------------------------------------
        string bookpath = "datafile/bookDB/" + name + "-" + author + ".txt";
        ofstream file(bookpath, ios::trunc);
        if (!file.is_open()) {
            cerr << "대출자 삭제 관련 파일 열기 실패" << endl;
            exit(1);
        }

        file << translator + "_" + publisher + "_" + publishYear << endl;
        file << "대출자명단" << endl;
        file << "1." << endl;
        file << "예약자명단" << endl;
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
        cout << "대출자 없음" << endl;
    }
}



void Book::addReserve(Student* user) // 윤재원 (강지윤이 리팩토링 좀 해놨는데 문제 생기면 말해주세요)
{
    for (auto stdu : newrS) {
        // split
        string id = stdu.substr(0, stdu.find("_"));
        if (id == user->getId()) {
            cout << "이미 예약중인 도서입니다." << endl;
            return;
        }
    }

    string user_ = user->getId() + "_" + user->getName() + "_" + user->getS_id();
    newrS.push_back(user_); // 예약자 추가

    //---------------------------------------------------------------
    //-------------------book 파일 수정 code-------------------------
    //---------------------------------------------------------------
    string bookpath = "datafile/bookDB/" + name + "-" + author + ".txt";
    ofstream file(bookpath, ios::trunc);
    if (!file.is_open()) {
        cerr << "예약자 추가 관련 파일 열기 실패" << endl;
        exit(1);
    }

    file << translator + "_" + publisher + "_" + publishYear << endl;
    file << "대출자명단" << endl;
    file << "1." << borrower << endl;
    file << "예약자명단" << endl;
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

void Book::deleteReserve(Student* user) // 윤재원 : 책파일에서 예약자 삭제
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
    //-------------------book 파일 수정 code-------------------------
    //---------------------------------------------------------------
    string bookpath = "datafile/bookDB/" + name + "-" + author + ".txt";
    ofstream file(bookpath, ios::trunc);
    if (!file.is_open()) {
        cerr << "예약자 삭제 관련 파일 열기 실패" << endl;
        exit(1);
    }

    file << translator + "_" + publisher + "_" + publishYear << endl;
    file << "대출자명단" << endl;
    file << "1." << borrower << endl;
    file << "예약자명단" << endl;
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

bool Book::getBorrowTF() const // 현재 빌린 사람이 존재하는가?
{
    // 강지윤 : 대출 가능 여부 판별
    if (borrower.size() == 0)  return false;
    return true;
}

int Book::getReserveStudentsSize() const
{
    return newrS.size();
}

bool Book::isFirstRSisME(Student* me) const // 사용시에 예약자 존재확실할 때만 사용
{   
    string id = newrS[0].substr(0, newrS[0].find("_"));
    if (me->getS_id() == id) return true; 
    return false;
}

bool Book::operator== (Book book) { // 윤재원
    if (this->name == book.name && this->author == book.author && this->translator == book.translator && this->publisher == book.publisher && this->publishYear == book.publishYear) {
        return true;
    }
    else return false;
}