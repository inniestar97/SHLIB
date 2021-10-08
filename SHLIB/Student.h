#pragma once
#include "User.h"
#include "Book.h"

class Student :
    public User
{
private:
    int current_menu; // 현재 메뉴 번호
    string name;
    string sid;
    bool isOverdue;
    bool isBlacklist;

    struct BorrowInfo { // 대출 책 정보
        Book book;
        string returnDate; // 형변환 필요 (long 자료형?)
    };

    vector<Book> searchResult; // 책 검색 결과 리스트
    vector<Book> bookList; // 전체 책 리스트
    vector<BorrowInfo> borrowBookList; // 대출한 책 리스트
    vector<Book> reserveBookList; // 예약한 책 리스트
    vector<Book> bookBasketList; // 장바구니

public:
    //Constructor
    Student(); 
    //Destructor 
    ~Student();

    void menu(); // 사용자 모드 메뉴
    void initBookList(); // 책 리스트 불러오기
    void searchBookMenu();// 자료검색 메뉴
    void bookBasketMenu(); // 장바구니 메뉴
    void borrowBook(); // 장바구니 -> 일괄 대출
    void deleteBook(); // 장바구니 -> 도서 선택 삭제
    void reserveBook(); // 장바구니 -> 도서 선택 예약
    void myPageMenu(); // 마이페이지 메뉴
    void returnBook(int booknum); // 마이페이지 -> 책 반납
    void extendBook(int booknum); // 마이페이지 -> 책 연장
    void cancelReserveBook(int booknum); // 마이페이지 -> 책 예약 취소

    void quit(); //돌아가기 (각 메뉴마다 있는)

	void bookListPrint(int listNum, bool nameTF, bool authorTF, bool borrowTF, bool reserveNumTF); // 도서 리스트 출력 - 강지윤

    void setCurrent_menu(int menu);
    void setName(string name);
    void setSid(string sid);
    void setIsOverdue(bool check);
    void setIsBlacklist(bool check);
    int getCurrent_menu() const;
    string getName() const;
    string getsid() const;
    bool getIsOverdue() const;
    bool getIsBlacklist() const;
};

