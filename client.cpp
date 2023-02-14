#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);

int main(int argc, char *argv[]) {

	// 서버, 클라이언트 소켓 파일 디스크립터 변수 선언
	int sock;

	// sockaddr_in 구조체 변수 선언
	struct sockaddr_in server_addr;
	char message[30];
	int str_len;

	if (argc != 3) {
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	// 소켓 생성
	// 소켓을 생성하는 순간에는 서버 소켓과 클라이언트 소켓으로 나뉘지 않는다                                                                                        .
	// bind listen함수의 호출이 이어지면 서버 소켓, connect 함수의 호출로 이어지면 클라이언트 소켓
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error");

	// 서버 정보 초기화 및 정보 할당
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;						// AF_INET - IPv4를 쓴다는 의미
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);		// 연결할 서버 IP 설정
	server_addr.sin_port = htons(atoi(argv[2]));			// Port 설정

	// 서버에 connect 요청을 하고, 그에 따른 결과값을 저장
	// -1인 경우 연결 오류 발생
	int connect_result = connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if (connect_result == -1)
		error_handling("connect() error");

	//서버로 데이터 보냄
	char data[30] = "test218";
	write(sock, data, sizeof(data));

	// 서버로부터 데이터를 읽고, 데이터 내용과 길이를 저장함
	char data2[30];
	str_len = read(sock, data2, sizeof(data2) - 1);
	if (str_len == -1)
		error_handling("read() error");

	// 메시지를 출력함
	printf("Message from server:%s\n", data2);

	// 소켓 연결 종료
	close(sock);

	return 0;

}

void error_handling(char *message) {
	// 에러 발생 시 에러 메시지를 출력하고 프로그램을 종료함
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}