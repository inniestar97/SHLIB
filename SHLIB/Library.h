#pragma once
class Library
{
private:
	int current_menu; // 현재 메뉴
public:
	//vector<Book*> bookList; // 처음 로그인한 후 북리스트 초기화. 어떻게 할지 다음 시간에 정하겠습니다. (윤재원)
	Library();
	void startMenu();

	void login(); // 로그인
	void makeAccount(); // 회원가입
	void initBookList(); // 북리스트 초기화 (cpp에 아직 안만들었습니다. 데이터 파일 정한 후 만드는게 좋을 것 같습니다. - 윤재원) (먼저 Student에서 구현하였습니다)
	// void changeDate();

	void setCurrent_menu(int current_menu);
	int getCurrent_menu() const;
};
