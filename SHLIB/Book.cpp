#include "Book.h"
#include "Student.h"
#include "grammarCheck.h"
#include <vector>
#include <iostream>
#include <fstream>

/*	string name; // 책이름
	string author; // 책 저자
	string translator; // 역자
	string publisher; // 출판사
	string publishYear; // 발행년도
	Student borrower; // 대출자
	vector<Student> reserveStudents; // 예약한 사람들  */

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

	string info;
	file >> info;
	this->translator = info.substr(0, info.find('_'));
	info = info.substr(info.find('_') + 1, string::npos);
	this->publisher = info.substr(0, info.find('_'));
	info = info.substr(info.find('_') + 1, string::npos);
	this->publishYear = info.substr(0, info.find('_'));

	file >> info; // info -> "대출자명단"
	
	file >> info;
	if (info != "예약자명단") {  // 대출자가 있는경우
		this->borrower = new Student(info.substr(0, info.find('_'))); // borrower -> 대출 학생
		file >> info; // info-> "예약자명단"
	} else {
		this->borrower = nullptr;
	}
	getline(file, info); // 개행문자 제거
	while (getline(file, info)) { // 예약자 아이디_이름_학번
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

void Book::addBorrow(Student* student) { // 윤재원 (임시 출력 메시지)
	if (borrower == nullptr) { // 대출자가 없다면
		borrower = student;

		string bookpath = "datafile/bookDB" + name + "-" + author + ".txt";

		ofstream file(bookpath, ios::out);
		if (!file.is_open()) {
			cerr << "datafile/bookDB/" + name + "-" + author + ".txt is Not Open for addBorrow." << endl;
			exit(1);
		}

		file << translator + "_" + publisher + "_" + publishYear << endl;
		file << "대출자명단" << endl;
		file << student->getId() + "_" + student->getName() + "_" + student->getS_id() << endl;
		file << "예약자명단" << endl;
		for (size_t i = 0; i < reserveStudents.size(); i++) {
			string reID = reserveStudents[i]->getId();
			string reName = reserveStudents[i]->getName();
			string reSID = reserveStudents[i]->getS_id();
			file << reID + "_" + reName + "_" + reSID << endl;
		}
		file.close();

	} else { // 대출자가 있다면
		cout << "대출 불가" << endl;
	}
}

void Book::deleteBorrow() { // 윤재원 (임시 출력 메시지)
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
		file << "대출자명단" << endl << "예약자명단" << endl;

		for (size_t i = 0; i < reserveStudents.size(); i++) {
			string reID = reserveStudents[i]->getId();
			string reName = reserveStudents[i]->getName();
			string reSID = reserveStudents[i]->getS_id();
			file << reID + "_" + reName + "_" + reSID << endl;
		}
		file.close();

	} else {
		cout << "대출자 없음" << endl;
	}
}

void Book::addReserve(Student* user) // 윤재원 (강지윤이 리팩토링 좀 해놨는데 문제 생기면 말해주세요)
{
	// 이미 예약자 명단에 있을 경우 예외처리
	for (auto stdu : reserveStudents) {
		if (stdu == user) {
			cout << "이미 예약중인 도서입니다." << endl;
			return;
		}
	}
	reserveStudents.push_back(user);

	string bookpath = "datafile/bookDB" + name + "-" + author + ".txt";
	ofstream file(bookpath, ios::out);
	if (!file.is_open()) {
		cerr << "datafile/bookDB/" + name + "-" + author + ".txt is Not Open for addReserve." << endl;
		exit(1); // 근데 exit 지양하는 게 좋지 않을까요? 추후에 수정 예정? - 강지
	}

	file << translator + "_" + publisher + "_" + publishYear << endl;
	file << "대출자명단" << endl;
	file << borrower->getId() + "_" + borrower->getName() + "_" + borrower->getS_id() << endl;
	file << "예약자명단" << endl;
	for (size_t i = 0; i < reserveStudents.size(); i++) {
		string reID = reserveStudents[i]->getId();
		string reName = reserveStudents[i]->getName();
		string reSID = reserveStudents[i]->getS_id();
		file << reID + "_" + reName + "_" + reSID << endl;
	}
	file.close();
}

void Book::deleteReserve(Student* user) // 윤재원
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
	file << "대출자명단" << endl;
	file << borrower->getId() + "_" + borrower->getName() + "_" + borrower->getS_id() << endl;
	file << "예약자명단" << endl;
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
	// 강지윤 : 대출 가능 여부 판별
	if (borrower == nullptr) return false;
	return true;
}

vector<Student*> Book::getReservStudents() const
{
	// 강지윤 : 예약자 가져오는 
	return reserveStudents;
}

bool Book::operator== (Book book) { // 윤재원
	if (this->name == book.name && this->author == book.author && this->translator == book.translator && this->publisher == book.publisher && this->publishYear == book.publishYear) {
		return true;
	}
	else return false;
}