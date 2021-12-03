#pragma once
#include "User.h"
#include "Book.h"
#include "Library.h"
using namespace std;

typedef struct _BorrowInfo { // 대출 책 정보
	Book* book;
	string borrowDate; // 대출날짜
	string dueDate;	// 반납기한
} BorrowInfo;

class Student :
	public User
{
private:

	int current_menu; // 현재 메뉴 번호
	/*
		학생정보

		상위클래스 User.h
	string id; 아이디
	string password; 비밀번호
	------------------------------------------*/	
	string name; // 학생이름
	string s_id; // 학번
	string limitDate; // 제한상태 여부 및 제한 해제 날짜.
	/* ^제한상태일경우 limitDate = 날짜  제한상태 아닐경우 limitDate = "false" */
	int limitedStack; // 제적상태 누적
	vector<BorrowInfo> borrowBookList; // 대출한 책 리스트 -> 2차구현
	vector<Book*> reserveBookList; // 예약한 책 리스트
	/*----------------------------------------*/

	/* 소멸자 부분 신경 써 줘야 합니다. -> 각 멤버함수에서 해야함*/
	vector<Book*> bookList; // 책 리스트 (에러나서 static은 임시로 삭제)
	vector<Book*> searchResult; // 책 검색 결과 리스트
	vector<Book*> bookBasketList; // 장바구니

public:
	//Constructor
	Student() = delete;
	Student(string id);
	//Destructor
	~Student();

	void menu(); // 사용자 모드 메뉴
	void initBookList(); // 책 리스트 불러오기
	void searchBookMenu();// 자료검색 메뉴
	void bookBasketMenu(); // 장바구니 메뉴
	// void borrowBook(); // 장바구니 -> 일괄 대출 /*일괄대출 없어졌으면 삭제해두 대나용?*/
	void sel_borrowBook(); // 장바구니 -> 선택 대출
	void deleteBook(); // 장바구니 -> 도서 선택 삭제
	void reserveBook(); // 장바구니 -> 도서 선택 예약
	void myPageMenu(); // 마이페이지 메뉴
	void returnBook(int bi); // 마이페이지 -> 책 반납
	void extendBook(int bi); // 마이페이지 -> 책 연장
	void cancelReserveBook(int booknum); // 마이페이지 -> 책 예약 취소

	void bookListPrint(vector<Book*> book, bool borrowListTF, bool nameTF, bool authorTF, bool borrowTF, bool reserveNumTF) const; // 도서 리스트 출력 - 강지윤

	void setCurrent_menu(int menu);
	void setName(string name);
	void setS_id(string s_id);
	void setLimitDate(string date);

	int getCurrent_menu() const;
	string getName() const;
	string getS_id() const;
	string getLimitDate() const;
	bool operator== (Student student);
};
