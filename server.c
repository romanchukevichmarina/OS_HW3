#include <stdio.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <stdlib.h>
#include <string.h> 
#include <unistd.h> 
#include <signal.h>

#define MAXPENDING 5 

int servStudSock;
int servTeachSock;

void DieWithError(char *errorMessage){
    close(servStudSock);
    close(servTeachSock);
    perror(errorMessage);
    exit(0);
}

int createSocket(int port, in_addr_t servInAddr)
{
    int servSock;
    struct sockaddr_in servAddr;

    if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = servInAddr;
    servAddr.sin_port = htons(port);

    if (bind(servSock, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
        DieWithError("bind() failed");

    return servSock;
}

int AcceptConnection(int servSock)
{
    int studentSock;
    struct sockaddr_in echoStudentAddr;
    unsigned int studentLen;

    studentLen = sizeof(echoStudentAddr);

    if ((studentSock = accept(servSock, (struct sockaddr *)&echoStudentAddr,
                           &studentLen)) < 0)
        DieWithError("accept() failed");

    printf("Handling %s\n", inet_ntoa(echoStudentAddr.sin_addr));

    return studentSock;
}

void HandleTCPClient(int studentSocket, int teacherSocket)
{
    pid_t pid;
    int recvMsgSize;

    if ((recvMsgSize = recv(studentSocket, &pid, sizeof(int), 0)) < 0)
        DieWithError("recv() failed");

    printf("Student %d is in the queue\n", pid);
    if (send(teacherSocket, &pid, sizeof(int), 0) != sizeof(int))
        DieWithError("send() failed");

    printf("Student %d leaves the exam\n", pid);

    if ((recvMsgSize = recv(teacherSocket, &pid, sizeof(int), 0)) < 0)
        DieWithError("recv() failed");

    if (send(studentSocket, &pid, sizeof(int), 0) != sizeof(int))
        DieWithError("send() failed");

    close(studentSocket);
}

void sigfunc(int sig)
{
    if (sig != SIGINT && sig != SIGTERM)
    {
        return;
    }
    close(servStudSock);
    close(servTeachSock);
    printf("disconnected\n");
    exit(0);
}

int main(int argc, char *argv[])
{
    signal(SIGINT, sigfunc);
    signal(SIGTERM, sigfunc);

    int studentSock;
    int teacherSock;
    struct sockaddr_in echoServAddr;
    struct sockaddr_in echoStudentAddr;
    unsigned int servStudentPort;
    unsigned int servTeacherPort;

    if (argc != 4) 
    {
        fprintf(stderr, "Usage:  %s <Server Address> <Port for Students> <Port for Teacher>\n", argv[0]);
        exit(1);
    }

    in_addr_t servAddr;
    if ((servAddr = inet_addr(argv[1])) < 0)
    {
        DieWithError("Invalid address");
    }

    servStudentPort = atoi(argv[2]);
    servTeacherPort = atoi(argv[3]);

    servStudSock = createSocket(servStudentPort, servAddr);
    servTeachSock = createSocket(servTeacherPort, servAddr);

    if (listen(servTeachSock, MAXPENDING) < 0)
        DieWithError("listen() failed");

    teacherSock = AcceptConnection(servTeachSock);

    if (listen(servStudSock, MAXPENDING) < 0)
        DieWithError("listen() failed");

    for (;;)
    {
        studentSock = AcceptConnection(servStudSock);
        HandleTCPClient(studentSock, teacherSock);
    }
}