#pragma once
#include <regex>

using std::string;
using std::regex;
 
bool check_id(string id); 
bool check_password(string pw); 
bool check_Name(string name); //ȸ����
bool check_book(string name); 
bool check_author(string name);//���ڸ�
bool check_translator(string name);//���ڸ�
bool check_studentID(string s_id);
bool check_publisher(string pub);
bool check_year(string year);
int input(string msg, int a, int b); // �Է� message, [a, b] ������ ���� �Է�


string getCurrent_date();
void setCurrent_date(string date);
int getDiff_date(string comp, string date);	// ���糯¥(date) - ���س�¥(comp)  /*comp ���� : "20211020"*/
string getAfter_date(string date, int day);	// ���糯¥�κ��� day�� �� ��¥
