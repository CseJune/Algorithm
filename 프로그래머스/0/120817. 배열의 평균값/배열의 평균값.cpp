#include <vector>
using namespace std;

double solution(vector<int> numbers) {
    double sum = 0;
    for (int num : numbers) {
        sum += num;
    }
    return sum / numbers.size();
}