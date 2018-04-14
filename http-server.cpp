#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "web.h"

void error_handling(string message);
void read_childproc(int sig);
void exit_proc(int sig);

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;

    pid_t pid;
    struct sigaction act;
    struct sigaction sigIntHandler;
    socklen_t adr_sz;
    if(argc != 2) {
        std::cout << "Usage : " << argv[0] << " <port>\n";
        exit(1);
    }

    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);

    sigIntHandler.sa_handler = exit_proc;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1)
        error_handling("bind() error");
    if(listen(serv_sock, 5)==-1)
        error_handling("listen() error");

    std::cout << "myserver is running... \n"
            << "application starting on http://localhost:" << argv[1]<< "\nlistening on tcp://"
              << inet_ntoa(serv_adr.sin_addr) << ":" << argv[1] << "\n"
            << "Use Ctrl-C to stop\n";

    while(1)
    {
        adr_sz=sizeof(clnt_adr);
        clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
        if(clnt_sock == -1)
            continue;
        else
            std::cout << "new client connected...\n";
        pid = fork();
        if(pid == -1)
        {
            close(clnt_sock);
            continue;
        }
        if(pid == 0)
        {
            close(serv_sock);

            Web web(clnt_sock); //web server

            close(clnt_sock);
            std::cout << "client disconnected...\n";
            return 0;
        }
        else
            close(clnt_sock);
    }
    close(serv_sock);
    return 0;
}

void read_childproc(int sig)
{
    pid_t pid;
    int status;
    pid = waitpid(-1, &status, WNOHANG);
    std::cout << "removed proc id: "<< pid << "\n";
}
void exit_proc(int sig){
    std::cout << "\nGoodbye!\nExiting\n";
    exit(1);
}

void error_handling(string message)
{
    std::cerr << message << "\n";
    exit(1);
}
