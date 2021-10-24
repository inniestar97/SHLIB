#pragma once

using namespace std;

class Student; // (Student - Library) ���� ���

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