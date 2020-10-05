#include "Prime.h"
#include <algorithm>
#include <cmath>
#include <iostream>

using namespace std;

void buildPrimes(int n)
{
    if (prime.size() == 0) {
        prime.push_back(2);
        prime.push_back(3);
    }

    for (int i = prime.back() + 2; n > 0; i += 2) {

        bool isprime = true;
        for (int j = 0; isprime && j < prime.size() && prime[j] <= sqrt(i);
             j++) {
            if (i % prime[j] == 0)
                isprime = false;
        }

        if (isprime)
            prime.push_back(i), n--;
    }
}

int findPrimeMoreThan(double d)
{
    auto up = upper_bound(prime.begin(), prime.end(), d);

    // check up is exist and more than d
    // if not found then build prime table
    while (up == prime.end()) {
        buildPrimes(1);
        up = upper_bound(prime.begin(), prime.end(), d);
    }

    return *up;
}
