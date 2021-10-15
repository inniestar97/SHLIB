#include "Book.h"
#include "Student.h"
#include "grammarCheck.h"
#include <vector>
#include <iostream>

/*	string name; // 책이름
	string author; // 책 저자
	string translator; // 역자
	string publisher; // 출판사
	string publishYear; // 발행년도
	Student borrower; // 대출자
	vector<Student> reserveStudents; // 예약한 사람들  */

Book::Book(string na, string au, string tr, string publisher, string year) { // 윤재원
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

void Book::addBorrow(Student* student) { // 윤재원 (임시 출력 메시지)
	if (borrower != nullptr) {
		borrower = new Student();
	} else {
		cout << "대출 불가" << endl;
	}
}

void Book::deleteBorrow() { // 윤재원 (임시 출력 메시지)
	if (borrower != nullptr) {
		delete borrower;
		borrower = nullptr;
	} else {
		cout << "대출자 없음" << endl;
	}
}

void Book::addReserve(Student* user) // 윤재원
{
	// 이미 예약자 명단에 있을 경우 예외처리
	for (auto std:reserveStudents) {
		if (std->getId() == user->getId()) {
			cout << "이미 예약함" << endl;
			return;
		}
	}
	reserveStudents.push_back(user);
}

void Book::deleteReserve(Student* user) // 윤재원
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

bool Book::operator== (Book book) { // 윤재원
	if (this->name == book.name && this->author == book.author && this->translator == book.translator && this->publisher == book.publisher && this->publishYear == book.publishYear) {
		return true;
	}
	else return false;
}