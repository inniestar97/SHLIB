#include "Library.h"
#include "Student.h"
#include "Admin.h"
#include "grammarCheck.h"
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <locale>
#include <filesystem>
#include <io.h>

using namespace std;

Library::Library()
	:current_menu(0)
{
	// 강지윤 
	time_t timet = time(nullptr);
	struct tm stm; 
	localtime_s(&stm, &timet);

	char buf[100];
	
	if (strftime(buf, sizeof(buf), "%Y%m%d", &stm)) { //Year Month Day 붙어서 저장
		//current_date = buf;
		setCurrent_date(buf);
	}
} 

//완성
void Library::startMenu()
{
	int num;
	while (true) {
		cout << "1. 로그인" << endl;
		cout << "2. 회원 가입" << endl;
		cout << "3. 시스템 종료" << endl;

		cin >> num;
		setCurrent_menu(num);

		switch (num) {
		case 1:
			login();
			break;
		case 2:
			makeAccount();
			break;
		case 3:
			return;// 종료
		}
	}
}

//조현서 확인중 - 파일 여닫기 체크 해야함
void Library::login()
{
	User* user;
	string t_id;
	string t_password;
	int tt;
	
	ifstream read_ID_file;
	// 로그인 정보 입력
	while (true) {
		cout << "아이디 : ";
		cin >> t_id;

		// 아이디 문법 형식 확인 -  실패하면 return 할지(시작화면으로 이동) continue (다시 입력) 선택
		if (!check_id(t_id)) {
			cout << "올바르지 않은 아이디입니다." << endl;
			cout << "다시 입력 하시려면 'Y'를, 이전화면으로 돌아가시려면아무키나 눌러주세요." << endl;
			cin >> tt;
			if (tt != 'Y')
				return;
			else
				continue;
		}

		// 아이디 존재 여부 확인
		string id_file = "datafile/User/" + t_id + ".txt";
		if (_access(id_file.c_str(), 0) == -1)// 아이디가 존재하지 않을경우
		{
			cout << "존재하지 않는 아이디입니다." << endl;
			cout << "다시 입력 하시려면 'Y'를, 이전화면으로 돌아가시려면아무키나 눌러주세요." << endl;
			cin >> tt;
			if (tt != 'Y')
				return;
			else
				continue;
		}
		else { // 아이디가 존재하는 경우
			read_ID_file.open(id_file);
			break;
		}
	}

	while (true) {
		cout << "비밀번호 : ";
		cin >> t_password;

		//실패하면 return 할지(시작화면으로 이동) continue (다시 입력) 선택
		if (!check_password(t_password)) {
			cout << "올바르지 않은 비밀번호입니다." << endl;
			cout << "다시 입력 하시려면 'Y'를, 이전화면으로 돌아가시려면아무키나 눌러주세요." << endl;
			cin >> tt;
			if (tt != 'Y') {
				read_ID_file.close();
				return;
			}
			else
				continue;
		}

		string std_info; // 학생 패스워드
		getline(read_ID_file, std_info);
		string std_password = std_info.substr(0, std_info.find('_' | '\n'));
		if (std_password != t_password) { // 사용자의 password 와 다르면
			cout << "회원님의 비밀번호와 일치하지 않습니다." << endl;
			cout << "다시 입력 하시려면 'Y'를, 이전화면으로 돌아가시려면아무키나 눌러주세요." << endl;
			cin >> tt;
			if (tt != 'Y') {
				read_ID_file.close();
				return;
			}
			else
				continue;
		}
		else { // 비밀번호가 일치하는경우
			break;
		}
	}
	read_ID_file.close();

	if (t_id == "admin") {
		user = new Admin();
	}
	else {
		user = new Student(t_id);
	}

	if (dynamic_cast<Student*>(user) != nullptr) { //학생이면
		Student* std = dynamic_cast<Student*>(user);
		while (std->getCurrent_menu() != 4) {
			std->menu();
			// 메뉴에서 로그아웃 (4번) 선택시 while문을 빠져나옴
		}
		delete std;
		std = nullptr;
	}
	else if (dynamic_cast<Admin*>(user) != nullptr) { // 관리자 라면
		Admin* ad = dynamic_cast<Admin*>(user);
		while (ad->getCurrent_menu() != 4) {
			ad->menu();
			// 메뉴에서 로그아웃 (4번) 선택시 while문을 빠져나옴
		}
		delete ad;
		ad = nullptr;
	}

	delete user;
	user = nullptr;
}

// 조현서 회원가입
void Library::makeAccount()
{
	string t_id;
	int tt;

	while (true) {
		cout << "아이디 : ";
		cin >> t_id;

		// 아이디 문법 형식 확인
		//싪패하면 return 할지(시작화면으로 이동) continue (다시 입력) 선택
		if (!check_id(t_id)) {
			cout << "올바르지 않은 아이디입니다." << endl;
			cout << "다시 입력 하시려면 'Y'를, 이전화면으로 돌아가시려면아무키나 눌러주세요." << endl;
			cin >> tt;
			if (tt != 'Y') {
				return;
			}
		}

		string id_file = "datafile/User/" + t_id + ".txt";
		if (_access(id_file.c_str(), 0) == 0) { // 아이디가 존재할 경우
			/*이미 존재하는 아이디 체크*/
			cout << "이미 존재하는 아이디입니다." << endl;
			cout << "다시 입력 하시려면 'Y'를, 이전화면으로 돌아가시려면아무키나 눌러주세요." << endl;
			cin >> tt;
			if (tt != 'Y')
				return;
		}
		else { // 아이디가 존재하지 않는 경우
			break;
		}
	}

	string t_password;
	while (true) {
		cout << "비밀번호 : ";
		cin >> t_password;

		if (!check_password(t_password)) {
			cout << "올바르지 않은 비밀번호입니다." << endl;
			cout << "다시 입력 하시려면 'Y'를, 이전화면으로 돌아가시려면아무키나 눌러주세요." << endl;
			cin >> tt;
			if (tt != 'Y') {
				return;
			}
		}
	}

	string t_name;
	while (true) {
		cout << "이름 : ";
		cin >> t_name;

		if (!check_Name(t_name)) {
			cout << "올바르지 않은 이름입니다." << endl;
			cout << "다시 입력 하시려면 'Y'를, 이전화면으로 돌아가시려면아무키나 눌러주세요." << endl;
			cin >> tt;
			if (tt != 'Y') {
				return;
			}
		}
	}

	string t_sid;
	while (true) {
		cout << "학번 : ";
		cin >> t_sid;

		if (!check_studentID(t_sid)) {
			cout << "올바르지 않은 학번입니다." << endl;
			cout << "다시 입력 하시려면 'Y'를, 이전화면으로 돌아가시려면아무키나 눌러주세요." << endl;
			cin >> tt;
			if (tt != 'Y') {
				return;
			}
		}

		bool flag = false;
		//이미 가입되어 있는 학번인지 확인->c++17
		for (auto& file : filesystem::directory_iterator(filesystem::current_path().string() + "datafile/User/")) 
		{
			ifstream fs(file.path());    //open the file	
			
			string info;
			fs >> info; // 비밀번호_이름_학번
			
			string t1,t2,fsid;
			t1 = info.substr(info.find('_') + 1, string::npos); // 이름_학번
			t2 = info.substr(0, info.find('_')); // 이름
			fsid = info.substr(info.find('_') + 1, string::npos); // 학번
			fs.close();
			if (fsid == t_sid){
				flag= true;
				break;
			}
		}

		if (flag) {
			cout << "이미 가입되어 있는 학번입니다." << endl;
			cout << "다시 입력 하시려면 'Y'를, 이전화면으로 돌아가시려면아무키나 눌러주세요." << endl;
			cin >> tt;
			if (tt != 'Y') {
				return;
			}		
		} else {
			break;
		}
	}

	// 개인 파일 생성 완료
	ofstream new_student_file("datafile/User/" + t_id + ".txt");

	new_student_file << t_password << "_" << t_name << "_" << t_sid << endl;
	new_student_file << "false" << endl; // 연체여부 초기화 false
	new_student_file << "false" << endl << endl; // blackList 여부 초기화 false

	new_student_file << "대출도서정보" << endl;
	new_student_file << "예약도서정보" << endl;

	new_student_file.close();
}

void Library::setCurrent_menu(int current_menu)
{
	this->current_menu = current_menu;
}

int Library::getCurrent_menu() const
{
	return current_menu;
}
