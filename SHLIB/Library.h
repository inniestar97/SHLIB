#pragma once
#include <string>

class Library
{
private:
	int current_menu; // ���� �޴�
	std::string current_date; // ���� ��¥
public:
	//vector<Book*> bookList; // ó�� �α����� �� �ϸ���Ʈ �ʱ�ȭ. ��� ���� ���� �ð��� ���ϰڽ��ϴ�. (�����)
	Library();
	~Library() = default;
	void startMenu();

	void login(); // �α���
	void makeAccount(); // ȸ������
	void initBookList(); // �ϸ���Ʈ ��
	// void changeDate();

	void setCurrent_menu(int current_menu);
	int getCurrent_menu() const;

	std::string getCurrent_date() const; // ���� ��¥
	int getDiff_date(std::string comp) const;	// ���糯¥ - ���س�¥  /*comp ���� : "20211023"*/
	std::string getAfter_date(int day) const;	// ���糯¥�κ��� day�� �� ��¥

};
