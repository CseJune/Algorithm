#include <string>
#include <vector>
#include <iostream>

using namespace std;

vector<long long> solution(int x, int n) {
    vector<long long> answer(n);
    for(int i = 0; i < n; ++i){
        answer[i] = static_cast<long long>(x) * (i + 1);
    }
    return answer ;
}