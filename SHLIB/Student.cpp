#include "Student.h"
#include "grammarCheck.h"
#include "Book.h"
#include "Library.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <climits>
#include <io.h>
#include <filesystem>
#include <set>

#define BASKETMAX 10
#define BORROWMAX 3
#define RESERVEUSERMAX 3
#define RESERVEBOOKMAX 5

// using namespace std;
using std::cout;

Student::Student(string id)
    : current_menu(0)
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

    /* 
    "datafile/User/" + id + ".txt" 파일 양식

    [비밀번호]_[이름]_[학번]
    (제한상태일 경우) 날짜 / (제한상태가 아닐경우) false
    (제한상태 누적 횟수)

    대출도서정보
    1.[도서명]_[저자명]_[역자]_[출판사]_[대출일]_[반납일]
    2.[도서명]_[저자명]_[역자]_[출판사]_[대출일]_[반납일]
    3.
    예약도서정보
    1.[도서명]_[저자명]_[역자]_[출판사]
    2.[도서명]_[저자명]_[역자]_[출판사]
    3.[도서명]_[저자명]_[역자]_[출판사]    
    */

    string info;
    file >> info; // info = 비밀번호_이름_학번
    this->password = info.substr(0, info.find('_')); // 비밀번호
    info = info.substr(info.find('_') + 1, string::npos); // info = 이름_학번
    this->name = info.substr(0, info.find('_')); // 이름
    this->s_id = info.substr(info.find('_') + 1, string::npos); // 학번

    file >> info; // 제한상태 여부
    //if (info != "false") limitDate = info; // 제한상태일 경우
    //else limitDate = "false"; // 제한상태가 아닐경우
    limitDate = info;

    file >> info; // 제한상태 누적
    limitedStack = stoi(info);

    file >> info; // info = "대출도서 정보"
    getline(file, info);
    for (size_t i = 0; i < 3; i++) {
        getline(file, info); // info : x.[도서명]_[저자명]_[역지]_~~요런거

        if (info.size() > 3) { // 대출도서 ㅇㅇ
            info = info.substr(2, string::npos);

            //file >> info; // info = 도서명_저자명_역자_출판사_발행연도_대출일_반납일
            string split;
            stringstream ss(info);
            vector<string> b_info; b_info.clear();
            while (getline(ss, split, '_')) {
                b_info.push_back(split);
            }

            /*
                b_info[0] = 도서명
                b_info[1] = 저자명
                b_info[2] = 역자
                b_info[3] = 출판사 
                b_info[4] = 대출일
                b_info[5] = 반납일
            */
            BorrowInfo br;
            br.book = new Book(b_info[0], b_info[1]);
            br.borrowDate = b_info[4];
            br.dueDate = b_info[5];

            borrowBookList.push_back(br);
        }
    }

    file >> info; // info = "예약도서 정보"
    getline(file, info);
    for (size_t i = 0; i < 3; i++) {
        getline(file, info);

        if (info.size() > 3) { // 예약도서 ㅇㅇ
            info = info.substr(2, string::npos);

            string split;
            stringstream ss(info);
            vector<string> b_info; b_info.clear();
            while (getline(ss, split, '_')) {
                b_info.push_back(split);
            }

            /*
                b_info[0] = 도서명
                b_info[1] = 저자명
                b_info[2] = 역자
                b_info[3] = 출판사
                b_info[4] = 발행연도
            */

            reserveBookList.push_back(new Book(b_info[0], b_info[1]));
        }
    }

    while (file.is_open()) file.close(); // 파일 닫어주면 생성자 끗! 
   
     /* 제한상태 돌입 여부 판별 */
    // Idea: 책을 1권 이상 빌렸을 때: 빌린 책들마다 반납 날짜를 확인해서 하나라도 연체된 경우 제한상태(true)에 돌입한다.
    if (borrowBookList.size() > 0) {
        overDueBookNum = 0;
        bool isOverdue = false;
        for (auto bookinfo : borrowBookList) {
            if (getDiff_date(bookinfo.dueDate, getCurrent_date()) > 0) { // 앞 < 뒤 면 양수
                isOverdue = true;
                overDueBookNum++;
                limitDate = "true";
            }
        }
        if (isOverdue) {
            // 제한상태 돌입 -> 반납할 때 사용 (ReturnBook 함수)

            // 모든 예약 취소
            for (auto book : reserveBookList) {
                book->deleteReserve(this);
            }
            reserveBookList.clear();

            //-----------------------------------------------------------------
            //---------------- UserId.txt 파일 수정 code ----------------------
            //-----------------------------------------------------------------
            ofstream fileLimited("datafile/User/" + id + ".txt", ios::trunc);
            if (!fileLimited.is_open()) {
                cerr << "생성자 제한상태 관련 UserId.txt 파일 open 실패" << endl;
                exit(1);
            }

            fileLimited << password + "_" + name + "_" + s_id << endl;
            fileLimited << limitDate << endl;
            fileLimited << limitedStack << endl << endl;

            fileLimited << "대출도서정보" << endl;
            for (size_t i = 1; i <= 3; i++) {
                fileLimited << i << ".";
                if (borrowBookList.size() >= i) {
                    BorrowInfo bi = borrowBookList.at(i - 1);
                    fileLimited << bi.book->getName() << "_";
                    fileLimited << bi.book->getAuthor() << "_";
                    fileLimited << bi.book->getTranslator() << "_";
                    fileLimited << bi.book->getPublisher() << "_";
                    fileLimited << bi.borrowDate << "_";
                    fileLimited << bi.dueDate;
                }
                fileLimited << endl;
            }
            
            fileLimited << "예약도서정보" << endl;
            for (size_t i = 1; i <= 3; i++) {
                fileLimited << i << "." << endl;
            }

            while (!fileLimited.is_open()) fileLimited.close();
            //-----------------------------------------------------------------
            //-----------------------------------------------------------------
            //-----------------------------------------------------------------
        } 
    }

    /* 제한 상태 해제 : 해제 날짜에 도착 */
    if (limitDate != "false" && limitDate != "true") {
        if(getDiff_date(limitDate, getCurrent_date()) > 0) { // 앞 < 뒤 면 양수
            limitDate = "false";
            //-----------------------------------------------------------------
            //---------------- UserId.txt 파일 수정 code ----------------------
            //-----------------------------------------------------------------
            ofstream fileLimited("datafile/User/" + id + ".txt", ios::trunc);
            if (!fileLimited.is_open()) {
                cerr << "생성자 제한상태 관련 UserId.txt 파일 open 실패" << endl;
                exit(1);
            }

            fileLimited << password + "_" + name + "_" + s_id << endl;
            fileLimited << limitDate << endl;
            fileLimited << limitedStack << endl << endl;

            fileLimited << "대출도서정보" << endl;
            for (size_t i = 1; i <= 3; i++) {
                fileLimited << i << ".";
                if (borrowBookList.size() >= i) {
                    BorrowInfo bi = borrowBookList.at(i - 1);
                    fileLimited << bi.book->getName() << "_";
                    fileLimited << bi.book->getAuthor() << "_";
                    fileLimited << bi.book->getTranslator() << "_";
                    fileLimited << bi.book->getPublisher() << "_";
                    fileLimited << bi.borrowDate << "_";
                    fileLimited << bi.dueDate;
                }
                fileLimited << endl;
            }

            fileLimited << "예약도서정보" << endl;
            for (size_t i = 1; i <= 3; i++) {
                fileLimited << i << "." << endl;
            }

            while (!fileLimited.is_open()) fileLimited.close();
        }
    }
}

Student::~Student()
{
    /*
        현재 동적할당 된 부분 (학생 정보에서만) 
        borrowBookList 내부에서 Book 객체
        reserveBookList에서 Book 객체
    */
   /*---------------대출 벡터, 예약벡터 정리--------------*/
   for (size_t i = 0; i < borrowBookList.size(); i++) {
       delete borrowBookList.at(i).book;
       borrowBookList.at(i).book = nullptr;
   }
   for (size_t i = 0; i < reserveBookList.size(); i++) {
       delete reserveBookList.at(i);
       reserveBookList.at(i) = nullptr;
   }
   /*----------------------------------------------------*/
   

}

void Student::menu() // 사용자 모드 메뉴
{
    int selectNum;
    cout << "=============\n사용자 모드 메뉴\n=============\n";
    cout << "1. 자료 검색\n2. 장바구니\n3. 마이페이지\n4. 로그아웃\n";
    cout << "============================================\n";

    selectNum = input("\n메뉴 선택:", 1, 4);

    // selectNum에 따라 메뉴 호출
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
    bookList.clear();
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

        bookList.push_back(new Book(na, au));
    }

    return;
}

void Student::searchBookMenu() // 자료검색 - 윤재원
{
    if (limitDate == "true") {
        cout << "제한상태이므로 자료 검색 메뉴를 사용하실 수 없습니다. 연체하신 책을 모두 반납해주세요." << endl;
        return;
    }
    if (limitDate != "false") {
        cout << "제한상태이므로 자료 검색 메뉴를 사용하실 수 없습니다. 제한상태 해제일: [" << limitDate << "]" << endl;
        return;
    }
    this->initBookList();
    searchResult.clear(); // 벡터 초기화
    int basketListNum;
    cout << "1. 도서명으로 검색\n2. 저자명으로 검색\n3. 메인메뉴로 돌아가기\n";
    basketListNum = input("\n메뉴선택: ", 1, 3);
//    while (getchar() != '\n');

    switch (basketListNum) {
    case 1:
    {
        string bookName;
        cout << "도서명 (뒤로 가려면 :q 를 입력하세요) >> ";
        getline(cin, bookName);

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
        getline(cin, bookauthor);
        bookauthor = bookauthor.substr(0, bookauthor.find('\n'));
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
        bookListPrint(searchResult, true, true, true, true);

        // cout << "=======================" << endl << "1. 장바구니 담기\n2.이전 페이지 이동하기\n3. 다음 페이지 이동하기\n4. 돌아가기" << endl << "=======================" << endl;
        cout << "=======================" << endl << "1. 장바구니 담기\n2. 돌아가기" << endl << "=======================" << endl;

        int option;
        option = input( "\n메뉴를 선택하세요: ", 1, 2);

        if (option == 1) {
            int bookbasketListNum;
            bool isExistBasket = false;
            //bookbasketListNum = input("\n장바구니에 담을 책 번호를 선택하세요: ", 1, bookBasketList.size() + 1);
            bookbasketListNum = input("\n장바구니에 담을 책 번호를 선택하세요: ", 1, searchResult.size());


            // 장바구니에 있으면 담기 실패
            for (auto book : bookBasketList) {
                if (*book == *searchResult[(int)(bookbasketListNum - 1)]) { 
                    cout << "장바구니에 이미 담은 책입니다. 다시 선택해주세요." << endl;
                    isExistBasket = true;
                    break;
                }
            }
            
            if (!isExistBasket) {
                bookBasketList.push_back(searchResult[(int)(bookbasketListNum - 1)]);
                cout << "[" << searchResult[(int)(bookbasketListNum - 1)]->getName() << "]을 장바구니에 담았습니다." << endl;
            }
        }
        else
            return;
    }
}

void Student::bookBasketMenu()// 장바구니 메뉴 - 강지윤
{
    if (limitDate == "true") {
        cout << "제한상태이므로 장바구니 메뉴를 사용하실 수 없습니다. 연체하신 책을 모두 반납해주세요." << endl;
        return;
    }
    if (limitDate != "false") {
        cout << "제한상태이므로 장바구니 메뉴를 사용하실 수 없습니다. 제한상태 해제일: [" << limitDate << "]" << endl;
        return;
    }
    int basketListNum;

    while (1) {
        cout << "\n장바구니\n";
        bookListPrint(bookBasketList, true, true, true, true);
        // 일단 1. 일괄대출 -> 선택대출로 변경 (대출 제한 1권이면)
        cout << "\n-----------------------------------------\n";
        cout << "\t1. 도서 선택 대출\n\t2. 도서 선택 삭제\n\t3. 도서 선택 예약\n\t4. 돌아가기";
        cout << "\n-----------------------------------------\n";

        basketListNum = input("\n메뉴선택: ", 1, 4);

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


void Student::sel_borrowBook() // 장바구니 -> 선택대출 
{
    size_t basketListNum = bookBasketList.size();
    while (true) {
        // 장바구니가 빈 경우
        if (bookBasketList.empty()) {
            cout << "------------------------------------------------\n";
            cout << "장바구니에 담은 책이 없습니다. \n"; 
            cout << "------------------------------------------------\n";
            // 메뉴로 돌아갑니다.
            return;
        }
        // 창 초기화 필요
        bookListPrint(bookBasketList, true, true, true, true);

        cout << "------------------------------------------------";

        int select; // +1 해서 생각해야 됨.
        
        select = input("\n대출할 책의 번호를 입력하세요 (0을 입력하면 메뉴로 돌아갑니다.): ", 0, bookBasketList.size());

        if (select == 0) {
            cout << "\n메뉴로 돌아갑니다.\n";
            cout << "------------------------------------------------\n";
            return;
        }

        // 3. 대출횟수 소진 - 대출불가
        if (borrowBookList.size() >= 3) { // 1권 -> 3권 
            cout << "------------------------------------------------\n";
            cout << "대출횟수를 모두 소진하였습니다.\n";
            cout << "------------------------------------------------\n";
            return;
        }

        // 4. 연체된 경우 - 대출 불가
        // -> 4. 제한상태인 경우 - 대출불가로 변경
        if (limitDate != "false") {
            cout << "------------------------------------------------\n";
            cout << "제한상태라 대출이 불가합니다.\n";
            cout << "------------------------------------------------\n";
            return;
        }

        if (bookBasketList.at((int)(select - 1))->getBorrowTF()) { // 1. 이미 대출된 책.
            cout << "------------------------------------------------\n";
            cout << "다른 사용자가 대출 중입니다.\n";
            cout << "------------------------------------------------\n";
            continue;
        } // 2. 첫번째 예약자 != 나
        else if (bookBasketList.at((int)(select - 1))->getReserveStudentsSize() > 0 && !bookBasketList.at((int)(select - 1))->isFirstRSisME(this)) {
            cout << "------------------------------------------------\n";
            cout << "우선 예약자가 대기중입니다.\n";
            cout << "------------------------------------------------\n";
            continue;
        }
        else { // 대출 완료.
                
            BorrowInfo bi_temp;
            bi_temp.book = bookBasketList.at((int)(select - 1));
            bi_temp.borrowDate = getCurrent_date();
            bi_temp.dueDate = getAfter_date(getCurrent_date(), 14);
            borrowBookList.push_back(bi_temp);

            // basket에 있는 책 삭제
            bookBasketList.at((int)(select - 1))->addBorrow(this);
            bookBasketList.erase(bookBasketList.begin() + select - 1); // 삭제

            //-----------------------------------------------------------------
            //----------------UserId.txt 파일 수정 code-------------------------
            //-----------------------------------------------------------------
            string stu_path = "datafile/User/" + id + ".txt";
            ofstream student_file(stu_path, ios::trunc);
            if (!student_file) {
                cout << "대출완료시 파일 수정 open 실패" << endl;
                return;
            }

            student_file << password << "_" << name << "_" << s_id << endl;
            student_file << limitDate << endl;
            student_file << limitedStack << endl << endl;

            student_file << "대출도서정보" << endl;
            for (size_t i = 1; i <= 3; i++) {
                student_file << i << ".";
                if (borrowBookList.size() >= i) {
                    BorrowInfo bi = borrowBookList.at(i - 1);
                    student_file << bi.book->getName() << "_";
                    student_file << bi.book->getAuthor() << "_";
                    student_file << bi.book->getTranslator() << "_";
                    student_file << bi.book->getPublisher() << "_";
                    student_file << bi.borrowDate << "_";
                    student_file << bi.dueDate;
                }
                student_file << endl;
            }
            student_file << "예약도서정보" << endl;
            for (size_t i = 1; i <= 3; i++) {
                student_file << i << ".";
                if (reserveBookList.size() >= i) {
                    Book* b = reserveBookList.at(i - 1);
                    student_file << b->getName() << "_";
                    student_file << b->getAuthor() << "_";
                    student_file << b->getTranslator() << "_";
                    student_file << b->getPublisher() << "_";
                }
                student_file << endl;
            }

            while (!student_file.is_open()) student_file.close();
            //-----------------------------------------------------------------
            //-----------------------------------------------------------------
            //-----------------------------------------------------------------

            cout << "해당 도서의 대출이 완료되었습니다.\n";
            cout << "------------------------------------------------\n";
        }
    }
}

void Student::deleteBook() // 장바구니 -> 도서 선택 삭제
{
    while (true) {
        // 장바구니가 빈 경우
        if (bookBasketList.empty()) {
            cout << "------------------------------------------------\n";
            cout << "장바구니에 담은 책이 없습니다. 메뉴로 돌아갑니다. \n"; //엔터키를 누르면 
            cout << "------------------------------------------------\n";
            // string yn;
            // cin >> yn;
            return;
        }
        // 창 초기화 필요
        bookListPrint(bookBasketList, true, true, true, true);

        cout << "------------------------------------------------";

        int select; // +1 해서 생각해야 됨.
        select = input("\n삭제할 책의 번호를 입력하세요 (0을 입력하면 메뉴로 돌아갑니다.): ", 0, bookBasketList.size());
        
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

void Student::reserveBook() // 장바구니 -> 도서 선택 예약 (데이터 파일 다루기 필요)
{
    while (true) {
        // 장바구니가 빈 경우
        if (bookBasketList.empty()) {
            cout << "------------------------------------------------\n";
            cout << "장바구니에 담은 책이 없습니다. 메뉴로 돌아갑니다. \n"; //엔터키를 누르면 
            cout << "------------------------------------------------\n";
            // string yn;
            // cin >> yn;
            return;
        }

        // 사용자의 예약 횟수를 모두 소진한 경우 break;
        if (reserveBookList.size() >= RESERVEUSERMAX) {
            cout << "------------------------------------------------\n";
            cout << "예약횟수가 모두 소진되었습니다. 메뉴로 돌아갑니다. \n"; //엔터키를 누르면 
            cout << "------------------------------------------------\n";

            // string yn;
            // cin >> yn;
            return;
        }
        // 창 초기화 필요
        bookListPrint(bookBasketList, true, true, true, true);
        cout << "------------------------------------------------";

        int select; // +1 해서 생각해야 됨.
        select = input("\n예약할 책의 번호를 입력하세요. (0을 입력하면 메뉴로 돌아갑니다.): ", 0, bookBasketList.size());

        if (select == 0) {
            cout << "\n메뉴로 돌아갑니다.\n";
            cout << "------------------------------------------------\n";
            return;
        }


        // 예약불가 : 도서별 예약가능인원 (5명) 초과 (사용자 예약 횟수 초과는 위에서 다룸)
        if (bookBasketList.at((int)(select - 1))->getReserveStudentsSize() >= RESERVEBOOKMAX) {
            cout << "------------------------------------------------\n";
            cout << "해당 도서의 예약 가능 인원수가 초과되었습니다.\n";
        }
        else if (bookBasketList.at((int)(select - 1))->getBorrower() == (id + "_" + name + "_" + s_id)) { // 이미 사용자가 대출중인 책인 경우
            cout << "------------------------------------------------\n";
            cout << "해당 도서를 이미 대출중입니다.\n";
        }
        else {

            reserveBookList.push_back(bookBasketList.at((int)(select - 1))); //예약
            bookBasketList.at((int)(select - 1))->addReserve(this);
            bookBasketList.erase(bookBasketList.begin() + select - 1); // 장바구니에서 예약된 도서 삭제 (혹시 사용자가 이중으로 선택할까봐)

            //-----------------------------------------------------------------
            //----------------UserId.txt 파일 수정 code-------------------------
            //-----------------------------------------------------------------
			ofstream file("datafile/User/" + id + ".txt", ios::trunc);
			if (!file.is_open()) {
				cerr << "예약 관련 UserId.txt 파일 open 실패" << endl;
				exit(1);
			}

			file << password + "_" + name + "_" + s_id << endl;
            file << limitDate << endl;
            file << limitedStack << endl << endl;

            file << "대출도서정보" << endl;
            for (size_t i = 1; i <= 3; i++) {
                file << i << ".";
                if (borrowBookList.size() >= i) {
                    BorrowInfo bi = borrowBookList.at(i - 1);
                    file << bi.book->getName() << "_";
                    file << bi.book->getAuthor() << "_";
                    file << bi.book->getTranslator() << "_";
                    file << bi.book->getPublisher() << "_";
                    file << bi.borrowDate << "_";
                    file << bi.dueDate;
                }
                file << endl;
            }
            file << "예약도서정보" << endl;
            for (size_t i = 1; i <= 3; i++) {
                file << i << ".";
                if (reserveBookList.size() >= i - 1) {
                    Book* b = reserveBookList.at(i - 1);
                    file << b->getName() << "_";
                    file << b->getAuthor() << "_";
                    file << b->getTranslator() << "_";
                    file << b->getPublisher() << "_";
                }
                file << endl;
            }

            while (!file.is_open()) file.close();
            //-----------------------------------------------------------------
            //-----------------------------------------------------------------
            //-----------------------------------------------------------------

            cout << "------------------------------------------------\n";
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
        cout << "------------------------------------------------";

        num = input("\n메뉴선택: ", 1, 3);

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
                if(!borrowBookList.empty()){
                // if (borrow != nullptr) {
                    for(int bi=0;bi<borrowBookList.size();bi++){
                        Book* borrow = borrowBookList.at(bi).book;
                        cout << "======= "<<bi+1 << "번 ======="<<endl;
                        cout << "도서명 : " + borrow->getName() << endl;
                        cout << "저자 : " + borrow->getAuthor() << endl;
                        cout << "출판사 : " + borrow->getPublisher() << endl;
                        cout << "발행 연도 : " + borrow->getPublishYear() << endl;
                        cout << "반납 날짜 : " << borrowBookList.at(bi).dueDate << endl;
                        if (limitDate != "false") // 제한상태인 경우 - true
                            cout << "제한상태 여부 : O" << endl;
                        else //제한상태 아닌 경우 - false
                            cout << "제한상태 여부 : X"  << endl;

                        
                        if ((limitDate == "false") && borrow->getReserveStudentsSize() == 0)
                            cout << "연장 가능 여부 : O" << endl;
                        else
                            cout << "연장 가능 여부 : X" << endl;
                        cout << "------------------------------------------------\n";
                        cout << endl;
                    }
                }
                else {
                    cout << "현재 대출 중인 도서가 없습니다.\n";
                    cout << "------------------------------------------------\n";
                    cout << endl;
                    break;
                }


                if (!borrowBookList.empty()) {
                    //반납과 연장 + 돌아가기 메뉴 추가
                    cout << "------------------------------------------------\n";
                    cout << "1. 반납하기\n2. 연장하기\n3. 돌아가기\n";
                    cout << "------------------------------------------------";

                    u1 = input("\n메뉴선택: ", 1, 3);

                    if (u1 == 1) {
                        
                        int bnum = input("\n반납할 도서 번호를 선택하세요(취소:0): ", 0, borrowBookList.size());
                        
                        cout << "------------------------------------------------\n";
                        cout << "정말 반납하시겠습니까? (YES: Y/y NO: Any Key)" << endl;
                        cout << ">> ";
                        
                        getline(cin, answer);

                        if (answer == "Y" || answer == "y") {
                            //cout << "도서를 반납합니다." << endl;
                            returnBook(bnum - 1);
                        }
                        else {
                            cout << "------------------------------------------------\n";
                            cout << "도서 반납이 취소되었습니다.\n";
                            cout << "------------------------------------------------\n";
                        }
                    }
                    else if (u1 == 2) {
                        int bnum = input("\n연장할 도서 번호를 선택하세요(취소:0): ", 0, borrowBookList.size());
                        extendBook(bnum - 1);

                        // cout << "------------------------------------------------\n";
                        // cout << "도서 대출을 연장합니다.\n";
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

                if (reserveBookList.size() == 0) { // 예약도서 없는 경우
                    cout << "------------------------------------------------";
                    int qu;
                    qu = input("\n 0번 선택 시 이전 메뉴로 돌아갑니다:: ", 0, 0);

                    if (qu == 0) 
                        break;
                }
                bookListPrint(reserveBookList, true, true, true, true);


                //돌아가기 옵션 추가 - 0번 선택 시
                cout << "------------------------------------------------";
                int booknum;
                booknum = input("\n예약을 취소할 도서 번호를 선택해주세요(0번 선택 시 이전 메뉴로 돌아갑니다): ", 0, reserveBookList.size()+1);

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
    }
}


void Student::returnBook(int bi) // 마이페이지 -> 책 반납 //조수빈
{
    //vector<BorrowInfo> borrowBookList에서 해당 도서 삭제
    //vector<BorrowInfo> BI; 
    //BI = borrowBookList;
    //BI.erase(BI.begin()+booknum-1);
    
    if(!borrowBookList.empty()){
    // if (borrow != nullptr) {
        Book* borrow = borrowBookList.at(bi).book;
        borrow->deleteBorrow(); // 책에서 대출자 삭제
        
        ofstream file("datafile/User/" + id + ".txt", ios::out);
        if (!file.is_open()) {
            cerr << "datafile/User/" + id + ".txt file is not Open for delete borrow Book" << endl;
            exit(1);
        }

        /* [비밀번호]_[이름]_[학번]
        제한상태 여부(true / false)
        제한상태 누적 횟수 Default = 0
        */

        file << password << "_" << name << "_" << s_id << endl;
        // 연체여부 확인 후 제한 상태 변경       
        if (getDiff_date(borrowBookList.at(bi).dueDate, getCurrent_date()) > 0) { // 앞 < 뒤 면 양수
            if (limitDate == "true") {
                overDueBookNum--;
                if (overDueBookNum == 0) { // 연체된 책 모두 반납하면 true -> 제한상태날짜로 변경.
                    limitedStack++;
                    limitDate = getAfter_date(getCurrent_date(), 14 * limitedStack);
                }
            }
        }
        file << limitDate << endl << limitedStack << endl;

        borrowBookList.erase(borrowBookList.begin() + bi);

        file << "대출도서정보" << endl;
        for (size_t i = 1; i <= 3; i++) {
            file << i << ".";
            if (borrowBookList.size() >= i) {
                BorrowInfo x = borrowBookList.at(i - 1);
                file << x.book->getName() << "_";
                file << x.book->getAuthor() << "_";
                file << x.book->getTranslator() << "_";
                file << x.book->getPublisher() << "_";
                file << x.borrowDate << "_";
                file << x.dueDate;
            }
            file << endl;
        }
        file << "예약도서정보" << endl;
        for (size_t i = 1; i <= 3; i++) {
            file << i << ".";
            if (reserveBookList.size() >= i) {
                Book* b = reserveBookList.at(i - 1);
                file << b->getName() << "_";
                file << b->getAuthor() << "_";
                file << b->getTranslator() << "_";
                file << b->getPublisher() << "_";
            }
            file << endl;
        }

        while (!file.is_open()) file.close();

        borrow = nullptr;

        cout << "------------------------------------------------\n";
        cout << "해당 도서의 반납이 완료되었습니다.\n";
        cout << "------------------------------------------------\n";
    }

    else {
        cout << "------------------------------------------------\n";
        cout << "반납할 도서가 존재하지 않습니다." << endl;
        cout << "------------------------------------------------\n";
    }

}

void Student::extendBook(int bi) // 마이페이지 -> 책 연장 //조수빈
{
    bool reserveNumFlag = false; //예약자 존재여부 저장

    //if (borrow != nullptr) {
    if(!borrowBookList.empty()){
        BorrowInfo bInfo = borrowBookList.at(bi); // 현재 연장하려는 책
        Book* borrow = bInfo.book;

        if (borrow->getReserveStudentsSize() == 0) // 연장하려는 책에 예약자가 존재 안하는 경우
            reserveNumFlag = false;
        else // 예약자가 존재하는 경우
            reserveNumFlag = true;

        if ((limitDate == "false") && !reserveNumFlag) { 
            // 연장에 문제가 없는경우 제한상태X 예약자X
            bInfo.dueDate = getAfter_date(bInfo.dueDate, 14);

            //-----------------------------------------------------------------
            //----------------UserId.txt 파일 수정 code-------------------------
            //-----------------------------------------------------------------
            ofstream file("datafile/User/" + id + ".txt", ios::trunc);
            if (!file.is_open()) {
                cerr << "책 연장여부 관련 UserId.txt 파일 open 실패" << endl;
                exit(1);
            }

			file << password + "_" + name + "_" + s_id << endl;
            file << limitDate << endl;
            file << limitedStack << endl << endl;

            file << "대출도서정보" << endl;
            for (size_t i = 1; i <= 3; i++) {
                file << i << ".";
                if (borrowBookList.size() >= i) {
                    BorrowInfo x = borrowBookList.at(i - 1);
                    file << x.book->getName() << "_";
                    file << x.book->getAuthor() << "_";
                    file << x.book->getTranslator() << "_";
                    file << x.book->getPublisher() << "_";
                    file << x.borrowDate << "_";
                    file << x.dueDate;
                }
                file << endl;
            }
            file << "예약도서정보" << endl;
            for (size_t i = 1; i <= 3; i++) {
                file << i << ".";
                if (reserveBookList.size() >= i) {
                    Book* b = reserveBookList.at(i - 1);
                    file << b->getName() << "_";
                    file << b->getAuthor() << "_";
                    file << b->getTranslator() << "_";
                    file << b->getPublisher() << "_";
                }
                file << endl;
            }

            while (!file.is_open()) file.close();
            //-----------------------------------------------------------------
            //-----------------------------------------------------------------
            //-----------------------------------------------------------------

            cout << "------------------------------------------------\n";
            cout << "해당 도서 연장이 완료되었습니다.\n";
            cout << "------------------------------------------------\n";
        }
        else if (limitDate != "false") {
            //연체된 경우
            cout << "------------------------------------------------\n";
            cout << "사용자는 현재 제한상태이므로 연장이 불가능합니다.\n";
            cout << "------------------------------------------------\n";
        }
        else if (reserveNumFlag) {
            //예약자가 존재하는 경우
            cout << "------------------------------------------------\n";
            cout << "해당 도서는 다른 사용자가 이미 예약한 도서로, \n연장이 불가능합니다.\n";
            cout << "------------------------------------------------\n";
        }
    }
    else {
        cout << "------------------------------------------------\n";
        cout << "연장할 수 있는 책이 존재하지 않습니다." << endl;
        cout << "------------------------------------------------\n";
    }
}

void Student::cancelReserveBook(int booknum) // 마이페이지 -> 책 예약 취소(데이터파일) //조수빈
{
    if (reserveBookList.size() != 0) {
        Book* b = reserveBookList.at(booknum - 1); // 예약을 취소할 책
        b->deleteReserve(this); // 책에서 예약자 삭제

        reserveBookList.erase(reserveBookList.begin() + booknum - 1); // 윤재원 수정

		//-----------------------------------------------------------------
		//----------------UserId.txt 파일 수정 code-------------------------
		//-----------------------------------------------------------------
        ofstream file("datafile/User/" + id + ".txt", ios::trunc);
        if (!file.is_open()) {
            cerr << "예약 취소 수정 관련 UserId.txt파일 open 실패" << endl;
            exit(1);
        }

		file << password + "_" + name + "_" + s_id << endl;
		file << limitDate << endl;
		file << limitedStack << endl << endl;

		file << "대출도서정보" << endl;
		for (size_t i = 1; i <= 3; i++) {
			file << i << ".";
			if (borrowBookList.size() >= i) {
				BorrowInfo bi = borrowBookList.at(i - 1);
				file << bi.book->getName() << "_";
				file << bi.book->getAuthor() << "_";
				file << bi.book->getTranslator() << "_";
				file << bi.book->getPublisher() << "_";
				file << bi.borrowDate << "_";
				file << bi.dueDate;
			}
			file << endl;
		}
		file << "예약도서정보" << endl;
		for (size_t i = 1; i <= 3; i++) {
			file << i << ".";
			if (reserveBookList.size() >= i) {
				Book* b = reserveBookList.at(i - 1);
				file << b->getName() << "_";
				file << b->getPublisher() << "_";
			}
			file << endl;
		}

		while (!file.is_open()) file.close();
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------


        cout << "------------------------------------------------\n";
        cout << "해당 도서 예약이 취소되었습니다.\n";
        cout << "------------------------------------------------\n";
    }
    else {
        cout << "------------------------------------------------\n";
        cout << "예약을 취소할 수 있는 도서가 존재하지 않습니다." << endl;
        cout << "------------------------------------------------\n";
    }

}

// Book vector, 얘네들은 출력 여부 -> (대출중인 책인지, 도서명, 저자명, 대출가능여부, 예약인원수) - 강지윤
// Book 객체에 따라 출력하는 부분이 달라져서 bool로 받았습니다. 
// 추후에 bool안쓰고 어떤 리스트냐에 따라 함수 (개선하
// ...면 좋겠지만 그러면 book 에 멤버를 하나 들여야할 듯.
void Student::bookListPrint(vector<Book*> book, bool nameTF, bool authorTF, bool borrowTF, bool reserveNumTF) const
{

    int listSize = book.size();

    for (int i = -1; i < listSize; i++) { // index: -1은 상단바 출력, 0부터 책 출력
        if (i == -1) {
            cout << "\n" << "    " << (nameTF ? "[도서명]" : "") << "\t" << (authorTF ? "[저자명]" : "") << "\t" << "[역자]\t[출판사]";
            cout << "\t" << (borrowTF ? "[대출가능여부]" : "") << "\t" << (reserveNumTF ? "[예약인원수]" : "");
            cout << "\n---------------------------------------------------------------------------------------------\n";
        }
        else {
            cout << "\n" << i+1 << "번: " << (nameTF ? book[i]->getName() : "") << "\t" << (authorTF ? book[i]->getAuthor() : "") << "\t" << book[i]->getTranslator() << "\t" << book[i]->getPublisher();
            if (borrowTF) { //대출가능여부
                cout << "\t";
                if (book[i]->getBorrowTF()) {
                    cout << "X";
                }
                else cout << "O";
            }
            cout << "\t" << (reserveNumTF ? to_string(book[i]->getReserveStudentsSize()) : "");
            
        }
    }
    cout << "\n---------------------------------------------------------------------------------------------\n";
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

void Student::setLimitDate(string date)
{
    this->limitDate = date;
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

string Student::getLimitDate() const
{
    return limitDate;
}


int Student::getBorrowListNum() const
{
    return borrowBookList.size();
}

string Student::getBookName(int bi) const
{
    if(getBorrowListNum() < bi){
        string nu = "NULL";
        return nu;
    }
    return borrowBookList.at(bi).book->getName();
}

string Student::getBorrowDate(int bi) const{
    if(getBorrowListNum() < bi){
        string nu = "NULL";
        return nu;
    }
    return borrowBookList.at(bi).borrowDate;
}

string Student::getDueDate(int bi) const{

    if(getBorrowListNum() < bi){
        string nu = "NULL";
        return nu;
    }
    return borrowBookList.at(bi).dueDate;
}


bool Student::operator==(Student student) 
{
    if (this->s_id == student.s_id) { // 학번 같으면 동일 인물
        return true;
    }
    else return false;
}

bool Student::operator>=(Student Student) {
    int s_min = 99999999;
    int c_min = 99999999;

    string s;
    for (size_t i = 0; i < 3; i++) {
        s = Student.getDueDate(i);
        s.erase(remove(s.begin(), s.end(), '.'));
        s_min = min(s_min, stoi(s));
    }

    for (size_t i = 0; i < 3; i++) {
        s = borrowBookList.at(i).dueDate;
        s.erase(remove(s.begin(), s.end(), '.'));
        c_min = min(c_min, stoi(s));
    }

    return c_min <= s_min;
}