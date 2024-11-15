#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>  // sort 함수를 사용하기 위한 헤더
#include <limits>     // 최대, 최소값을 계산하기 위한 헤더
#include "file.cpp"     // File 클래스가 별도 파일에 정의되어 있다고 가정

using namespace std;

class Course {
private:
    string courseName;
    string courseID;
    string professorName;

    map<string, map<string, int>> studentGrades;  // 학생 성적 저장
    map<string, map<string, int>> studentRank;    // 학생 등수 저장

    double meanScore[4];  // 평균점수
    int maxScore[4];      // 최대점수
    int minScore[4];      // 최소점수
    size_t studentCount;  // 학생 수

public:
    // 생성자
    Course(const string& cName, const string& cID, const string& pName)
        : courseName(cName), courseID(cID), professorName(pName), studentCount(0) {
        for (int i = 0; i < 4; ++i) {
            maxScore[i] = numeric_limits<int>::min();
            minScore[i] = numeric_limits<int>::max();
            meanScore[i] = 0.0;
        }
    }

    // 학생 성적을 가져와 map에 저장하는 함수
    void setGradeMap(File& file) {
        vector<vector<string>> data = file.getCSV();  // File 객체에서 CSV 데이터 가져오기

        studentCount = data.size() - 1;  // 첫 번째 행은 헤더이므로 제외

        for (size_t i = 1; i < data.size(); ++i) {
            string studentID = data[i][3];  // 학번
            
            // 성적 항목별로 저장할 map
            map<string, int> grades;
            grades["중간고사"] = stoi(data[i][4]);
            grades["기말고사"] = stoi(data[i][5]);
            grades["과제"] = stoi(data[i][6]);
            grades["출석"] = stoi(data[i][7]);

            studentGrades[studentID] = grades;
        }

        cout << "성적이 성공적으로 저장되었습니다." << endl;
    }

    // 각 항목별로 등수를 계산하는 함수
    void calculateRankAndStatistics() {
        vector<pair<string, int>> midtermScores;
        vector<pair<string, int>> finalScores;
        vector<pair<string, int>> assignmentScores;
        vector<pair<string, int>> attendanceScores;

        int sum[4] = {0, 0, 0, 0};  // 합계를 계산하기 위한 배열

        // 학생들의 성적을 벡터에 삽입 (학번, 점수)
        for (const auto& student : studentGrades) {
            midtermScores.push_back({student.first, student.second.at("중간고사")});
            finalScores.push_back({student.first, student.second.at("기말고사")});
            assignmentScores.push_back({student.first, student.second.at("과제")});
            attendanceScores.push_back({student.first, student.second.at("출석")});

            // 각 항목별로 합계 계산, 최대값, 최소값 갱신
            int scores[4] = {student.second.at("중간고사"), student.second.at("기말고사"), student.second.at("과제"), student.second.at("출석")};
            for (int i = 0; i < 4; ++i) {
                sum[i] += scores[i];
                if (scores[i] > maxScore[i]) maxScore[i] = scores[i];
                if (scores[i] < minScore[i]) minScore[i] = scores[i];
            }
        }

        // 평균 계산
        for (int i = 0; i < 4; ++i) {
            meanScore[i] = static_cast<double>(sum[i]) / studentCount;
        }

        // 각 성적을 내림차순으로 정렬
        auto compare = [](const pair<string, int>& a, const pair<string, int>& b) {
            return a.second > b.second;
        };
        sort(midtermScores.begin(), midtermScores.end(), compare);
        sort(finalScores.begin(), finalScores.end(), compare);
        sort(assignmentScores.begin(), assignmentScores.end(), compare);
        sort(attendanceScores.begin(), attendanceScores.end(), compare);

        // 등수 계산 및 저장
        for (size_t i = 0; i < midtermScores.size(); ++i) {
            studentRank[midtermScores[i].first]["중간고사"] = i + 1;
            studentRank[finalScores[i].first]["기말고사"] = i + 1;
            studentRank[assignmentScores[i].first]["과제"] = i + 1;
            studentRank[attendanceScores[i].first]["출석"] = i + 1;
        }

        cout << "등수와 통계가 성공적으로 계산되었습니다." << endl;
    }

    // 성적과 등수, 통계 결과 출력 함수
    void printGradesAndStatistics() const {
        const string labels[4] = {"중간고사", "기말고사", "과제", "출석"};

        for (const auto& student : studentGrades) {
            cout << "학번: " << student.first << endl;
            for (int i = 0; i < 4; ++i) {
                string category = labels[i];
                cout << category << ": " << student.second.at(category) 
                     << " (Rank: " << studentRank.at(student.first).at(category) << "/" << studentCount
                     << ", Min: " << minScore[i] << ", Max: " << maxScore[i]
                     << ", Mean: " << meanScore[i] << ")" << endl;
            }
            cout << endl;
        }
    }

// 특정 학생의 성적과 통계 출력 함수
    void printStudentStatistics(const string& studentID) const {
        if (studentGrades.find(studentID) == studentGrades.end()) {
            cout << "해당 학번(" << studentID << ")의 학생 정보가 없습니다." << endl;
            return;
        }

        const string labels[4] = {"중간고사", "기말고사", "과제", "출석"};

        cout << "학번: " << studentID << endl;
        for (int i = 0; i < 4; ++i) {
            string category = labels[i];
            cout << category << ": " << studentGrades.at(studentID).at(category) 
                 << " (Rank: " << studentRank.at(studentID).at(category) << "/" << studentCount
                 << ", Min: " << minScore[i] << ", Max: " << maxScore[i]
                 << ", Mean: " << meanScore[i] << ")" << endl;
        }
        cout << endl;
    }
};
