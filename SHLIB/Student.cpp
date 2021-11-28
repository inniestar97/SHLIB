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
#include <set>

#define BASKETMAX 10
#define BORROWMAX 3
#define RESERVEUSERMAX 3
#define RESERVEBOOKMAX 5

using std::cout;

// using namespace std;

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

    /* "datafile/User/" + id + ".txt" ���� ���

    [��й�ȣ]_[�̸�]_[�й�]
    ���ѻ��� ����(true / false)
    ���ѻ��� ���� Ƚ�� Default = 0

    ���⵵������
    1. [������]_[���ڸ�]_[����]_[���ǻ�]_[������]_[�ݳ���]
    2. [������]_[���ڸ�]_[����]_[���ǻ�]_[������]_[�ݳ���]
    3. .
    ���൵������
    1. [������]_[���ڸ�]_[����]_[���ǻ�]
    2. [������]_[���ڸ�]_[����]_[���ǻ�]
    3. [������]_[���ڸ�]_[����]_[���ǻ�]

    */

    string info;
    file >> info; // info = ��й�ȣ_�̸�_�й�
    this->password = info.substr(0, info.find('_')); // ��й�ȣ
    info = info.substr(info.find('_') + 1, string::npos); // info = �̸�_�й�
    this->name = info.substr(0, info.find('_')); // �̸�
    this->s_id = info.substr(info.find('_') + 1, string::npos); // �й�

    file >> info; // ���ѻ��� ����
    if (info != "false") limitDate = info; // ���ѻ����� ���
    else limitDate = "false"; // ���ѻ��°� �ƴҰ��

    file >> info; // ���ѻ��� ����
    limitedStack = stoi(info);

    file >> info; // info = "���⵵�� ����"
    cin.ignore(); // ���� ����
    for (size_t i = 0; i < 3; i++) {
        getline(cin, info); // info : 1. [������]_[���ڸ�]_[����]_~~�䷱��

        if (info.size() > 4) { // ���⵵�� ����
            info = info.substr(3, string::npos);

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
                b_info[4] = ���࿬��
                b_info[5] = ������
                b_info[6] = �ݳ���
            */

            BorrowInfo br;
            br.book = new Book(b_info[0], b_info[1]);
            br.borrowDate = b_info[5];
            br.dueDate = b_info[6];

            borrowBookList.push_back(br);
        }
    }

    file >> info; // info = "���൵�� ����"
    cin.ignore();
    for (size_t i = 0; i < 3; i++) {
        getline(cin, info);

        if (info.size() > 4) { // ���൵�� ����
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
    // �Ҹ��ڵ� ���ؾ� �մϴ�.
    // 
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
    cout << "=============\n";

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
        bookListPrint(searchResult, false, true, true, true, true);

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
    if (limitDate != "false") {
        cout << "���ѻ����̹Ƿ� ��ٱ��� �޴��� ����Ͻ� �� �����ϴ�. ���ѻ��� ������: [" << limitDate << "]" << endl;
        return;
    }
    int basketListNum;

    while (1) {
        cout << "\n��ٱ���\n";
        bookListPrint(bookBasketList, false, true, true, true, true);
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
        bookListPrint(bookBasketList, false, true, true, true, true);

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
            //borrow = bookBasketList.at((int)(select - 1)); // ���� (���� ���� 1���̸� �̰Ű�, ���� ���� �Ǽ� ���� �þ�� ����Ʈ�� �°� �����ؾߵ�.)
                
            BorrowInfo bi_temp;
            bi_temp.book = bookBasketList.at((int)(select - 1));
            bi_temp.borrowDate = getCurrent_date();
            bi_temp.dueDate = getAfter_date(getCurrent_date(), 14);
            borrowBookList.emplace_back(bi_temp);

            bookBasketList.at((int)(select - 1))->addBorrow(this);
            bookBasketList.erase(bookBasketList.begin() + select - 1); // ����
            
            isLimited = false; // �� �ִ� �� �³�?

            // �л��������� ������Ʈ
            string stu_path = "datafile/User/" + id + ".txt";
            ofstream student_file(stu_path, ios::out);
            if (!student_file) {
                cout << "���� open ����" << endl;
                return;
            }

            student_file << password << "_" << name << "_" << s_id << endl;
            student_file << (isLimited ? "true" : "false") << endl;

            student_file << "���⵵������" << endl;
            
            /*if (borrow != nullptr) {
                student_file << borrow->getName() << "_" << borrow->getAuthor() << "_"
                << borrow->getTranslator() << "_"
                << borrowDate << "_" << dueDate << endl;
            }*/

            if(!borrowBookList.empty()){
                for(int bi=0;bi<borrowBookList.size();bi++){
                    student_file << borrowBookList.at(bi).book->getName() << "_" << borrowBookList.at(bi).book->getAuthor() << "_"
                    << borrowBookList.at(bi).book->getTranslator() << "_"
                    << borrowBookList.at(bi).borrowDate << "_" << borrowBookList.at(bi).dueDate << endl;
                }
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

void Student::deleteBook() // ��ٱ��� -> ���� ���� ����
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
        else if (bookBasketList.at((int)(select - 1))->getBorrower() == this) { // �̹� ����ڰ� �������� å�� ���
            cout << "------------------------------------------------\n";
            cout << "�ش� ������ �̹� �������Դϴ�.\n";
        }
        else {
            cout << "------------------------------------------------\n";

            reserveBookList.emplace_back(bookBasketList.at((int)(select - 1))); //����
            bookBasketList.at((int)(select - 1))->addReserve(this);
            bookBasketList.erase(bookBasketList.begin() + select - 1); // ��ٱ��Ͽ��� ����� ���� ���� (Ȥ�� ����ڰ� �������� �����ұ��)

			ofstream file("datafile/User/" + id + ".txt", ios::trunc);
			if (!file.is_open()) {
				cerr << "datafile/User/" + id + ".txt is not open for write addReserve in Book Class" << endl;
				exit(1);
			}

			file << password + "_" + name + "_" + s_id << endl;
			file << boolalpha << limitDate << endl;
			file << "���⵵������" << endl;  
            /*if (borrow != nullptr) {
                file << borrow->getName() << "_" << borrow->getAuthor() << "_"
                << borrow->getTranslator() << "_"
                << borrowDate << "_" << dueDate << endl;
            }*/
            if(!borrowBookList.empty()){
                for(int bi=0;bi<borrowBookList.size();bi++){
                    file << borrowBookList.at(bi).book->getName() << "_" << borrowBookList.at(bi).book->getAuthor() << "_"
                    << borrowBookList.at(bi).book->getTranslator() << "_"
                    << borrowBookList.at(bi).borrowDate << "_" << borrowBookList.at(bi).dueDate << endl;
                }
            }
            file << "���൵������" << endl;
            for (size_t i = 0; i < reserveBookList.size(); i++) {
                Book* book = reserveBookList.at(i);
                file << book->getName() << "_" << book->getAuthor() << "_"
                << book->getTranslator() << "_" << book->getPublisher() << "_"
                << book->getPublishYear() << endl;
            }

            file.close();


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
                    cout << "���� ���� ���� ������ �����ϴ�. \n";
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
                            cout << "������ �ݳ��մϴ�." << endl;
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

                        cout << "------------------------------------------------\n";
                        cout << "���� ������ �����մϴ�.\n";
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
                bookListPrint(reserveBookList, false, true, true, true, true);


                //���ư��� �ɼ� �߰� - 0�� ���� ��
                cout << "------------------------------------------------";
                int booknum;
                booknum = input("\n���� ��ȣ�� �������ּ���(0�� ���� �� ���� �޴��� ���ư��ϴ�): ", 0, reserveBookList.size()+1);

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
        ���ѻ��� ���� Ƚ�� Default = 0*/

        file << password << "_" << name << "_" << s_id << endl;
        // ��ü���� Ȯ�� �� ���� ���� ����
        if (getDiff_date(borrowBookList.at(bi).dueDate, getCurrent_date()) > 0) // ��ü�� ��� - true
            file << "true" << endl << "false" << endl << endl;
        else //��ü���� ���� ��� - false
            file << "false" << endl << "false" << endl << endl;
        file << "���⵵������" << endl;
        /* �߰��ؾ� �� */

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

void Student::extendBook(int bi) // ���������� -> å ���� //������
{
    bool reserveNumFlag = false; //������ ���翩�� ����

    //if (borrow != nullptr) {
    if(!borrowBookList.empty()){
        Book* borrow = borrowBookList.at(bi).book;
        if (borrow->getReserveStudentsSize() == 0) // �����ڰ� ���� ����
            reserveNumFlag = false;
        else // �����ڰ� �����ϴ� ���
            reserveNumFlag = true;
        if ((limitDate == "false") && !reserveNumFlag) { // ���忡 ������ ���°�� -> ���ѻ��� ����, ������ ����
            dueDate = getAfter_date(dueDate, 14);

            ofstream file("datafile/User/" + id + ".txt", ios::out);
            if (!file.is_open()) {
                cerr << "datafile/User/" + id + ".txt file is not Open for expend duedate" << endl;
                exit(1);
            }

            file << password << "_" << name << "_" << s_id << endl;
            file << "false" << endl << "false" << endl << endl;
            file << "���⵵������" << endl;
            if(!borrowBookList.empty()){
                for(int bi = 0; bi<borrowBookList.size();bi++){
                    Book* borrow = borrowBookList.at(bi).book;
                    file << borrow->getName() << "_" << borrow->getAuthor() << "_" << borrow->getTranslator();
                    file << borrow->getPublisher() << "_";
                    file << borrowBookList.at(bi).borrowDate << "_" << borrowBookList.at(bi).dueDate << endl;
                }
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
    //vector<Book> reserveBookList���� �ش� ���� ����
    if (reserveBookList.size() != 0) {
        reserveBookList.at(booknum - 1)->deleteReserve(this); // å���� ������ ����

        reserveBookList.erase(reserveBookList.begin() + booknum - 1); // ����� ����

        ofstream file("datafile/User/" + id + ".txt", ios::trunc);
        if (!file.is_open()) {
            cerr << "datafile/User/" + id + ".txt is not open for cancelReserveBook." << endl;
            exit(1);
        }
        file << password << "_" << name << "_" << s_id << endl;
        file << "false" << endl << "false" << endl << endl;
        file << "���⵵������" << endl;

        if(!borrowBookList.empty()){
            for(int bi = 0; bi<borrowBookList.size();bi++){
                Book* borrow = borrowBookList.at(bi).book;
                file << borrow->getName() << "_" << borrow->getAuthor() << "_" << borrow->getTranslator();
                file << borrow->getPublisher() << "_";
                file << borrowBookList.at(bi).borrowDate << "_" << borrowBookList.at(bi).dueDate << endl;
            }
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
            cout << "\n" << "    " << (nameTF ? "[������]" : "") << "\t" << (authorTF ? "[���ڸ�]" : "") << "\t" << "[����]\t[���ǻ�]";
            if (borrowListTF) { //�����̸�
                cout << "\t" << "[��ü����]" << "\t" << "[�ݳ���¥]" << "\t" << "[���尡�ɿ���]";
            }
            cout << "\t" << (borrowTF ? "[���Ⱑ�ɿ���]" : "") << "\t" << (reserveNumTF ? "[�����ο���]" : "");
            cout << "\n---------------------------------------------------------------------------------------------\n";
        }
        else {
            if (borrowListTF) { //������Ȳ (1�� ���� 1���̶� �ݺ��� ���� ������,,)
                cout << "\n" << i + 1 << ".\t" << (nameTF ? book[i]->getName() : "") << "\t" << (authorTF ? book[i]->getAuthor() : "") << "\t" << book[i]->getTranslator() << "\t" << book[i]->getPublisher();

                cout << "\t" << ((limitDate != "false") ? "O\t" : "X\t") << dueDate << "\t" << canExtend;

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

bool Student::operator==(Student student) // ������
{
    if (this->s_id == student.s_id) { // �й� ������ ���� �ι�
        return true;
    }
    else return false;
}