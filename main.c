#include <stdio.h>
#include <time.h>
#include <gmp.h>

// Computes (F(n), F(n+1)) using fast doubling.
// f and f1 are output parameters, and n is an unsigned long.
void fib_fast_doubling(mpz_t f, mpz_t f1, unsigned long n) {
    if (n == 0) {
        mpz_set_ui(f, 0);  // F(0) = 0
        mpz_set_ui(f1, 1); // F(1) = 1
        return;
    }

    // Temporary variables for recursion
    mpz_t a, b;
    mpz_inits(a, b, NULL);

    // Recursively compute F(n/2) and F(n/2 + 1)
    fib_fast_doubling(a, b, n / 2);

    // Now, compute:
    // c = a * (2*b - a) = F(2k)
    // d = a^2 + b^2     = F(2k + 1)
    mpz_t c, d, temp;
    mpz_inits(c, d, temp, NULL);

    // temp = 2 * b - a
    mpz_mul_ui(temp, b, 2);  // temp = 2*b
    mpz_sub(temp, temp, a);  // temp = 2*b - a
    mpz_mul(c, a, temp);     // c = a * (2*b - a)

    // d = a^2 + b^2
    mpz_mul(d, a, a);        // d = a^2
    mpz_mul(temp, b, b);     // temp = b^2
    mpz_add(d, d, temp);     // d = a^2 + b^2

    // Depending on the parity of n, set the final values.
    if ((n & 1) == 0) {      // n is even
        mpz_set(f, c);       // F(n) = c
        mpz_set(f1, d);      // F(n+1) = d
    } else {                 // n is odd
        mpz_set(f, d);       // F(n) = d
        mpz_add(f1, c, d);   // F(n+1) = c + d
    }

    // Clear temporary variables.
    mpz_clears(a, b, c, d, temp, NULL);
}

int main(void) {

    printf("give me the sequence number :");
    unsigned long n ; // Example exponent
    scanf("%lu",&n);
    mpz_t F, F1;
    mpz_inits(F, F1, NULL);
    clock_t start = clock();
    fib_fast_doubling(F, F1, n);
    clock_t end = clock();
    double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    // Print only the number of digits to avoid flooding your terminal.
    size_t num_digits = mpz_sizeinbase(F, 10);
    gmp_printf("Fibonacci(%lu) \nresult: %Zd", n, F1); // using F1 as temporary storage for digit count
    printf("\nTime taken: %.15lf seconds", cpu_time_used);

    mpz_clears(F, F1, NULL);
    return 0;
}
