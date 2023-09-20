#include <iostream>

using namespace std;

class primegenerator {
private:
    int number_of_primes_found;
    int current_prime;

public:
    primegenerator() {
        number_of_primes_found = 0;
        current_prime = 2;
    }

    int get_number_of_primes_found() {
        return number_of_primes_found;
    }

    int nextprime() {
        int next_prime = current_prime + 1;
        while (!isprime(next_prime)) {
            next_prime++;
        }

        current_prime = next_prime;
        number_of_primes_found++;

        return current_prime;
    }

    bool isprime(int number) {
        for (int i = 2; i * i <= number; i++) {
            if (number % i == 0) {
                return false;
            }
        }

        return true;
    }
};

int main() {
    int nprimes;

    cout << "Enter the number of primes: ";
    cin >> nprimes;

    primegenerator sequence;
    while (sequence.get_number_of_primes_found() < nprimes) {
        int number = sequence.nextprime();
        cout << "Number " << number << " is prime" << '\n';
    }

    return 0;
}