#include "Library.h"
#include "Book.h"
#include "Admin.h"
#include "Student.h"

int main(void) {

	Library lib;

	while (lib.getCurrent_menu() != 3) { // 시스템 종료가 아닐때 까지 계속
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