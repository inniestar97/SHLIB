#include "Student.h"
#include "grammarCheck.h"
#include "Book.h"
#include "Library.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <climits>
#include <io.h>
#include <filesystem>
#include <set>

#define BASKETMAX 10
#define BORROWMAX 3
#define RESERVEUSERMAX 3
#define RESERVEBOOKMAX 5

// using namespace std;
using std::cout;

Student::Student(string id)
    : current_menu(0)
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

    /* 
    "datafile/User/" + id + ".txt" ���� ���

    [��й�ȣ]_[�̸�]_[�й�]
    (���ѻ����� ���) ��¥ / (���ѻ��°� �ƴҰ��) false
    (���ѻ��� ���� Ƚ��)

    ���⵵������
    1.[������]_[���ڸ�]_[����]_[���ǻ�]_[������]_[�ݳ���]
    2.[������]_[���ڸ�]_[����]_[���ǻ�]_[������]_[�ݳ���]
    3.
    ���൵������
    1.[������]_[���ڸ�]_[����]_[���ǻ�]
    2.[������]_[���ڸ�]_[����]_[���ǻ�]
    3.[������]_[���ڸ�]_[����]_[���ǻ�]    
    */

    string info;
    file >> info; // info = ��й�ȣ_�̸�_�й�
    this->password = info.substr(0, info.find('_')); // ��й�ȣ
    info = info.substr(info.find('_') + 1, string::npos); // info = �̸�_�й�
    this->name = info.substr(0, info.find('_')); // �̸�
    this->s_id = info.substr(info.find('_') + 1, string::npos); // �й�

    file >> info; // ���ѻ��� ����
    //if (info != "false") limitDate = info; // ���ѻ����� ���
    //else limitDate = "false"; // ���ѻ��°� �ƴҰ��
    limitDate = info;

    file >> info; // ���ѻ��� ����
    limitedStack = stoi(info);

    file >> info; // info = "���⵵�� ����"
    getline(file, info);
    for (size_t i = 0; i < 3; i++) {
        getline(file, info); // info : x.[������]_[���ڸ�]_[����]_~~�䷱��

        if (info.size() > 3) { // ���⵵�� ����
            info = info.substr(2, string::npos);

            //file >> info; // info = ������_���ڸ�_����_���ǻ�_���࿬��_������_�ݳ���
            string split;
            stringstream ss(info);
            vector<string> b_info; b_info.clear();
            while (getline(ss, split, '_')) {
                b_info.push_back(split);
            }

            /*
                b_info[0] = ������
                b_info[1] = ���ڸ�
                b_info[2] = ����
                b_info[3] = ���ǻ� 
                b_info[4] = ������
                b_info[5] = �ݳ���
            */
            BorrowInfo br;
            br.book = new Book(b_info[0], b_info[1]);
            br.borrowDate = b_info[4];
            br.dueDate = b_info[5];

            borrowBookList.push_back(br);
        }
    }

    file >> info; // info = "���൵�� ����"
    getline(file, info);
    for (size_t i = 0; i < 3; i++) {
        getline(file, info);

        if (info.size() > 3) { // ���൵�� ����
            info = info.substr(2, string::npos);

            string split;
            stringstream ss(info);
            vector<string> b_info; b_info.clear();
            while (getline(ss, split, '_')) {
                b_info.push_back(split);
            }

            /*
                b_info[0] = ������
                b_info[1] = ���ڸ�
                b_info[2] = ����
                b_info[3] = ���ǻ�
                b_info[4] = ���࿬��
            */

            reserveBookList.push_back(new Book(b_info[0], b_info[1]));
        }
    }

    while (file.is_open()) file.close(); // ���� �ݾ��ָ� ������ ��! 
   
     /* ���ѻ��� ���� ���� �Ǻ� */
    // Idea: å�� 1�� �̻� ������ ��: ���� å�鸶�� �ݳ� ��¥�� Ȯ���ؼ� �ϳ��� ��ü�� ��� ���ѻ���(true)�� �����Ѵ�.
    if (borrowBookList.size() > 0) {
        overDueBookNum = 0;
        bool isOverdue = false;
        for (auto bookinfo : borrowBookList) {
            if (getDiff_date(bookinfo.dueDate, getCurrent_date()) > 0) { // �� < �� �� ���
                isOverdue = true;
                overDueBookNum++;
                limitDate = "true";
            }
        }
        if (isOverdue) {
            // ���ѻ��� ���� -> �ݳ��� �� ��� (ReturnBook �Լ�)

            // ��� ���� ���
            for (auto book : reserveBookList) {
                book->deleteReserve(this);
            }
            reserveBookList.clear();

            //-----------------------------------------------------------------
            //---------------- UserId.txt ���� ���� code ----------------------
            //-----------------------------------------------------------------
            ofstream fileLimited("datafile/User/" + id + ".txt", ios::trunc);
            if (!fileLimited.is_open()) {
                cerr << "������ ���ѻ��� ���� UserId.txt ���� open ����" << endl;
                exit(1);
            }

            fileLimited << password + "_" + name + "_" + s_id << endl;
            fileLimited << limitDate << endl;
            fileLimited << limitedStack << endl << endl;

            fileLimited << "���⵵������" << endl;
            for (size_t i = 1; i <= 3; i++) {
                fileLimited << i << ".";
                if (borrowBookList.size() >= i) {
                    BorrowInfo bi = borrowBookList.at(i - 1);
                    fileLimited << bi.book->getName() << "_";
                    fileLimited << bi.book->getAuthor() << "_";
                    fileLimited << bi.book->getTranslator() << "_";
                    fileLimited << bi.book->getPublisher() << "_";
                    fileLimited << bi.borrowDate << "_";
                    fileLimited << bi.dueDate;
                }
                fileLimited << endl;
            }
            
            fileLimited << "���൵������" << endl;
            for (size_t i = 1; i <= 3; i++) {
                fileLimited << i << "." << endl;
            }

            while (!fileLimited.is_open()) fileLimited.close();
            //-----------------------------------------------------------------
            //-----------------------------------------------------------------
            //-----------------------------------------------------------------
        } 
    }

    /* ���� ���� ���� : ���� ��¥�� ���� */
    if (limitDate != "false" && limitDate != "true") {
        if(getDiff_date(limitDate, getCurrent_date()) > 0) { // �� < �� �� ���
            limitDate = "false";
            //-----------------------------------------------------------------
            //---------------- UserId.txt ���� ���� code ----------------------
            //-----------------------------------------------------------------
            ofstream fileLimited("datafile/User/" + id + ".txt", ios::trunc);
            if (!fileLimited.is_open()) {
                cerr << "������ ���ѻ��� ���� UserId.txt ���� open ����" << endl;
                exit(1);
            }

            fileLimited << password + "_" + name + "_" + s_id << endl;
            fileLimited << limitDate << endl;
            fileLimited << limitedStack << endl << endl;

            fileLimited << "���⵵������" << endl;
            for (size_t i = 1; i <= 3; i++) {
                fileLimited << i << ".";
                if (borrowBookList.size() >= i) {
                    BorrowInfo bi = borrowBookList.at(i - 1);
                    fileLimited << bi.book->getName() << "_";
                    fileLimited << bi.book->getAuthor() << "_";
                    fileLimited << bi.book->getTranslator() << "_";
                    fileLimited << bi.book->getPublisher() << "_";
                    fileLimited << bi.borrowDate << "_";
                    fileLimited << bi.dueDate;
                }
                fileLimited << endl;
            }

            fileLimited << "���൵������" << endl;
            for (size_t i = 1; i <= 3; i++) {
                fileLimited << i << "." << endl;
            }

            while (!fileLimited.is_open()) fileLimited.close();
        }
    }
}

Student::~Student()
{
    /*
        ���� �����Ҵ� �� �κ� (�л� ����������) 
        borrowBookList ���ο��� Book ��ü
        reserveBookList���� Book ��ü
    */
   /*---------------���� ����, ���຤�� ����--------------*/
   for (size_t i = 0; i < borrowBookList.size(); i++) {
       delete borrowBookList.at(i).book;
       borrowBookList.at(i).book = nullptr;
   }
   for (size_t i = 0; i < reserveBookList.size(); i++) {
       delete reserveBookList.at(i);
       reserveBookList.at(i) = nullptr;
   }
   /*----------------------------------------------------*/
   

}

void Student::menu() // ����� ��� �޴�
{
    int selectNum;
    cout << "=============\n����� ��� �޴�\n=============\n";
    cout << "1. �ڷ� �˻�\n2. ��ٱ���\n3. ����������\n4. �α׾ƿ�\n";
    cout << "============================================\n";

    selectNum = input("\n�޴� ����:", 1, 4);

    // selectNum�� ���� �޴� ȣ��
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
    if (limitDate == "true") {
        cout << "���ѻ����̹Ƿ� �ڷ� �˻� �޴��� ����Ͻ� �� �����ϴ�. ��ü�Ͻ� å�� ��� �ݳ����ּ���." << endl;
        return;
    }
    if (limitDate != "false") {
        cout << "���ѻ����̹Ƿ� �ڷ� �˻� �޴��� ����Ͻ� �� �����ϴ�. ���ѻ��� ������: [" << limitDate << "]" << endl;
        return;
    }
    this->initBookList();
    searchResult.clear(); // ���� �ʱ�ȭ
    int basketListNum;
    cout << "1. ���������� �˻�\n2. ���ڸ����� �˻�\n3. ���θ޴��� ���ư���\n";
    basketListNum = input("\n�޴�����: ", 1, 3);
//    while (getchar() != '\n');

    switch (basketListNum) {
    case 1:
    {
        string bookName;
        cout << "������ (�ڷ� ������ :q �� �Է��ϼ���) >> ";
        getline(cin, bookName);

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
        getline(cin, bookauthor);
        bookauthor = bookauthor.substr(0, bookauthor.find('\n'));
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
        bookListPrint(searchResult, true, true, true, true);

        // cout << "=======================" << endl << "1. ��ٱ��� ���\n2.���� ������ �̵��ϱ�\n3. ���� ������ �̵��ϱ�\n4. ���ư���" << endl << "=======================" << endl;
        cout << "=======================" << endl << "1. ��ٱ��� ���\n2. ���ư���" << endl << "=======================" << endl;

        int option;
        option = input( "\n�޴��� �����ϼ���: ", 1, 2);

        if (option == 1) {
            int bookbasketListNum;
            bool isExistBasket = false;
            //bookbasketListNum = input("\n��ٱ��Ͽ� ���� å ��ȣ�� �����ϼ���: ", 1, bookBasketList.size() + 1);
            bookbasketListNum = input("\n��ٱ��Ͽ� ���� å ��ȣ�� �����ϼ���: ", 1, searchResult.size());


            // ��ٱ��Ͽ� ������ ��� ����
            for (auto book : bookBasketList) {
                if (*book == *searchResult[(int)(bookbasketListNum - 1)]) { 
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
    if (limitDate == "true") {
        cout << "���ѻ����̹Ƿ� ��ٱ��� �޴��� ����Ͻ� �� �����ϴ�. ��ü�Ͻ� å�� ��� �ݳ����ּ���." << endl;
        return;
    }
    if (limitDate != "false") {
        cout << "���ѻ����̹Ƿ� ��ٱ��� �޴��� ����Ͻ� �� �����ϴ�. ���ѻ��� ������: [" << limitDate << "]" << endl;
        return;
    }
    int basketListNum;

    while (1) {
        cout << "\n��ٱ���\n";
        bookListPrint(bookBasketList, true, true, true, true);
        // �ϴ� 1. �ϰ����� -> ���ô���� ���� (���� ���� 1���̸�)
        cout << "\n-----------------------------------------\n";
        cout << "\t1. ���� ���� ����\n\t2. ���� ���� ����\n\t3. ���� ���� ����\n\t4. ���ư���";
        cout << "\n-----------------------------------------\n";

        basketListNum = input("\n�޴�����: ", 1, 4);

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


void Student::sel_borrowBook() // ��ٱ��� -> ���ô��� 
{
    size_t basketListNum = bookBasketList.size();
    while (true) {
        // ��ٱ��ϰ� �� ���
        if (bookBasketList.empty()) {
            cout << "------------------------------------------------\n";
            cout << "��ٱ��Ͽ� ���� å�� �����ϴ�. \n"; 
            cout << "------------------------------------------------\n";
            // �޴��� ���ư��ϴ�.
            return;
        }
        // â �ʱ�ȭ �ʿ�
        bookListPrint(bookBasketList, true, true, true, true);

        cout << "------------------------------------------------";

        int select; // +1 �ؼ� �����ؾ� ��.
        
        select = input("\n������ å�� ��ȣ�� �Է��ϼ��� (0�� �Է��ϸ� �޴��� ���ư��ϴ�.): ", 0, bookBasketList.size());

        if (select == 0) {
            cout << "\n�޴��� ���ư��ϴ�.\n";
            cout << "------------------------------------------------\n";
            return;
        }

        // 3. ����Ƚ�� ���� - ����Ұ�
        if (borrowBookList.size() >= 3) { // 1�� -> 3�� 
            cout << "------------------------------------------------\n";
            cout << "����Ƚ���� ��� �����Ͽ����ϴ�.\n";
            cout << "------------------------------------------------\n";
            return;
        }

        // 4. ��ü�� ��� - ���� �Ұ�
        // -> 4. ���ѻ����� ��� - ����Ұ��� ����
        if (limitDate != "false") {
            cout << "------------------------------------------------\n";
            cout << "���ѻ��¶� ������ �Ұ��մϴ�.\n";
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
        else { // ���� �Ϸ�.
                
            BorrowInfo bi_temp;
            bi_temp.book = bookBasketList.at((int)(select - 1));
            bi_temp.borrowDate = getCurrent_date();
            bi_temp.dueDate = getAfter_date(getCurrent_date(), 14);
            borrowBookList.push_back(bi_temp);

            // basket�� �ִ� å ����
            bookBasketList.at((int)(select - 1))->addBorrow(this);
            bookBasketList.erase(bookBasketList.begin() + select - 1); // ����

            //-----------------------------------------------------------------
            //----------------UserId.txt ���� ���� code-------------------------
            //-----------------------------------------------------------------
            string stu_path = "datafile/User/" + id + ".txt";
            ofstream student_file(stu_path, ios::trunc);
            if (!student_file) {
                cout << "����Ϸ�� ���� ���� open ����" << endl;
                return;
            }

            student_file << password << "_" << name << "_" << s_id << endl;
            student_file << limitDate << endl;
            student_file << limitedStack << endl << endl;

            student_file << "���⵵������" << endl;
            for (size_t i = 1; i <= 3; i++) {
                student_file << i << ".";
                if (borrowBookList.size() >= i) {
                    BorrowInfo bi = borrowBookList.at(i - 1);
                    student_file << bi.book->getName() << "_";
                    student_file << bi.book->getAuthor() << "_";
                    student_file << bi.book->getTranslator() << "_";
                    student_file << bi.book->getPublisher() << "_";
                    student_file << bi.borrowDate << "_";
                    student_file << bi.dueDate;
                }
                student_file << endl;
            }
            student_file << "���൵������" << endl;
            for (size_t i = 1; i <= 3; i++) {
                student_file << i << ".";
                if (reserveBookList.size() >= i) {
                    Book* b = reserveBookList.at(i - 1);
                    student_file << b->getName() << "_";
                    student_file << b->getAuthor() << "_";
                    student_file << b->getTranslator() << "_";
                    student_file << b->getPublisher() << "_";
                }
                student_file << endl;
            }

            while (!student_file.is_open()) student_file.close();
            //-----------------------------------------------------------------
            //-----------------------------------------------------------------
            //-----------------------------------------------------------------

            cout << "�ش� ������ ������ �Ϸ�Ǿ����ϴ�.\n";
            cout << "------------------------------------------------\n";
        }
    }
}

void Student::deleteBook() // ��ٱ��� -> ���� ���� ����
{
    while (true) {
        // ��ٱ��ϰ� �� ���
        if (bookBasketList.empty()) {
            cout << "------------------------------------------------\n";
            cout << "��ٱ��Ͽ� ���� å�� �����ϴ�. �޴��� ���ư��ϴ�. \n"; //����Ű�� ������ 
            cout << "------------------------------------------------\n";
            // string yn;
            // cin >> yn;
            return;
        }
        // â �ʱ�ȭ �ʿ�
        bookListPrint(bookBasketList, true, true, true, true);

        cout << "------------------------------------------------";

        int select; // +1 �ؼ� �����ؾ� ��.
        select = input("\n������ å�� ��ȣ�� �Է��ϼ��� (0�� �Է��ϸ� �޴��� ���ư��ϴ�.): ", 0, bookBasketList.size());
        
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

void Student::reserveBook() // ��ٱ��� -> ���� ���� ���� (������ ���� �ٷ�� �ʿ�)
{
    while (true) {
        // ��ٱ��ϰ� �� ���
        if (bookBasketList.empty()) {
            cout << "------------------------------------------------\n";
            cout << "��ٱ��Ͽ� ���� å�� �����ϴ�. �޴��� ���ư��ϴ�. \n"; //����Ű�� ������ 
            cout << "------------------------------------------------\n";
            // string yn;
            // cin >> yn;
            return;
        }

        // ������� ���� Ƚ���� ��� ������ ��� break;
        if (reserveBookList.size() >= RESERVEUSERMAX) {
            cout << "------------------------------------------------\n";
            cout << "����Ƚ���� ��� �����Ǿ����ϴ�. �޴��� ���ư��ϴ�. \n"; //����Ű�� ������ 
            cout << "------------------------------------------------\n";

            // string yn;
            // cin >> yn;
            return;
        }
        // â �ʱ�ȭ �ʿ�
        bookListPrint(bookBasketList, true, true, true, true);
        cout << "------------------------------------------------";

        int select; // +1 �ؼ� �����ؾ� ��.
        select = input("\n������ å�� ��ȣ�� �Է��ϼ���. (0�� �Է��ϸ� �޴��� ���ư��ϴ�.): ", 0, bookBasketList.size());

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
        else if (bookBasketList.at((int)(select - 1))->getBorrower() == (id + "_" + name + "_" + s_id)) { // �̹� ����ڰ� �������� å�� ���
            cout << "------------------------------------------------\n";
            cout << "�ش� ������ �̹� �������Դϴ�.\n";
        }
        else {

            reserveBookList.push_back(bookBasketList.at((int)(select - 1))); //����
            bookBasketList.at((int)(select - 1))->addReserve(this);
            bookBasketList.erase(bookBasketList.begin() + select - 1); // ��ٱ��Ͽ��� ����� ���� ���� (Ȥ�� ����ڰ� �������� �����ұ��)

            //-----------------------------------------------------------------
            //----------------UserId.txt ���� ���� code-------------------------
            //-----------------------------------------------------------------
			ofstream file("datafile/User/" + id + ".txt", ios::trunc);
			if (!file.is_open()) {
				cerr << "���� ���� UserId.txt ���� open ����" << endl;
				exit(1);
			}

			file << password + "_" + name + "_" + s_id << endl;
            file << limitDate << endl;
            file << limitedStack << endl << endl;

            file << "���⵵������" << endl;
            for (size_t i = 1; i <= 3; i++) {
                file << i << ".";
                if (borrowBookList.size() >= i) {
                    BorrowInfo bi = borrowBookList.at(i - 1);
                    file << bi.book->getName() << "_";
                    file << bi.book->getAuthor() << "_";
                    file << bi.book->getTranslator() << "_";
                    file << bi.book->getPublisher() << "_";
                    file << bi.borrowDate << "_";
                    file << bi.dueDate;
                }
                file << endl;
            }
            file << "���൵������" << endl;
            for (size_t i = 1; i <= 3; i++) {
                file << i << ".";
                if (reserveBookList.size() >= i - 1) {
                    Book* b = reserveBookList.at(i - 1);
                    file << b->getName() << "_";
                    file << b->getAuthor() << "_";
                    file << b->getTranslator() << "_";
                    file << b->getPublisher() << "_";
                }
                file << endl;
            }

            while (!file.is_open()) file.close();
            //-----------------------------------------------------------------
            //-----------------------------------------------------------------
            //-----------------------------------------------------------------

            cout << "------------------------------------------------\n";
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
        cout << "------------------------------------------------";

        num = input("\n�޴�����: ", 1, 3);

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
                if(!borrowBookList.empty()){
                // if (borrow != nullptr) {
                    for(int bi=0;bi<borrowBookList.size();bi++){
                        Book* borrow = borrowBookList.at(bi).book;
                        cout << "======= "<<bi+1 << "�� ======="<<endl;
                        cout << "������ : " + borrow->getName() << endl;
                        cout << "���� : " + borrow->getAuthor() << endl;
                        cout << "���ǻ� : " + borrow->getPublisher() << endl;
                        cout << "���� ���� : " + borrow->getPublishYear() << endl;
                        cout << "�ݳ� ��¥ : " << borrowBookList.at(bi).dueDate << endl;
                        if (limitDate != "false") // ���ѻ����� ��� - true
                            cout << "���ѻ��� ���� : O" << endl;
                        else //���ѻ��� �ƴ� ��� - false
                            cout << "���ѻ��� ���� : X"  << endl;

                        
                        if ((limitDate == "false") && borrow->getReserveStudentsSize() == 0)
                            cout << "���� ���� ���� : O" << endl;
                        else
                            cout << "���� ���� ���� : X" << endl;
                        cout << "------------------------------------------------\n";
                        cout << endl;
                    }
                }
                else {
                    cout << "���� ���� ���� ������ �����ϴ�.\n";
                    cout << "------------------------------------------------\n";
                    cout << endl;
                    break;
                }


                if (!borrowBookList.empty()) {
                    //�ݳ��� ���� + ���ư��� �޴� �߰�
                    cout << "------------------------------------------------\n";
                    cout << "1. �ݳ��ϱ�\n2. �����ϱ�\n3. ���ư���\n";
                    cout << "------------------------------------------------";

                    u1 = input("\n�޴�����: ", 1, 3);

                    if (u1 == 1) {
                        
                        int bnum = input("\n�ݳ��� ���� ��ȣ�� �����ϼ���(���:0): ", 0, borrowBookList.size());
                        
                        cout << "------------------------------------------------\n";
                        cout << "���� �ݳ��Ͻðڽ��ϱ�? (YES: Y/y NO: Any Key)" << endl;
                        cout << ">> ";
                        
                        getline(cin, answer);

                        if (answer == "Y" || answer == "y") {
                            //cout << "������ �ݳ��մϴ�." << endl;
                            returnBook(bnum - 1);
                        }
                        else {
                            cout << "------------------------------------------------\n";
                            cout << "���� �ݳ��� ��ҵǾ����ϴ�.\n";
                            cout << "------------------------------------------------\n";
                        }
                    }
                    else if (u1 == 2) {
                        int bnum = input("\n������ ���� ��ȣ�� �����ϼ���(���:0): ", 0, borrowBookList.size());
                        extendBook(bnum - 1);

                        // cout << "------------------------------------------------\n";
                        // cout << "���� ������ �����մϴ�.\n";
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
                    cout << "------------------------------------------------";
                    int qu;
                    qu = input("\n 0�� ���� �� ���� �޴��� ���ư��ϴ�:: ", 0, 0);

                    if (qu == 0) 
                        break;
                }
                bookListPrint(reserveBookList, true, true, true, true);


                //���ư��� �ɼ� �߰� - 0�� ���� ��
                cout << "------------------------------------------------";
                int booknum;
                booknum = input("\n������ ����� ���� ��ȣ�� �������ּ���(0�� ���� �� ���� �޴��� ���ư��ϴ�): ", 0, reserveBookList.size()+1);

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
    }
}


void Student::returnBook(int bi) // ���������� -> å �ݳ� //������
{
    //vector<BorrowInfo> borrowBookList���� �ش� ���� ����
    //vector<BorrowInfo> BI; 
    //BI = borrowBookList;
    //BI.erase(BI.begin()+booknum-1);
    
    if(!borrowBookList.empty()){
    // if (borrow != nullptr) {
        Book* borrow = borrowBookList.at(bi).book;
        borrow->deleteBorrow(); // å���� ������ ����
        
        ofstream file("datafile/User/" + id + ".txt", ios::out);
        if (!file.is_open()) {
            cerr << "datafile/User/" + id + ".txt file is not Open for delete borrow Book" << endl;
            exit(1);
        }

        /* [��й�ȣ]_[�̸�]_[�й�]
        ���ѻ��� ����(true / false)
        ���ѻ��� ���� Ƚ�� Default = 0
        */

        file << password << "_" << name << "_" << s_id << endl;
        // ��ü���� Ȯ�� �� ���� ���� ����       
        if (getDiff_date(borrowBookList.at(bi).dueDate, getCurrent_date()) > 0) { // �� < �� �� ���
            if (limitDate == "true") {
                overDueBookNum--;
                if (overDueBookNum == 0) { // ��ü�� å ��� �ݳ��ϸ� true -> ���ѻ��³�¥�� ����.
                    limitedStack++;
                    limitDate = getAfter_date(getCurrent_date(), 14 * limitedStack);
                }
            }
        }
        file << limitDate << endl << limitedStack << endl;

        borrowBookList.erase(borrowBookList.begin() + bi);

        file << "���⵵������" << endl;
        for (size_t i = 1; i <= 3; i++) {
            file << i << ".";
            if (borrowBookList.size() >= i) {
                BorrowInfo x = borrowBookList.at(i - 1);
                file << x.book->getName() << "_";
                file << x.book->getAuthor() << "_";
                file << x.book->getTranslator() << "_";
                file << x.book->getPublisher() << "_";
                file << x.borrowDate << "_";
                file << x.dueDate;
            }
            file << endl;
        }
        file << "���൵������" << endl;
        for (size_t i = 1; i <= 3; i++) {
            file << i << ".";
            if (reserveBookList.size() >= i) {
                Book* b = reserveBookList.at(i - 1);
                file << b->getName() << "_";
                file << b->getAuthor() << "_";
                file << b->getTranslator() << "_";
                file << b->getPublisher() << "_";
            }
            file << endl;
        }

        while (!file.is_open()) file.close();

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

void Student::extendBook(int bi) // ���������� -> å ���� //������
{
    bool reserveNumFlag = false; //������ ���翩�� ����

    //if (borrow != nullptr) {
    if(!borrowBookList.empty()){
        BorrowInfo bInfo = borrowBookList.at(bi); // ���� �����Ϸ��� å
        Book* borrow = bInfo.book;

        if (borrow->getReserveStudentsSize() == 0) // �����Ϸ��� å�� �����ڰ� ���� ���ϴ� ���
            reserveNumFlag = false;
        else // �����ڰ� �����ϴ� ���
            reserveNumFlag = true;

        if ((limitDate == "false") && !reserveNumFlag) { 
            // ���忡 ������ ���°�� ���ѻ���X ������X
            bInfo.dueDate = getAfter_date(bInfo.dueDate, 14);

            //-----------------------------------------------------------------
            //----------------UserId.txt ���� ���� code-------------------------
            //-----------------------------------------------------------------
            ofstream file("datafile/User/" + id + ".txt", ios::trunc);
            if (!file.is_open()) {
                cerr << "å ���忩�� ���� UserId.txt ���� open ����" << endl;
                exit(1);
            }

			file << password + "_" + name + "_" + s_id << endl;
            file << limitDate << endl;
            file << limitedStack << endl << endl;

            file << "���⵵������" << endl;
            for (size_t i = 1; i <= 3; i++) {
                file << i << ".";
                if (borrowBookList.size() >= i) {
                    BorrowInfo x = borrowBookList.at(i - 1);
                    file << x.book->getName() << "_";
                    file << x.book->getAuthor() << "_";
                    file << x.book->getTranslator() << "_";
                    file << x.book->getPublisher() << "_";
                    file << x.borrowDate << "_";
                    file << x.dueDate;
                }
                file << endl;
            }
            file << "���൵������" << endl;
            for (size_t i = 1; i <= 3; i++) {
                file << i << ".";
                if (reserveBookList.size() >= i) {
                    Book* b = reserveBookList.at(i - 1);
                    file << b->getName() << "_";
                    file << b->getAuthor() << "_";
                    file << b->getTranslator() << "_";
                    file << b->getPublisher() << "_";
                }
                file << endl;
            }

            while (!file.is_open()) file.close();
            //-----------------------------------------------------------------
            //-----------------------------------------------------------------
            //-----------------------------------------------------------------

            cout << "------------------------------------------------\n";
            cout << "�ش� ���� ������ �Ϸ�Ǿ����ϴ�.\n";
            cout << "------------------------------------------------\n";
        }
        else if (limitDate != "false") {
            //��ü�� ���
            cout << "------------------------------------------------\n";
            cout << "����ڴ� ���� ���ѻ����̹Ƿ� ������ �Ұ����մϴ�.\n";
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
}

void Student::cancelReserveBook(int booknum) // ���������� -> å ���� ���(����������) //������
{
    if (reserveBookList.size() != 0) {
        Book* b = reserveBookList.at(booknum - 1); // ������ ����� å
        b->deleteReserve(this); // å���� ������ ����

        reserveBookList.erase(reserveBookList.begin() + booknum - 1); // ����� ����

		//-----------------------------------------------------------------
		//----------------UserId.txt ���� ���� code-------------------------
		//-----------------------------------------------------------------
        ofstream file("datafile/User/" + id + ".txt", ios::trunc);
        if (!file.is_open()) {
            cerr << "���� ��� ���� ���� UserId.txt���� open ����" << endl;
            exit(1);
        }

		file << password + "_" + name + "_" + s_id << endl;
		file << limitDate << endl;
		file << limitedStack << endl << endl;

		file << "���⵵������" << endl;
		for (size_t i = 1; i <= 3; i++) {
			file << i << ".";
			if (borrowBookList.size() >= i) {
				BorrowInfo bi = borrowBookList.at(i - 1);
				file << bi.book->getName() << "_";
				file << bi.book->getAuthor() << "_";
				file << bi.book->getTranslator() << "_";
				file << bi.book->getPublisher() << "_";
				file << bi.borrowDate << "_";
				file << bi.dueDate;
			}
			file << endl;
		}
		file << "���൵������" << endl;
		for (size_t i = 1; i <= 3; i++) {
			file << i << ".";
			if (reserveBookList.size() >= i) {
				Book* b = reserveBookList.at(i - 1);
				file << b->getName() << "_";
				file << b->getPublisher() << "_";
			}
			file << endl;
		}

		while (!file.is_open()) file.close();
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------
		//-----------------------------------------------------------------


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
// ���Ŀ� bool�Ⱦ��� � ����Ʈ�Ŀ� ���� �Լ� (������
// ...�� �������� �׷��� book �� ����� �ϳ� �鿩���� ��.
void Student::bookListPrint(vector<Book*> book, bool nameTF, bool authorTF, bool borrowTF, bool reserveNumTF) const
{

    int listSize = book.size();

    for (int i = -1; i < listSize; i++) { // index: -1�� ��ܹ� ���, 0���� å ���
        if (i == -1) {
            cout << "\n" << "    " << (nameTF ? "[������]" : "") << "\t" << (authorTF ? "[���ڸ�]" : "") << "\t" << "[����]\t[���ǻ�]";
            cout << "\t" << (borrowTF ? "[���Ⱑ�ɿ���]" : "") << "\t" << (reserveNumTF ? "[�����ο���]" : "");
            cout << "\n---------------------------------------------------------------------------------------------\n";
        }
        else {
            cout << "\n" << i+1 << "��: " << (nameTF ? book[i]->getName() : "") << "\t" << (authorTF ? book[i]->getAuthor() : "") << "\t" << book[i]->getTranslator() << "\t" << book[i]->getPublisher();
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
    cout << "\n---------------------------------------------------------------------------------------------\n";
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

void Student::setLimitDate(string date)
{
    this->limitDate = date;
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

string Student::getLimitDate() const
{
    return limitDate;
}


int Student::getBorrowListNum() const
{
    return borrowBookList.size();
}

string Student::getBookName(int bi) const
{
    if(getBorrowListNum() < bi){
        string nu = "NULL";
        return nu;
    }
    return borrowBookList.at(bi).book->getName();
}

string Student::getBorrowDate(int bi) const{
    if(getBorrowListNum() < bi){
        string nu = "NULL";
        return nu;
    }
    return borrowBookList.at(bi).borrowDate;
}

string Student::getDueDate(int bi) const{

    if(getBorrowListNum() < bi){
        string nu = "NULL";
        return nu;
    }
    return borrowBookList.at(bi).dueDate;
}


bool Student::operator==(Student student) 
{
    if (this->s_id == student.s_id) { // �й� ������ ���� �ι�
        return true;
    }
    else return false;
}

bool Student::operator>=(Student Student) {
    int s_min = 99999999;
    int c_min = 99999999;

    string s;
    for (size_t i = 0; i < 3; i++) {
        s = Student.getDueDate(i);
        s.erase(remove(s.begin(), s.end(), '.'));
        s_min = min(s_min, stoi(s));
    }

    for (size_t i = 0; i < 3; i++) {
        s = borrowBookList.at(i).dueDate;
        s.erase(remove(s.begin(), s.end(), '.'));
        c_min = min(c_min, stoi(s));
    }

    return c_min <= s_min;
}