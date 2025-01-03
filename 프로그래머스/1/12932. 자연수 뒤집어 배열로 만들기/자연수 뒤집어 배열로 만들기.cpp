#include <string>
#include <vector>
#include <iostream>

using namespace std;

vector<int> solution(long long n) {
    vector<int> answer;
    string str = to_string(n);
    for(auto it = str.rbegin(); it != str.rend(); ++it){
        answer.push_back(*it - '0');
    }
    return answer;
}