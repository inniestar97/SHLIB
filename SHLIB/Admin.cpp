#include "Admin.h"
#include "grammarCheck.h"
#include <iostream>

using namespace std;

Admin::Admin()
	:current_menu(0)
{
	
}

Admin::~Admin()
{
	
}

void Admin::menu() // 관리자 메뉴
{
	int num;
	cout << "<관리자 모드>\n" << endl;
	cout << "1. 도서 추가" << endl;
	cout << "2. 도서 삭제" << endl;
	cout << "3. 회원 모니터링" << endl;
	cout << "4. 로그아웃\n\n" << endl;
	cout << "메뉴 선택 : ";

	cin >> num;
	setCurrent_menu(num);

	switch(num){
	case 1:
		addBookMenu();
		break;
	case 2:
		deleteBookMenu();
		break;
	case 3:
		monitoring();
		break;
	case 4:
		
	}

}

//고
void Admin::addBookMenu() // 도서추가
{
	cout << "<도서 추가>\n 도서명/저자명/역자/출판사/발행년도\n\n";
	cout << "1. 위와 같이 '/'구분자로 앞뒤 공백 없이 구분하여 입력해 주세요.\n";
	cout << "2. 역자가 없을 시 칸을 비워주세요. ex) 도서명/저자명//출판사/발행년도\n";
	cout << "3. ':q'를 입력 시 관릭자 모드의 메인 메뉴로 들어갑니다.\n\n";
	cout << "-----------------------------------------------------------------------\n";
	cout << "도서 정보 : ";

	// 입력한 도서가 이미 존재하는지 확인
	//있으면 메뉴로 돌아감
	cout << "이미 해당 도서가 존재합니다.\n";
	menu();

	//없으면 해당 도서를 데이터파일에 추가
	
}

//고
void Admin::deleteBookMenu() // 도서 삭제
{
	int n;
	cout << "<도서 삭제>\n";
	cout << "1. 도서명으로 삭제\n";
	cout << "2. 저자명으로 삭제\n";
	cout << "3. 돌아가기\n\n";
	cout << "선택 : ";

	cin >> n;
	switch(n) {
	case 1:
		cout << "도서명을 입력하세요 : ";
		break;
	case 2:
		cout << "저자명을 입력하세요 : ";
		break;
	case 3:
		menu();
		break;
	}

	// 입력한 도서가 존재하는지 확인
	//있으면 삭제
	//없으면 없다고 출력하고 메뉴로 돌아감

}

// 고
void Admin::monitoring() // 회원 모니터링
{
	int n, cnum;
	cout << "<회원 모니터링>\n";
	cout << "1. 연체자 명단.\n";
	cout << "2. 대출자 명단\n";
	cout << "3. 블랙리스트\n";
	cout << "4. 돌아가기\n";
	cout << "선택 : ";

	cin >> n;

	switch(n) {
	case 1:
		cout << "<연체자 명단>\n";
		for(auto omem : overdueList) {
			
		}
		cout << "블랙리스트에 추가할 회원 번호 입력 (뒤로 가려면 ':q'를 입력하세요)\n";
		cout << ">> ";
		cin >> cnum;

		break;
	case 2:
		cout << "<대출자 명단>\n";
		for(auto bmem : borrowList) {
			
		}
		cout << "(뒤로 가려면 ':q'를 입력하세요)\n";
		cout << ">> ";
		cin >> cnum;

		break;
	case 3:
		cout << "<블랙리스트>\n";
		for(auto blackmem : blackList) {
			
		}
		cout << "블랙리스트에서 제거할 회원 번호 입력 (뒤로 가려면 ':q'를 입력하세요)\n";
		cout << ">> ";
		cin >> cnum;

		break;
	case 4:
		menu();
		break;
	}
}

void Admin::setCurrent_menu(int menu)
{
	this->current_menu = menu;
}

int Admin::getCurrent_menu() const
{
	return current_menu;
}
