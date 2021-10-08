#include "Library.h"
#include "Student.h"
#include "Admin.h"
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

//조현서
void Library::login()
{
	User* user;
	string t_id;
	string t_password;
	
	// 로그인 정보 입력
	while (true){
		cout<<"아이디 : ";
		cin>>t_id;
		
		// 아이디 문법 형식 확인
		
		//싪패하면 return 할지(시작화면으로 이동) continue (다시 입력) 선택


		cout<<"비밀번호 : ";
		cin>>t_password;



		//성공하면 break

		//싪패하면 return 할지(시작화면으로 이동) continue (다시 입력) 선택

	}

	/*
	들어온 아이디에 따라 둘 중 하나 동적할당
	*/

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
