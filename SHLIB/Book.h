#pragma once
#include <string>
#include <vector>

using namespace std;

class Student;

class Book
{
private:
	string name; // 책이름
	string author; // 책 저자
	//const wchar_t* translator; // 역자
	//const wchar_t* publisher; // 출판사
	string translator;
	string publisher;
	string publishYear; // 발행년도
	//string borrower; // 대출자 정보, 예약자 정보 ID로 저장하면 어떨까요
	//vector<string> reserveStudents;

	
	// Student 호출하는거 안되서 일단 주석처리
	Student* borrower; // 대출자
	//vector<Student*> reserveStudents; // 예약한 사람들
	vector<string> newrS; // 아이디_이름_학번

public:
	// Constructor
	Book() = delete;
	Book(string na, string au);
	Book(string na, string au, Student* me);
	// Destructor
	~Book();

	void addBorrow(Student* student); // 대출자 추가
	void deleteBorrow(); // 대출자 삭제

	void addReserve(Student* user); // 예약자 추가
	void deleteReserve(Student* user); // 예약자 삭제

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

	//강지윤 student - booklistprint 부분에 필요한 함수
	bool getBorrowTF() const;
	//vector<Student*> getReservStudents();
	int getReserveStudentsSize() const;
	bool isFirstRSisME(Student* me) const;
	
	bool operator== (Book book);
};
