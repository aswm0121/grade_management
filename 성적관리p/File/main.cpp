
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "file.cpp"

using namespace std;



int main() {
    // '경영학원론_1.csv' 파일을 불러와서 출력 및 데이터 반환
    string class_name;
    string c_num;
    string studentID;
    cout << "점수를 확인할 강의를 입력하시오: ";
    cin >> class_name;
    cout << "분반을 입력하시오: ";
    cin >> c_num;
    cout << "학번을 입력하시오: ";
    cin >> studentID;
    
    
   
    File file("./grade_csv/" + class_name + "_" + c_num + ".csv");
    file.loadCSV();  // 파일 불러오기
    file.getCSV(); // return csv data(vector)
    file.viewFile();








}





