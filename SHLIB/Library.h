#pragma once
class Library
{
private:
	int current_menu; // ���� �޴�
public:
	//vector<Book*> bookList; // ó�� �α����� �� �ϸ���Ʈ �ʱ�ȭ. ��� ���� ���� �ð��� ���ϰڽ��ϴ�. (�����)
	Library();
	void startMenu();

	void login(); // �α���
	void makeAccount(); // ȸ������
	void initBookList(); // �ϸ���Ʈ �ʱ�ȭ (cpp�� ���� �ȸ�������ϴ�. ������ ���� ���� �� ����°� ���� �� �����ϴ�. - �����) (���� Student���� �����Ͽ����ϴ�)
	// void changeDate();

	void setCurrent_menu(int current_menu);
	int getCurrent_menu() const;
};
