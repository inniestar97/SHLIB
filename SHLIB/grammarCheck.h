#pragma once
#include <regex>

using std::string;
using std::regex;
 
bool check_id(string id); 
bool check_password(string pw); 
bool check_Name(string name); //회원명
bool check_book(string name); 
bool check_author(string name);//저자명
bool check_translator(string name);//역자명
bool check_studentID(string s_id);
bool check_publisher(string pub);
bool check_year(string year);
int input(string msg, int a, int b); // 입력 message, [a, b] 범위의 정수 입력


string getCurrent_date();
void setCurrent_date(string date);
int getDiff_date(string comp, string date);	// 현재날짜(date) - 기준날짜(comp)  /*comp 예시 : "20211020"*/
string getAfter_date(string date, int day);	// 현재날짜로부터 day일 후 날짜
