#pragma once
#include "User.h"
#include "Book.h"
#include "Library.h"
using namespace std;

class Student :
	public User
{
private:
	vector<Book*> bookList; // 책 리스트 (에러나서 static은 임시로 삭제)
	int current_menu; // 현재 메뉴 번호
	string name; // 학생이름
	string s_id; // 학번
	string id; // 학생아이디
	bool isOverdue; // 연체여부
	bool isBlacklist; // 블랙리스트 여부
	//int borrowBookNum; // 대출권수 (1차 구현에서는 1권만 가능)
	Book* borrow; // 대출 책은 한 권만 가능 , 대출한 책
	string borrowDate; // 대출 날짜
	string dueDate;	// 반납기한
	
	/* -> 2차구현
	struct BorrowInfo { // 대출 책 정보
		Book* book;
		long borrowDate;
	};
	*/

	vector<Book*> searchResult; // 책 검색 결과 리스트
	//vector<BorrowInfo> borrowBookList; // 대출한 책 리스트 -> 2차구현
	vector<Book*> reserveBookList; // 예약한 책 리스트
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
	void borrowBook(); // 장바구니 -> 일괄 대출
	void sel_borrowBook(); // 장바구니 -> 선택 대출
	void deleteBook(); // 장바구니 -> 도서 선택 삭제
	void reserveBook(); // 장바구니 -> 도서 선택 예약
	void myPageMenu(); // 마이페이지 메뉴
	void returnBook(int booknum); // 마이페이지 -> 책 반납
	void extendBook(int booknum); // 마이페이지 -> 책 연장
	void cancelReserveBook(int booknum); // 마이페이지 -> 책 예약 취소

	void quit(); //돌아가기 (각 메뉴마다 있는)

	void bookListPrint(vector<Book*> book, bool borrowListTF, bool nameTF, bool authorTF, bool borrowTF, bool reserveNumTF) const; // 도서 리스트 출력 - 강지윤

	void setCurrent_menu(int menu);
	void setName(string name);
	void setS_id(string s_id);
	void setId(string id);
	void setIsOverdue(bool check);
	void setIsBlacklist(bool check);

	int getCurrent_menu() const;
	string getName() const;
	string getS_id() const;
	string getId() const;
	bool getIsOverdue() const;
	bool getIsBlacklist() const;
	string getBorrowDate() const;
	string getDueDate() const;
	string getBookName() const;
	bool operator== (Student student);
};
