#include "Admin.h"
#include "grammarCheck.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <filesystem>
#include <sstream>
#include <io.h>

using namespace std;

Admin::Admin()
	:current_menu(0)
{
	/*
	 * Admin로그인시 대출자리스트, 블랙리스트의 모든 파일내용을 불러옴 
	 * 불러온 후, 각 멤버변수 vector 에 push
	 */
	string info;
	ifstream borrowFile, blackFile, bookFile;
	//ifstream overdueFile;
	borrowFile.open("datafile/User/forAdmin/borrowList.txt");
	if (!borrowFile.is_open()) {
		cerr << "datafile/User/forAdmin/borrowList.txt is not Open\n";
		exit(1);
	} else {
		while (getline(borrowFile, info)) {
			string studentID = info.substr(0, info.find('_')); // 학생아이디
			borrowList.push_back(new Student(studentID)); // 학생정보 -> 대출자리스트에
		}
	}
	while (borrowFile.is_open()) borrowFile.close();

	blackFile.open("datafile/User/forAdmin/blackList.txt");
	if (!blackFile.is_open()) {
		cerr << "datafile/User/forAdmin/borrowList.txt is not Open\n";
		exit(1);
	} else {
		while (getline(blackFile, info)) {
			string studentID = info.substr(0, info.find('_'));
			blackList.push_back(new Student(studentID)); // 학생정보 -> 블랙 리스트에
		}
	}
	while (blackFile.is_open()) blackFile.close();

	for (auto& file : filesystem::directory_iterator(filesystem::current_path().string() + "\\datafile\\bookDB\\")) {
		string path = file.path().string();

		stringstream ss(path);
		vector<string> last_path;
		string split;
		while (getline(ss, split, '\\')) {
			last_path.push_back(split);
		}

		split = last_path[last_path.size() - 1]; // 디렉토리 "책.txt"
		last_path.clear();

		string na = split.substr(0, split.find('-'));
		string au = split.substr(split.find('-') + 1, split.find(".txt") - (split.find('-') + 1));

		booklist.push_back(new Book(na, au));
	}
	
	// 대체자 명단에서 연체자 명단 추출
	for (Student* bmem: borrowList){
		if (getDiff_date(bmem->getDueDate(), getCurrent_date()) > 0){ // 원래 : stoi(bmem->getDueDate()) - stoi(getCurrent_date()) < 0
			overdueList.push_back(bmem);
			bmem->setIsOverdue(true);
		}
	}
}

Admin::~Admin()
{
	for (size_t i = 0; i < borrowList.size(); i++) {
		delete borrowList.at(i);
		borrowList.at(i) = nullptr;
	}
	borrowList.clear();
	for (size_t i = 0; i < borrowList.size(); i++) {
		delete overdueList.at(i);
		overdueList.at(i) = nullptr;
	}
	overdueList.clear();
	for (size_t i = 0; i < borrowList.size(); i++) {
		delete blackList.at(i);
		blackList.at(i) = nullptr;
	}
	blackList.clear();
}

void Admin::menu()
{
	while (true) {
		int num;
		cout << "<관리자 모드>\n" << endl;
		cout << "1. 도서 추가" << endl;
		cout << "2. 도서 삭제" << endl;
		cout << "3. 회원 모니터링" << endl;
		cout << "4. 로그아웃\n" << endl;
		num = input("\n메뉴 선택 : ", 1, 4);
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
			string path = "datafile/User/forAdmin/blackList.txt";
			ofstream file(path, ios::out);
			if (!file.is_open()) {
				cerr << "blackList file is not open for change" << endl;
				exit(1);
			}

			for (size_t i = 0; i < blackList.size(); i++) {
				file << blackList[i]->getId() << "_" << blackList[i]->getName() << "_" << blackList[i]->getS_id() << endl;
			}

			while (file.is_open()) file.close();

			return;
		}
	}
}

//완성
void Admin::addBookMenu() // 도서추가
{
	while (true) {
		cout << "<도서 추가>\n 도서명/저자명/역자/출판사/발행년도\n\n";
		cout << "1. 위와 같이 '/'구분자로 앞뒤 공백 없이 구분하여 입력해 주세요.\n";
		cout << "2. 역자가 없을 시 칸을 비워주세요. ex) 도서명/저자명//출판사/발행년도\n";
		cout << "3. \":q\"를 입력 시 관리자 모드의 메인 메뉴로 들어갑니다.\n\n";
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
		string book_file_name = "datafile/bookDB/" + a[0] + "-" + a[1] + ".txt";
		if (_access(book_file_name.c_str(), 0) == 0) { // 파일이 이미 존재하는 경우
			cout << "이미 해당 도서가 존재합니다.\n";
			continue;
		}

		ofstream new_book_file(book_file_name); // 쓰기 파일 생성
		//없으면 해당 도서를 데이터파일에 추가
		string write_new_book_file;
		if (a[2] == "") { // 역자가 없다면
			write_new_book_file = "없음_" + a[3] + "_" + a[4];
		} else { // 역자가 있다면
			write_new_book_file = a[2] + "_" + a[3] + "_" + a[4];
		}
		new_book_file << write_new_book_file << endl;
		new_book_file << "대출자명단\n예약자명단\n";
		new_book_file.close();
		
		booklist.push_back(new Book(a[0], a[1])); // 책 리스트에 추가하고 종료
		a.clear();
	}
}

//완성
void Admin::deleteBookMenu() // 도서 삭제 
{
	int n;
	int i = 0;
	bool flag = false;
	while(true){
		cout << "<도서 삭제>\n";
		cout << "1. 도서명으로 삭제\n";
		cout << "2. 저자명으로 삭제\n";
		cout << "3. 돌아가기\n";
		n = input("\n선택 : ", 1, 3);

		string a_name; // 저자명
		string b_name; // 도서명

		switch (n) {
		case 1:
			while(true) {
				cout << "도서명을 입력하세요(뒤로 가려면 \":q\"를 입력하세요) :";
				cin >> b_name;
				i = 0;
				if (b_name==":q")
					break;
				if (!check_book(b_name)) {
					cout << "도서명이 문법 형식에 맞지 않습니다";
					continue;
				}
				vector<Book*> a;
				flag = false;
				//일치하는 책 목록 생성
				for (Book* book : booklist) {
					if	(book->getName() == b_name) {
						a.push_back(book);
						flag = true;
					}

				}
				//책이 있는가
				if (flag) {
					string inp;
					int c;
					while(true) {
						i = 0;
						cout<<"도서명 : "<< b_name <<endl;
						cout<<"    [저자명]\t[역자]\t[출판사]\t[발행년도]"<<endl;
						for (Book* book : a){
							i++;
							cout<<i<<". "<<book->getAuthor()<<"\t"<<book->getTranslator()<<"\t"<<book->getPublisher()<<"\t"<<book->getPublishYear()<<endl;
						}
						cout<<"번호 입력(뒤로 가려면 \":q\"를 누르세요): ";
						cin.ignore();
						getline(cin, inp);
						if(inp==":q")
							break;
						// 숫자가 아니면
						bool isdigit_num = true;
						for (size_t i = 0; i < inp.size(); i++) {
							if (isdigit(inp[i]) == 0) {
								cout << "숫자가 아닙니다" << endl;
								isdigit_num = false;
								break;
							}
						}	
						if (isdigit_num == false) continue;
						
						c = stoi(inp);
						
						if (c<1 || c>a.size()){
							cout << "올바른 범위가 아닙니다." << endl;
							continue;
						}
						//누군가가 이미 빌렸으면 -> 다시 입력받아요!
						if (a[c-1]->getBorrowTF()){
							cout<<"아직 반납되지 않은 책입니다"<<endl;
						}
						else{
							//벡터 수정
							booklist.erase(booklist.begin() + c-1);
							//도서 파일 삭제
							string str = "datafile/bookDB/" + a[c-1]->getName() + "-" + a[c-1]->getAuthor() + ".txt";
							remove(str.c_str());
							cout<<"삭제 완료!"<<endl;
							return;
						}
					}
					
					break;
				}
				else
					cout<<"해당 도서가 존재하지 않습니다."<<endl;
			}
			break;
		case 2:
			while(true) {
				cout << "저자명을 입력하세요(뒤로 가려면 \":q\"를 입력하세요) :";
				cin >> a_name;
				i = 0;
				if (a_name==":q")
					break;
					
				if (!check_author(a_name)) {
					cout << "저자명이 문법 형식에 맞지 않습니다";
					continue;
				}
				vector<Book*> a;
				//일치하는 책 목록 생성
				for (Book* book : booklist) {
					if	(book->getAuthor() == a_name) {
						a.push_back(book);
						flag = true;
					}
				}
				//책이 있는가
				if (flag) {
					string inp;
					int c;
					while(true) {
						i = 0;
						cout<<"저자명 : "<< a_name <<endl;
						cout<<"    [도서명]	  [역자]	    [출판사]     [발행년도]";
						for (Book* book : a) {
							i++;
							cout<<i<<". "<<book->getName()<<" "<<book->getTranslator()<<" "<<book->getPublisher()<<" "<<book->getPublishYear()<<endl;
						}
						getline(cin, inp);
						if(inp==":q")
							break;
						// 숫자가 아니면
						bool isdigit_num = true;
						for (size_t i = 0; i < inp.size(); i++) {
							if (isdigit(inp[i]) == 0) {
								cout << "숫자가 아닙니다" << endl;
								isdigit_num = false;
								break;
							}
						}	
						if (isdigit_num == false) continue;
						c=stoi(inp);
						if (c < 1 || c > a.size()) {
							cout << "올바른 범위가 아닙니다." << endl;
							continue;
						}
						//누군가가 이미 빌렸으면
						if (a[c-1]->getBorrowTF()) {
							cout<<"아직 반납되지 않은 책입니다"<<endl;
						}
						else{
							//벡터수정
							booklist.erase(booklist.begin() + c-1);
							//도서 파일 삭제
							string str = "datafile/bookDB/" + a[c-1]->getName() + "-" + a[c-1]->getAuthor() + ".txt";
							remove(str.c_str());
							cout<<"삭제 완료!"<<endl;
							return;
						}
					}	
					break;
				}
				else
					cout<<"해당 도서가 존재하지 않습니다."<<endl;
			}
			break;
		case 3:
			return;
		default:
			cout<<"1~3사이의 정수를 입력해 주세요"<<endl;
		}
	}
}


//완성
void Admin::monitoring() // 회원 모니터링
{
	int n;
	string cnum;
	while(true) {
		cout << "<회원 모니터링>\n";
		cout << "1. 연체자 명단.\n";
		cout << "2. 대출자 명단\n";
		cout << "3. 블랙리스트\n";
		cout << "4. 돌아가기\n";

		n = input("\n선택 : ", 1, 4);

		int i = 0, c;

		switch(n) {
		case 1:
			cout << "<연체자 명단>\n";
			cout<< " [학번] [이름] [대출중인 도서] [대출일] [반납일] [연체일수] " <<endl;
			for(Student* omem : overdueList) {//연체일수 남음
				i++;
				cout<< i <<". "<< omem->getS_id()<< " " << omem->getName() <<" "<< omem->getBorrowDate() <<" "<< omem->getDueDate()<<" "<< getDiff_date(omem->getDueDate(), getCurrent_date())<<endl;
			}
			
			while(cnum != ":q") {
				cout << "블랙리스트에 추가할 회원 번호 입력 (뒤로 가려면 \":q\"를 입력하세요)\n";
				for (size_t i = 0; i < 3; i++) {
					cout << "." << endl;
				}
				while (true) {
					cout << ">> ";
					//cin >> cnum;
					getline(cin, cnum);
					if (cnum == ":q") break;
					// 숫자가 아니면
					bool isdigit_num = true;
					for (size_t i = 0; i < cnum.size(); i++) {
						if (isdigit(cnum[i]) == 0) {
							cout << "숫자가 아닙니다." << endl;
							isdigit_num = false;
							break;
						}
					}
					if (isdigit_num == false) continue;

					// 숫자면
					c = stoi(cnum); // 숫자로 변경
					if (c >= 1 && c <= overdueList.size()) { // 범위 안에 있다면
						break;
					}
					else {
						cout << "올바른 범위가 아닙니다." << endl;
					}
				}

				if (cnum == ":q") break;
				c = stoi(cnum);
				//overdueList[c-1]블랙리스트에 추가
				bool isinBlack = false;
				for(Student* bmem : blackList) {
					if (bmem->getS_id() == overdueList[c - 1]->getS_id()){
						isinBlack = true;
						break;
					}
				}
				if (isinBlack) {
					cout<<"이미 블랙리스트에 있는 멤버입니다."<<endl;
				}else {
					blackList.push_back(overdueList[c - 1]);

					overdueList[c - 1]->setIsOverdue(false);
					overdueList[c - 1]->setIsBlacklist(true);
					// 책 자동 반납
					overdueList[c - 1]->returnBook();
					cout<<"블랙리스트 추가 완료!"<<endl;
					return;
				}

			}
			cnum = "";
			break;
		case 2:
			cout << "<대출자 명단>\n";
			//borrowlist정렬			
    		for (int ii = 0; ii < borrowList.size(); ii++) {
				for (int jj = 0; jj < borrowList.size() - ii - 1; jj++) {
					if (borrowList[jj] > borrowList[jj + 1]) {
						Student* temp = borrowList[jj];
						borrowList[jj] = borrowList[jj + 1];	
						borrowList[jj + 1] = borrowList[jj];
					}
				}
			}
			while(cnum!=":q"){
				cout << "[학번] [이름] [대출중인 도서] [대출일] [반납예정일]" << endl;
				for (Student* bmem : borrowList) {
					i++;
					cout<< i<<". "<<bmem->getS_id()<< " " << bmem->getName() << " " <<bmem->getBookName()<<" "<< bmem->getBorrowDate()<<" "<<bmem->getDueDate()<<endl;
				}

				cout << "(뒤로 가려면 \":q\"를 입력하세요)\n";
				for (size_t i = 0; i < 3; i++) {
					cout << "." << endl;
				}
				cout << ">> ";
				cin >> cnum;
			}
			cnum = "";
			break;
		case 3:
			cout << "<블랙리스트>\n";
			while(true){
				cout<<" [학번] [이름]"<<endl;

				for (Student* blackmem : blackList) {
					i++;
					cout<<i<<". "<<blackmem->getS_id()<<" "<<blackmem->getName()<<" "<< endl;
				}
				cout << "블랙리스트에서 제거할 회원 번호 입력 (뒤로 가려면 \":q\"를 입력하세요)\n";
				for (size_t i = 0; i < 3; i++) {
					cout << "." << endl;
				}
				while (true) {
					cout << ">> ";
					getline(cin, cnum);
					if (cnum == ":q") {
						break;
					}
					// 숫자가 아니면
					bool isdigit_num = true;
					for (size_t i = 0; i < cnum.size(); i++) {
						if (isdigit(cnum[i]) == 0) {
							cout << "숫자가 아닙니다" << endl;
							isdigit_num = false;
							break;
						}
					}
					if (isdigit_num == false) continue;

					// 숫자면
					c = stoi(cnum); // 숫자로 변경
					if (c >= 1 && c <= blackList.size()) { // 범위 안에 있다면
						break;
					}
					else {
						cout << "올바른 범위가 아닙니다." << endl;
					}
				}
				if(cnum==":q")
					break;
				else {
					//블랙리스트에서 blackmem[c-1]제거
					int c = stoi(cnum);
					blackList.erase(blackList.begin() + c-1);

					//파일에서도 제거(blackmem[c-1].getName().txt에서는 isBlackList지움
					blackList[c - 1]->setIsOverdue(false);
					blackList[c - 1]->setIsBlacklist(false);

					//파일에도 추가(blackList[c-1].getName().txt)
					string path = "datafile/User/" + blackList[c - 1]->getId() + ".txt";

					ofstream file(path, ios::out);
					if (!file.is_open()) {
						cerr << path + "is not open for eliminate blackList" << endl;
						exit(1);
					}
					file << blackList[c - 1]->getPassword() << "_" << blackList[c - 1]->getName() << "_" << blackList[c - 1]->getS_id() << endl;
					file << boolalpha << blackList[c - 1]->getIsOverdue() << endl;
					file << boolalpha << blackList[c - 1]->getIsBlacklist() << endl;
					file << endl;
					file << "대출도서정보" << endl;
					file << "예약도서정보" << endl;
					for (size_t i = 0; i < blackList[c - 1]->getReserveBookList().size(); i++) {
						Book* book = blackList[c - 1]->getReserveBookList().at(i);
						file << book->getName() << "_" << book->getAuthor() << "_" << book->getTranslator() << "_" << book->getPublisher() << "_" << book->getPublishYear() << endl;
					}
					
					file.close();
				}
			}
			cnum = "";
			break;
		case 4:
			return;
		default:
			cout << "1~4사이의 정수를 입력해 주세요" << endl;
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