#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

int main() {
    kill(getpid(), SIGKILL);
    return 0;
}