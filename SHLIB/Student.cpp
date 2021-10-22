#include "Student.h"
#include "grammarCheck.h"
#include "Book.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#define BASKETMAX 10
#define BORROWMAX 1

Student::Student(string id)
	:id(id), current_menu(0)
{
	ifstream std_file;

	while (!std_file.is_open()) { // 학생 파일 open
		std_file.open("datafile/User/" + id + ".txt");
	}

	string std_info;
	getline(std_file, std_info);

	std_info = std_info.substr(std_info.find('_') + 1, string::npos);
	this->name = std_info.substr(0, std_info.find('_')); // 학생 이름
	std_info = std_info.substr(std_info.find('_') + 1, string::npos);
	this->s_id = std_info.substr(0, std_info.find('\n'));

	getline(std_file, std_info);
	std_info = std_info.substr(0, std_info.find('\n'));
	if (std_info == "false") isOverdue = false;
	else this->isOverdue = true;
	getline(std_file, std_info);
	std_info = std_info.substr(0, std_info.find('\n'));
	if (std_info == "false") isBlacklist = false;
	else this->isBlacklist = true;

	getline(std_file, std_info); // 빈줄 \n
	getline(std_file, std_info); // std_info => '대출도서정보\n"
	getline(std_file, std_info); // 대출한 책 정보가 std_info 에 담긴다

	std_info = std_info.substr(0, std_info.find('\n')); // 대출한 책 정보

	stringstream ss(std_info);
	string token;
	vector<string> word;
	while (getline(ss, token, '_')) {
		word.push_back(token);
	}
	this->borrow = new Book(word.at(0), word.at(2), word.at(3), word.at(4), word.at(5));

	getline(std_file, std_info);
	while (getline(std_file, std_info)) {
		std_info = std_info.substr(0, std_info.find('\n'));

		stringstream ss(std_info);
		string token;
		vector<string> word;
		while (getline(ss, token, '_')) {
			word.push_back(token);
		}
		this->reserveBookList.push_back(new Book(word.at(0), word.at(1), word.at(2), word.at(3), word.at(4)));
	}

	std_file.close();
}

Student::~Student()
{
	// 동적할당 된 모든 부분 메모리 해제
	delete borrow;
	borrow = nullptr;

	for (size_t i = 0; i < searchResult.size(); i++) {
		delete searchResult.at(i);
			searchResult.at(i) = nullptr;
	}
	for (size_t i = 0; i < reserveBookList.size(); i++) {
		delete reserveBookList.at(i);
			reserveBookList.at(i) = nullptr;
	}
	for (size_t i = 0; i < bookBasketList.size(); i++) {
		delete bookBasketList.at(i);
			bookBasketList.at(i) = nullptr;
	}
}

void Student::menu() // 사용자 모드 메뉴
{
	int basketListNum;
	cout << "1. 자료 검색\n2. 장바구니\n3. 마이페이지\n4. 로그아웃\n";
	cin >> basketListNum;
	// basketListNum에 따라 메뉴 호출
	setCurrent_menu(basketListNum);
}

void Student::initBookList() {
	// data file 읽어와서 booklist에 저장 - 생성자에서 만드는 중이라 필요 없을듯합니다.
}

void Student::searchBookMenu() // 자료검색 - 윤재원
{
	searchResult.clear(); // 벡터 초기화
	int basketListNum;
	cout << "1. 도서명으로 검색\n2. 저자명으로 검색\n3. 메인메뉴로 돌아가기\n";
	cin >> basketListNum;

	switch (basketListNum) {
	case 1:
	{
		string bookName;
		cout << "도서명 (뒤로 가려면 :q 를 입력하세요) >> ";
		cin >> bookName;
		if (bookName == ":q") return;

		for (auto book : bookList) {
			// 제목 속에 keyword 있는지 확인
			if (book->getName().find(bookName) != string::npos) { // 검색 성공
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
			if (book->getAuthor().find(bookauthor) != string::npos) { // 검색 성공
				searchResult.push_back(book);
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
		bookListPrint(5, true, true, true, true);
		// cout << "=======================" << endl << "1. 장바구니 담기\n2.이전 페이지 이동하기\n3. 다음 페이지 이동하기\n4. 돌아가기" << endl << "=======================" << endl;
		// 이전 페이지, 다음 페이지 구현은 프린트 함수에서.. 할지 안할지 모름
		cout << "=======================" << endl << "1. 장바구니 담기\n2. 돌아가기" << endl << "=======================" << endl;
		cout << "메뉴를 선택하세요: " << endl;

		int option;
		cin >> option;

		if (option == 1) {
			cout << "장바구니에 담을 책 번호를 선택하세요: ";
			int bookbasketListNum;
			bool isExistBasket = false;
			cin >> bookbasketListNum;

			// 장바구니에 있으면 담기 실패
			// #define BASKETMAX 10 추가 예정 -- 개수 제한 (윤재원)
			for (auto book : bookBasketList) {
				if (book == searchResult[bookbasketListNum - 1]) {
					cout << "장바구니에 이미 담은 책입니다. 다시 선택해주세요." << endl;
					isExistBasket = true;
					break;
				}
			}

			if (!isExistBasket) {
				bookBasketList.push_back(searchResult[bookbasketListNum - 1]);
				cout << "[" << searchResult[bookbasketListNum - 1]->getName() << "]을 장바구니에 담았습니다." << endl;
			}
		}
		else
			return;
	}
}

void Student::bookBasketMenu()// 장바구니 메뉴 - 강지윤
{
	int basketListNum;
	while (1) {
		cout << "\n장바구니\n";
		bookListPrint(4, true, true, true, true);
		cout << "\n-----------------------------------------\n";
		cout << "\t1. 선택 대출\n\t2. 도서 선택 삭제\n\t3. 도서 선택 예약\n\t4. 돌아가기"; // 일단 1. 일괄대출 -> 선택대출로 변경 (대출 제한 1권이면)
		cout << "\n-----------------------------------------\n";
		cout << "\n메뉴선택:";
		cin >> basketListNum;

		if (!cin) { // cin 예외처리
			cout << "1~4의 정수로 입력해주세요.\n";
			cin.ignore(INT_MAX, '\n');
			cin.clear();

			rewind(stdin);
		}
		else if (basketListNum > 4 || basketListNum < 1) {
			cout << "1~4의 정수로 입력해주세요.\n";
		}
		else break;
	}

	switch (basketListNum) {
	case 1:
		//borrowBook();   // 얘는 일괄대출! 대출 제한 10권일 때!
		sel_borrowBook(); // 대출 제한 1권이면 선택대출!
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

// 얘는 일괄대출
void Student::borrowBook() // 장바구니 -> 일괄대출 (데이터 파일 다루기 필요) - 강지윤
{
	size_t basketListNum = bookBasketList.size(); // 윤재원 수정: int -> size_t ㅇㅋ
	// size_t borrowbooknum = borrowBookList.size(); // 대출 권수를 1권으로 줄여서 주석처리

	/*  ----------------------------------
		대출 불가할 경우
		1. 이미 대출된 경우
		2. 첫번째 예약자 != 나
		3. 잔여 대출횟수 < 장바구니 list size 
	    -----------------------------------  */

	if(bookBasketList.empty()){
		cout << "------------------------------------------------\n";
		cout << "장바구니에 담은 책이 없습니다. 엔터키를 누르면 메뉴로 돌아갑니다. \n";
		cout << "------------------------------------------------\n";
		// string yn;
		// cin >> yn;
		return;
	} 

	vector<int> cant;
	bool flag3 = false;
	for (int i = 0; i < basketListNum; i++) {
		if (!bookBasketList.at(i)->getBorrowTF()) { // 1. 이미 대출됨.
			cant.emplace_back(i);
		}
		else if (bookBasketList.at(i)->getReservStudents().size() > 0) {
			if (bookBasketList.at(i)->getReservStudents().at(0) != this) { // 2. 첫번째 예약자 != 나
				cant.emplace_back(i);
			}
		}
	}

	if (BORROWMAX - borrowbooknum < basketListNum) { // 3. 잔여 대출횟수 < 장바구니 list size
		flag3 = true;
	}
	if (flag3) {
		cout << " -- 대출 가능 횟수가 장바구니보다 " << basketListNum - (BORROWMAX - borrowbooknum) << "권 적습니다. 장바구니를 덜어주세요. -- \n";
	}
	if (!cant.empty()) { // 대출 불가
		cout << " ------\t 대출불가 리스트\t ------\n";
		cout << "([장바구니번호. 제목, 저자])\n";
		for (int i = 0; i < cant.size(); i++) {
			cout << to_string(cant.at(i)) << ".\t제목 : " << bookBasketList.at(cant.at(i))->getName()
				<< " , 저자 : " << bookBasketList.at(cant.at(i))->getAuthor() << "\n";
		}
		return;
	}
	else { // 대출가능하면 여기
		for (int i = 0; i < basketListNum; i++) {
			bookBasketList.at(i)->addBorrow(this);
			//borrowBookList.emplace_back(bookBasketList[i], "20211015"); // 윤재원: 에러나서 잠시 주석처리함
		}
	}
}


// 얘는 선택대출
void Student::sel_borrowBook() // 장바구니 -> 선택대출 (데이터 파일 다루기 필요) - 강지윤
{
	size_t basketListNum = bookBasketList.size(); 
	while(true){
		// 장바구니가 빈 경우
		if(bookBasketList.empty()){
			cout << "------------------------------------------------\n";
			cout << "장바구니에 담은 책이 없습니다. 엔터키를 누르면 메뉴로 돌아갑니다. \n";
			cout << "------------------------------------------------\n";
			// string yn;
			// cin >> yn;
			return;
		} 

	}

}

void Student::deleteBook() // 장바구니 -> 도서 선택 삭제 (데이터 파일 다루기 필요) - 강지윤
{
	while(true){
		// 장바구니가 빈 경우
		if(bookBasketList.empty()){
			cout << "------------------------------------------------\n";
			cout << "장바구니에 담은 책이 없습니다. 엔터키를 누르면 메뉴로 돌아갑니다. \n";
			cout << "------------------------------------------------\n";
			// string yn;
			// cin >> yn;
			return;
		} 
		// 창 초기화 필요
		bookListPrint(4, true, true, true, true);
		cout << "------------------------------------------------\n";
		cout << "삭제할 책의 번호를 입력하세요 (0을 입력하면 메뉴로 돌아갑니다.): ";
		
		int select; // +1 해서 생각해야 됨.
		cin >> select;
		if(select == 0){
			cout << "\n메뉴로 돌아갑니다.\n";
			cout << "------------------------------------------------\n";
			return;
		}
		cout << "------------------------------------------------\n";
		bookBasketList.erase(bookBasketList.begin()+select-1); // 삭제
		cout << "해당 도서의 삭제가 완료되었습니다.\n";						
	}
}
void Student::reserveBook() // 장바구니 -> 도서 선택 예약 (데이터 파일 다루기 필요) - 강지윤
{
	while(true){
		// 장바구니가 빈 경우
		if(bookBasketList.empty()){
			cout << "------------------------------------------------\n";
			cout << "장바구니에 담은 책이 없습니다. 엔터키를 누르면 메뉴로 돌아갑니다. \n";
			cout << "------------------------------------------------\n";
			// string yn;
			// cin >> yn;
			return;
		} 

		// 사용자의 예약 횟수를 모두 소진한 경우 break;
		if(reserveBookList.size()>=3) { 
			cout << "------------------------------------------------\n";
			cout << "예약횟수가 모두 소진되었습니다. 엔터키를 누르면 메뉴로 돌아갑니다. \n";
			cout << "------------------------------------------------\n";

			// string yn;
			// cin >> yn;
			return;
		}
		// 창 초기화 필요
		bookListPrint(4, true, true, true, true);
		cout << "------------------------------------------------\n";
		cout << "대출할 책의 번호를 입력하세요 (0을 입력하면 메뉴로 돌아갑니다.): ";
		
		int select; // +1 해서 생각해야 됨.
		cin >> select; // 혹시 동일한 번호를 입력하게됨ㄴ... 일단 예약하면 장바구니에서 삭제해
		if(select == 0){
			cout << "\n메뉴로 돌아갑니다.\n";
			cout << "------------------------------------------------\n";
			return;
		}
		// 예약불가 : 도서별 예약가능인원 (5명) 초과 (사용자 예약 횟수 초과는 위에서 다룸) 
		if(bookBasketList.at(select-1)->getReservStudents().size() >= 5){
			cout << "------------------------------------------------\n";
			cout << "해당 도서의 예약 가능 인원수가 초과되었습니다.\n";
		}else{
			cout << "------------------------------------------------\n";

			reserveBookList.emplace_back(bookBasketList.at(select-1)); //예약
			bookBasketList.erase(bookBasketList.begin()+select-1); // 장바구니에서 예약된 도서 삭제 (혹시 사용자가 이중으로 선택할까봐)

			cout << "해당 도서의 예약이 완료되었습니다.\n";				
		}
		
	}
}

void Student::myPageMenu()// 마이페이지 메뉴 //조수빈
{
	int num; //메뉴 선택
	int u1; //1. 대출현황에서의 사용자 선택 (반납/연장)
	int booknum; //도서 번호 선택

	while (1) {
		cout << "------------------------------------------------\n";
		cout << "1. 대출 현황\n2. 예약 현황\n3. 돌아가기\n";
		cout << "------------------------------------------------\n";
		cout << "메뉴선택: ";
		cin >> num;
		switch (num) {
		case 1:

			while (1) {
				//총 대출권수와 대출도서 목록 출력
				bookListPrint(2, true, true, true, true);

				//반납과 연장
				cout << "------------------------------------------------\n";
				cout << "1. 반납하기\n2. 연장하기\n";
				cout << "------------------------------------------------\n";
				cout << "메뉴선택: ";
				cin >> u1;
				if (u1 == 1) {
					cout << "도서 번호를 선택해주세요: ";
					cin >> booknum;
					returnBook(booknum);
					break;
				}
				else if (u1 == 2) {
					cout << "도서 번호를 선택해주세요: ";
					cin >> booknum;
					extendBook(booknum);
					break;
				}
				else {
					cout << "메뉴를 다시 선택해주세요.\n";
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
			cout << "메뉴로 돌아갑니다.\n";
			break;
		default:
			cout << "메뉴를 다시 선택해주세요.\n";
		}
	}
}

void Student::returnBook(int booknum) // 마이페이지 -> 책 반납 //조수빈
{
	//vector<BorrowInfo> borrowBookList에서 해당 도서 삭제
	vector<BorrowInfo> BI; // 윤재원 수정: BorrowInfo* -> BorrowInfo
	BI = borrowBookList;

	BI.erase(BI.begin() + booknum);
	cout << "해당 도서의 반납이 완료되었습니다.\n";
}

void Student::extendBook(int booknum) // 마이페이지 -> 책 연장 //조수빈
{
	vector<BorrowInfo> BI;
	BI = borrowBookList;
	bool reserveNumFlag = false; //예약자 존재여부 저장 (윤재원 수정: 에러때문에 임시로 false 처리)
	int reserveNum = BI.at(booknum).book->getReserveStudents().size();
	// .book 추가했어요

   //윤재원 수정: 에러나서 잠시 주석처리 - 조수빈 수정완료
	if (reserveNum == 0)
		reserveNumFlag = false;
	else
		reserveNumFlag = true;

	//미완성
	if (!getIsOverdue() && !reserveNumFlag)
		//연장에 문제 없는 경우 - 연장 실제로 해야 함 - 날짜 다루어야 함
		cout << "해당 도서 연장이 완료되었습니다.\n";
	else if (getIsOverdue())
		//연체된 경우
		cout << "해당 도서는 연체된 도서로, 연장이 불가능합니다.\n";
	else if (reserveNumFlag)
		//예약자가 존재하는 경우
		cout << "해당 도서는 다른 사용자가 이미 예약한 도서로, 연장이 불가능합니다.\n";
}

void Student::cancelReserveBook(int booknum) // 마이페이지 -> 책 예약 취소 //조수빈
{
	//vector<Book> reserveBookList에서 해당 도서 삭제
	vector<Book*> RL;
	RL = reserveBookList;
	RL.erase(RL.begin() + booknum);
	cout << "해당 도서 예약이 취소되었습니다.\n";
}

void Student::quit() //돌아가기
{
}

// Book객체(1. bookList, 2. borrowBookList, 3. reserveBookList, 4.bookBasket, 5.serachResult 을 listNum으로 입력), 도서명, 저자명, 대출가능여부, 예약인원수
void Student::bookListPrint(int listNum, bool nameTF, bool authorTF, bool borrowTF, bool reserveNumTF) { //도서리스트 출력 - 강지윤(1,3,4,5), 윤재원(2)
	vector<Book*> List;
	vector<BorrowInfo>* BI; // 2. borrowBookList 때문에 만든거
	int listSize = 0;
	switch (listNum) {
	case 1: // 도서리스트
		List = bookList;
		listSize = List.size();
		break;
	case 2: // 대출현황
		BI = &borrowBookList;
		listSize = BI->size();
		break;
	case 3: // 예약현황
		List = reserveBookList;
		listSize = List.size();
		break;
	case 4: // 장바구니
		List = bookBasketList;
		listSize = List.size();
		break;
	case 5: //검색결과
		List = searchResult;
		listSize = List.size();
		break;
	}

	for (int i = -1; i < listSize; i++) { // index: -1은 상단바 출력, 0부터 책 출력
		if (i == -1) {
			cout << "\n" << (nameTF ? "[도서명]" : "") << "\t" << (authorTF ? "[저자명]" : "") << "\t" << "[역자]\t[출판사]";
			if (listNum == 2) {
				cout << "\t" << "[연체여부]" << "\t" << "[반납날짜]" << "\t" << "[연장가능여부]";
			}
			cout << "\t" << (borrowTF ? "[대출가능여부]" : "") << "\t" << (reserveNumTF ? "[예약인원수]" : "");
			cout << "\n-------------------------------------------\n";
		}
		else {
			if (listNum == 2) { //대출현황
				cout << "\n" << i + 1 << ".\t" << (nameTF ? BI->at(i).book->getName() : "") << "\t" << (authorTF ? BI->at(i).book->getAuthor() : "") << "\t" << BI->at(i).book->getTranslator() << "\t" << BI->at(i).book->getPublisher();

				// ---- 윤재원 (미완성하다가 끝남) ---- 화여기 Student.cpp 내에
				// cout<<"\t"<</*연체여부*/<<"\t"<<returnDate<<"\t"<</*연장가능여부*/; //대출현황에만

				// ---- 여기까지

				if (borrowTF) { //대출가능여부
					cout << "\t";

					if (BI->at(i).book->getBorrowTF()) {
						cout << "X";
					}
					else cout << "O";
				}
				if (reserveNumTF) {//예약인원수
					cout << "\t" << BI->at(i).book->getReservStudents().size();
				}
			}
			else {
				cout << "\n" << (nameTF ? List.at(i)->getName() : "") << "\t" << (authorTF ? List.at(i)->getAuthor() : "") << "\t" << List.at(i)->getTranslator() << "\t" << List.at(i)->getPublisher();
				if (borrowTF) { //대출가능여부
					cout << "\t";
					if (List.at(i)->getBorrowTF()) {
						cout << "X";
					}
					else cout << "O";
				}
				cout << "\t" << (reserveNumTF ? to_string(List.at(i)->getReservStudents().size()) : "");
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

void Student::setS_id(string s_id)
{
	this->s_id = s_id;
}
void Student::setId(string id)
{
	this->id = id;
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

string Student::getS_id() const {
	return s_id;
}

string Student::getId() const
{
	return id;
}

bool Student::getIsOverdue() const
{
	return isOverdue;
}

bool Student::getIsBlacklist() const
{
	return isBlacklist;
}

string Student::getBorrowDate() const
{
	return borrowDate;
}

string Student::getBookName() const
{
	return borrow->getName();
}
bool Student::operator==(Student student) // 강지윤
{
	if (this->s_id == student.s_id) { // 학번 같으면 동일 인물
		return true;
	}
	else return false;
}