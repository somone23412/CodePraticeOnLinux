#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>

//没加错误处理
int main (int argc, char* argv[]) {
    int cfd;
    struct sockaddr_in server_addr;
    char write_buf[BUFSIZ];
    char read_buf[BUFSIZ];
    int read_n, write_n;

    if(argc < 3) {
        printf("client ip port.\n");
    }
    
    //1. socket
    cfd = socket(AF_INET, SOCK_STREAM, 0);

    //客户端可以不 bind
    inet_pton(AF_INET, argv[1], &server_addr.sin_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));

    //2. connect
    printf("connecting to server %s:%s", argv[1], argv[2]);
    
    if(0 == connect(cfd, (struct sockaddr *)&server_addr, sizeof(server_addr))) {
        printf("\nserver connected!\n");
    }else{
        perror("connect");
        exit(1);
    }
    while(write_n = read(STDIN_FILENO, write_buf, sizeof(write_buf))){ //从 stdin 把要写的内容读进 writebuff，也可用 fgets(buf, sizeof(buf), stdin);
        write(cfd, write_buf, write_n);
        read_n = read(cfd, read_buf, sizeof(read_buf));
        write(STDOUT_FILENO, read_buf, read_n);
    }
    close(cfd);

    return 0;
}