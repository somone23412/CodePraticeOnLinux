#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>

#include <string.h>
#include <ctype.h>

#define SERVER_IP "192.168.2.217"
#define SERVER_PORT 6666
#define OPEN_MAX 65536

//主要是 epoll 的用法，为了逻辑清楚，【去掉了错误处理部分】，客户端用 nc 模拟或直接用之前的客户端即可
int main() {
    int lfd, epfd, nready, connfd, readn;
    struct sockaddr_in serveraddr, clientaddr;
    socklen_t client_len;
    struct epoll_event tmp_evt, evts[OPEN_MAX]; //事件节点和节点数组
    char str_buf[BUFSIZ], read_buf[BUFSIZ];
    
    lfd = socket(AF_INET, SOCK_STREAM, 0); //【建立连接】1. socket

    int opt = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); //端口复用

    serveraddr.sin_family = AF_INET; //【建立连接】2. bind
    serveraddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &serveraddr.sin_addr);
    if (-1 == bind(lfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr))) {
        perror("bind");
        exit(1);
    }

    listen(lfd, 1000); //【建立连接】3. listen

    epfd = epoll_create(OPEN_MAX); //【epoll】1. epoll
    tmp_evt.data.fd = lfd;
    tmp_evt.events = EPOLLIN | EPOLLET; //【非阻塞IO】1. 边沿触发
    epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &tmp_evt); //【epoll】2. 把 listenfd 和对应的 evt 放进 epoll 红黑树

    while(1) {
        nready = epoll_wait(epfd, evts, OPEN_MAX, -1); //【epoll】3. 循环阻塞监听事件
        for (int i = 0; i < nready; i++) {
            if (evts[i].events == EPOLLIN){ //监听可读事件
                if(evts[i].data.fd == lfd) { //lfd 可读，说明是新连接
                    client_len = INET_ADDRSTRLEN; //客户端地址长度是传入传出参数
                    int client_fd = accept(lfd, (struct sockaddr*)&clientaddr, &client_len); //【建立连接】4. accept
                    fcntl(client_fd, F_SETFL, fcntl(client_fd, F_GETFL) | O_NONBLOCK); //【非阻塞 IO】2. clientfd 设为非阻塞
                    printf("Connection fd[%d] from %s:%d accepted.\n", 
                        tmp_evt.data.fd,
                        inet_ntop(AF_INET, &clientaddr.sin_addr, str_buf, client_len),
                        ntohs(clientaddr.sin_port)
                    );
                    tmp_evt.data.fd = client_fd;
                    epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &tmp_evt); //【epoll】4. 把客户端加到树
                } else { //处理客户端发来的数据
                    int client_fd = evts[i].data.fd;
                    while((readn = read(client_fd, read_buf, BUFSIZ)) > 0){ //【非阻塞IO】3. 轮询读到空
                        for (int i = 0; i < readn; i++){
                            read_buf[i] = toupper(read_buf[i]);
                        }
                        write(client_fd, read_buf, readn);
                    }
                    if(readn == 0) { //读到文件尾，说明客户端断开（连接时就算读空也不会读到 0），也有可能有其它条件，这里简化
                        printf("client fd[%d] disconnected.\n", client_fd);
                        close(client_fd);
                        epoll_ctl(epfd, EPOLL_CTL_DEL, client_fd, NULL); //【epoll】5. 从树移除客户端
                        continue;
                    }
                }
            }
            if (evts[i].events == EPOLLOUT){}; //监听可写事件
            if (evts[i].events == EPOLLERR){}; //监听错误事件
        }
    }

    return 0;
}

// quyan@DGX-1-1:~/linuxc/4net$ ./epoll_server
// Connection fd[5] from 192.168.2.217:59568 accepted.
// Connection fd[6] from 192.168.2.217:59570 accepted.
// client fd[5] disconnected.
// client fd[6] disconnected.