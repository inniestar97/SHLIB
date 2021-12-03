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
	for (auto& file : filesystem::directory_iterator(filesystem::current_path().string() + "\\datafile\\User\\")) {
		string path = file.path().string();
		stringstream ss(path);
		vector<string> last_path;
		string split;
		while (getline(ss, split, '\\')) {
			last_path.push_back(split);
		}
		split = last_path[last_path.size() - 1]; // 디렉토리 "userId.txt"
		last_path.clear();

		if (split == "admin.txt")
			continue;

		split = split.substr(0, split.find(".txt")); // 아이디
		Student* std = new Student(split);
		if (std->getBorrowListNum() > 0) {
			borrowList.push_back(std);
		} else {
			delete std;
			std = nullptr;
		}
	}

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
	
}

Admin::~Admin()
{
	for (size_t i = 0; i < borrowList.size(); i++) {
		delete borrowList.at(i);
		borrowList.at(i) = nullptr;
	}
	borrowList.clear();

	for (size_t i = 0; i < booklist.size(); i++) {
		delete booklist.at(i);
		booklist.at(i) = nullptr;
	}
	booklist.clear();
}

void Admin::menu()
{
	while (true) {
		int num;
		cout << "<관리자 모드>\n" << endl;
		cout << "1. 도서 추가" << endl;
		cout << "2. 도서 삭제" << endl;
		cout << "3. 대출자 명단" << endl;
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
			cout<<"로그아웃 되었습니다."<<endl;
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
		getline(cin, inp_s);
		if (inp_s == ":q") {
			cin.ignore();
			return;
		}

		//inp_s /갯수 세기
		int slashNum=0;
		for (int i = 0; i < inp_s.length(); i++) //for문으로 순환문 size 개수만큼 문자열
		{
			if (inp_s[i] == '/') 
				slashNum += 1;
		}
		if (slashNum != 4){
			cout << "입력이 문법 형식에 맞지 않습니다"<<endl;
			continue;
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
			cout << "책제목이 문법 형식에 맞지 않습니다"<<endl;
			continue;
		}
		if (!check_author(a[1])) {
			cout << "저자명이 문법 형식에 맞지 않습니다"<<endl;
			continue;
		}
		if (!check_translator(a[2])) {
			cout << "역자가 문법 형식에 맞지 않습니다"<<endl;
			continue;
		}
		if (!check_publisher(a[3])) {
			cout << "출판사가 문법 형식에 맞지 않습니다"<<endl;
			continue;
		}
		if (!check_year(a[4])) {
			cout << "발행년도가 문법 형식에 맞지 않습니다"<<endl;
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
		new_book_file << "대출자명단\n";
		for (size_t i = 1; i <= 3; i++) {
			new_book_file << i << "." << endl;

		}
		new_book_file << "예약자명단\n";
		for (size_t i = 1; i <= 3; i++) {
			new_book_file << i << "." << endl;

		}
		new_book_file.close();
		
		booklist.push_back(new Book(a[0], a[1])); // 책 리스트에 추가하고 종료
		cout<<"도서 추가 완료"<<endl;
		a.clear();
		break;
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
				if (b_name==":q"){
					cin.ignore();
					break;
				}
				if (!check_book(b_name)) {
					cout << "도서명이 문법 형식에 맞지 않습니다"<<endl;
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
						if(inp==":q"){
	
							break;
						}
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
				if (a_name==":q"){
					cin.ignore();
					break;
				}
				if (!check_author(a_name)) {
					cout << "저자명이 문법 형식에 맞지 않습니다"<<endl;
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
						cout<<"    [도서명]\t[역자]\t[출판사]\t[발행년도]"<<endl;
						for (Book* book : a) {
							i++;
							cout<<i<<". "<<book->getName()<<" "<<book->getTranslator()<<" "<<book->getPublisher()<<" "<<book->getPublishYear()<<endl;
						}
						getline(cin, inp);
						if(inp==":q"){
							cin.ignore();
							break;
						}
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


//정렬 관련 student에서 해결하면
void Admin::monitoring() // 대출자 명단
{
	string quit;
	
	//borrowlist정렬			
   	for (int i = 0; i < borrowList.size(); i++) {
		for (int j = 0; j < borrowList.size() - i - 1; j++) {
			if (borrowList[j] >= borrowList[j + 1]) {
				Student* temp = borrowList[j];
				borrowList[j] = borrowList[j + 1];	
				borrowList[j + 1] = borrowList[j];
			}
		}
	}
	cout << "<대출자 명단>\n";		
	cout << "[학번] [이름]\n";
	cout<< "[대출중인 도서] [대출일] [반납예정일]\n";
	int i = 0;
	for (Student* bmem : borrowList) {
		i++;
		cout<< i <<". " << bmem->getS_id() << " " << bmem->getName() << "\n" ;
		
		for(int bi=0;bi<bmem->getBorrowListNum();bi++){
			cout << "\t\t\t" << bmem->getBookName(bi) <<" "<< bmem->getBorrowDate(bi) << " " << bmem->getDueDate(bi) << endl;
		}
	}
	cout << "(뒤로 가려면 \":q\"를 입력하세요)\n";
	while(quit !=":q") {
		cout << ">> ";
		cin >> quit;
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