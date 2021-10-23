#pragma once

class Library
{
private:
	int current_menu; // ���� �޴�
	string current_date; // ���� ��¥
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

	string getCurrent_date() const; // ���� ��¥
	int getDiff_date(string comp) const;	// ���糯¥ - ���س�¥  /*comp ���� : "20211023"*/
	string getAfter_date(int day) const;	// ���糯¥�κ��� day�� �� ��¥

};
