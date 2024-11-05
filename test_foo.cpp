#include <vector>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

extern "C" int foo(vector<int>* v, const char* FILE_NAME){
    string line;
    int line_cnt = 0;
    cout << FILE_NAME;
    ifstream rfile(FILE_NAME);
    while (getline(rfile, line)){
        v->push_back(1);
        cout << line << endl;
        line_cnt ++;
        printf("Line count: %d \n", line_cnt);
    } 
    return line_cnt;
}

int main() {
    auto t = new vector<int> ;
    auto line_cnt = 0;
    cout << t->size() << endl;
    auto s = "/home/ctn008/tetraDMO-Receiver/file.txt";
    cout << "calling foo function" << endl;
    line_cnt = foo(t, s);
    cout << endl << "Line count in foo:" << line_cnt << endl;
    cout << endl << t->size();
    return 0;
}
