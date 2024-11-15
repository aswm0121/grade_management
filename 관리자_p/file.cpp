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
};