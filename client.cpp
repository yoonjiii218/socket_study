#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);

int main(int argc, char *argv[]) {

	// ����, Ŭ���̾�Ʈ ���� ���� ��ũ���� ���� ����
	int sock;

	// sockaddr_in ����ü ���� ����
	struct sockaddr_in server_addr;
	char message[30];
	int str_len;

	if (argc != 3) {
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	// ���� ����
	// ������ �����ϴ� �������� ���� ���ϰ� Ŭ���̾�Ʈ �������� ������ �ʴ´�                                                                                        .
	// bind listen�Լ��� ȣ���� �̾����� ���� ����, connect �Լ��� ȣ��� �̾����� Ŭ���̾�Ʈ ����
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error");

	// ���� ���� �ʱ�ȭ �� ���� �Ҵ�
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;						// AF_INET - IPv4�� ���ٴ� �ǹ�
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);		// ������ ���� IP ����
	server_addr.sin_port = htons(atoi(argv[2]));			// Port ����

	// ������ connect ��û�� �ϰ�, �׿� ���� ������� ����
	// -1�� ��� ���� ���� �߻�
	int connect_result = connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if (connect_result == -1)
		error_handling("connect() error");

	//������ ������ ����
	char data[30] = "test218";
	write(sock, data, sizeof(data));

	// �����κ��� �����͸� �а�, ������ ����� ���̸� ������
	char data2[30];
	str_len = read(sock, data2, sizeof(data2) - 1);
	if (str_len == -1)
		error_handling("read() error");

	// �޽����� �����
	printf("Message from server:%s\n", data2);

	// ���� ���� ����
	close(sock);

	return 0;

}

void error_handling(char *message) {
	// ���� �߻� �� ���� �޽����� ����ϰ� ���α׷��� ������
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}