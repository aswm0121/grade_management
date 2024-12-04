#include <iostream>
using namespace std;
#include "file.cpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <iomanip>
#include <numeric> // for std::accumulate

using namespace std;


class Course2 {
private:
    string professor;
    double midWeight, finalWeight, taskWeight, attendanceWeight;
    double aRatio, bRatio, cRatio;
    vector<vector<string>> studentData;
    map<string, char> gradeMap; // 학번별 성적 저장
    map<string, double> totalScores; // 학번별 총점 저장

public:
vector<vector<string>> raw_data;
    // 생성자
    Course2(const vector<vector<string>>& data) {
        raw_data = data;
        if (data.size() < 3) {
            cerr << "CSV 데이터가 충분하지 않습니다!" << endl;
            // exit(1);
        }

        const auto& courseInfo = data[1]; // 1이어야 - 첫번째 행(0)은 컬럼이기 때문에
        if (courseInfo.size() < 10) {
            cerr << "강의 정보가 누락되었습니다!" << endl;
            exit(1);
        }

        try {
            professor = courseInfo[2];
            midWeight = stod(courseInfo[3]);
            finalWeight = stod(courseInfo[4]);
            taskWeight = stod(courseInfo[5]);
            attendanceWeight = stod(courseInfo[6]);
            aRatio = stod(courseInfo[7]);
            bRatio = stod(courseInfo[8]);
            cRatio = stod(courseInfo[9]);
        } catch (const invalid_argument& e) {
            cerr << "강의 정보 값이 유효하지 않습니다: ";
            for (const auto& val : courseInfo) {
                cerr << "[" << val << "] ";
            }
            cerr << endl;
            exit(1);
        }

        // 학생 데이터 초기화 (3번째 행은 무시)
        for (size_t i = 3; i < data.size(); ++i) {
            studentData.push_back(data[i]);
        }

        // // studentData 출력
        // cout << "학생 데이터 초기화 결과:" << endl;
        // for (const auto& row : studentData) {
        //     cout << row.size() << endl;
        //     for (const auto& col : row) {
        //         cout << col << " ";
        //     }
        //     cout << endl;
        // }
    }

    // 학생 성적 계산 함수
    void calculateGrades() {
        vector<pair<double, string>> scores; // 총점과 학번을 저장

        // 학생별 총점 계산
        for (const auto& student : studentData) {
            if (student.size() < 6) {
                cerr << "학생 데이터가 올바르지 않습니다!" <<student.size() << endl;
                continue;
            }

            string id = student[1];
            double mid = stod(student[2]);
            double finalScore = stod(student[3]);
            double task = stod(student[4]);
            double attendance = stod(student[5]);

            // 최종 점수 계산
            double totalScore = mid * midWeight + finalScore * finalWeight +
                                task * taskWeight + attendance * attendanceWeight;

            scores.emplace_back(totalScore, id);
            totalScores[id] = totalScore; // 학번별 총점 저장
        }

        // 점수 내림차순 정렬
        sort(scores.begin(), scores.end(), [](const pair<double, string>& a, const pair<double, string>& b) {
            return a.first > b.first;
        });

        // 학생 수
        int totalStudents = scores.size();

        // 등급 기준 계산
        int numA = static_cast<int>(totalStudents * aRatio);
        // int numB = static_cast<int>(totalStudents * bRatio) - numA;
        int numB = static_cast<int>(totalStudents * bRatio);
        int numC = static_cast<int>(totalStudents * cRatio);

        // 성적 할당
        for (int i = 0; i < totalStudents; ++i) {
            if (i < numA) {
                gradeMap[scores[i].second] = 'A';
            } else if (i < numB) {
                gradeMap[scores[i].second] = 'B';
            } else if (i < numC) {
                gradeMap[scores[i].second] = 'C';
            } else {
                gradeMap[scores[i].second] = 'D';
            }
        }

        // map의 모든 내용을 출력
    // for (const auto& pair : gradeMap) {
    //     std::cout << pair.first << ": " << pair.second << std::endl;
    // }
    }

    // 학번으로 성적 조회 함수
    void getGradeByStudentId(const string& studentId) {
        auto scoreIt = totalScores.find(studentId);
        auto gradeIt = gradeMap.find(studentId);

        if (scoreIt != totalScores.end() && gradeIt != gradeMap.end()) {
            cout << "학번: " << studentId << ", 총점: " << fixed << setprecision(2)
                 << scoreIt->second << ", 성적: " << gradeIt->second << " (가중치 - 중간: " << midWeight
                 << ", 기말: " << finalWeight
                 << ", 과제: " << taskWeight
                 << ", 출석: " << attendanceWeight << ")" << endl;
        } else {
            cout << "해당 학번(" << studentId << ")에 대한 정보를 찾을 수 없습니다." << endl;
        }
    }



    // 통계 계산 함수
    void viewStatistics(const string& studentId) {
        // 통계 계산 대상 컬럼
        vector<string> headers = {"중간고사 점수", "기말고사 점수", "과제 점수", "출석 점수"};
        vector<int> columnIndexes = {2, 3, 4, 5}; // 중간, 기말, 과제, 출석 점수 인덱스

        // 학생 데이터에서 해당 학번의 학생을 찾기
        auto studentIt = find_if(studentData.begin(), studentData.end(),
                                 [&studentId](const vector<string>& student) {
                                     return student[1] == studentId;
                                 });

        if (studentIt == studentData.end()) {
            cerr << "해당 학번(" << studentId << ")에 해당하는 학생을 찾을 수 없습니다." << endl;
            return;
        }

        // 학생 데이터
        const vector<string>& student = *studentIt;
        // cout << "학번 : " << studentId << endl;

        // 각 점수 컬럼에 대해 통계 계산
        for (size_t i = 0; i < columnIndexes.size(); ++i) {
            int colIndex = columnIndexes[i];
            string header = headers[i];

            vector<int> scores;
            for (const auto& s : studentData) {
                scores.push_back(stoi(s[colIndex]));
            }

            // 통계 계산
            int minScore = *min_element(scores.begin(), scores.end());
            int maxScore = *max_element(scores.begin(), scores.end());
            double meanScore = accumulate(scores.begin(), scores.end(), 0.0) / scores.size();

            // 학생의 점수 및 랭크 계산
            int studentScore = stoi(student[colIndex]);
            int rank = count_if(scores.begin(), scores.end(), [studentScore](int score) {
                return score > studentScore;
            }) + 1;

            // 출력
            cout << header << " : " << studentScore
                 << " (Rank: " << rank << "/" << scores.size()
                 << ", Min: " << minScore
                 << ", Max: " << maxScore
                 << ", Mean: " << fixed << setprecision(1) << meanScore << ")" << endl;
        }
    }
};

// int main() {
//     // 파일 로드
//     File file("./grade_csv/경영학원론_3.csv");
//     file.loadCSV();
//     vector<vector<string>> csvData = file.getCSV();

//     // Course2 객체 생성 및 성적 계산
//     Course2 course(csvData);
//     course.calculateGrades();

//     // 사용자 입력으로 학번 조회
//     string studentId;
//     cout << "학번을 입력하세요: ";
//     cin >> studentId;

//     course.getGradeByStudentId(studentId);
//     course.viewStatistics(studentId);


//     return 0;
// }
