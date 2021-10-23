#pragma once
#include <regex>

using std::string;
using std::regex;
 
bool check_id(string id);
bool check_password(string pw);
bool check_Name(string name);//회원명
bool check_book(string name);
bool check_author(string name);//저자명
bool check_translator(string name);//역자명
bool check_studentID(string s_id);
bool check_publisher(string pub);
bool check_year(string year);