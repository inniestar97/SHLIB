#include "Student.h"
#include "grammarCheck.h"
#include "Book.h"
#include "Library.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <climits>
#include <io.h>
#include <filesystem>


#define BASKETMAX 10
#define BORROWMAX 1

// using namespace std;

Student::Student(string id)
	: current_menu(0), isOverdue(false), isBlacklist(false), canExtend(true)
{
	this->id = id;
	/*
	학생 아이디를 이용, 학생의 아이디를 인자로 받아들이는 생성자.
	datafile/User/학생id.txt 에 있는 학생의 모든 정보를 student 클래스에 저장
	*/
	ifstream file;
	file.open("datafile/User/" + id + ".txt");
	if (!file.is_open()) {
		cerr << "datafile/User/" + id + ".txt file is not open" << endl;
		exit(1);
	}

	string info;
	file >> info; // info = 비밀번호_이름_학번
	this->password = info.substr(0, info.find('_')); // 비밀번호
	info = info.substr(info.find('_') + 1, string::npos); // info = 이름_학번
	this->name = info.substr(0, info.find('_')); // 이름
	this->s_id = info.substr(info.find('_') + 1, string::npos); // 학번

	file >> info; // 연체여부
	if (info == "true") isOverdue = true;

	file >> info; // 블랙리스트 여부 
	if (info == "true") isBlacklist = true;

	file >> info; // info = "대출도서 정보"
	file >> info; // info = 도서명_저자명_역자_출판사_발행연도_대출일_반납일

	string split;
	stringstream ss(info);
	vector<string> b_info; b_info.clear();
	while (getline(ss, split, '_')) {
		b_info.push_back(split);
	}

	// 수정
	if (b_info.size() > 1) {
		borrow = new Book(b_info[0], b_info[1]);
	}

	if (borrow != nullptr) {
		// 연장 가능 여부 -> + 2차 때 연장횟수 제한 둬야 함.
		if (isOverdue || borrow->getReservStudents().size() > 0) { // 연체 or 예약자 존재
			canExtend = false;
		}
	}
	else canExtend = false; // 대출한 책 없는 경우엔 연장도 불가


	/*
	 * 요건 어따가 넣야 하나 ㅠㅠ 
	 * b_info[5] => 대출일
	 * b_info[6] => 반납일
	 */

	file >> info; // info = "예약도서 정보"
	getline(file, info); // info = "\n"
	while (getline(file, info)) { // 예약도서정보 vector에 push
		info = info.substr(0, info.find('\n'));
		// info = 도서명_저자명_역자_출판사_발행연도
		ss.clear(); b_info.clear();
		ss.str(info);
		while (getline(ss, split, '_')) {
			b_info.push_back(split);
		}
		reserveBookList.push_back(new Book(b_info[0], b_info[1]));
	}

	while (file.is_open()) file.close();

	this->initBookList();
	
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
	searchResult.clear();
	for (size_t i = 0; i < reserveBookList.size(); i++) {
		delete reserveBookList.at(i);
		reserveBookList.at(i) = nullptr;
	}
	reserveBookList.clear();
	for (size_t i = 0; i < bookBasketList.size(); i++) {
		delete bookBasketList.at(i);
		bookBasketList.at(i) = nullptr;
	}
	bookBasketList.clear();
}

void Student::menu() // 사용자 모드 메뉴
{
	int selectNum;
	cout << "=============\n사용자 모드 메뉴\n================\n";
	cout << "1. 자료 검색\n2. 장바구니\n3. 마이페이지\n4. 로그아웃\n";
	cin >> selectNum;
	// basketListNum에 따라 메뉴 호출
	setCurrent_menu(selectNum);
	switch (selectNum) {
	case 1:
		searchBookMenu(); break;
	case 2:
		bookBasketMenu(); break;
	case 3:
		myPageMenu(); break;
	default:
		cout << "사용자 모드 메뉴를 종료합니다." << endl;
		return;
	}
}

void Student::initBookList() { // 수정 중 (윤재원)
	// data file 읽어와서 booklist에 저장 
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
		string au = split.substr(split.find('-') + 1, split.find('.') - (split.find('-') + 1));

		bookList.push_back(new Book(na, au));
	}

	return;
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
		cout << "검색 결과가 없습니다. 메인 메뉴로 돌아갑니다." << endl;
		return;
	}

	while (true) {
		bookListPrint(searchResult, false, true, true, true, true);

		// cout << "=======================" << endl << "1. 장바구니 담기\n2.이전 페이지 이동하기\n3. 다음 페이지 이동하기\n4. 돌아가기" << endl << "=======================" << endl;
		// 이전 페이지, 다음 페이지 구현은 프린트 함수에서.. 할지 안할지 모름
		cout << "=======================" << endl << "1. 장바구니 담기\n2. 돌아가기" << endl << "=======================" << endl;
		cout << "메뉴를 선택하세요: " << endl;

		int option;
		cin >> option;

		if (option == 1) {
			cout << "장바구니에 담을 책 번호를 선택하세요: ";
			int bookbasketListNum = 1;
			bool isExistBasket = false;
			cin >> bookbasketListNum;

			// 장바구니에 있으면 담기 실패
			for (auto book : bookBasketList) {
				if (book == searchResult[(int) (bookbasketListNum - 1)]) {
					cout << "장바구니에 이미 담은 책입니다. 다시 선택해주세요." << endl;
					isExistBasket = true;
					break;
				}
			}

			if (!isExistBasket) {
				bookBasketList.push_back(searchResult[(int) (bookbasketListNum - 1)]);
				cout << "[" << searchResult[(int) (bookbasketListNum - 1)]->getName() << "]을 장바구니에 담았습니다." << endl;
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
		bookListPrint(bookBasketList, false, true, true, true, true);
		// 일단 1. 일괄대출 -> 선택대출로 변경 (대출 제한 1권이면)
		cout << "\n-----------------------------------------\n";
		cout << "\t1. 도서 선택 대출\n\t2. 도서 선택 삭제\n\t3. 도서 선택 예약\n\t4. 돌아가기"; 
		cout << "\n-----------------------------------------\n";

		while(1){
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
			else break; //제대로 선택했다면
		}
		switch (basketListNum) {
			case 1:
				cout << "------------------------------------------------\n";
				cout << "\t도서 선택 대출\n";
				cout << "------------------------------------------------\n";
				//borrowBook();   // 얘는 일괄대출! 대출 제한 10권일 때!
				sel_borrowBook(); // 대출 제한 1권이면 선택대출!
				break;
			case 2:
				cout << "------------------------------------------------\n";
				cout << "\t도서 선택 삭제\n";
				cout << "------------------------------------------------\n";
				deleteBook();
				break;
			case 3:
				cout << "------------------------------------------------\n";
				cout << "\t도서 선택 예약\n";
				cout << "------------------------------------------------\n";
				reserveBook();
				break;
			case 4:
				cout << "------------------------------------------------\n";
				cout << "이전 메뉴로 돌아갑니다.\n";
				cout << "------------------------------------------------\n";
				return;
			default:
				cout << "------------------------------------------------\n";
				cout << "메뉴를 다시 선택해주세요.\n";
				cout << "------------------------------------------------\n";
				break;
		}
	}

	
}

// 얘는 일괄대출 --> 2차 때 사용 or 폐기
void Student::borrowBook() // 장바구니 -> 일괄대출 (데이터 파일 다루기 필요) - 강지윤
{
	size_t basketListNum = bookBasketList.size(); // 윤재원 수정: int -> size_t ㅇㅋ
	size_t borrowbooknum = 0; // 2차 1 -> /*borrowBookList.size()*/
	if (borrow != nullptr) borrowbooknum = 1; // 2차 때 폐기

	/*  ----------------------------------
		대출 불가할 경우
		1. 이미 대출된 경우
		2. 첫번째 예약자 != 나
		3. 잔여 대출횟수 < 장바구니 list size
		-----------------------------------  */

	if (bookBasketList.empty()) {
		cout << "------------------------------------------------\n";
		cout << "장바구니에 담은 책이 없습니다.\n"; // 엔터키를 누르면 메뉴로 돌아갑니다. 
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
			borrowDate = getCurrent_date();
			dueDate = getAfter_date(borrowDate, 14);
			bookBasketList.at(i)->addBorrow(this);
			//borrowBookList.emplace_back(bookBasketList[i], "20211015"); // 윤재원: 에러나서 잠시 주석처리함
		}
	}
}

// 얘는 선택대출 --> 대출 1권 제한이라 추가로 만들긴 했는데, 2차 때 대출 권수 늘어나면서 폐기될 수도 있는 함수임.
void Student::sel_borrowBook() // 장바구니 -> 선택대출 (데이터 파일 다루기 필요) - 강지윤
{
	size_t basketListNum = bookBasketList.size();
	while (true) {
		// 장바구니가 빈 경우
		if (bookBasketList.empty()) {
			cout << "------------------------------------------------\n";
			cout << "장바구니에 담은 책이 없습니다. \n"; // 엔터키를 누르면 메뉴로 돌아갑니다.
			cout << "------------------------------------------------\n";
			// string yn;
			// cin >> yn;
			return;
		}
		// 창 초기화 필요
		bookListPrint(bookBasketList, false, true, true, true, true);

		cout << "------------------------------------------------\n";
		cout << "대출할 책의 번호를 입력하세요 (0을 입력하면 메뉴로 돌아갑니다.): ";

		int select; // +1 해서 생각해야 됨.
		cin >> select;
		if (select == 0) {
			cout << "\n메뉴로 돌아갑니다.\n";
			cout << "------------------------------------------------\n";
			return;
		}

		// 3. 대출횟수 소진
		if (borrow != nullptr) { // 이미 대출중이면 (대출 제한 1권이면 이거고, 만약 대출 권수 제한 늘어나면 리스트에 맞게 변경해야됨.)
			cout << "------------------------------------------------\n";
			cout << "대출횟수를 모두 소진하였습니다.\n";
			cout << "------------------------------------------------\n";
		}

		if (!bookBasketList.at((int) (select - 1))->getBorrowTF()) { // 1. 이미 대출된 책.
			cout << "------------------------------------------------\n";
			cout << "다른 사용자가 대출 중입니다.\n";
			cout << "------------------------------------------------\n";
			continue;
		}
		else if (bookBasketList.at((int) (select - 1))->getReservStudents().size() > 0) {
			if (bookBasketList.at((int) (select - 1))->getReservStudents().at(0) != this) { // 2. 첫번째 예약자 != 나
				cout << "------------------------------------------------\n";
				cout << "우선 예약자가 대기중입니다.\n";
				cout << "------------------------------------------------\n";
				continue;
			}
			else { // 대출 완.
				borrow = bookBasketList.at((int) (select - 1)); // 대출 (대출 제한 1권이면 이거고, 만약 대출 권수 제한 늘어나면 리스트에 맞게 변경해야됨.)
				bookBasketList.erase(bookBasketList.begin() + select - 1); // 삭제
				borrowDate = getCurrent_date();
				dueDate = getAfter_date(borrowDate, 14);
				bookBasketList.at((int) (select - 1))->addBorrow(this);
				cout << "해당 도서의 대출이 완료되었습니다.\n";
				cout << "------------------------------------------------\n";
			}
		}
	}
}

void Student::deleteBook() // 장바구니 -> 도서 선택 삭제 (데이터 파일 다루기 필요) - 강지윤
{
	while (true) {
		// 장바구니가 빈 경우
		if (bookBasketList.empty()) {
			cout << "------------------------------------------------\n";
			cout << "장바구니에 담은 책이 없습니다. 엔터키를 누르면 메뉴로 돌아갑니다. \n";
			cout << "------------------------------------------------\n";
			// string yn;
			// cin >> yn;
			return;
		}
		// 창 초기화 필요
		bookListPrint(bookBasketList, false, true, true, true, true);

		cout << "------------------------------------------------\n";
		cout << "삭제할 책의 번호를 입력하세요 (0을 입력하면 메뉴로 돌아갑니다.): ";

		int select; // +1 해서 생각해야 됨.
		cin >> select;
		if (select == 0) {
			cout << "\n메뉴로 돌아갑니다.\n";
			cout << "------------------------------------------------\n";
			return;
		}
		cout << "------------------------------------------------\n";
		bookBasketList.erase(bookBasketList.begin() + select - 1); // 삭제
		cout << "해당 도서의 삭제가 완료되었습니다.\n";
	}
}
void Student::reserveBook() // 장바구니 -> 도서 선택 예약 (데이터 파일 다루기 필요) - 강지윤
{
	while (true) {
		// 장바구니가 빈 경우
		if (bookBasketList.empty()) {
			cout << "------------------------------------------------\n";
			cout << "장바구니에 담은 책이 없습니다. 엔터키를 누르면 메뉴로 돌아갑니다. \n";
			cout << "------------------------------------------------\n";
			// string yn;
			// cin >> yn;
			return;
		}

		// 사용자의 예약 횟수를 모두 소진한 경우 break;
		if (reserveBookList.size() >= 3) {
			cout << "------------------------------------------------\n";
			cout << "예약횟수가 모두 소진되었습니다. 엔터키를 누르면 메뉴로 돌아갑니다. \n";
			cout << "------------------------------------------------\n";

			// string yn;
			// cin >> yn;
			return;
		}
		// 창 초기화 필요
		bookListPrint(bookBasketList, false, true, true, true, true);
		cout << "------------------------------------------------\n";
		cout << "예약할 책의 번호를 입력하세요 (0을 입력하면 메뉴로 돌아갑니다.): ";

		int select; // +1 해서 생각해야 됨.
		cin >> select; // 혹시 동일한 번호를 입력하게됨ㄴ... 일단 예약하면 장바구니에서 삭제해
		if (select == 0) {
			cout << "\n메뉴로 돌아갑니다.\n";
			cout << "------------------------------------------------\n";
			return;
		}


		// 예약불가 : 도서별 예약가능인원 (5명) 초과 (사용자 예약 횟수 초과는 위에서 다룸)
		if (bookBasketList.at((int) (select - 1))->getReservStudents().size() >= 5) {
			cout << "------------------------------------------------\n";
			cout << "해당 도서의 예약 가능 인원수가 초과되었습니다.\n";
		}else if(bookBasketList.at((int) (select - 1))->getBorrower() == this){ // 이미 사용자가 대출중인 책인 경우
			cout << "------------------------------------------------\n";
			cout << "해당 도서를 이미 대출중입니다.\n";
		}
		else {
			cout << "------------------------------------------------\n";

			reserveBookList.emplace_back(bookBasketList.at((int) (select - 1))); //예약
			bookBasketList.erase(bookBasketList.begin() + select - 1); // 장바구니에서 예약된 도서 삭제 (혹시 사용자가 이중으로 선택할까봐)

			cout << "해당 도서의 예약이 완료되었습니다.\n";
		}
	}
}

void Student::myPageMenu()// 마이페이지 메뉴 //조수빈
{
	int num; //메뉴 선택
	int u1; //1. 대출현황에서의 사용자 선택 (반납/연장)
	//int booknum; //도서 번호 선택
	string answer;

	while (1) {
		cout << "------------------------------------------------\n";
		cout << "1. 대출 현황\n2. 예약 현황\n3. 돌아가기\n";
		cout << "------------------------------------------------\n";
		cout << "메뉴선택: ";
		cin >> num;
		switch (num) {
		case 1:

			while (1) {
				// //총 대출권수와 대출도서 목록 출력
				// cout << "------------------------------------------------\n";
				// cout << "총 대출 권수 : ";
				// //cout << borrowBookList->size() << endl;
				// cout << ((borrow == nullptr) ? 0 : 1) << endl;
				// cout << "\n";
				
				// 강지윤 수정 - 2차 때는 printborrow없애고, bookListPrint만 남겨놓으면 돼요.
				// vector<Book*> printborrow;
				// printborrow.emplace_back(borrow);
				// bookListPrint(printborrow, true, true, true, true, true);

				cout << "------------------------------------------------\n";
				cout << "<현재 대출 도서>\n";

				if (borrow != nullptr) {
					cout << "도서명 : " + borrow->getName() << endl;
					cout << "저자 : " + borrow->getAuthor() << endl;
					cout << "출판사 : " + borrow->getPublisher() << endl;
					cout << "발행 연도 : " + borrow->getPublishYear() << endl;
					cout << "------------------------------------------------\n";
					cout << endl;
				}
				else {
					cout << "현재 대출 중인 도서가 없습니다. \n";
					cout << "------------------------------------------------\n";
					cout << endl;
					break;
				}
				

				if (borrow != nullptr) {
					//반납과 연장 + 돌아가기 메뉴 추가
					cout << "------------------------------------------------\n";
					cout << "1. 반납하기\n2. 연장하기\n3. 돌아가기\n";
					cout << "------------------------------------------------\n";

					cout << "메뉴선택: ";
					cin >> u1;

					if (u1 == 1) {
						cout << "------------------------------------------------\n";
						cout << "정말 반납하시겠습니까? (YES: Y/y NO: Any Key)" << endl; // 한권이면 필요없음
						cout << ">> ";
						cin >> answer;

						if (answer == "Y" || answer == "y") {
							cout << "도서가 반납되었습니다." << endl;
							returnBook();
						}
						else {
							cout << "------------------------------------------------\n";
							cout << "도서 반납이 취소되었습니다.\n";
							cout << "------------------------------------------------\n";
						}
					}
					else if (u1 == 2) {
						cout << "------------------------------------------------\n";
						cout << "도서 대출을 연장합니다.";
						extendBook();
					}
					else if (u1 == 3) {
						cout << "------------------------------------------------\n";
						cout << "메뉴로 돌아갑니다.\n";
						cout << "------------------------------------------------\n";
						break;
					}
					else {
						cout << "------------------------------------------------\n";
						cout << "메뉴를 다시 선택해주세요.\n";
						cout << "------------------------------------------------\n";
					}
					system("cls");
				}
			}
			break;
		case 2: 

			//예약 취소
			while (1) {
				//총 예약권수와 예약도서 목록 출력
				cout << "------------------------------------------------\n";
				cout << "총 예약 권수 : ";
				cout << reserveBookList.size() << endl;
				cout << "\n";

				if(reserveBookList.size() == 0){ // 예약도서 없는 경우
					cout << "------------------------------------------------\n";
					cout << "0번 선택 시 이전 메뉴로 돌아갑니다: ";
					int qu;
					cin >> qu;
					if (qu == 0)
						break;
				}
				bookListPrint(reserveBookList, false, true, true, true, true);


				//돌아가기 옵션 추가 - 0번 선택 시
				cout << "------------------------------------------------\n";
				cout << "도서 번호를 선택해주세요(0번 선택 시 이전 메뉴로 돌아갑니다): ";
				int booknum;
				cin >> booknum;
				if (booknum == 0)
					break;
				else if (booknum > 0 && booknum <= reserveBookList.size())
					cancelReserveBook(booknum);
				else {
					cout << "------------------------------------------------\n";
					cout << "번호를 다시 선택해주세요.\n";
					cout << "------------------------------------------------\n";
				}
			}
			break;

		case 3:
			cout << "------------------------------------------------\n";
			cout << "이전 메뉴로 돌아갑니다.\n";
			cout << "------------------------------------------------\n";
			return;
		default:
			cout << "------------------------------------------------\n";
			cout << "메뉴를 다시 선택해주세요.\n";
			cout << "------------------------------------------------\n";
			break;
		}
		system("cls");
	}
}


void Student::returnBook() // 마이페이지 -> 책 반납 - 데이터파일 처리 필요//조수빈
{
	//vector<BorrowInfo> borrowBookList에서 해당 도서 삭제
	//vector<BorrowInfo> BI; 
	//BI = borrowBookList;
	//BI.erase(BI.begin()+booknum-1);

	/* 윤재원: 파일 처리 필요!! - 나의 정보 변경, 책 파일에도 정보 변경 필요 ************************/
	if (borrow != nullptr) {
		borrow->deleteBorrow(); // 책에서 대출자 삭제

		ofstream file("datafile/User/" + id + ".txt", ios::out);
		if (!file.is_open()) {
			cerr << "datafile/User/" + id + ".txt file is not Open for delete borrow Book" << endl; 
			exit(1);
		}

		file << password << "_" << name << "_" << s_id << endl;
		//연체여부 확인 후 변경해서 작성
		if (getDiff_date(dueDate, getCurrent_date()) > 0) //연체된 경우 - true
			file << "true" << endl << "false" << endl << endl;
		else //연체되지 않은 경우 - false
			file << "false" << endl << "false" << endl << endl;
		file << "대출도서정보" << endl;
		file << "예약도서정보" << endl;
		for (size_t i = 0; i < reserveBookList.size(); i++) {
			Book* x = reserveBookList.at(i);
			file << x->getName() << "_" << x->getAuthor() << "_";
			file << x->getTranslator() << "_" << x->getPublisher() << "_";
			file << x->getPublishYear() << endl;
		}
		file.close();
		delete borrow;	
		//borrow = nullptr;

		cout << "------------------------------------------------\n";
		cout << "해당 도서의 반납이 완료되었습니다.\n";
		cout << "------------------------------------------------\n";
	}

	else {
		cout << "반납할 도서가 존재하지 않습니다." << endl;
	}
	
}

void Student::extendBook() // 마이페이지 -> 책 연장 //조수빈
{
	//vector<BorrowInfo> BI;
	//BI = borrowBookList;
	bool reserveNumFlag = false; //예약자 존재여부 저장
	
	//int reserveNum = BI.at(booknum - 1).book->getReserveStudents().size();
	
	if (borrow != nullptr){
		if (borrow->getReservStudents().size() == 0) // 예약자가 존재 ㄴㄴ
			reserveNumFlag = false;
		else // 예약자가 존재하는 경우
			reserveNumFlag = true;	
	}
	
	if (!isOverdue && !reserveNumFlag) { // 연장에 문제가 없는경우 -> 연체 ㄴㄴ, 예약자 ㄴㄴ
		dueDate = getAfter_date(dueDate, 14);
		
		ofstream file("datafile/User/" + id + ".txt", ios::out);
		if (!file.is_open()) {
		cerr << "datafile/User/" + id + ".txt file is not Open for expend duedate" << endl; 
		exit(1);
		}

		file << password << "_" << name << "_" << s_id << endl;
		file << "false" << endl << "false" << endl << endl;
		file << "대출도서정보" << endl;
		if(borrow != nullptr) {
			file << borrow->getName() << "_" << borrow->getAuthor() << "_";
			file << borrow->getTranslator() << "_";
			file << borrowDate << "_" << dueDate << endl;
		}
		file << "예약도서정보" << endl;
		for (size_t i = 0; i < reserveBookList.size(); i++) {
			Book* x = reserveBookList.at(i);
			file << x->getName() << "_" << x->getAuthor() << "_";
			file << x->getTranslator() << "_" << x->getPublisher() << "_";
			file << x->getPublishYear() << endl;
		}
		file.close();

		cout << "------------------------------------------------\n";
		cout << "해당 도서 연장이 완료되었습니다.\n";
		cout << "------------------------------------------------\n";
	}
	else if (isOverdue) {
		//연체된 경우
		cout << "------------------------------------------------\n";
		cout << "해당 도서는 연체된 도서로, \n연장이 불가능합니다.\n";
		cout << "------------------------------------------------\n";
	}
	else if (reserveNumFlag) {
		//예약자가 존재하는 경우
		cout << "------------------------------------------------\n";
		cout << "해당 도서는 다른 사용자가 이미 예약한 도서로, \n연장이 불가능합니다.\n";
		cout << "------------------------------------------------\n";
	}

	system("cls");
}

void Student::cancelReserveBook(int booknum) // 마이페이지 -> 책 예약 취소 //조수빈
{
	//vector<Book> reserveBookList에서 해당 도서 삭제
	reserveBookList.at(booknum - 1)->deleteReserve(this); // 책에서 예약자 삭제

	reserveBookList.erase(reserveBookList.begin() + booknum - 1); // 윤재원 수정

	ofstream file("datafile/User/" + id + ".txt");
	if (!file.is_open()) {
		cerr << "datafile/User/" + id + ".txt is not open for cancelReserveBook." << endl;
		exit(1);
	}
	file << password << "_" << name << "_" << s_id << endl;
	file << "false" << endl << "false" << endl << endl;
	file << "대출도서정보" << endl;
	if (borrow != nullptr) {
		file << borrow->getName() << "_" << borrow->getAuthor() << "_" << borrow->getTranslator();
		file << borrow->getPublisher() << "_" << endl;
		file << borrowDate << "_" << dueDate << endl;
	}
	file << "예약도서정보" << endl;
	for (size_t i = 0; i < reserveBookList.size(); i++) {
		Book* x = reserveBookList.at(i);
		file << x->getName() << "_" << x->getAuthor() << "_";
		file << x->getTranslator() << "_" << x->getPublisher() << "_";
		file << x->getPublishYear() << endl;
	}
	file.close();

	cout << "------------------------------------------------\n";
	cout << "해당 도서 예약이 취소되었습니다.\n";
	cout << "------------------------------------------------\n";
}

// Book vector, 얘네들은 출력 여부 -> (대출중인 책인지, 도서명, 저자명, 대출가능여부, 예약인원수) - 강지윤
// Book 객체에 따라 출력하는 부분이 달라져서 bool로 받았습니다. 
// 추후에 bool안쓰고 어떤 리스트냐에 따라 함수 자체에서 처리하도록 개선하
// ...면 좋겠지만 그러면 book 에 멤버를 하나 들여야할 듯.
void Student::bookListPrint(vector<Book*> book, bool borrowListTF, bool nameTF, bool authorTF, bool borrowTF, bool reserveNumTF) const
{
	int listSize = book.size();

	for (int i = -1; i < listSize; i++) { // index: -1은 상단바 출력, 0부터 책 출력
		if (i == -1) {
			cout << "\n" << (nameTF ? "[도서명]" : "") << "\t" << (authorTF ? "[저자명]" : "") << "\t" << "[역자]\t[출판사]";
			if (borrowListTF) { //대출이면
				cout << "\t" << "[연체여부]" << "\t" << "[반납날짜]" << "\t" << "[연장가능여부]";
			}
			cout << "\t" << (borrowTF ? "[대출가능여부]" : "") << "\t" << (reserveNumTF ? "[예약인원수]" : "");
			cout << "\n-------------------------------------------\n";
		}
		else {
			if (borrowListTF) { //대출현황 (1차 때는 1권이라 반복문 쓸모 없지만,,)
				cout << "\n" << i + 1 << ".\t" << (nameTF ? book[i]->getName() : "") << "\t" << (authorTF ? book[i]->getAuthor() : "") << "\t" << book[i]->getTranslator() << "\t" << book[i]->getPublisher();

				cout << "\t" << (isOverdue ? "O\t" : "X\t") << dueDate << "\t" << canExtend;

				if (borrowTF) { //대출가능여부
					cout << "\t";

					if (book.at(i)->getBorrowTF()) {
						cout << "X";
					}
					else cout << "O";
				}
				if (reserveNumTF) {//예약인원수
					cout << "\t" << book[i]->getReservStudents().size();
				}
			}
			else {
				cout << "\n" << (nameTF ? book[i]->getName() : "") << "\t" << (authorTF ? book[i]->getAuthor() : "") << "\t" << book[i]->getTranslator() << "\t" << book[i]->getPublisher();
				if (borrowTF) { //대출가능여부
					cout << "\t";
					if (book[i]->getBorrowTF()) {
						cout << "X";
					}
					else cout << "O";
				}
				cout << "\t" << (reserveNumTF ? to_string(book[i]->getReservStudents().size()) : "");
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

string Student::getDueDate() const
{
	return dueDate;
}

string Student::getBookName() const
{
	return borrow->getName();
}

vector<Book*> Student::getReserveBookList()
{
	return reserveBookList;
}


bool Student::operator==(Student student) // 강지윤
{
	if (this->s_id == student.s_id) { // 학번 같으면 동일 인물
		return true;
	}
	else return false;
}
