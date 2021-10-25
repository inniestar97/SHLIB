#include "grammarCheck.h"
#include <string>
#include <string.h>
#include <iostream>
#include <regex>

using namespace std;

//완성
bool check_id(string id) {
	//앞자리 공백 삭제
   //앞자리 공백 삭제
   const auto st = id.find_first_not_of(" ");
   id = id.substr(st);

   //공백 포함여부
   if (id.find(" ") != string::npos) {
      return false;
   }

   //길이 제한
   if (id.length() < 6 || id.length() > 15)
      return false;

   //한글,특수문자 잇으면 안됨
   regex r0("[|~!@#`$=%^&*\\-_+?></.;:,\"]");
   if (regex_search(id, r0)) {
      return false;
   }

   const char* check = id.c_str();
   for (size_t i = 0; i < id.size(); i++) {
      if (check[i] & 0x80 == 1) {
         return false;
      }
   }

   regex r2("[a-zA-Z1-9_]{5,15}");
   if (regex_search(id, r2)) {
      return true;
   }
   else {
      return false;
   }

}

//완성
bool check_password(string pw) {
	//앞자리 공백 삭제
	const auto st = pw.find_first_not_of(" ");
	pw = pw.substr(st);

	//공백 포함여부
	if (pw.find(" ") != string::npos) {
		return false;
	}

	const char* check = pw.c_str();
	for (size_t i = 0; i < pw.size(); i++) {
		if (check[i] & 0x80 == 1) {
			return false;
		}
		
	}

	regex r1("[a-zA-Z1-9]{6,19}");
	if (!regex_search(pw, r1)) {
		return false;
	}

	regex xx("[a-zA-Z]");
	if (!regex_search(pw, xx)) {
		return false;
	}

	regex xx1("[1-9]");
	if (!regex_search(pw, xx1)) {
		return false;
	}

	regex r2("[|~!@#`$=%^&*\-_+?></.;:,\'\"]{1,}");
	if (!regex_search(pw, r2)) {
		return false;
	}

	//길이 제한
	if (pw.size() < 8 || pw.size() > 20)
		return false;

	return true;
}

//완성
bool check_Name(string name) {
	if (name.find(" ") != string::npos) { // 공백
		return false;
	}

	bool isKorea = true;

	const char* check = name.c_str();
	for (size_t i = 0; i < name.size(); i++) {
		if ((check[i] & 0x80) == 1) { // 한글아니면
			isKorea = false;
			break;
		}
	}

	if (!isKorea) return false;

	regex r1("[a-zA-Z1-9]{1,}");
	if (regex_search(name, r1)) {
		return false;
	}

	regex r2("[|~!@#`$=%^&*\-_+?></.;:,\'\"]{1,}");
	if (regex_search(name, r2)) {
		return false;
	}

	if (name.length() < 4 || name.length() > 12)
		return false;

	return true;

}

// 완성
bool check_book(string name) {
	return true;
}

// 완성 
bool check_author(string name) {

	bool isKorea = true;
	const char* check = name.c_str();
	for (size_t i = 0; i < name.size(); i++) {
		if (check[i] & 0x80 == 1) { // 한글이면
			isKorea = false;
		}
	}

	if (!isKorea) return false;

	regex r1("[a-zA-Z1-9]{1,}");
	if (regex_search(name, r1)) {
		return false;
	}

	regex r2("[|~!@#`$=%^&*\-_+?></.;:,\'\"]{1,}");
	if (regex_search(name, r2)) {
		return false;
	}

	return true;
}

// 완성
bool check_translator(string name) {

	if (name.length() == 0)
		return true;

	if (name.find(" ") != string::npos) { // 공백
		return false;
	}
	
	//길이 제한
	if (name.length() < 1 || name.length() > 10)
		return false;

	bool isKorea = true;
	const char* check = name.c_str();
	for (size_t i = 0; i < name.size(); i++) {
		if (check[i] & 0x80 == 1) { // 한글이면
			isKorea = false;
		}
	}

	if (!isKorea) return false;

	regex r1("[a-zA-Z1-9]{1,}");
	if (regex_search(name, r1)) {
		return false;
	}

	regex r2("[|~!@#`$=%^&*\-_+?></.;:,\'\"]{1,}");
	if (regex_search(name, r2)) {
		return false;
	}

	return true;
}


bool check_studentID(string s_id)
{
	if (s_id.length() != 9)
		return false;

//	if (s_id.find(" ")) {
//		return false;
//	}

	for (size_t i = 0; i < s_id.size(); i++) {
		if (isdigit(s_id[i]) == false) {
			return false;
		}
	}

	int id = stoi(s_id);

	if (id < 193100000)
		return false;

	return true;
}

//완성
bool check_publisher(string pub) {

	if (pub.find(" ") != string::npos) { // 공백
		return false;
	}
	regex r("[|~!@#`$=%^&*\-_+?></.;:,\'\"]{1,}");
	if (regex_search(pub, r)) {
		return false;
	}

	return true;
}

//
bool check_year(string year) {
	if (year.length() != 4)
		return false;
	return !year.empty() && find_if(year.begin(), year.end(), [](unsigned char c) { return !isdigit(c); }) == year.end();
}

int input(string msg, int a, int b){ // 입력 메시지, [a, b] 정수 입력
    int c;
    string inp;
    while (true) {
        cout << msg;
		getline(cin, inp);

        if (inp.size() == 0) {
            cout << "숫자를 입력하세요." << endl;;
            continue;
        }

		// 숫자 판별
		bool isdigit_num = true;
		for (size_t i = 0; i < inp.size(); i++) {
			if (isdigit(inp[i]) == 0) {
				cout << "숫자가 아닙니다. 하나의 숫자만 입력하세요." << endl;
				isdigit_num = false;
				
				break;
			}
		}
		
        //숫자 아니면
        if (isdigit_num == false) continue;
        
        //숫자면
		c = stoi(inp); 
        if(c > b || c < a){
            cout << a <<" ~ "<<b<< " 사이의 정수로 입력해주세요.\n";
            continue;
        }
		break;
	}
    return c;
}


string current_date; // 현재 날짜

string getCurrent_date()
{
	return current_date;
}

void setCurrent_date(string date)
{
	current_date = date;
	return;
}


int getDiff_date(string comp, string date) // 기준날짜랑 현재날짜 차이 (일수로) - 강지윤
{
	// 기준 : dft, 현재 : timet 
	time_t dft, timet;
	struct tm s_dft, stm;
	s_dft.tm_year = stoi(comp.substr(0, 4)) - 1900;
	s_dft.tm_mon = stoi(comp.substr(5, 2)) - 1;
	s_dft.tm_mday = stoi(comp.substr(8, 2));
	s_dft.tm_hour = 0; s_dft.tm_min = 0; s_dft.tm_sec = 0; s_dft.tm_isdst = 0;

	stm.tm_year = stoi(date.substr(0, 4)) - 1900;
	stm.tm_mon = stoi(date.substr(5, 2)) - 1;
	stm.tm_mday = stoi(date.substr(8, 2));
	stm.tm_hour = 0; stm.tm_min = 0; stm.tm_sec = 0; stm.tm_isdst = 0;

	dft = mktime(&s_dft);
	timet = mktime(&stm);
	double diff = difftime(timet, dft);
	int df_day, df_hour, df_min;
	df_day = diff / (int)(60 * 60 * 24);

	return df_day;
}

string getAfter_date(string date, int day) // 현재날짜로부터 day일 후 날짜 - 강지윤
{
	time_t dft, timet;
	struct tm s_dft, stm;
	stm.tm_year = stoi(date.substr(0, 4)) - 1900;
	stm.tm_mon = stoi(date.substr(5, 2)) - 1;
	stm.tm_mday = stoi(date.substr(8, 2));
	stm.tm_hour = 0; stm.tm_min = 0; stm.tm_sec = 0; stm.tm_isdst = 0;

	stm.tm_mday += day;
	int days[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	bool flag = false;
	if (stm.tm_year % 4 == 0) {
		flag = true;
		if (stm.tm_year % 100 == 0) {
			flag = false;
			if (stm.tm_year % 400 == 0) {
				flag = true;
			}
		}
	}

	if (flag) days[1] += 1;

	if (stm.tm_mday > days[stm.tm_mon]) { // 더 크면
		stm.tm_mday -= days[stm.tm_mon];
		stm.tm_mon++;
		if (stm.tm_mon > 11) { // mon은 [0 ~ 11]
			stm.tm_mon = 0;
			stm.tm_year++;
		}
	}

	string mday = "";
	if (to_string(stm.tm_mday).size() == 1) {
		mday = "0" + to_string(stm.tm_mday);
	}else {
		mday = to_string(stm.tm_mday);
	}


	string temp = to_string(stm.tm_year + 1900) + "." + to_string(stm.tm_mon + 1) + "." + mday;

	return temp;
}