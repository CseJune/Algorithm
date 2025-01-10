#include <string>
#include <vector>

using namespace std;

bool solution(int x) {
    int n = x; // 원래 숫자 저장
    int sum = 0; // 자릿수 더하는거 저장
    while (x > 0) // x가 0보다 커지면 루프 탈출
    {
        sum += x % 10; // 자릿수 1의자리부터 더하기
        x /= 10; // 맨마지막 자릿수 삭제하기
    }
    return n % sum == 0;
}