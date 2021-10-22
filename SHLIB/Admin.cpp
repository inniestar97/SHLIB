#include "Admin.h"
#include "grammarCheck.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

Admin::Admin()
	:current_menu(0)
{
	string info;

	ifstream borrowFile("datafile/Admin/borrowList.txt");
	if (!borrowFile.is_open()) {
		cerr << "datafile/Admin/borrowList.txt is not Open\n";
	}
	while (getline(borrowFile, info)) {
		
	}
}

Admin::~Admin()
{	
}

//완성
void Admin::menu()
{
	while (true) {
		int num;
		cout << "<관리자 모드>\n" << endl;
		cout << "1. 도서 추가" << endl;
		cout << "2. 도서 삭제" << endl;
		cout << "3. 회원 모니터링" << endl;
		cout << "4. 로그아웃\n\n" << endl;
		cout << "메뉴 선택 : ";
		cin >> num;
		setCurrent_menu(num);
		switch (num) {
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
			return;
		}
	}
}

//고
void Admin::addBookMenu() // 도서추가
{
	while (true) {
		cout << "<도서 추가>\n 도서명/저자명/역자/출판사/발행년도\n\n";
		cout << "1. 위와 같이 '/'구분자로 앞뒤 공백 없이 구분하여 입력해 주세요.\n";
		cout << "2. 역자가 없을 시 칸을 비워주세요. ex) 도서명/저자명//출판사/발행년도\n";
		cout << "3. ':q'를 입력 시 관릭자 모드의 메인 메뉴로 들어갑니다.\n\n";
		cout << "-----------------------------------------------------------------------\n";
		cout << "도서 정보 : ";

		string inp_s;
		cin >> inp_s;
		if (inp_s == ":q") {
			return;
		}

		vector<string> a; // 입력된 도서정보
		//입력 도서의 문법 규칙 확인
		size_t prev = 0, cur;
		cur = inp_s.find('/'); // 구분자: '/'
		while (cur != string::npos) 
		{
			string sub_str = inp_s.substr(prev, cur - prev); // 문자열 split
			a.push_back(sub_str);
			prev = cur + 1;
			cur = inp_s.find('/', prev);
		}
		a.push_back(inp_s.substr(prev, cur - prev));// 마지막 split

		//문법 규칙 검사
		if (!check_book(a[0])) {
			cout << "책제목이 문법 형식에 맞지 않습니다";
			continue;
		}
		if (!check_author(a[1])) {
			cout << "저자명이 문법 형식에 맞지 않습니다";
			continue;
		}
		if (!check_translator(a[2])) {
			cout << "역자가 문법 형식에 맞지 않습니다";
			continue;
		}
		if (!check_publisher(a[3])) {
			cout << "출판사가 문법 형식에 맞지 않습니다";
			continue;
		}
		if (!check_year(a[4])) {
			cout << "발행년도가 문법 형식에 맞지 않습니다";
			continue;
		}

		// 입력한 도서가 이미 존재하는지 확인
		string book_file_name = a[0] + "-" + a[1] + ".txt";
		fstream new_book_file("datafile/bookDB/" + book_file_name);
		if (new_book_file.is_open()) { // 파일이 이미 존재하는 경우
			cout << "이미 해당 도서가 존재합니다.\n";
			new_book_file.close();
			continue;
		}

		//없으면 해당 도서를 데이터파일에 추가
		string write_new_book_file;
		if (a[2] == "") { // 역자가 없다면
			write_new_book_file = "._" + a[3] + "_" + a[4] + "\n";
		}
		else {
			write_new_book_file = a[2] + "_" + a[3] + "_" + a[4] + "\n";
		}
		new_book_file << write_new_book_file;
		new_book_file << "대출자명단\n예약자명단\n";

		new_book_file.close();

		ofstream new_book_info("datafile/bookSearch.txt", ios::app);
		if (!new_book_info.is_open()) {
			cerr << "Cannot open the datafile/bookSearch.txt" << endl;
			exit(1000);
		}
		new_book_info << a[0] + "_" + a[1] + "_" + write_new_book_file << "_true_0\n";
		a.clear();
	}
}

//고
void Admin::deleteBookMenu() // 도서 삭제
{
	int n;
	while(true){
		cout << "<도서 삭제>\n";
		cout << "1. 도서명으로 삭제\n";
		cout << "2. 저자명으로 삭제\n";
		cout << "3. 돌아가기\n\n";
		cout << "선택 : ";
		cin >> n;
		string a_name;
		string b_name;

		switch (n) {
		case 1:
			cout << "도서명을 입력하세요 : ";
			cin >> b_name;
			// 있는가->삭제

			// 없는가-> 없다 출력 -> 끝
				
			break;
		case 2:
			cout << "저자명을 입력하세요 : ";
			cin >> a_name;

			//있는가 -> 삭제

			//없는가 -> 없다 출력 끝

			break;
		case 3:
			return;
		default:
			cout<<"1~3사이의 정수를 입력해 주세요"<<endl;
		}

	}
}

// 고
void Admin::monitoring() // 회원 모니터링
{
	int n;
	string cnum;
	while(true){
		cout << "<회원 모니터링>\n";
		cout << "1. 연체자 명단.\n";
		cout << "2. 대출자 명단\n";
		cout << "3. 블랙리스트\n";
		cout << "4. 돌아가기\n";
		cout << "선택 : ";

		cin >> n;
		int i = 0;
		
		switch(n) {
		case 1:
			cout << "<연체자 명단>\n";
			for(auto omem : overdueList) {
				i++;
				cout<< i<<". "<<omem.getName()<<endl;
			}
			
			while(cnum != ":q") {
				cout << "블랙리스트에 추가할 회원 번호 입력 (뒤로 가려면 ':q'를 입력하세요)\n";
				cout << ">> ";
				cin >> cnum;	
				int c= stoi(cnum);
				//overdueList[i-1]블랙리스트에 추가  -> 이미 블랙리스트에 존재하면? - 기획서엔 없음
				blackList.push_back(overdueList[c-1]);
					
				//파일에서도 제거(overdueList[i-1].getName().txt, admin.txt 모두)

			}
			break;
		case 2:
			cout << "<대출자 명단>\n";
			borrowList.sort(borrowList.begin(),borrowList.end(),compare);
			while(cnum!=":q"){
				cout<< "[학번] [이름] [대출중인 도서] [대출일] [반납예정일]"<<endl;
				for(auto bmem : borrowList) {
					// 대출중인 도서가 여러개라면 ? - 기획서에 없음 - 정렬도 해야함
					cout<< bmem.getsid() <<" "<<bmem.getName() <<" "<< endl;
				}

				cout << "(뒤로 가려면 ':q'를 입력하세요)\n";
				cout << ">> ";
				cin >> cnum;
			}
			break;
		case 3:
			cout << "<블랙리스트>\n";
			while(true){
				cout<<" [학번] [이름] [블랙리스트 변경일]"<<endl;//블랙리스트 변경일이 뭔지 모르겟네요.

				for(auto blackmem : blackList) {
					//출력 형식 고쳐야함 - 블랙리스트 변경일?
					cout<< blackmem.getsid()<<" "<<blackmem.getName()<<" "<< endl;
				}
				cout << "블랙리스트에서 제거할 회원 번호 입력 (뒤로 가려면 ':q'를 입력하세요)\n";
				cout << ">> ";
				cin >> cnum;
				if(cnum==":q")
					break;
				else{
					//블랙리스트에서 blackmem[c-1]제거
					int c =stoi(cnum);
					blackList.erase(blackList.begin() + c-1);
					
					//파일에서도 제거(blackmem[c-1].getName().txt, admin.txt모두)

				}
			}
			break;
		case 4:
			return;
		default:
			cout<<"1~4사이의 정수를 입력해 주세요"<<endl;
		}
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
bool Admin::compare(Student &a,Student &b)
{
	//정렬 어떻게함..
}