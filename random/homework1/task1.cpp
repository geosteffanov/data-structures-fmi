#include <iostream>
#include <queue>
using namespace std;
int CrownSequence(int N, int p) {
 queue<int> hq;
 int index = -1;
 hq.push(N);
    while(true) {
        int crr = hq.front();
        hq.pop();
        index++;
        if (index == p)
            return crr;
        hq.push(crr + 1);
        hq.push(2 * crr);
    }
 }
int main() {
   for (int i = 0; i < 20; i++) {
    cout << CrownSequence(6, i) << endl;
   }

}
