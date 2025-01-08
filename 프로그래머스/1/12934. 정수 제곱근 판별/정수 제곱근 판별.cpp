#include <string>
#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

long long solution(long long n) {
    long long x = static_cast<long long>(sqrt(n)); // n의 제곱근 = x
    if (x * x == n){
        return (x+1) * (x+1);
    }
    else return -1;
}