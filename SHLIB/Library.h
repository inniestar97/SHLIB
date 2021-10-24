#pragma once

using namespace std;

class Student; // (Student - Library) 서로 사용

class Library
{
private:
	int current_menu; // 현재 메뉴
public:
	Library();
	~Library() = default;
	void startMenu();

	void login(); // 로그인
	void makeAccount(); // 회원가입
	void initBookList(); // 북리스트 초
	// void changeDate();

	void setCurrent_menu(int current_menu);
	int getCurrent_menu() const;
};