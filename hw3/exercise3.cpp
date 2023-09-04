// Daniel Reta, dr37277, danreta

#include <iostream>

using namespace std;

int main() {
    int max = 0;

    // loop for all starting numbers
    for (int n = 1; n <= 1000; n++) {
        int length = 1;
        int current = n;

        // verify starting is not 1 and calculate next integer
        while (current != 1) {
            if (current % 2 == 0) {
                current = current / 2;
            }
            else {
                current = 3 * current + 1;
            }

            // keep track of current length
            length++;
        }

        // check if current length is longer than previous maximum
        if (length > max) {
            max = length;
            int start_number = n;
            cout << start_number << endl;
        }
    }

    return 0;
}