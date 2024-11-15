
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "course.cpp"
#include "UserManager.cpp" // UserManager 클래스 포함

using namespace std;

// 단일 파일에서 학번과 이름을 확인하는 함수 ========================
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

    
    cout << "╠══════════════════════════════════════════════════════════╣" << endl;
    cout << "║                                                          ║" << endl;
    cout << "║   Welcome to the Grade Management System (GMS)!          ║" << endl;
    cout << "║                                                          ║" << endl;
    cout << "║   What you can do based on your role:                    ║" << endl;
    cout << "║                                                          ║" << endl;
    cout << "║     [1] 학생(Student):                                   ║" << endl;
    cout << "║         - View your grades                               ║" << endl;
    cout << "║         - Analyze your performance statistics            ║" << endl;
    cout << "║                                                          ║" << endl;
    cout << "║     [2] 교수(Professor):                                 ║" << endl;
    cout << "║         - View and manage student grades                 ║" << endl;
    cout << "║         - Analyze class performance                      ║" << endl;
    cout << "║                                                          ║" << endl;
    cout << "║     [3] 관리자(Admin):                                   ║" << endl;
    cout << "║         - Add or remove users                            ║" << endl;
    cout << "║         - Manage user and grade data                     ║" << endl;
    cout << "║                                                          ║" << endl;
    cout << "║                                                          ║" << endl;
    cout << "╚══════════════════════════════════════════════════════════╝" << endl;


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

    // 학생 파일에서 확인-> 학생이 맞을 경우---------------------------------------------------------------------------
    if (verifyInFile(studentFile, inputId, inputName)) {
        cout << "환영합니다 (" << inputName << ")학생님!" << endl;
        // 강의 csv 파일을 불러와서 출력 및 데이터 반환
        string class_name;
        string c_num;
        string studentID;

        cout << "점수를 확인할 강의를 입력하세요: ";
        cin >> class_name;
        cout << "분반을 입력하세요: ";
        cin >> c_num;
        cout << "학번을 입력하세요: ";
        cin >> studentID;
        
        
        Course course(class_name, c_num, "홍길동");
        File file("./grade_csv/" + class_name + "_" + c_num + ".csv");
        file.loadCSV();  // 파일 불러오기


        // file 객체에서 CSV 파일의 성적을 불러와서 map형태로 저장 
        // {'20240001': {'중간고사':100, '기말고사':90, '과제':50, '출석 점수':20}, }
        course.setGradeMap(file);


        // 성적 통계 계산 
        course.calculateRankAndStatistics();

        // 성적 및 통계 출력
        cout << endl;
        // course.printGradesAndStatistics();


    // // 특정 학생의 성적 및 통계 출력
        cout << endl;
        course.printStudentStatistics(studentID);
        return 0;


    }



// 교수 파일에서 확인
//     교수로 들어왔을 때==========================================================================================
    else if (verifyInFile(professorFile, inputId, inputName)) {
        cout << "환영합니다 (" << inputName << ")교수님!" << endl;
    }



// 관리자 id: 202412345, 이름:김기락
//     관리자로 들어왔을 때==========================================================================================
    else if (inputId == 202412345 && inputName == "김기락") {

        cout << "관리자님 환영합니다..! 할 일을 선택해주세요..!" << endl;
        while (true) {
            int choice, id;
            string name;
            cout << "\n1. 학생 추가\n2. 교수 추가\n3. 학생 삭제\n4. 교수 삭제\n";
            cout << "5. 학생 수정\n6. 교수 수정\n7. 학생 출력\n8. 교수 출력\n9. 종료\n";
            cout << "선택: ";
            cin >> choice;

            switch (choice) {
            case 1:  // 학생 추가
                cout << "학생 ID와 이름 입력: ";
                cin >> id >> name;
                studentManager.addUser(id, name);
                break;
            case 2:  // 교수 추가
                cout << "교수 ID와 이름 입력: ";
                cin >> id >> name;
                professorManager.addUser(id, name);
                break;
            case 3:  // 학생 삭제
                cout << "삭제할 학생 ID 입력: ";
                cin >> id;
                studentManager.deleteUser(id);
                break;
            case 4:  // 교수 삭제
                cout << "삭제할 교수 ID 입력: ";
                cin >> id;
                professorManager.deleteUser(id);
                break;
            case 5:  // 학생 수정
                cout << "수정할 학생 ID와 새로운 이름 입력: ";
                cin >> id >> name;
                studentManager.modifyUser(id, name);
                break;
            case 6:  // 교수 수정
                cout << "수정할 교수 ID와 새로운 이름 입력: ";
                cin >> id >> name;
                professorManager.modifyUser(id, name);
                break;
            case 7:  // 학생 출력
                studentManager.displayUsers();
                break;
            case 8:  // 교수 출력
                professorManager.displayUsers();
                break;
            case 9:  // 종료
                return 0;
            default:
                cout << "잘못된 입력입니다.\n";
            }
    }


    }




    // 그 외
    else {
        cout << "사용자가 존재하지 않습니다." << endl;
    }

   


}
