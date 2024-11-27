#include <iostream>
#include <string>
#include "mode.cpp" // mode.cpp 포함

using namespace std;

int main() {
    main_display();

    string studentFile = "./관리자_파일/students.txt";
    string professorFile = "./관리자_파일/professors.txt";

    UserManager studentManager(studentFile);
    UserManager professorManager(professorFile);

    int inputId;
    string inputName;

    cout << "ID(or 학번)을 입력하세요: ";
    cin >> inputId;
    cout << "이름을 입력하세요: ";
    cin >> inputName;
    cout << "\n\n";

    if (verifyInFile(studentFile, inputId, inputName)) {
        studentMode(inputId, inputName);

    } else if (verifyInFile(professorFile, inputId, inputName)) {
        professorMode(inputId, inputName);

    } 
    else if (inputId == 202412345 && inputName == "김기락") {
        adminMode(studentManager, professorManager);
    } 
    else {
        cout << "사용자가 존재하지 않습니다." << endl;
    }

    return 0;
}
