#include <iostream>

using namespace std;


#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

class File {
private:
    string filename;
    vector<vector<string>> data;

public:
    // 생성자
    File(const string& fname) : filename(fname) {}

    // CSV 파일 로드 함수
    void loadCSV() {
        ifstream file(filename);
        string line;

        if (file.is_open()) {
            while (getline(file, line)) {
                vector<string> row;
                stringstream ss(line);
                string cell;

                while (getline(ss, cell, ',')) {
                    row.push_back(cell);
                }
                data.push_back(row);
            }
            file.close();
        } else {
            cerr << "파일을 열 수 없습니다." << endl;
        }
    }

    // 파일 내용 출력 함수
    void viewFile() const {
        for (const auto& row : data) {
            for (const auto& cell : row) {
                cout << cell << " ";
            }
            cout << endl;
        }
    }

    // 벡터 형태의 CSV 데이터를 반환하는 함수
    vector<vector<string>> getCSV() const {
        return data;
    }


void saveToFile(const string& filename, const vector<vector<string>>& data) {
    // ofstream 객체 생성 (덮어쓰기 모드로 열기)
    ofstream file(filename); // 'ios::trunc'를 명시적으로 추가 (기본 설정이긴 함)

    if (!file.is_open()) {
        cerr << "파일을 저장할 수 없습니다!" << endl;
        return;
    }

    for (const auto& row : data) {
        for (size_t i = 0; i < row.size(); ++i) {
            file << row[i];
            if (i != row.size() - 1) file << ","; // 쉼표로 구분
        }
        file << endl; // 한 행 끝에서 줄 바꿈
    }

    file.close(); // 파일 닫기
    cout << "데이터가 성공적으로 " << filename << "에 저장되었습니다." << endl;
}


};