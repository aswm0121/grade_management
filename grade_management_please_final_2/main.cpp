#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip> // setw() 사용
#include "UserManager.cpp" // UserManager 클래스 포함
#include "Users.cpp"
#include "display.cpp"

using namespace std;

// 파일에서 학번과 이름을 확인하는 함수 ========================
bool verifyInFile(const string& filePath, int inputId, const string& inputName) {
    ifstream file(filePath);

    if (!file.is_open()) {
        cerr << "파일을 열 수 없습니다: " << filePath << endl;
        return false;
    }

    int id;
    string name;

    // 파일에서 학번과 이름을 읽어와 비교
    while (file >> id >> name) {
        if (id == inputId && name == inputName) {
            return true; // 일치하면 true 반환
        }
    }

    return false; // 파일 끝까지 확인해도 없으면 false 반환
} // 단일 파일에서 학번과 이름을 확인하는 함수 Fin ========================

int main() {
    main_display();

    string studentFile = "./관리자_파일/students.txt";  // 학생 정보 파일
    string professorFile = "./관리자_파일/professors.txt";  // 교수 정보 파일

    // 관리자_파일
    UserManager studentManager(studentFile);    // 학생 파일
    UserManager professorManager(professorFile); // 교수 파일

    int inputId;
    string inputName;

    cout << "ID(or 학번)을 입력하세요: ";
    cin >> inputId;
    cout << "이름을 입력하세요: ";
    cin >> inputName;
    cout << "\n\n" << endl;

    // 학생 파일에서 확인 -> 학생이 맞을 경우 ---------------------------------------------------------------------------
    if (verifyInFile(studentFile, inputId, inputName)) {
        while (true) { // 학생 모드 루프
            int choice;
            student_display();
            cin >> choice;

            if (choice == 0) {
                cout << "학생 모드를 종료합니다." << endl;
                break;
            }

            string class_name, c_num, studentID;
            cout << "점수를 확인할 강의를 입력하세요: ";
            cin >> class_name;
            cout << "분반을 입력하세요: ";
            cin >> c_num;
            cout << "학번을 입력하세요: ";
            cin >> studentID;

            File file("./grade_csv/" + class_name + "_" + c_num + ".csv");
            string route = "./grade_csv/" + class_name + "_" + c_num + ".csv";
            file.loadCSV();  // 파일 불러오기
            Course2 course(file.getCSV());

            // 특정 학생 성적 출력
            Student s(inputName, studentID, course);

            if (choice == 1) {
                s.getGrade();
            } else if (choice == 2) {
                string category;
                cout << "중간,기말, 과제, 출석 중 수정할 부분을 입력하시오: ";
                cin >> category;
                s.submitAppeal(category, route);
            } else {
                cout << "잘못된 입력입니다." << endl;
            }
        }
    }
    // 교수 파일에서 확인 -> 교수로 로그인==============================================================================
    else if (verifyInFile(professorFile, inputId, inputName)) {
        while (true) { // 교수 모드 루프
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
            } else {
                cout << "잘못된 입력입니다. 다시 시도하세요." << endl;
            }
        }
    }
    // 관리자 확인 -> 관리자로 로그인 =================================================================================
    else if (inputId == 202412345 && inputName == "김기락") {
        while (true) { // 관리자 모드 루프
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
                    return 0;
                default:
                    cout << "잘못된 입력입니다. 다시 시도하세요." << endl;
            }
        }
    }
    // 사용자 확인 실패
    else {
        cout << "사용자가 존재하지 않습니다." << endl;
    }

    return 0;
}
