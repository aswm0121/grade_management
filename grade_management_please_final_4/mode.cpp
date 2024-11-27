#include <iostream>
#include <fstream>
#include <string>
#include "UserManager.cpp"
#include "Users.cpp"
#include "display.cpp"

using namespace std;

// 파일에서 학번과 이름을 확인하는 함수
bool verifyInFile(const string& filePath, int inputId, const string& inputName) {
    ifstream file(filePath);

    if (!file.is_open()) {
        cerr << "파일을 열 수 없습니다: " << filePath << endl;
        return false;
    }

    int id;
    string name;
    while (file >> id >> name) {
        if (id == inputId && name == inputName) {
            return true;
        }
    }

    return false;
}

// 학생 모드
void studentMode(int inputId, const string& inputName) {
    while (true) {
        int choice;
        student_display();
        cin >> choice;

        if (choice == 0) {
            cout << "학생 모드를 종료합니다." << endl;
            break;
        }

        string class_name, c_num, studentID;
        studentID = to_string(inputId);
        cout << "점수를 확인할 강의를 입력하세요: ";
        cin >> class_name;
        cout << "분반을 입력하세요: ";
        cin >> c_num;
        // cout << "학번을 입력하세요: ";
        // cin >> studentID;

        File file("./grade_csv/" + class_name + "_" + c_num + ".csv");
        string route = "./grade_csv/" + class_name + "_" + c_num + ".csv";
        file.loadCSV(); // 파일 불러오기
        Course2 course(file.getCSV());

        Student s(inputName, studentID, course);

        if (choice == 1) {
            s.getGrade();
        } else if (choice == 2) {
            s.viewStatistics();
        }
        else if (choice == 3) {
            string category;
            cout << "중간, 기말, 과제, 출석 중 수정할 부분을 입력하시오: ";
            cin >> category;
            s.submitAppeal(category, route);
        } else {
            cout << "잘못된 입력입니다." << endl;
        }
    }
}

// 교수 모드
void professorMode(int inputId, const string& inputName) {
    while (true) {
        int choice2;
        professor_display();
        cin >> choice2;

        if (choice2 == 0) {
            cout << "교수 모드를 종료합니다." << endl;
            break;
        }

        string course_name, class_num, studentId;
        cout << "강의를 입력하세요: ";
        cin >> course_name;
        cout << "분반을 입력하세요: ";
        cin >> class_num;
        Professor professor("./grade_csv/" + course_name + "_" + class_num + ".csv");

        if (choice2 == 1) {
            float mid, final, assign, attend;
            cout << "중간, 기말, 과제, 출석 가중치 입력: ";
            cin >> mid >> final >> assign >> attend;
            professor.weight_fix(course_name, class_num, mid, final, assign, attend);
            professor.saveToFile("./grade_csv/" + course_name + "_" + class_num + ".csv");
        } else if (choice2 == 2) {
            float aweight, bweight, cweight;
            cout << "a, b, c 비율 입력: ";
            cin >> aweight >> bweight >> cweight;
            professor.abc_fix(course_name, class_num, aweight, bweight, cweight);
            professor.saveToFile("./grade_csv/" + course_name + "_" + class_num + ".csv");
        } else if (choice2 == 3) {
            string category;
            double score;
            cout << "학번을 입력하시오: ";
            cin >> studentId;
            cout << "중간, 기말, 과제, 출석 중 수정할 것을 입력하시오: ";
            cin >> category;
            cout << "점수를 입력하시오: ";
            cin >> score;
            professor.grade_fix(course_name, class_num, studentId, category, score);
            professor.saveToFile("./grade_csv/" + course_name + "_" + class_num + ".csv");
        } else if (choice2 == 4) {
            professor.view_appeal(course_name, class_num);
        } 
        else if (choice2 == 5) {
            professor.viewData();
        } 
        else {
            cout << "잘못된 입력입니다. 다시 시도하세요." << endl;
        }
    }
}

// 관리자 모드
void adminMode(UserManager& studentManager, UserManager& professorManager) {
    while (true) {
        int choice, id;
        string name;

        admin_display();
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "학생 ID와 이름 입력: ";
                cin >> id >> name;
                studentManager.addUser(id, name);
                break;
            case 2:
                cout << "교수 ID와 이름 입력: ";
                cin >> id >> name;
                professorManager.addUser(id, name);
                break;
            case 3:
                cout << "삭제할 학생 ID 입력: ";
                cin >> id;
                studentManager.deleteUser(id);
                break;
            case 4:
                cout << "삭제할 교수 ID 입력: ";
                cin >> id;
                professorManager.deleteUser(id);
                break;
            case 5:
                cout << "수정할 학생 ID와 새로운 이름 입력: ";
                cin >> id >> name;
                studentManager.modifyUser(id, name);
                break;
            case 6:
                cout << "수정할 교수 ID와 새로운 이름 입력: ";
                cin >> id >> name;
                professorManager.modifyUser(id, name);
                break;
            case 7:
                studentManager.displayUsers();
                break;
            case 8:
                professorManager.displayUsers();
                break;
            case 9:
                cout << "관리자 모드를 종료합니다." << endl;
                return;
            default:
                cout << "잘못된 입력입니다. 다시 시도하세요." << endl;
        }
    }
}
