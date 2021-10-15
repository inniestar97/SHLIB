#include "Library.h"
#include "Student.h"
#include "Admin.h"
#include "grammarCheck.h"
#include <iostream>
#include <string>

using namespace std;

Library::Library()
	:current_menu(0)
{
}

//조현서
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
			int tt;
			cin >> tt;
			if (tt!='Y'){
				return;
			}
		}
	}

	while (true) {
		cout<<"비밀번호 : ";
		cin>>t_password;

		//싪패하면 return 할지(시작화면으로 이동) continue (다시 입력) 선택
		if (!check_password(t_password)) {
			cout << "올바르지 않은 비밀번호입니다." << endl;
			cout << "다시 입력 하시려면 'Y'를, 이전화면으로 돌아가시려면아무키나 눌러주세요." << endl;
			int tt;
			cin >> tt;
			if (tt!='Y'){
				return;
			}
		}
	}


	if (t_id=="admin"){
		user=new Student();
	}else{
		user = new Admin();
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
	else if (dynamic_cast<Admin*>(user) != nullptr) { // 관리자면
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

void Library::makeAccount()
{
	
}

void Library::setCurrent_menu(int current_menu)
{
	this->current_menu = current_menu;
}

int Library::getCurrent_menu() const
{
	return current_menu;
}
