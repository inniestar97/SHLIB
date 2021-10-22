#pragma once
#include "User.h"
#include "Student.h"
#include <vector>

class Admin :
	public User
{
private:
	int current_menu; // 현재 관리자 메뉴
	vector<Student> borrowList; // 대출자 리스트
	vector<Student> overdueList;  // 연체자 리스트
	vector<Student> blackList; // 블랙리스트

public:
	//Constructor
	Admin();
	//Destructor
	~Admin();

	void menu(); // 관리자 메뉴
	void addBookMenu(); // 도서추가
	void deleteBookMenu(); // 도서 삭제
	void monitoring(); // 회원 모니터링

	void setCurrent_menu(int menu);
	int getCurrent_menu() const;
	bool compare(Student &a,Student &b);//도서연체일로 정렬?
};
