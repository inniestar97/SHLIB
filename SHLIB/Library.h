#pragma once

class Library
{
private:
	int current_menu; // 현재 메뉴
	string current_date; // 현재 날짜
public:
	//vector<Book*> bookList; // 처음 로그인한 후 북리스트 초기화. 어떻게 할지 다음 시간에 정하겠습니다. (윤재원)
	Library();
	~Library() = default;
	void startMenu();

	void login(); // 로그인
	void makeAccount(); // 회원가입
	void initBookList(); // 북리스트 초
	// void changeDate();

	void setCurrent_menu(int current_menu);
	int getCurrent_menu() const;

	string getCurrent_date() const; // 현재 날짜
	int getDiff_date(string comp) const;	// 현재날짜 - 기준날짜  /*comp 예시 : "20211023"*/
	string getAfter_date(int day) const;	// 현재날짜로부터 day일 후 날짜

};
