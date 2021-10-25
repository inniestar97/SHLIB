#include "Book.h"
#include "Student.h"
#include "grammarCheck.h"
#include <vector>
#include <iostream>
#include <fstream>


/*   string name; // 책이름
   string author; // 책 저자
   string translator; // 역자
   string publisher; // 출판사
   string publishYear; // 발행년도
   Student borrower; // 대출자
   vector<Student> reserveStudents; // 예약한 사람들  */

Book::Book(string na, string au)
    :name(na), author(au)
{
    /*
     * 책 이름-저자명 을 인자로 생성시 책 정보를 불러 책 객체로 저장
     * 이름 저자 역자 출판사 발행연도 대출자 예약자를 가지고있음
     */

    ifstream file;
    file.open("datafile/bookDB/" + na + "-" + au + ".txt");
    if (!file.is_open()) {
        cerr << "datafile/bookDB/" + na + "-" + au + ".txt file is not open." << endl;
        exit(1);
    }

    string info;
    getline(file, info);
    this->translator = info.substr(0, info.find('_'));
    info = info.substr(info.find('_') + 1, string::npos);
    this->publisher = info.substr(0, info.find('_'));
    info = info.substr(info.find('_') + 1, string::npos);
    this->publishYear = info.substr(0, info.find('_'));

    file >> info; // info -> "대출자명단"

    file >> info; 

    if (info != "예약자명단") {  // 대출자가 있는경우
        this->borrower = new Student(info.substr(0, info.find('_'))); // borrower -> 대출 학생
        file >> info; // info-> "예약자명단"
    }
    else { // 여긴 없는겨우
        this->borrower = nullptr;
    }

    getline(file, info); // 개행문자 제거

    while (getline(file, info)) { // 예약자 아이디_이름_학번
        //reserveStudents.push_back(new Student(info.substr(0, info.find("_"))));
        newrS.emplace_back(info); // 예약자 string으로 변경

    }
    file.close();

    cout << na << " 책 등록 완료 (임시 메시지)" << endl;
}


Book::Book(string na, string au, Student* me)
    :name(na), author(au)
{
    /*
     * 책 이름-저자명 을 인자로 생성시 책 정보를 불러 책 객체로 저장
     * 이름 저자 역자 출판사 발행연도 대출자 예약자를 가지고있음
     */

    ifstream file;
    file.open("datafile/bookDB/" + na + "-" + au + ".txt");
    if (!file.is_open()) {
        cerr << "datafile/bookDB/" + na + "-" + au + ".txt file is not open." << endl;
        exit(1);
    }

    string info;
    getline(file, info);
    this->translator = info.substr(0, info.find('_'));
    info = info.substr(info.find('_') + 1, string::npos);
    this->publisher = info.substr(0, info.find('_'));
    info = info.substr(info.find('_') + 1, string::npos);
    this->publishYear = info.substr(0, info.find('_'));

    file >> info; // info -> "대출자명단"

    file >> info;

    if (info != "예약자명단") {  // 대출자가 있는경우
        this->borrower = me; // borrower -> 대출 학생
        file >> info; // info-> "예약자명단"
    }
    else {
        this->borrower = nullptr;
    }
    getline(file, info); // 개행문자 제거
    while (getline(file, info)) { // 예약자 아이디_이름_학번
        //reserveStudents.push_back(new Student(info.substr(0, info.find("_"))));
        newrS.emplace_back(info); // 예약자 string으로 변경
    }
    file.close();

    cout << na << " 책 등록 완료 (임시 메시지)" << endl;
}

Book::~Book()
{
    if (borrower != nullptr) {
        borrower = nullptr;
    }

    /*for (size_t i = 0; i < reserveStudents.size(); i++) {
        delete reserveStudents.at(i);
        reserveStudents.at(i) = nullptr;
    }
    reserveStudents.clear();*/
}

void Book::addBorrow(Student* student) { // 윤재원 (임시 출력 메시지)
    if (borrower == nullptr) { // 대출자가 없다면
        borrower = student;

        string bookpath = "datafile/bookDB/" + name + "-" + author + ".txt";

        ofstream file(bookpath, ios::out);
        if (!file.is_open()) {
            cerr << "datafile/bookDB/" + name + "-" + author + ".txt is Not Open for addBorrow." << endl;
            exit(1);
        }

        file << translator + "_" + publisher + "_" + publishYear << endl;
        file << "대출자명단" << endl;
        file << student->getId() + "_" + student->getName() + "_" + student->getS_id() << endl;
        file << "예약자명단" << endl;
        /*for (size_t i = 0; i < reserveStudents.size(); i++) {
            string reID = reserveStudents[i]->getId();
            string reName = reserveStudents[i]->getName();
            string reSID = reserveStudents[i]->getS_id();
            file << reID + "_" + reName + "_" + reSID << endl;
        }*/
        for (size_t i = 0; i < newrS.size(); i++) { // 예약자 string으로 변경
            file << newrS[i] << endl;
        }
        file.close();

        string brList = "datafile/User/forAdmin/borrowList.txt";
        ofstream file_2(brList, ios::ate);
        if (!file_2.is_open()) {
            cerr << "datafile/User/forAdmin/borrowList.txt file is not open for addBorrow in Book Class" << endl;
            exit(1); // 비정상 종료
        }

        file_2 << student->getId() << "_" << student->getName() << "_" << student->getS_id() << "-";
        file_2 << this->name << "_" << this->author << "_" << this->translator << "_" << this->publisher << "_" << this->publishYear << endl; 
        file_2.close();
    }
    else { // 대출자가 있다면
        cout << "대출 불가" << endl;
    }
}

void Book::deleteBorrow() { // 윤재원 (임시 출력 메시지)
    if (borrower != nullptr) {
        string later = borrower->getId();
        borrower = nullptr;

        string bookpath = "datafile/bookDB/" + name + "-" + author + ".txt";
        ofstream file(bookpath, ios::trunc);
        if (!file.is_open()) {
            cerr << "datafile/bookDB/" + name + "-" + author + ".txt is Not Open for deleteBorrower." << endl;
            exit(1);
        }

        file << translator + "_" + publisher + "_" + publishYear << endl;
        file << "대출자명단" << endl << "예약자명단" << endl;

        /*for (size_t i = 0; i < reserveStudents.size(); i++) {
            string reID = reserveStudents[i]->getId();
            string reName = reserveStudents[i]->getName();
            string reSID = reserveStudents[i]->getS_id();
            file << reID + "_" + reName + "_" + reSID << endl;
        }*/
        for (size_t i = 0; i < newrS.size(); i++) { // 예약자 string으로 변경
            file << newrS[i] << endl;
        }
        file.close();

        string deList = "datafile/User/forAdmin/borrowList.txt";
        string new_deList = "datafile/User/forAdmin/borrowList'.txt";
        ifstream file_1(deList);
        ofstream file_2(new_deList);
        if (!file_2.is_open() && !file_1.is_open()) {
            cerr << "datafile/User/forAdmin/borrowList.txt is not open for deleteBorrow in BookClass" << endl;
            exit(1);
        }
        string s;
        while (getline(file_1, s)) {
            if (s.substr(0, s.find("_")) == later) {
                continue;
            }

            file_2 << s;
        }

        file_1.close();
        file_2.close();
        
        remove(deList.c_str());
        rename(new_deList.c_str(), deList.c_str());

    }
    else {
        cout << "대출자 없음" << endl;
    }
}



void Book::addReserve(Student* user) // 윤재원 (강지윤이 리팩토링 좀 해놨는데 문제 생기면 말해주세요)
{
    // 이미 예약자 명단에 있을 경우 예외처리
    /*for (auto stdu : reserveStudents) {
        if (stdu == user) {
            cout << "이미 예약중인 도서입니다." << endl;
            return;
        }
    }
    reserveStudents.push_back(user);*/

    // 위에 코드 예약자 string으로 변경
    for (auto stdu : newrS) {
        // split
        string id = stdu.substr(0, stdu.find("_"));
        if (id == user->getId()) {
            cout << "이미 예약중인 도서입니다." << endl;
            return;
        }
    }
    string user_ = user->getId() + "_" + user->getName() + "_" + user->getS_id();
    newrS.emplace_back(user_);

    string bookpath = "datafile/bookDB/" + name + "-" + author + ".txt";
    ofstream file(bookpath, ios::out);
    if (!file.is_open()) {
        cerr << "datafile/bookDB/" + name + "-" + author + ".txt is Not Open for addReserve." << endl;
        exit(1); // 근데 exit 지양하는 게 좋지 않을까요? 추후에 수정 예정? - 강지
    }

    file << translator + "_" + publisher + "_" + publishYear << endl;
    file << "대출자명단" << endl;
    if (borrower != nullptr) {
        file << borrower->getId() + "_" + borrower->getName() + "_" + borrower->getS_id() << endl;
    }
    file << "예약자명단" << endl;

    /*for (size_t i = 0; i < reserveStudents.size(); i++) {
        string reID = reserveStudents[i]->getId();
        string reName = reserveStudents[i]->getName();
        string reSID = reserveStudents[i]->getS_id();
        file << reID + "_" + reName + "_" + reSID << endl;
    }*/
    // 예약자 string으로 변경
    for (size_t i = 0; i < newrS.size(); i++) {
        file << newrS[i] << endl;
    }

    file.close();

}

void Book::deleteReserve(Student* user) // 윤재원 : 책파일에서 예약자 삭제
{
    /*for (auto std : reserveStudents) {
        if (std->getId() == user->getId()) {
            reserveStudents.erase(find(reserveStudents.begin(), reserveStudents.end(), user));
            break;
        }
    }*/

    // 예약자 string으로 변경
    for (auto stdu : newrS) {
        string id = stdu.substr(0, stdu.find("_"));

        if (id == user->getId()) {
            string fi = user->getId() + "_" + user->getName() + "_" + user->getS_id();
            newrS.erase(find(newrS.begin(), newrS.end(), fi));
            break;
        }
    }

    string bookpath = "datafile/bookDB/" + name + "-" + author + ".txt";
    ofstream file(bookpath, ios::out);
    if (!file.is_open()) {
        cerr << "datafile/bookDB/" + name + "-" + author + ".txt is Not Open for addReserve." << endl;
        exit(1);
    }

    file << translator + "_" + publisher + "_" + publishYear << endl;
    file << "대출자명단" << endl;
    if (borrower != nullptr) {
        file << borrower->getId() + "_" + borrower->getName() + "_" + borrower->getS_id() << endl;
    }
    file << "예약자명단" << endl;
    /*for (size_t i = 0; i < reserveStudents.size(); i++) {
         string reID = reserveStudents[i]->getId();
         string reName = reserveStudents[i]->getName();
         string reSID = reserveStudents[i]->getS_id();
         file << reID + "_" + reName + "_" + reSID << endl;
     }*/

     // 예약자 string으로 변경
    for (size_t i = 0; i < newrS.size(); i++) {
        file << newrS[i] << endl;
    }

    file.close();
}

void Book::setName(string name)
{
    this->name = name;
}

void Book::setAuthor(string author)
{
    this->author = author;
}

void Book::setTranslator(string translator)
{
    this->translator = translator;
}

void Book::setPublisher(string publisher)
{
    this->publisher = publisher;
}

void Book::setPublishYear(string publishYear)
{
    this->publishYear = publishYear;
}

string Book::getName() const
{
    return name;
}

string Book::getAuthor() const
{
    return author;
}

string Book::getTranslator() const
{
    return translator;
}

string Book::getPublisher() const
{
    return publisher;
}

string Book::getPublishYear() const
{
    return publishYear;
}

Student* Book::getBorrower() const
{
    return borrower;
}

bool Book::getBorrowTF() const // 현재 빌린 사람이 존재하는가?
{
    // 강지윤 : 대출 가능 여부 판별
    if (borrower == nullptr) return false;
    return true;
}

int Book::getReserveStudentsSize() const
{
    return newrS.size();
}

bool Book::isFirstRSisME(Student* me) const // 사용시에 예약자 존재확실할 때만 사용
{   
    string id = newrS[0].substr(0, newrS[0].find("_"));
    if (me->getS_id() == id) return true; 
    return false;
}

bool Book::operator== (Book book) { // 윤재원
    if (this->name == book.name && this->author == book.author && this->translator == book.translator && this->publisher == book.publisher && this->publishYear == book.publishYear) {
        return true;
    }
    else return false;
}