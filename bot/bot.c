#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main() {
    int x, y, time;

    // Seed the random number generator with the current time
    // srand(time(NULL));

    // Generate random numbers in the specified ranges
    x = rand() % 40;  // Generates a random number between 0 and 39
    y = rand() % 16;  // Generates a random number between 0 and 15

    // time has no limit, so it can be set to any value
    time = 123456789;  // Example value

    // Print the generated numbers
    printf("x = %d\n", x);
    printf("y = %d\n", y);
    printf("time = %d\n", time);

    return 0;
}