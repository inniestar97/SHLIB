#include "Library.h"
#include "Book.h"
#include "Admin.h"
#include "Student.h"

#include <iostream>

using namespace std;

int main(void) {
	//Book::Book(string na, string au, string tr, string publisher, string year) { // 윤재원
	/*Book book("제목", "작가", "역자", "출판사", "2000");
	Student student;

	student.menu();

	*/

	Library lib;

	while (lib.getCurrent_menu() != 3)
	{ // 시스템 종료가 아닐때 까지 계속
		lib.startMenu();
	}

	return 0;
}

/*

Library -> 시작과 동시에 전체 library 를 관리하는 class
Book -> 책에 대한 정보가 들어있는 class
User -> Admin 과 Student의 부모 클래스
Admin -> 관리자로 접속됐을때 생성되는 클래스
Student -> 학생으로 접속했을때 생성되는 클래스

*/
