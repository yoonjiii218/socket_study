#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);

int main(int argc, char *argv[]) {

	// ����, Ŭ���̾�Ʈ ���� ���� ��ũ���� ���� ����
	int server_sock;
	int client_sock;

	// sockaddr_in ����ü ���� ����
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;

	// accept �ϱ����� Ŭ���̾�Ʈ ����ü ũ�� ���۾� �� �Լ�ȣ��
	socklen_t client_addr_size;

	char message[] = "Hello World!";

	if (argc != 2) { //�� ��Ʈ�� �Է¹���
		printf("Usage:%s <port>\n", argv[0]);
		exit(1);
	}

	// socket �Լ�ȣ���� ���ؼ� ������ ����
	server_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (server_sock == -1)
		error_handling("socket() error");

	// ������ �� ���̹Ƿ� ���� ������ ������
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;					// AF_INET - IPv4�� ���ٴ� �ǹ�
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);	// INADDR_ANY�� �ڵ����� �� ��ǻ�Ϳ� �����ϴ� ��ī�� �� ��밡���� ��ī���� IP�ּҸ� ����϶�� �ǹ�
	server_addr.sin_port = htons(atoi(argv[1]));		// Port ����

	// bind �Լ�ȣ���� ���ؼ� IP �ּҿ� Port ��ȣ�� �Ҵ�
	int bind_result = bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if (bind_result == -1)
		error_handling("bind() error");
	
	// listen()���� ������������ ���� Ŭ���̾�Ʈ ��û ���
	int listen_result = listen(server_sock, 5);
	if (listen_result == -1)
		error_handling("listen() error");

	// ���� ��û�� ������ ���� accept �Լ��� ȣ���ϰ� ����
	// ���� ��û�� ���� ���¿��� �� �Լ��� ȣ��Ǹ�, �����û�� ���� ������ �Լ��� ��ȯ���� ����
	client_addr_size = sizeof(client_addr);
	client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_addr_size);
	if (client_sock == -1)
		error_handling("accept() error");

	// Ŭ���̾�Ʈ�κ��� �����͸� �а�, ������ ����� ���̸� ������
	char data[30]; //���ڿ��� �� �ձ��ڿ� �ι��ڸ� ������ �� ��� �� ����
	int str_len = read(client_sock, data, sizeof(data) - 1);
	if (str_len == -1)
		error_handling("read() error");

	// �޽����� �����
	printf("Ŭ���̾�Ʈ�� �̰� ����:%s\n", data);

	// write �Լ��� �����͸� �����ϴ� ����� �Լ��ε�, �� ������ ������ٴ� ���� �����û�� �־��ٴ� ��
	data[0] = 'Z';
	data[1] = 'H';
	data[2] = 'Q';
	write(client_sock, data, sizeof(data));

	// ���� ����
	close(client_sock);
	close(server_sock);

	return 0;
}

void error_handling(char *message) {
	// ���� �߻� �� ���� �޽����� ����ϰ� ���α׷��� ������
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
