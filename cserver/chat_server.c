#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define BUF_SIZE 1024
void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
	WSADATA	wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;

	char message[BUF_SIZE];
	int strLen, i;
	int clntAdrSize;

	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup ERROR");

	hServSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (hServSock == INVALID_SOCKET)
		ErrorHandling("socket ERROR");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htons(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("bind ERROR");

	if (listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen ERROR");

	clntAdrSize = sizeof(clntAddr);
	hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &clntAdrSize);

	if (hClntSock == INVALID_SOCKET)
		ErrorHandling("accept ERROR");
	else
	{
		char* str = inet_ntoa(clntAddr.sin_addr);
		printf("Connected client %d\n", hClntSock);
		printf("%s 님 접속\n", str);
	}

	while ((strLen = recv(hClntSock, message, BUF_SIZE - 1, 0)) != 0)
	{
		message[strLen] = 0;
		printf("상대방 메시지 : %s", message);
		printf("내 메시지(종료 Q) : ");
		fgets(message, BUF_SIZE, stdin);

		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;

		send(hClntSock, message, strLen, 0);
	}
	printf("Disconnected client %d\n", hClntSock);

	closesocket(hClntSock);
	closesocket(hServSock);
	WSACleanup();
	return 0;
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
