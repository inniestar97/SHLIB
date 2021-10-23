#pragma once
using namespace std;
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
string current_date; // 현재 날짜
string getCurrent_date(); // 현재 날짜
int getDiff_date(string comp, string date);	// 현재날짜(date) - 기준날짜(comp)  /*comp 예시 : "20211023"*/
string getAfter_date(string date, int day);	// 현재날짜로부터 day일 후 날짜