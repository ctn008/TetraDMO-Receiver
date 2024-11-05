// vector_python.cpp
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

using namespace std;

extern "C" int foo(vector<int>* v, const char* FILE_NAME){
    string line;
    int line_cnt = 0;
    cout << FILE_NAME;
    ifstream myfile(FILE_NAME);
    while (getline(myfile, line)) {
    	v->push_back(1);
    	cout << line;
    	line_cnt ++;
    	printf("Line counted: %d \n", line_cnt);
    }
    return line_cnt;
}

extern "C" {
    vector<int>* new_vector(){
        return new vector<int>;
    }
    void delete_vector(vector<int>* v){
        cout << "destructor called in C++ for " << v << endl;
        delete v;
    }
    int vector_size(vector<int>* v){
        return v->size();
    }
    int vector_get(vector<int>* v, int i){
        return v->at(i);
    }
    void vector_push_back(vector<int>* v, int i){
        v->push_back(i);
    }
}
