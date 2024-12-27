#include <string>
#include <vector>

using namespace std;

string solution(int num) {
    string answer = "0";
    if (num % 2 == 0){
        answer = "Even";
    }
    else {
        answer = "Odd";
    }
    return answer;
}