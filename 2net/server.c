#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <ctype.h>

#define SERV_IP "192.168.2.217"
#define SERV_PORT 6666

int main() {
    int sfd, cfd;
    struct sockaddr_in server_addr, client_addr; //ipv4 address
    socklen_t client_addr_len;
    char client_str_ip[INET_ADDRSTRLEN];
    char read_buf[BUFSIZ]; //#define BUFSIZ _IO_BUFSIZ
    int read_n;

    //1. socket
    sfd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, SERV_IP, &server_addr.sin_addr); //"AF_INET...The address is converted to a struct in_addr and copied to dst, which must be sizeof(struct in_addr) (4) bytes (32 bits) long."
    //也可以用 server_addr.sin_addr.s_addr = htonl(INADDR_ANY) 宏指代一个本地有效的 IP

    //2. bind
    bind(sfd, (struct sockaddr * )&server_addr, sizeof(server_addr));

    //3. listen
    listen(sfd, 128);

    client_addr_len = sizeof(client_addr); //client_addr_len 为accept的传入传出参数，所以既要提前赋值，又要之后传入

    while(1){ //循环和客户端建立连接

        //4. accept
        cfd = accept(sfd, (struct sockaddr *)&client_addr, &client_addr_len); //阻塞等待
        inet_ntop(client_addr.sin_family, &client_addr.sin_addr, client_str_ip, INET6_ADDRSTRLEN); //把客户端地址转成字符串
        printf("connect accepted! From client ip = %s, port = %d.\n", client_str_ip, ntohs(client_addr.sin_port));

        while(read_n = read(cfd, read_buf, sizeof(read_buf))){ //循环读 buf
            write(STDOUT_FILENO, read_buf, read_n); //服务器端显示读到的内容
            for (int i = 0; i < read_n; i++){
                read_buf[i] = toupper(read_buf[i]);
            }
            write(cfd, read_buf, read_n);
        }

        printf("client exit.\n");
        close(cfd);
    }
    close(sfd);


    return 0;
}
// [client]
// quyan@DGX-1-1:~/linuxc/2net$ nc 127.0.0.1 6666
// ^C
// quyan@DGX-1-1:~/linuxc/2net$ nc 127.0.0.1 6666
// hello
// HELLO
// ^C
// quyan@DGX-1-1:~/linuxc/2net$ nc 127.0.0.1 6666
// hello world
// HELLO WORLD
// ^C

// [server]
// quyan@DGX-1-1:~/linuxc/2net$ ./server
// connect accepted! From client ip = 127.0.0.1, port = 38502.
// client exit.
// connect accepted! From client ip = 127.0.0.1, port = 38504.
// hello
// client exit.
// connect accepted! From client ip = 127.0.0.1, port = 38510.
// hello world
// client exit.
// ^C