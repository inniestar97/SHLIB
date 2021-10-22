#include "Library.h"
#include "Student.h"
#include "Admin.h"
#include "grammarCheck.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

Library::Library()
	:current_menu(0)
{
}

//완성
void Library::startMenu()
{
	int num;
	while(true){
		cout << "1. 로그인" << endl;
		cout << "2. 회원 가입" << endl;
		cout << "3. 시스템 종료" << endl;

		cin >> num;
		setCurrent_menu(num);

		switch(num){
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

//조현서 보류중 데이터 파일 필요부분 안함
void Library::login()
{
	User* user;
	string t_id;
	string t_password;
	int tt;
	// 로그인 정보 입력
	while (true) {
		cout << "아이디 : ";
		cin >> t_id;

		// 아이디 문법 형식 확인 ->
		/*이미 존재하는 아이디 체크  x*/

		//싪패하면 return 할지(시작화면으로 이동) continue (다시 입력) 선택
		if (!check_id(t_id)) {
			cout << "올바르지 않은 아이디입니다." << endl;
			cout << "다시 입력 하시려면 'Y'를, 이전화면으로 돌아가시려면아무키나 눌러주세요." << endl;
			cin >> tt;
			if (tt!='Y'){
				return;
			}
		}
	}

	while (true) {
		cout<<"비밀번호 : ";
		cin>>t_password;

		//실패하면 return 할지(시작화면으로 이동) continue (다시 입력) 선택
		if (!check_password(t_password)) {
			cout << "올바르지 않은 비밀번호입니다." << endl;
			cout << "다시 입력 하시려면 'Y'를, 이전화면으로 돌아가시려면아무키나 눌러주세요." << endl;
			cin >> tt;
			if (tt!='Y'){
				return;
			}
		}
	}

	if (t_id=="admin"){
		user = new Admin();
	}else{
		user = new Student();
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
	
	else if (dynamic_cast<Admin*>(user) != nullptr) {
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

		// 아이디 문법 형식 확인 ->
		/*이미 존재하는 아이디 체크  x*/

		//싪패하면 return 할지(시작화면으로 이동) continue (다시 입력) 선택
		if (!check_id(t_id)) {
			cout << "올바르지 않은 아이디입니다." << endl;
			cout << "다시 입력 하시려면 'Y'를, 이전화면으로 돌아가시려면아무키나 눌러주세요." << endl;
			cin >> tt;
			if (tt!='Y'){
				return;
			}
		}
	}

	string t_password;
	while (true) {
		cout<<"비밀번호 : ";
		cin>>t_password;

		if (!check_password(t_password)) {
			cout << "올바르지 않은 비밀번호입니다." << endl;
			cout << "다시 입력 하시려면 'Y'를, 이전화면으로 돌아가시려면아무키나 눌러주세요." << endl;
			cin >> tt;
			if (tt!='Y'){
				return;
			}
		}
	}

	string t_name;
	while (true) {
		cout<<"이름 : ";
		cin>>t_name;

		if (!check_Name(t_name)) {
			cout << "올바르지 않은 이름입니다." << endl;
			cout << "다시 입력 하시려면 'Y'를, 이전화면으로 돌아가시려면아무키나 눌러주세요." << endl;
			cin >> tt;
			if (tt!='Y'){
				return;
			}
		}
	}

	string t_sid;
	while (true) {
		cout<<"학번 : ";
		cin>>t_sid;
		//이미 가입되어 있는 학번인지 확인해야함
		if (!check_studentID(t_sid)) {
			cout << "올바르지 않은 학번입니다." << endl;
			cout << "다시 입력 하시려면 'Y'를, 이전화면으로 돌아가시려면아무키나 눌러주세요." << endl;
			cin >> tt;
			if (tt!='Y'){
				return;
			}
		}
	}

	// 개인 파일 생성 완료 -> 이상인 좋아용
	ofstream new_student_file("datafile/Student/" + t_id + ".txt");
	if (!new_student_file.is_open()) {
		cerr << "datafile/Student/" + t_id + ".txt	file is Not Open" << endl;
	}

	new_student_file << t_password << "_" << t_name << "_" << t_sid << endl;
	new_student_file << "false" << endl; // 연체여부 초기화 false
	new_student_file << "false" << endl << endl; // blackList 여부 초기화 false

	new_student_file << "대출 도서 정보" << endl;
	new_student_file << "예약 도서 정보" << endl;

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
