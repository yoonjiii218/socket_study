#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);

int main(int argc, char *argv[]) {

	// 서버, 클라이언트 소켓 파일 디스크립터 변수 선언
	int server_sock;
	int client_sock;

	// sockaddr_in 구조체 변수 선언
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;

	// accept 하기위한 클라이언트 구조체 크기 밑작업 및 함수호출
	socklen_t client_addr_size;

	char message[] = "Hello World!";

	if (argc != 2) {
		printf("Usage:%s <port>\n", argv[0]);
		exit(1);
	}

	// socket 함수호출을 통해서 소켓을 생성
	server_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (server_sock == -1)
		error_handling("socket() error");

	// 서버를 열 것이므로 관련 정보를 적용함
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;					// AF_INET - IPv4를 쓴다는 의미
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);	// INADDR_ANY는 자동으로 이 컴퓨터에 존재하는 랜카드 중 사용가능한 랜카드의 IP주소를 사용하라는 의미
	server_addr.sin_port = htons(atoi(argv[1]));		// Port 설정

	// bind 함수호출을 통해서 IP 주소와 Port 번호를 할당
	int bind_result = bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if (bind_result == -1)
		error_handling("bind() error");
	
	// listen()으로 서버소켓으로 오는 클라이언트 요청 대기
	int listen_result = listen(server_sock, 5);
	if (listen_result == -1)
		error_handling("listen() error");

	// 연결 요청의 수락을 위한 accept 함수를 호출하고 있음
	// 연결 요청이 없는 상태에서 이 함수가 호출되면, 연결요청이 있을 때까지 함수는 반환하지 않음
	client_addr_size = sizeof(client_addr);
	client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_addr_size);
	if (client_sock == -1)
		error_handling("accept() error");

	// write 함수는 데이터를 전송하는 기능의 함수인데, 이 문장이 실행됬다는 것은 연결요청이 있었다는 뜻
	write(client_sock, message, sizeof(message));

	// 클라이언트로부터 데이터를 읽고, 데이터 내용과 길이를 저장함
	message[0] = '\0';
	int str_len = read(client_sock, message, sizeof(message) - 1);
	if (str_len == -1)
		error_handling("read() error");

	// 메시지를 출력함
	printf("Message from client:%s\n", message);

	// 소켓 닫음
	close(client_sock);
	close(server_sock);

	return 0;
}

void error_handling(char *message) {
	// 에러 발생 시 에러 메시지를 출력하고 프로그램을 종료함
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
