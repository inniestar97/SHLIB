#pragma once
using namespace std;
class Library
{
private:
	int current_menu; // ���� �޴�

public:
	Library();
	~Library() = default;
	void startMenu();

	void login(); // �α���
	void makeAccount(); // ȸ������
	void initBookList(); // �ϸ���Ʈ ��
	// void changeDate();

	void setCurrent_menu(int current_menu);
	int getCurrent_menu() const;

	
};
string current_date; // ���� ��¥
string getCurrent_date(); // ���� ��¥
int getDiff_date(string comp, string date);	// ���糯¥(date) - ���س�¥(comp)  /*comp ���� : "20211023"*/
string getAfter_date(string date, int day);	// ���糯¥�κ��� day�� �� ��¥