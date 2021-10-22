#pragma once
#include "User.h"
#include "Student.h"
#include <vector>

class Admin :
	public User
{
private:
	int current_menu; // ���� ������ �޴�
	vector<Student> borrowList; // ������ ����Ʈ
	vector<Student> overdueList;  // ��ü�� ����Ʈ
	vector<Student> blackList; // ������Ʈ

public:
	//Constructor
	Admin();
	//Destructor
	~Admin();

	void menu(); // ������ �޴�
	void addBookMenu(); // �����߰�
	void deleteBookMenu(); // ���� ����
	void monitoring(); // ȸ�� ����͸�

	void setCurrent_menu(int menu);
	int getCurrent_menu() const;
	bool compare(Student &a,Student &b);//������ü�Ϸ� ����?
};
