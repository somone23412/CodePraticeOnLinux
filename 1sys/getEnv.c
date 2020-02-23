#include<stdlib.h>
#include<stdio.h>
#include<string.h>

int main() {
    printf("current LD_LIBRARY_PATH=%s\n", getenv("LD_LIBRARY_PATH"));
    setenv("ABD", "hello", 1);
    printf("ABD=%s\n", getenv("ABD"));
    setenv("ABD", "bufugai", 0);
    printf("ABD=%s\n", getenv("ABD"));
    setenv("ABD", "fugai", 1);
    printf("ABD=%s\n", getenv("ABD"));
    
    char s[1024] = {0};
    sprintf(s, "%s:append", getenv("ABD"));
    setenv("ABD", s, 1);
    printf("ABD=%s\n", getenv("ABD"));

    unsetenv("ABD");
    printf("ABD=%s\n", getenv("ABD"));
    
    return 0;
}