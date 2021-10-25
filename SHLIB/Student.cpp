#include "Student.h"
#include "grammarCheck.h"
#include "Book.h"
#include "Library.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <climits>
#include <io.h>
#include <filesystem>


#define BASKETMAX 10
#define BORROWMAX 1
#define RESERVEUSERMAX 3
#define RESERVEBOOKMAX 5

// using namespace std;

Student::Student(string id)
    : current_menu(0), isOverdue(false), isBlacklist(false), canExtend(true)
{
    this->id = id;
    /*
    �л� ���̵� �̿�, �л��� ���̵� ���ڷ� �޾Ƶ��̴� ������.
    datafile/User/�л�id.txt �� �ִ� �л��� ��� ������ student Ŭ������ ����
    */
    ifstream file;
    file.open("datafile/User/" + id + ".txt");
    if (!file.is_open()) {
        cerr << "datafile/User/" + id + ".txt file is not open" << endl;
        exit(1);
    }

    string info;
    file >> info; // info = ��й�ȣ_�̸�_�й�
    this->password = info.substr(0, info.find('_')); // ��й�ȣ
    info = info.substr(info.find('_') + 1, string::npos); // info = �̸�_�й�
    this->name = info.substr(0, info.find('_')); // �̸�
    this->s_id = info.substr(info.find('_') + 1, string::npos); // �й�

    file >> info; // ��ü����
    if (info == "true") isOverdue = true;

    file >> info; // ������Ʈ ���� 
    if (info == "true") isBlacklist = true;

    file >> info; // info = "���⵵�� ����"
    getline(file, info); // ���� ó��
    getline(file, info);

    //file >> info; // info = ������_���ڸ�_����_���ǻ�_���࿬��_������_�ݳ���

    string split;
    stringstream ss(info);
    vector<string> b_info; b_info.clear();
    while (getline(ss, split, '_')) {
        b_info.push_back(split);
    }

    // ����
    if (b_info.size() > 1) {
        borrow = new Book(b_info[0], b_info[1], this);
        this->borrowDate = b_info[3];
        this->dueDate = b_info[4];
    }

    if (borrow != nullptr) {
        // ���� ���� ���� -> + 2�� �� ����Ƚ�� ���� �־� ��.
        if (isOverdue || borrow->getReserveStudentsSize() > 0) { // ��ü or ������ ����
            canExtend = false;
        }
    }
    else canExtend = false; // ������ å ���� ��쿣 ���嵵 �Ұ�

    file >> info; // info = "���൵�� ����"
    getline(file, info); // info = "\n"
    while (getline(file, info)) { // ���൵������ vector�� push
        info = info.substr(0, info.find('\n'));
        // info = ������_���ڸ�_����_���ǻ�_���࿬��
        ss.clear(); b_info.clear();
        ss.str(info);
        while (getline(ss, split, '_')) {
            b_info.push_back(split);
        }
        reserveBookList.push_back(new Book(b_info[0], b_info[1])); // ���ѷ��� �ذ��ؾ� ��!!
    }

    while (file.is_open()) file.close();
   

}

Student::~Student()
{
    // �����Ҵ� �� ��� �κ� �޸� ����
    if (borrow == nullptr) {
        delete borrow;
        borrow = nullptr;
    }

    for (size_t i = 0; i < searchResult.size(); i++) {
        delete searchResult.at(i);
        searchResult.at(i) = nullptr;
    }
    searchResult.clear();
    for (size_t i = 0; i < reserveBookList.size(); i++) {
        delete reserveBookList.at(i);
        reserveBookList.at(i) = nullptr;
    }
    reserveBookList.clear();
    for (size_t i = 0; i < bookBasketList.size(); i++) {
        delete bookBasketList.at(i);
        bookBasketList.at(i) = nullptr;
    }
    bookBasketList.clear();
}

void Student::menu() // ����� ��� �޴�
{

    int selectNum;
    cout << "=============\n����� ��� �޴�\n================\n";
    cout << "1. �ڷ� �˻�\n2. ��ٱ���\n3. ����������\n4. �α׾ƿ�\n";
    cin >> selectNum;
    // basketListNum�� ���� �޴� ȣ��
    setCurrent_menu(selectNum);
    switch (selectNum) {
    case 1:
        searchBookMenu(); break;
    case 2:
        bookBasketMenu(); break;
    case 3:
        myPageMenu(); break;
    default:
        cout << "����� ��� �޴��� �����մϴ�." << endl;
        return;
    }
}

void Student::initBookList() { // ���� �� (�����)
   // data file �о�ͼ� booklist�� ���� 
    bookList.clear();
    for (auto& file : filesystem::directory_iterator(filesystem::current_path().string() + "\\datafile\\bookDB\\")) {
        string path = file.path().string();

        stringstream ss(path);
        vector<string> last_path;
        string split;
        while (getline(ss, split, '\\')) {
            last_path.push_back(split);
        }

        split = last_path[last_path.size() - 1]; // ���丮 "å.txt"
        last_path.clear();

        string na = split.substr(0, split.find('-'));
        string au = split.substr(split.find('-') + 1, split.find(".txt") - (split.find('-') + 1));

        bookList.push_back(new Book(na, au));
    }

    return;
}

void Student::searchBookMenu() // �ڷ�˻� - �����
{
    this->initBookList();
    searchResult.clear(); // ���� �ʱ�ȭ
    int basketListNum;
    cout << "1. ���������� �˻�\n2. ���ڸ����� �˻�\n3. ���θ޴��� ���ư���\n";
    cin >> basketListNum;

    switch (basketListNum) {
    case 1:
    {
        string bookName;
        cout << "������ (�ڷ� ������ :q �� �Է��ϼ���) >> ";
        cin >> bookName;
        if (bookName == ":q") return;

        for (auto book : bookList) {
            // ���� �ӿ� keyword �ִ��� Ȯ��
            if (book->getName().find(bookName) != string::npos) { // �˻� ����
                searchResult.push_back(book);
            }
        }
        break;
    }
    case 2:
    {
        string bookauthor;
        cout << "���ڸ� (�ڷ� ������ :q �� �Է��ϼ���) >> ";
        cin >> bookauthor;
        if (bookauthor == ":q") return;

        for (auto book : bookList) {
            // ���� �ӿ� keyword �ִ��� Ȯ��
            if (book->getAuthor().find(bookauthor) != string::npos) { // �˻� ����
                searchResult.push_back(book);
            }
        }
        break;
    }
    case 3:
        return;
    default:
        return;
    }

    if (searchResult.size() == 0) {
        cout << "�˻� ����� �����ϴ�. ���� �޴��� ���ư��ϴ�." << endl;
        return;
    }

    while (true) {
        bookListPrint(searchResult, false, true, true, true, true);

        // cout << "=======================" << endl << "1. ��ٱ��� ���\n2.���� ������ �̵��ϱ�\n3. ���� ������ �̵��ϱ�\n4. ���ư���" << endl << "=======================" << endl;
        // ���� ������, ���� ������ ������ ����Ʈ �Լ�����.. ���� ������ ��
        cout << "=======================" << endl << "1. ��ٱ��� ���\n2. ���ư���" << endl << "=======================" << endl;
        cout << "�޴��� �����ϼ���: " << endl;

        int option;
        cin >> option;

        if (option == 1) {
            cout << "��ٱ��Ͽ� ���� å ��ȣ�� �����ϼ���: ";
            int bookbasketListNum = 1;
            bool isExistBasket = false;
            cin >> bookbasketListNum;

            // ��ٱ��Ͽ� ������ ��� ����
            for (auto book : bookBasketList) {
                if (book == searchResult[(int)(bookbasketListNum - 1)]) {
                    cout << "��ٱ��Ͽ� �̹� ���� å�Դϴ�. �ٽ� �������ּ���." << endl;
                    isExistBasket = true;
                    break;
                }
            }

            if (!isExistBasket) {
                bookBasketList.push_back(searchResult[(int)(bookbasketListNum - 1)]);
                cout << "[" << searchResult[(int)(bookbasketListNum - 1)]->getName() << "]�� ��ٱ��Ͽ� ��ҽ��ϴ�." << endl;
            }
        }
        else
            return;
    }
}

void Student::bookBasketMenu()// ��ٱ��� �޴� - ������
{
    int basketListNum;
    while (1) {
        cout << "\n��ٱ���\n";
        bookListPrint(bookBasketList, false, true, true, true, true);
        // �ϴ� 1. �ϰ����� -> ���ô���� ���� (���� ���� 1���̸�)
        cout << "\n-----------------------------------------\n";
        cout << "\t1. ���� ���� ����\n\t2. ���� ���� ����\n\t3. ���� ���� ����\n\t4. ���ư���";
        cout << "\n-----------------------------------------\n";

        while (1) {
            cout << "\n�޴�����:";
            cin >> basketListNum;
            if (!cin) { // cin ����ó��
                cout << "1~4�� ������ �Է����ּ���.\n";
                cin.ignore(INT_MAX, '\n');
                cin.clear();
                rewind(stdin);
            }
            else if (basketListNum > 4 || basketListNum < 1) {
                cout << "1~4�� ������ �Է����ּ���.\n";
            }
            else break; //����� �����ߴٸ�
        }
        switch (basketListNum) {
        case 1:
            cout << "------------------------------------------------\n";
            cout << "\t���� ���� ����\n";
            cout << "------------------------------------------------\n";
            //borrowBook();   // ��� �ϰ�����! ���� ���� 10���� ��!
            sel_borrowBook(); // ���� ���� 1���̸� ���ô���!
            break;
        case 2:
            cout << "------------------------------------------------\n";
            cout << "\t���� ���� ����\n";
            cout << "------------------------------------------------\n";
            deleteBook();
            break;
        case 3:
            cout << "------------------------------------------------\n";
            cout << "\t���� ���� ����\n";
            cout << "------------------------------------------------\n";
            reserveBook();
            break;
        case 4:
            cout << "------------------------------------------------\n";
            cout << "���� �޴��� ���ư��ϴ�.\n";
            cout << "------------------------------------------------\n";
            return;
        default:
            cout << "------------------------------------------------\n";
            cout << "�޴��� �ٽ� �������ּ���.\n";
            cout << "------------------------------------------------\n";
            break;
        }
    }


}

// ��� �ϰ����� --> 2�� �� ��� or ���
void Student::borrowBook() // ��ٱ��� -> �ϰ����� (������ ���� �ٷ�� �ʿ�) - ������
{
    size_t basketListNum = bookBasketList.size(); // ����� ����: int -> size_t ����
    size_t borrowbooknum = 0; // 2�� 1 -> /*borrowBookList.size()*/
    if (borrow != nullptr) borrowbooknum = 1; // 2�� �� ���

    /*  ----------------------------------
       ���� �Ұ��� ���
       1. �̹� ����� ���
       2. ù��° ������ != ��
       3. �ܿ� ����Ƚ�� < ��ٱ��� list size
       4. ��ü�� ���
       -----------------------------------  */

    if (bookBasketList.empty()) {
        cout << "------------------------------------------------\n";
        cout << "��ٱ��Ͽ� ���� å�� �����ϴ�.\n"; // ����Ű�� ������ �޴��� ���ư��ϴ�. 
        cout << "------------------------------------------------\n";
        // string yn;
        // cin >> yn;
        return;
    }

    // 4. ��ü�� ��� - ���� �Ұ�
    if (isOverdue) {
        cout << "------------------------------------------------\n";
        cout << "��ü�Ǿ� ������ �Ұ��մϴ�.\n";
        cout << "------------------------------------------------\n";
        return;
    }
    vector<int> cant;
    bool flag3 = false;
    for (int i = 0; i < basketListNum; i++) {
        if (!bookBasketList.at(i)->getBorrowTF()) { // 1. �̹� �����.
            cant.emplace_back(i);
        }
        else if (bookBasketList.at(i)->getReserveStudentsSize() > 0) {
            if (!bookBasketList.at(i)->isFirstRSisME(this)) { // 2. ù��° ������ != ��
                cant.emplace_back(i);
            }
        }
    }


    if (BORROWMAX - borrowbooknum < basketListNum) { // 3. �ܿ� ����Ƚ�� < ��ٱ��� list size
        flag3 = true;
    }
    if (flag3) {
        cout << " -- ���� ���� Ƚ���� ��ٱ��Ϻ��� " << basketListNum - (BORROWMAX - borrowbooknum) << "�� �����ϴ�. ��ٱ��ϸ� �����ּ���. -- \n";
    }
    if (!cant.empty()) { // ���� �Ұ�
        cout << " ------\t ����Ұ� ����Ʈ\t ------\n";
        cout << "([��ٱ��Ϲ�ȣ. ����, ����])\n";
        for (int i = 0; i < cant.size(); i++) {
            cout << to_string(cant.at(i)) << ".\t���� : " << bookBasketList.at(cant.at(i))->getName()
                << " , ���� : " << bookBasketList.at(cant.at(i))->getAuthor() << "\n";
        }
        return;
    }
    else { // ���Ⱑ���ϸ� ����
        for (int i = 0; i < basketListNum; i++) {
            borrowDate = getCurrent_date();
            dueDate = getAfter_date(borrowDate, 14);
            bookBasketList.at(i)->addBorrow(this);

            // ���� �л��������� ������Ʈ - ������
            string stu_path = "datafile/User/" + id + ".txt";
            ofstream student_file(stu_path, ios::out);
            if (!student_file) {
                cout << "���� open ����" << endl;
                return;
            }

            student_file << password << "_" << name << "_" << s_id << endl;
            student_file << (isOverdue ? "true" : "false") << endl;
            student_file << (isBlacklist ? "true" : "false") << endl;

            student_file << "���⵵������" << endl;

            if (borrow != nullptr) {
                student_file << borrow->getName() << "_" << borrow->getAuthor() << "_"
                << borrow->getTranslator() << "_"
                << borrowDate << "_" << dueDate << endl;
            }
            student_file << "���൵������" << endl;
            for (size_t i = 0; i < reserveBookList.size(); i++) {
                Book* book = reserveBookList.at(i);
                student_file << book->getName() << "_" << book->getAuthor() << "_"
                << book->getTranslator() << "_" << book->getPublisher() << "_"
                << book->getPublishYear() << endl;
            }

            student_file.close();

            //borrowBookList.emplace_back(bookBasketList[i], "20211015"); // �����: �������� ��� �ּ�ó����
        }
    }
}

// ��� ���ô��� --> ���� 1�� �����̶� �߰��� ����� �ߴµ�, 2�� �� ���� �Ǽ� �þ�鼭 ���� ���� �ִ� �Լ���.
void Student::sel_borrowBook() // ��ٱ��� -> ���ô��� (������ ���� �ٷ�� �ʿ�) - ������
{
    size_t basketListNum = bookBasketList.size();
    while (true) {
        // ��ٱ��ϰ� �� ���
        if (bookBasketList.empty()) {
            cout << "------------------------------------------------\n";
            cout << "��ٱ��Ͽ� ���� å�� �����ϴ�. \n"; // ����Ű�� ������ �޴��� ���ư��ϴ�.
            cout << "------------------------------------------------\n";
            // string yn;
            // cin >> yn;
            return;
        }
        // â �ʱ�ȭ �ʿ�
        bookListPrint(bookBasketList, false, true, true, true, true);

        cout << "------------------------------------------------\n";
        cout << "������ å�� ��ȣ�� �Է��ϼ��� (0�� �Է��ϸ� �޴��� ���ư��ϴ�.): ";

        int select; // +1 �ؼ� �����ؾ� ��.
        cin >> select;
        if (select == 0) {
            cout << "\n�޴��� ���ư��ϴ�.\n";
            cout << "------------------------------------------------\n";
            return;
        }

        // 3. ����Ƚ�� ���� - ����Ұ�
        if (borrow != nullptr) { // �̹� �������̸� (���� ���� 1���̸� �̰Ű�, ���� ���� �Ǽ� ���� �þ�� ����Ʈ�� �°� �����ؾߵ�.)
            cout << "------------------------------------------------\n";
            cout << "����Ƚ���� ��� �����Ͽ����ϴ�.\n";
            cout << "------------------------------------------------\n";
            return;
        }

        // 4. ��ü�� ��� - ���� �Ұ�
        if (isOverdue) {
            cout << "------------------------------------------------\n";
            cout << "��ü�Ǿ� ������ �Ұ��մϴ�.\n";
            cout << "------------------------------------------------\n";
            return;
        }

        if (bookBasketList.at((int)(select - 1))->getBorrowTF()) { // 1. �̹� ����� å.
            cout << "------------------------------------------------\n";
            cout << "�ٸ� ����ڰ� ���� ���Դϴ�.\n";
            cout << "------------------------------------------------\n";
            continue;
        } // 2. ù��° ������ != ��
        else if (bookBasketList.at((int)(select - 1))->getReserveStudentsSize() > 0 && !bookBasketList.at((int)(select - 1))->isFirstRSisME(this)) {
            cout << "------------------------------------------------\n";
            cout << "�켱 �����ڰ� ������Դϴ�.\n";
            cout << "------------------------------------------------\n";
            continue;
        }
        else { // ���� ��.
            borrow = bookBasketList.at((int)(select - 1)); // ���� (���� ���� 1���̸� �̰Ű�, ���� ���� �Ǽ� ���� �þ�� ����Ʈ�� �°� �����ؾߵ�.)
            borrowDate = getCurrent_date();
            dueDate = getAfter_date(borrowDate, 14);
            bookBasketList.at((int)(select - 1))->addBorrow(this);
            bookBasketList.erase(bookBasketList.begin() + select - 1); // ����
            isOverdue = false;

            // ���� �л��������� ������Ʈ - ������
            string stu_path = "datafile/User/" + id + ".txt";
            ofstream student_file(stu_path, ios::out);
            if (!student_file) {
                cout << "���� open ����" << endl;
                return;
            }

            student_file << password << "_" << name << "_" << s_id << endl;
            student_file << (isOverdue ? "true" : "false") << endl;
            student_file << (isBlacklist ? "true" : "false") << endl;

            student_file << "���⵵������" << endl;
            
            if (borrow != nullptr) {
                student_file << borrow->getName() << "_" << borrow->getAuthor() << "_"
                << borrow->getTranslator() << "_"
                << borrowDate << "_" << dueDate << endl;
            }
            student_file << "���൵������" << endl;
            for (size_t i = 0; i < reserveBookList.size(); i++) {
                Book* book = reserveBookList.at(i);
                student_file << book->getName() << "_" << book->getAuthor() << "_"
                << book->getTranslator() << "_" << book->getPublisher() << "_"
                << book->getPublishYear() << endl;
            }

            student_file.close();

            cout << "�ش� ������ ������ �Ϸ�Ǿ����ϴ�.\n";
            cout << "------------------------------------------------\n";
        }
    }
}

void Student::deleteBook() // ��ٱ��� -> ���� ���� ���� - ������
{
    while (true) {
        // ��ٱ��ϰ� �� ���
        if (bookBasketList.empty()) {
            cout << "------------------------------------------------\n";
            cout << "��ٱ��Ͽ� ���� å�� �����ϴ�. ����Ű�� ������ �޴��� ���ư��ϴ�. \n";
            cout << "------------------------------------------------\n";
            // string yn;
            // cin >> yn;
            return;
        }
        // â �ʱ�ȭ �ʿ�
        bookListPrint(bookBasketList, false, true, true, true, true);

        cout << "------------------------------------------------\n";
        cout << "������ å�� ��ȣ�� �Է��ϼ��� (0�� �Է��ϸ� �޴��� ���ư��ϴ�.): ";

        int select; // +1 �ؼ� �����ؾ� ��.
        cin >> select;
        if (select == 0) {
            cout << "\n�޴��� ���ư��ϴ�.\n";
            cout << "------------------------------------------------\n";
            return;
        }
        cout << "------------------------------------------------\n";
        bookBasketList.erase(bookBasketList.begin() + select - 1); // ����
        cout << "�ش� ������ ������ �Ϸ�Ǿ����ϴ�.\n";
    }
}
void Student::reserveBook() // ��ٱ��� -> ���� ���� ���� (������ ���� �ٷ�� �ʿ�) - ������
{
    while (true) {
        // ��ٱ��ϰ� �� ���
        if (bookBasketList.empty()) {
            cout << "------------------------------------------------\n";
            cout << "��ٱ��Ͽ� ���� å�� �����ϴ�. ����Ű�� ������ �޴��� ���ư��ϴ�. \n";
            cout << "------------------------------------------------\n";
            // string yn;
            // cin >> yn;
            return;
        }

        // ������� ���� Ƚ���� ��� ������ ��� break;
        if (reserveBookList.size() >= RESERVEUSERMAX) {
            cout << "------------------------------------------------\n";
            cout << "����Ƚ���� ��� �����Ǿ����ϴ�. ����Ű�� ������ �޴��� ���ư��ϴ�. \n";
            cout << "------------------------------------------------\n";

            // string yn;
            // cin >> yn;
            return;
        }
        // â �ʱ�ȭ �ʿ�
        bookListPrint(bookBasketList, false, true, true, true, true);
        cout << "------------------------------------------------\n";
        cout << "������ å�� ��ȣ�� �Է��ϼ��� (0�� �Է��ϸ� �޴��� ���ư��ϴ�.): ";

        int select; // +1 �ؼ� �����ؾ� ��.
        cin >> select; // Ȥ�� ������ ��ȣ�� �Է��ϰԵʤ�... �ϴ� �����ϸ� ��ٱ��Ͽ��� ������
        if (select == 0) {
            cout << "\n�޴��� ���ư��ϴ�.\n";
            cout << "------------------------------------------------\n";
            return;
        }


        // ����Ұ� : ������ ���డ���ο� (5��) �ʰ� (����� ���� Ƚ�� �ʰ��� ������ �ٷ�)
        if (bookBasketList.at((int)(select - 1))->getReserveStudentsSize() >= RESERVEBOOKMAX) {
            cout << "------------------------------------------------\n";
            cout << "�ش� ������ ���� ���� �ο����� �ʰ��Ǿ����ϴ�.\n";
        }
        else if (bookBasketList.at((int)(select - 1))->getBorrower() == this) { // �̹� ����ڰ� �������� å�� ���
            cout << "------------------------------------------------\n";
            cout << "�ش� ������ �̹� �������Դϴ�.\n";
        }
        else {
            cout << "------------------------------------------------\n";

            reserveBookList.emplace_back(bookBasketList.at((int)(select - 1))); //����
            bookBasketList.erase(bookBasketList.begin() + select - 1); // ��ٱ��Ͽ��� ����� ���� ���� (Ȥ�� ����ڰ� �������� �����ұ��)
            bookBasketList.at((int)(select - 1))->addReserve(this);


            cout << "�ش� ������ ������ �Ϸ�Ǿ����ϴ�.\n";
        }
    }
}

void Student::myPageMenu()// ���������� �޴� //������
{
    int num; //�޴� ����
    int u1; //1. ������Ȳ������ ����� ���� (�ݳ�/����)
    //int booknum; //���� ��ȣ ����
    string answer;

    while (1) {
        cout << "------------------------------------------------\n";
        cout << "1. ���� ��Ȳ\n2. ���� ��Ȳ\n3. ���ư���\n";
        cout << "------------------------------------------------\n";
        cout << "�޴�����: ";
        cin >> num;
        switch (num) {
        case 1:

            while (1) {
                // //�� ����Ǽ��� ���⵵�� ��� ���
                // cout << "------------------------------------------------\n";
                // cout << "�� ���� �Ǽ� : ";
                // //cout << borrowBookList->size() << endl;
                // cout << ((borrow == nullptr) ? 0 : 1) << endl;
                // cout << "\n";

                // ������ ���� - 2�� ���� printborrow���ְ�, bookListPrint�� ���ܳ����� �ſ�.
                // vector<Book*> printborrow;
                // printborrow.emplace_back(borrow);
                // bookListPrint(printborrow, true, true, true, true, true);

                cout << "------------------------------------------------\n";
                cout << "<���� ���� ����>\n";

                if (borrow != nullptr) {
                    cout << "������ : " + borrow->getName() << endl;
                    cout << "���� : " + borrow->getAuthor() << endl;
                    cout << "���ǻ� : " + borrow->getPublisher() << endl;
                    cout << "���� ���� : " + borrow->getPublishYear() << endl;
                    cout << "------------------------------------------------\n";
                    cout << endl;
                }
                else {
                    cout << "���� ���� ���� ������ �����ϴ�. \n";
                    cout << "------------------------------------------------\n";
                    cout << endl;
                    break;
                }


                if (borrow != nullptr) {
                    //�ݳ��� ���� + ���ư��� �޴� �߰�
                    cout << "------------------------------------------------\n";
                    cout << "1. �ݳ��ϱ�\n2. �����ϱ�\n3. ���ư���\n";
                    cout << "------------------------------------------------\n";

                    cout << "�޴�����: ";
                    cin >> u1;

                    if (u1 == 1) {
                        cout << "------------------------------------------------\n";
                        cout << "���� �ݳ��Ͻðڽ��ϱ�? (YES: Y/y NO: Any Key)" << endl; // �ѱ��̸� �ʿ����
                        cout << ">> ";
                        cin >> answer;

                        if (answer == "Y" || answer == "y") {
                            returnBook();
                            cout << "������ �ݳ��Ǿ����ϴ�." << endl;
                        }
                        else {
                            cout << "------------------------------------------------\n";
                            cout << "���� �ݳ��� ��ҵǾ����ϴ�.\n";
                            cout << "------------------------------------------------\n";
                        }
                    }
                    else if (u1 == 2) {
                        cout << "------------------------------------------------\n";
                        cout << "���� ������ �����մϴ�.";
                        extendBook();
                    }
                    else if (u1 == 3) {
                        cout << "------------------------------------------------\n";
                        cout << "�޴��� ���ư��ϴ�.\n";
                        cout << "------------------------------------------------\n";
                        break;
                    }
                    else {
                        cout << "------------------------------------------------\n";
                        cout << "�޴��� �ٽ� �������ּ���.\n";
                        cout << "------------------------------------------------\n";
                    }
                    system("cls");
                }
            }
            break;
        case 2:

            //���� ���
            while (1) {
                //�� ����Ǽ��� ���൵�� ��� ���
                cout << "------------------------------------------------\n";
                cout << "�� ���� �Ǽ� : ";
                cout << reserveBookList.size() << endl;
                cout << "\n";

                if (reserveBookList.size() == 0) { // ���൵�� ���� ���
                    cout << "------------------------------------------------\n";
                    cout << "0�� ���� �� ���� �޴��� ���ư��ϴ�: ";
                    int qu;
                    cin >> qu;
                    if (qu == 0)
                        break;
                }
                bookListPrint(reserveBookList, false, true, true, true, true);


                //���ư��� �ɼ� �߰� - 0�� ���� ��
                cout << "------------------------------------------------\n";
                cout << "���� ��ȣ�� �������ּ���(0�� ���� �� ���� �޴��� ���ư��ϴ�): ";
                int booknum;
                cin >> booknum;
                if (booknum == 0)
                    break;
                else if (booknum > 0 && booknum <= reserveBookList.size())
                    cancelReserveBook(booknum);
                else {
                    cout << "------------------------------------------------\n";
                    cout << "��ȣ�� �ٽ� �������ּ���.\n";
                    cout << "------------------------------------------------\n";
                }
            }
            break;

        case 3:
            cout << "------------------------------------------------\n";
            cout << "���� �޴��� ���ư��ϴ�.\n";
            cout << "------------------------------------------------\n";
            return;
        default:
            cout << "------------------------------------------------\n";
            cout << "�޴��� �ٽ� �������ּ���.\n";
            cout << "------------------------------------------------\n";
            break;
        }
        system("cls");
    }
}


void Student::returnBook() // ���������� -> å �ݳ� - ���������� ó�� �ʿ�//������
{
    //vector<BorrowInfo> borrowBookList���� �ش� ���� ����
    //vector<BorrowInfo> BI; 
    //BI = borrowBookList;
    //BI.erase(BI.begin()+booknum-1);
    
    /* �����: ���� ó�� �ʿ�!! - ���� ���� ����, å ���Ͽ��� ���� ���� �ʿ� ************************/
    if (borrow != nullptr) {
        borrow->deleteBorrow(); // å���� ������ ����

        ofstream file("datafile/User/" + id + ".txt", ios::out);
        if (!file.is_open()) {
            cerr << "datafile/User/" + id + ".txt file is not Open for delete borrow Book" << endl;
            exit(1);
        }

        file << password << "_" << name << "_" << s_id << endl;
        //��ü���� Ȯ�� �� �����ؼ� �ۼ�
        if (getDiff_date(dueDate, getCurrent_date()) > 0) // ��ü�� ��� - true
            file << "true" << endl << "false" << endl << endl;
        else //��ü���� ���� ��� - false
            file << "false" << endl << "false" << endl << endl;
        file << "���⵵������" << endl;
        file << "���൵������" << endl;
        for (size_t i = 0; i < reserveBookList.size(); i++) {
            Book* x = reserveBookList.at(i);
            file << x->getName() << "_" << x->getAuthor() << "_";
            file << x->getTranslator() << "_" << x->getPublisher() << "_";
            file << x->getPublishYear() << endl;
        }
        file.close();

        borrow = nullptr;

        cout << "------------------------------------------------\n";
        cout << "�ش� ������ �ݳ��� �Ϸ�Ǿ����ϴ�.\n";
        cout << "------------------------------------------------\n";
    }

    else {
        cout << "------------------------------------------------\n";
        cout << "�ݳ��� ������ �������� �ʽ��ϴ�." << endl;
        cout << "------------------------------------------------\n";
    }

}

void Student::extendBook() // ���������� -> å ���� //������
{
    //vector<BorrowInfo> BI;
    //BI = borrowBookList;
    bool reserveNumFlag = false; //������ ���翩�� ����

    //int reserveNum = BI.at(booknum - 1).book->getReserveStudentsSize();

    if (borrow != nullptr) {
        if (borrow->getReserveStudentsSize() == 0) // �����ڰ� ���� ����
            reserveNumFlag = false;
        else // �����ڰ� �����ϴ� ���
            reserveNumFlag = true;
        if (!isOverdue && !reserveNumFlag) { // ���忡 ������ ���°�� -> ��ü ����, ������ ����
            dueDate = getAfter_date(dueDate, 14);

            ofstream file("datafile/User/" + id + ".txt", ios::out);
            if (!file.is_open()) {
                cerr << "datafile/User/" + id + ".txt file is not Open for expend duedate" << endl;
                exit(1);
            }

            file << password << "_" << name << "_" << s_id << endl;
            file << "false" << endl << "false" << endl << endl;
            file << "���⵵������" << endl;
            if (borrow != nullptr) {
                file << borrow->getName() << "_" << borrow->getAuthor() << "_";
                file << borrow->getTranslator() << "_";
                file << borrowDate << "_" << dueDate << endl;
            }
            file << "���൵������" << endl;
            for (size_t i = 0; i < reserveBookList.size(); i++) {
                Book* x = reserveBookList.at(i);
                file << x->getName() << "_" << x->getAuthor() << "_";
                file << x->getTranslator() << "_" << x->getPublisher() << "_";
                file << x->getPublishYear() << endl;
            }
            file.close();

            cout << "------------------------------------------------\n";
            cout << "�ش� ���� ������ �Ϸ�Ǿ����ϴ�.\n";
            cout << "------------------------------------------------\n";
        }
        else if (isOverdue) {
            //��ü�� ���
            cout << "------------------------------------------------\n";
            cout << "�ش� ������ ��ü�� ������, \n������ �Ұ����մϴ�.\n";
            cout << "------------------------------------------------\n";
        }
        else if (reserveNumFlag) {
            //�����ڰ� �����ϴ� ���
            cout << "------------------------------------------------\n";
            cout << "�ش� ������ �ٸ� ����ڰ� �̹� ������ ������, \n������ �Ұ����մϴ�.\n";
            cout << "------------------------------------------------\n";
        }
    }
    else {
        cout << "------------------------------------------------\n";
        cout << "������ �� �ִ� å�� �������� �ʽ��ϴ�." << endl;
        cout << "------------------------------------------------\n";
    }
    system("cls");
}

void Student::cancelReserveBook(int booknum) // ���������� -> å ���� ��� //������
{
    //vector<Book> reserveBookList���� �ش� ���� ����
    if (reserveBookList.size() != 0) {
        reserveBookList.at(booknum - 1)->deleteReserve(this); // å���� ������ ����

        reserveBookList.erase(reserveBookList.begin() + booknum - 1); // ����� ����

        ofstream file("datafile/User/" + id + ".txt");
        if (!file.is_open()) {
            cerr << "datafile/User/" + id + ".txt is not open for cancelReserveBook." << endl;
            exit(1);
        }
        file << password << "_" << name << "_" << s_id << endl;
        file << "false" << endl << "false" << endl << endl;
        file << "���⵵������" << endl;
        if (borrow != nullptr) {
            file << borrow->getName() << "_" << borrow->getAuthor() << "_" << borrow->getTranslator();
            file << borrow->getPublisher() << "_" << endl;
            file << borrowDate << "_" << dueDate << endl;
        }
        file << "���൵������" << endl;
        for (size_t i = 0; i < reserveBookList.size(); i++) {
            Book* x = reserveBookList.at(i);
            file << x->getName() << "_" << x->getAuthor() << "_";
            file << x->getTranslator() << "_" << x->getPublisher() << "_";
            file << x->getPublishYear() << endl;
        }
        file.close();

        cout << "------------------------------------------------\n";
        cout << "�ش� ���� ������ ��ҵǾ����ϴ�.\n";
        cout << "------------------------------------------------\n";
    }
    else {
        cout << "------------------------------------------------\n";
        cout << "������ ����� �� �ִ� ������ �������� �ʽ��ϴ�." << endl;
        cout << "------------------------------------------------\n";
    }

}

// Book vector, ��׵��� ��� ���� -> (�������� å����, ������, ���ڸ�, ���Ⱑ�ɿ���, �����ο���) - ������
// Book ��ü�� ���� ����ϴ� �κ��� �޶����� bool�� �޾ҽ��ϴ�. 
// ���Ŀ� bool�Ⱦ��� � ����Ʈ�Ŀ� ���� �Լ� ��ü���� ó���ϵ��� ������
// ...�� �������� �׷��� book �� ����� �ϳ� �鿩���� ��.
void Student::bookListPrint(vector<Book*> book, bool borrowListTF, bool nameTF, bool authorTF, bool borrowTF, bool reserveNumTF) const
{
    int listSize = book.size();

    for (int i = -1; i < listSize; i++) { // index: -1�� ��ܹ� ���, 0���� å ���
        if (i == -1) {
            cout << "\n" << (nameTF ? "[������]" : "") << "\t" << (authorTF ? "[���ڸ�]" : "") << "\t" << "[����]\t[���ǻ�]";
            if (borrowListTF) { //�����̸�
                cout << "\t" << "[��ü����]" << "\t" << "[�ݳ���¥]" << "\t" << "[���尡�ɿ���]";
            }
            cout << "\t" << (borrowTF ? "[���Ⱑ�ɿ���]" : "") << "\t" << (reserveNumTF ? "[�����ο���]" : "");
            cout << "\n-------------------------------------------\n";
        }
        else {
            if (borrowListTF) { //������Ȳ (1�� ���� 1���̶� �ݺ��� ���� ������,,)
                cout << "\n" << i + 1 << ".\t" << (nameTF ? book[i]->getName() : "") << "\t" << (authorTF ? book[i]->getAuthor() : "") << "\t" << book[i]->getTranslator() << "\t" << book[i]->getPublisher();

                cout << "\t" << (isOverdue ? "O\t" : "X\t") << dueDate << "\t" << canExtend;

                if (borrowTF) { //���Ⱑ�ɿ���
                    cout << "\t";

                    if (book.at(i)->getBorrowTF()) {
                        cout << "X";
                    }
                    else cout << "O";
                }
                if (reserveNumTF) {//�����ο���
                    cout << "\t" << book[i]->getReserveStudentsSize();
                }
            }
            else {
                cout << "\n" << (nameTF ? book[i]->getName() : "") << "\t" << (authorTF ? book[i]->getAuthor() : "") << "\t" << book[i]->getTranslator() << "\t" << book[i]->getPublisher();
                if (borrowTF) { //���Ⱑ�ɿ���
                    cout << "\t";
                    if (book[i]->getBorrowTF()) {
                        cout << "X";
                    }
                    else cout << "O";
                }
                cout << "\t" << (reserveNumTF ? to_string(book[i]->getReserveStudentsSize()) : "");
            }
        }
    }
}

void Student::setCurrent_menu(int menu)
{
    this->current_menu = menu;
}

void Student::setName(string name)
{
    this->name = name;
}

void Student::setS_id(string s_id)
{
    this->s_id = s_id;
}

void Student::setIsOverdue(bool check)
{
    this->isOverdue = check;
}

void Student::setIsBlacklist(bool check)
{
    this->isBlacklist = check;
}

int Student::getCurrent_menu() const
{
    return current_menu;
}

string Student::getName() const
{
    return name;
}

string Student::getS_id() const {
    return s_id;
}

bool Student::getIsOverdue() const
{
    return isOverdue;
}

bool Student::getIsBlacklist() const
{
    return isBlacklist;
}

string Student::getBorrowDate() const
{
    return borrowDate;
}

string Student::getDueDate() const
{
    return dueDate;
}

string Student::getBookName() const
{
    return borrow->getName();
}

vector<Book*> Student::getReserveBookList()
{
    return reserveBookList;
}


bool Student::operator==(Student student) // ������
{
    if (this->s_id == student.s_id) { // �й� ������ ���� �ι�
        return true;
    }
    else return false;
}