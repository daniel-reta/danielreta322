// Daniel Reta, dr37277, danreta
#include <iostream>
#include <vector>

using namespace std;

// split a vector into two vectors
void split(const vector<int>& inputVector, vector<int>& oddVector, vector<int>& evenVector) {
    for (int num : inputVector) {
        if (num % 2 == 0) {
            evenVector.push_back(num); // add it to the even vector
        }
        else {
            oddVector.push_back(num); // add it to the odd vector
        }
    }
}

int main() {
    // create the vector
    vector<int> inputVector;
    inputVector.push_back(5);
    inputVector.push_back(6);
    inputVector.push_back(2);
    inputVector.push_back(4);
    inputVector.push_back(5);

    vector<int> oddVector;
    vector<int> evenVector;

    // call the function
    split(inputVector, oddVector, evenVector);

    // print the vector with even numbers
    cout << "even numbers: ";
    for (int num : evenVector) {
        cout << num << " ";
    }
    cout << endl;

    // print the vector with odd numbers
    cout << "odd numbers: ";
    for (int num : oddVector) {
        cout << num << " ";
    }
    cout << endl;

    return 0;
}