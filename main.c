#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gmp.h>

// Multiplies two 2x2 matrices a and b, storing the result in result.
// All matrices are 2x2 arrays of mpz_t.
void matrixMultiply(mpz_t a[2][2], mpz_t b[2][2], mpz_t result[2][2]) {
    mpz_t temp[2][2];
    // Initialize temporary matrix elements
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < 2; j++){
            mpz_init(temp[i][j]);
        }
    }

    // temp = a * b
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < 2; j++){
            mpz_set_ui(temp[i][j], 0);
            for (int k = 0; k < 2; k++){
                mpz_t mult;
                mpz_init(mult);
                mpz_mul(mult, a[i][k], b[k][j]);
                mpz_add(temp[i][j], temp[i][j], mult);
                mpz_clear(mult);
            }
        }
    }

    // Copy temp into result
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < 2; j++){
            mpz_set(result[i][j], temp[i][j]);
            mpz_clear(temp[i][j]);
        }
    }
}

// Computes Fibonacci(n) using matrix exponentiation, storing the result in 'result'.
// 'n' is an mpz_t so that it can be arbitrarily large.
void fibonacci_matrix(mpz_t result, mpz_t n) {
    // For n <= 1, Fibonacci(n) = n.
    if (mpz_cmp_ui(n, 1) <= 0) {
        mpz_set(result, n);
        return;
    }

    // Initialize base matrix = [[1, 1], [1, 0]]
    mpz_t base[2][2];
    // Initialize identity matrix for result: [[1, 0], [0, 1]]
    mpz_t res[2][2];
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < 2; j++){
            mpz_init(base[i][j]);
            mpz_init(res[i][j]);
        }
    }
    mpz_set_ui(base[0][0], 1);
    mpz_set_ui(base[0][1], 1);
    mpz_set_ui(base[1][0], 1);
    mpz_set_ui(base[1][1], 0);

    mpz_set_ui(res[0][0], 1);
    mpz_set_ui(res[0][1], 0);
    mpz_set_ui(res[1][0], 0);
    mpz_set_ui(res[1][1], 1);

    // Create a copy of n for the exponent
    mpz_t exp;
    mpz_init_set(exp, n);

    // Temporary matrix for multiplication
    mpz_t temp[2][2];
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < 2; j++){
            mpz_init(temp[i][j]);
        }
    }

    // Exponentiation loop (logarithmic time)
    while (mpz_cmp_ui(exp, 0) > 0) {
        if (mpz_odd_p(exp)) {
            matrixMultiply(res, base, temp);
            // res = temp
            for (int i = 0; i < 2; i++){
                for (int j = 0; j < 2; j++){
                    mpz_set(res[i][j], temp[i][j]);
                }
            }
        }
        matrixMultiply(base, base, temp);
        // base = temp
        for (int i = 0; i < 2; i++){
            for (int j = 0; j < 2; j++){
                mpz_set(base[i][j], temp[i][j]);
            }
        }
        // exp /= 2  (shift right by 1 bit)
        mpz_fdiv_q_2exp(exp, exp, 1);
    }

    // Fibonacci(n) is stored in res[0][1]
    mpz_set(result, res[0][1]);

    // Clear all mpz_t variables
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < 2; j++){
            mpz_clear(base[i][j]);
            mpz_clear(res[i][j]);
            mpz_clear(temp[i][j]);
        }
    }
    mpz_clear(exp);
}

int main() {
    // Define some values of n as strings.
    // so here we use smaller examples. You can modify the input as needed.)
    const char *test_values[] = { "40", "1000000" };
    const int num_tests = sizeof(test_values) / sizeof(test_values[0]);

    for (int i = 0; i < num_tests; i++){
        mpz_t n, fib;
        mpz_init(n);
        mpz_init(fib);
        // Set n from the test string in base 10.
        if (mpz_set_str(n, test_values[i], 10) != 0) {
            fprintf(stderr, "Error converting string to mpz_t\n");
            exit(EXIT_FAILURE);
        }

        clock_t start = clock();
        fibonacci_matrix(fib, n);
        clock_t end = clock();
        double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

        gmp_printf("Fibonacci(%Zd) (matrix): %Zd\n", n, fib);
        printf("Time taken: %.15lf seconds\n\n", cpu_time_used);

        mpz_clear(n);
        mpz_clear(fib);
    }
    return 0;
}
