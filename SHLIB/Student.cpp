#include "Student.h"
#include "grammarCheck.h"
#include <iostream>
#include <string>

Student::Student()
{
}

Student::~Student()
{
}

void Student::menu() // 사용자 모드 메뉴
{
	int num;
	cout << "1. 자료 검색\n2. 장바구니\n3. 마이페이지\n4. 로그아웃\n";
	cin >> num;
	// num에 따라 메뉴 호출
	setCurrent_menu(num);
}

void Student::initBookList() { 
	// data file 읽어와서 booklist에 저장
}

void Student::searchBookMenu() // 자료검색 - 윤재원
{
	searchResult.clear(); // 벡터 초기화
	int num;
	cout << "1. 도서명으로 검색\n2. 저자명으로 검색\n3. 메인메뉴로 돌아가기\n";
	cin >> num;

	switch (num) {
	case 1:
	{
		string bookName;
		cout << "도서명 (뒤로 가려면 :q 를 입력하세요) >> ";
		cin >> bookName;
		if (bookName == ":q") return;

		for (auto book : bookList) {
			// 제목 속에 keyword 있는지 확인
			if (book.getName().find(keyword) != string::npos) { // 검색 성공
				searchResult.push_back(book);
			}
		}
		break;
	}
	case 2:
	{
		string bookauthor;
		cout << "저자명 (뒤로 가려면 :q 를 입력하세요) >> ";
		cin >> bookauthor;
		if (bookauthor == ":q") return;

		for (auto book : bookList) {
			// 저자 속에 keyword 있는지 확인
			if (book.getAuthor().find(keyword) != string::npos) { // 검색 성공
				searchBooks.push_back(book);
			}
		}
		break;
	}
	case 3:
		return;
	default:
		return;
	}

	if (searchResult.size() == 0) {
		cout << "검색 결과가 없습니다. 아무 키나 누르면 메인 메뉴로 돌아갑니다." << endl;
		return;
	}

	while (true) {
		// 검색 책 출력: 5번. 제목, 저자, 역자, 출판사, 연도, 대출가능여부, 예약인원 수
		bookListPrint(5, true, true, false, false, true, true);
		//cout << "=======================" << endl << "1. 장바구니 담기\n2.이전 페이지 이동하기\n3. 다음 페이지 이동하기\n4. 돌아가기" << endl << "=======================" << endl;
		// 이전 페이지, 다음 페이지 구현은 프린트 함수에서.. 할지 안할지 모름
		cout << "=======================" << endl << "1. 장바구니 담기\n2. 돌아가기" << endl << "=======================" << endl;
		cout << "메뉴를 선택하세요: " << endl;
		
		int option;
		cin >> option;

		if (option == 1) {
			cout << "장바구니에 담을 책 번호를 선택하세요: ";
			int bookNum;
			bool isExistBasket = false;
			cin >> bookNum;

			// 장바구니에 있으면 담기 실패
			for (auto book : bookBasketList) {
				if (book == searchResult[bookNum-1]) {
					cout << "장바구니에 이미 담은 책입니다. 다시 선택해주세요." << endl;
					isExistBasket = true;
					break;
				}
			}

			if (!isExistBasket) {
				bookBasketList.push_back(searchResult[bookNum - 1]);
				cout << "[" << searchResult[bookNum-1].getName() << "]을 장바구니에 담았습니다." << endl;
			}
		} else
			return;
	}
}

void Student::bookBasketMenu()// 장바구니 메뉴 - 강지윤
{
	int num;
	while(1){
		cout<<"\n장바구니\n";
		bookListPrint(4, true, true, true, true);
		cout<<"\n-----------------------------------------\n";
		cout << "\t1. 일괄 대출\n\t2. 도서 선택 삭제\n\t3. 도서 선택 예약\n\t4. 돌아가기";
		cout<<"\n-----------------------------------------\n";
		cout << "\n메뉴선택:";
		cin >> num;

		if(!cin){ // cin 예외처리
			cout<<"1~4의 정수로 입력해주세요.\n";
			cin.ignore(INT_MAX, '\n'); 
			cin.clear();

			rewind(stdin);
		}else if(num>4 || num < 1){
			cout<<"1~4의 정수로 입력해주세요.\n";
		}else break;
	}

	switch(num){
	case 1:
		borrowBook();
		break;
	case 2:
		deleteBook();
		break;
	case 3:
		reserveBook();
		break;
	case 4:
		quit();
		break;
	}

}


void Student::borrowBook() // 장바구니 -> 일괄대출 (데이터 파일 다루기 필요)
{
	
}

void Student::deleteBook() // 장바구니 -> 도서 선택 삭제 (데이터 파일 다루기 필요)
{

}
void Student::reserveBook() // 장바구니 -> 도서 선택 예약 (데이터 파일 다루기 필요)
{
}



void Student::myPageMenu()// 마이페이지 메뉴 //조수빈
{
	int num; //메뉴 선택
	int u1; //1. 대출현황에서의 사용자 선택 (반납/연장)
	int booknum; //도서 번호 선택
 
	while(1){
		cout << "------------------------------------------------\n";
		cout << "1. 대출 현황\n2. 예약 현황\n3. 돌아가기\n";
		cout << "------------------------------------------------\n";
		cout << "메뉴선택: ";
		cin >> num;
		switch (num) {
		case 1:
			//총 대출권수와 대출도서 목록 출력
			bookListPrint(2, true, true, true, true);


			//반납과 연장
			while (1) {
				cout << "------------------------------------------------\n";
				cout << "1. 반납하기\n2. 연장하기\n";
				cout << "------------------------------------------------\n";
				cout << "메뉴선택: ";
				cin >> u1;
				if (u1 == 1) {
					cout << "도서 번호를 선택해주세요: ";
					cin >> booknum;
					returnBook(booknum);
				}
				else if (u1 == 2) {
					cout << "도서 번호를 선택해주세요: ";
					cin >> booknum;
					extendBook(booknum);
				}
				else {
					cout << "메뉴를 다시 선택해주세요.\n";
					break
				}
			}
			break;
		case 2: //미완성
			//총 예약권수와 예약도서 목록 출력
			bookListPrint(3, true, true, true, true);

			//예약 취소
			cout << "도서 번호를 선택해주세요: ";
			cin >> booknum;
			cancelReserveBook(booknum);
			break;
		case 3:
			quit();
			break;
		default:
			cout << "메뉴를 다시 선택해주세요.\n";
			break;
		}
	}

}

void Student::returnBook(int booknum) // 마이페이지 -> 책 반납 //조수빈
{
	//vector<BorrowInfo> borrowBookList에서 해당 도서 삭제
	//borrowBookList.erase(borrowBookList.begin()+booknum);
	cout << "해당 도서의 반납이 완료되었습니다.\n";
}

void Student::extendBook(int booknum) // 마이페이지 -> 책 연장 //조수빈
{ //미완성
	if (getIsOverdue()){
		//연체된 경우
		cout << "해당 도서는 연체된 도서로, 연장이 불가능합니다.\n";
	}

	//예약자가 존재하는 경우
	cout << "해당 도서는 다른 사용자가 이미 예약한 도서로, 연장이 불가능합니다.\n";

	if (!getIsOverdue()){
		//연장에 문제 없는 경우 
		cout << "해당 도서 연장이 완료되었습니다.\n";
	}
	

}

void Student::cancelReserveBook(int booknum) // 마이페이지 -> 책 예약 취소 //조수빈
{
	//vector<Book> reserveBookList에서 해당 도서 삭제
	reserveBookList.erase(reserveBookList.begin()+booknum);
	cout << "해당 도서 예약이 취소되었습니다.\n";
}

void Student::quit() //돌아가기
{
}

// 강지윤 부분 수정 완. 
// Book객체(1. bookList, 2. borrowBookList, 3. reserveBookList, 4.bookBasket, 5.serachResult 을 num으로 입력), 도서명, 저자명, 대출가능여부, 예약인원수
void Student::bookListPrint(int listNum, bool nameTF, bool authorTF, bool borrowTF, bool reserveNumTF){ //도서리스트 출력 - 강지윤(1,3,4,5), 윤재원(2)

	vector<Book> *List;
	vector<BorrowInfo> *BI; // 2. borrowBookList 때문에 만든거
	int listSize = 0; 
	switch(listNum){
		case 1: // 도서리스트
			List = &bookList;
			listSize = List->size();
			break;
		case 2: // 대출현황
			BI = &borrowBookList;
			listSize = BI->size();
			break;
		case 3: // 예약현황
			List = &reserveBookList;
			listSize = List->size();
			break;
		case 4: // 장바구니
			List = &bookBasketList;
			listSize = List->size();
			break;
		case 5: //검색결과
			List = & searchResult;
			listSize = List->size();
			break;
	}

	for (int i = -1; i < listSize; i++) { // index: -1은 상단바 출력, 0부터 책 출력
		if (i == -1) {
			cout << "\n" << (nameTF ? "[도서명]" : "") << "\t" << (authorTF ? "[저자명]" : "") << "\t" << "[역자]\t[출판사]";
			if (listNum == 2) {
				cout << "\t" << "[연체여부]" << "\t" << "[반납날짜]" << "\t" << "[연장가능여부]";
			}
			cout << "\t" << (borrowTF ? "[대출가능여부]" : "") << "\t" << (reserveNumTF ? "[예약인원수]" : "");
		}
		else {
			if (listNum == 2) { //대출현황 

				cout << "\n" << i + 1 << ".\t" << (nameTF ? BI->at(i).book.getName() : "") << "\t" << (authorTF ? BI->at(i).book.getAuthor() : "") << "\t" << BI->at(i).book.getTranslator() << "\t" << BI->at(i).book.getPublisher();

				// ---- 윤재원 (미완성하다가 끝남) ---- 화여기 Student.cpp 내에
				// cout<<"\t"<</*연체여부*/<<"\t"<<returnDate<<"\t"<</*연장가능여부*/; //대출현황에만

				// ---- 여기까지

				if (borrowTF) { //대출가능여부
					cout << "\t";
					if (BI->at(i).book.getBorrowerTF()) {
						cout << "X";
					}
					else cout << "O";
				}
				if (reserveNumTF) {//예약인원수
					cout << "\t" << BI->at(i).book.getReservStudentsNum();
				}
			}
			else {
				cout << "\n" << (nameTF ? List->at(i).getName() : "") << "\t" << (authorTF ? List->at(i).getAuthor() : "") << "\t" << List->at(i).getTranslator() << "\t" << List->at(i).getPublisher();
				if (borrowTF) { //대출가능여부
					cout << "\t";
					if (List->at(i).getBorrowerTF()) {
						cout << "X";
					}
					else cout << "O";
				}
				cout << "\t" << (reserveNumTF ? to_string(List->at(i).getReservStudentsNum()) : "");
			}
		}
	} 

}

}

void Student::setCurrent_menu(int menu)
{
	this->current_menu = menu;
}

void Student::setName(string name)
{
	this->name = name;
}

void Student::setSid(string sid)
{
	this->sid = sid;
}

void Student::setIsOverdue(bool check)
{
	this->isOverdue = check;
}

void Student::setIsBlacklist(bool check)
{
	this->isBlacklist = check;
}

int Student::getCurrent_menu() const
{
	return current_menu;
}

string Student::getName() const
{
	return name;
}

string Student::getsid() const
{
	return sid;
}

bool Student::getIsOverdue() const
{
	return isOverdue;
}

bool Student::getIsBlacklist() const
{
	return isBlacklist;
}
