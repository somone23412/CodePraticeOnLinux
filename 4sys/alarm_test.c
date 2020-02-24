#include <stdio.h>
#include <unistd.h>

int main() {
    alarm(5);
    sleep(2);
    printf("last alarm remain %us.\n", alarm(5));
    sleep(10);
    return 0;
}

// [output]
// last alarm remain 3s.
// Alarm clock