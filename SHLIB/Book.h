#pragma once
#include <string>
#include <vector>
#include "Student.h"
 
using namespace std;
 
class Book
{
private:
	string name; // 책이름
	string author; // 책 저자
	string translator; // 역자
	string publisher; // 출판사
	string publishYear; // 발행년도
	Student borrower; // 대출자
	vector<Student> reserveStudents; // 예약한 사람들 
public:

	// Constructor
	Book()=delete;
	Book(string na, string au, string tr, string publisher, string year);
	// Destructor
	~Book();

	void addBorrow(Student& student); // 대출자 추가
	void deleteBorrow(); // 대출자 삭제

	void addReserve(Student& user); // 예약자 추가
	void deleteReserve(Student& user); // 예약자 삭제


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

	//void addBorrowStudent(Student& user); // 빌린사람 추가
	//void addReserveStudents(Student& user); // 예약자 추가

	bool operator== (Book book);
};

