#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include <iostream>
#include <fstream>
#include <map>
#include <string>

using namespace std;

// 학생 또는 교수 정보를 저장하는 클래스
class UserManager {
private:
    map<int, string> users;  // ID를 키로, 이름을 값으로 저장
    string filename;         // 파일 이름

public:
    UserManager(const string& file) : filename(file) {
        loadFromFile();
    }

    // 파일에서 데이터를 불러오기
    void loadFromFile() {
        ifstream inFile(filename);
        if (!inFile) {
            cerr << "파일을 열 수 없습니다: " << filename << endl;
            return;
        }

        users.clear();  // 기존 데이터 초기화
        int id;
        string name;
        while (inFile >> id >> name) {
            users[id] = name;
        }
        inFile.close();
    }

    // 파일에 저장
    void saveToFile() {
        ofstream outFile(filename);
        if (!outFile) {
            cerr << "파일을 열 수 없습니다: " << filename << endl;
            return;
        }

        for (const auto& user : users) {
            outFile << user.first << " " << user.second << endl;
        }
        outFile.close();
    }

    // 사용자 추가
    void addUser(int id, const string& name) {
        users[id] = name;
        saveToFile();
    }

    // 사용자 삭제
    void deleteUser(int id) {
        if (users.erase(id)) {
            cout << "ID " << id << " 삭제됨.\n";
        } else {
            cout << "ID " << id << "을(를) 찾을 수 없습니다.\n";
        }
        saveToFile();
    }

    // 사용자 수정
    void modifyUser(int id, const string& newName) {
        if (users.find(id) != users.end()) {
            users[id] = newName;
            cout << "ID " << id << "의 이름이 수정됨.\n";
            saveToFile();
        } else {
            cout << "ID " << id << "을(를) 찾을 수 없습니다.\n";
        }
    }

    // 사용자 출력
    void displayUsers() {
        for (const auto& user : users) {
            cout << "ID: " << user.first << " 이름: " << user.second << endl;
        }
    }
};

#endif
