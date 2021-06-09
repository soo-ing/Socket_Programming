#include <winsock.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <process.h>

#pragma warning(disable:4996)
#pragma comment(lib, "wsock32.lib")

#define _CRT_SECURE_NO_WARNINGS
#define PORT 9291			// �����Ʈ�� 9291
#define MAX_CLIENT 5		// �ִ� ��� �ο� �� 5��
#define ALLOW 65535			// �ִ� ���� ���� ���� ��ȣ 65535
#define MAXBUFSIZE 4000

void recv_client(void* ns);	// ������ �Լ� ������ Ÿ��
int client_num = 0;			//Ŭ���̾�Ʈ �����ڼ�
int seat = 0;				// Ŭ���̾�Ʈ ��ȣ
char welcome_ok[] = "�� Ŭ���̾�Ʈ, ȯ���մϴ�.\n====================\n1.�������\n2.�����˻�\n3.�����߰�\n====================\n";	// Welcome ���� �ʱ� ��
char welcome_full[] = "����á���ϴ�.\n";
char bookList[1000] = "\n========�������=======\n1. �����\n2. �� �غ�\n3. ���� �Ӽ�\n4. ��� 1\n5. ������ ����\n6. �־��� �ʰ� ����ϰ�\n7. ��� 2\n8. �鼦\n9. �ϰ� ���� ��� ��Ƶ� ������\n10. ���� �����ΰ�\n11. ������ ����\n12. 1cm ���̺�\n13. Ǫ���ƺ� �ſ�����\n14. ö���� ���\n15. ������ ��\n16. ���� �غ��ؿ� ���\n17. Ÿ���� �ؼ�\n18. ���̾�\n19. ������ ���ڵǱ� ����\n20. �� ������ �ʶ�\n";
char array1[] = "\n====================\n���� [�����] �Դϴ�\n--------------------\n���� : �����\n���� : �������丮\n�߰��� : 1943.04.06\n���� : 10\n====================\n";
char array2[] = "\n====================\n���� [�� �غ�] �Դϴ�\n--------------------\n���� : �� �غ�\n���� : �̼���\n�߰��� : 2020.03.01\n���� : 9.22\n====================\n";
char array3[] = "\n====================\n���� [���� �Ӽ�] �Դϴ�\n--------------------\n���� : ���� �Ӽ�\n���� : ���ȣ\n�߰��� : 2020.06.15\n���� : 9.2\n====================\n";
char array4[] = "\n====================\n���� [��� 1] �Դϴ�\n--------------------\n���� : ���1\n���� : ����������������\n�߰��� : 2020.05.30\n���� : 9.6\n====================\n";
char array5[] = "\n====================\n���� [������ ����] �Դϴ�\n--------------------\n���� : ������ ����\n���� : ���̳�\n�߰��� : 2020.05.27\n���� : 9.5\n====================\n";
char array6[] = "\n====================\n���� [�־��� �ʰ� ����ϰ�] �Դϴ�\n--------------------\n���� : �־��� �ʰ� ����ϰ�\n���� : �����\n�߰��� : 2020.05.15\n���� : 9.25\n====================\n";
char array7[] = "\n====================\n���� [��� 2] �Դϴ�\n--------------------\n���� : ���2\n���� : ����������������\n�߰��� : 2020.05.30\n���� : 9.67\n====================\n";
char array8[] = "\n====================\n���� [�鼦] �Դϴ�\n--------------------\n���� : �鼦\n���� : ���� ��Į\n�߰��� : 2020.04.27\n���� : 9.9\n====================\n";
char array9[] = "\n====================\n���� [�ϰ� ������� ��Ƶ� ������] �Դϴ�\n--------------------\n���� : �ϰ� ������� ��Ƶ� ������\n���� : ������\n�߰��� : 2019.11.04\n���� : 7.71\n====================\n";
char array10[] = "\n====================\n���� [���� �����ΰ�] �Դϴ�\n--------------------\n���� : ���� �����ΰ�\n���� : �ּ���\n�߰��� : 2020.06.08\n���� : 7.8\n====================\n";
char array11[] = "\n====================\n���� [������ ����] �Դϴ�\n--------------------\n���� : ������ ����\n���� : õ��\n�߰��� : 20.06.20\n���� : 8.5\n====================\n";
char array12[] = "\n====================\n���� [1cm ���̺�] �Դϴ�\n--------------------\n���� : 1cm���̺�\n���� : �¼�\n�߰��� : 20.01.21\n���� : 8.5\n====================\n";
char array13[] = "\n====================\n���� [Ǫ���ƺ� �ſ�����] �Դϴ�\n--------------------\n���� : Ǫ���ƺ� �ſ�����\n���� : �����\n�߰��� : 20.06.13\n���� : 8.1\n====================\n";
char array14[] = "\n====================\n���� [ö���� ���] �Դϴ�\n--------------------\n���� : ö�������\n���� : Ȳ����\n�߰��� : 20.05.15\n���� : 10\n====================\n";
char array15[] = "\n====================\n���� [������ ��] �Դϴ�\n--------------------\n���� :  ������ ��\n���� : �� ���� \n�߰��� : 16.08.10\n���� : 8.21\n====================\n";
char array16[] = "\n====================\n���� [���� �غ��ؿ� ���] �Դϴ�\n--------------------\n���� : ���� �غ��ؿ� ���\n���� : �迵��\n�߰��� : 20.04.29\n���� : 8.67\n====================\n";
char array17[] = "\n====================\n���� [Ÿ���� �ؼ�] �Դϴ�\n--------------------\n���� : Ÿ���� �ؼ� \n���� : ���� �۷����� \n�߰��� : 20.03.20\n���� : 9.57\n====================\n";
char array18[] = "\n====================\n���� [���̾�] �Դϴ�\n--------------------\n���� : ���̾� \n���� : �츣�� �켼 \n�߰��� : 17.10.30 \n���� : 9\n====================\n";
char array19[] = "\n====================\n���� [������ ���ڵǱ� ����] �Դϴ�\n--------------------\n���� : ������ ���ڵǱ� ����\n ���� : �� ��\n�߰��� : 20.01.15\n���� : 8.33\n====================\n";
char array20[] = "\n====================\n���� [�� ������ �ʶ�] �Դϴ�\n--------------------\n���� : �� ������ �ʶ� \n���� : ������ \n�߰��� : 18.12.05 \n���� : 9.25 \n====================\n";

int client_sock[ALLOW];		// client_sock (Ŭ���̾�Ʈ Welcome Socket)
HANDLE hMutex = 0;			// ���ý�

int main()
{
	printf(" ---------------------------------\n");
	printf("|\t��Ʈ��ũ ���α׷���       |\n");
	printf("|\t                          |\n");
	printf("|\t���� �˻� ���α׷�        |\n");
	printf("|\t                          |\n");
	printf("|\t2016154007 ��ο�         |\n");
	printf("|\t2016154033 ������         |\n");
	printf("|\t2016154034 õ����         |\n");
	printf("|\t                          |\n");
	printf(" ---------------------------------\n");

	// ���ý� ����
	hMutex = CreateMutex(NULL, FALSE, NULL);	// ���� ���н� ����
	if (!hMutex)
	{
		printf("Mutex ����\n");
		CloseHandle(hMutex);
		return 1;
	}

	// winsock �ʱ�ȭ
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(1, 1), &wsd) != 0)	// ��� ���� ������ 1.1
	{
		printf("Winsock ����\n");
		WSACleanup();
		return 1;
	}

	// Listening socket ����
	int s, addrsize, ret;
	sockaddr_in server, client;

	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == SOCKET_ERROR)
	{
		printf("socket() ����\n");
		closesocket(s);
		WSACleanup();
		return 1;
	}

	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);

	// Bind 
	if (bind(s, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("bind() ����\n");
		closesocket(s);
		WSACleanup();
		return 1;

	}

	printf("*������ �����..* '%d�ڸ�' ����\n", MAX_CLIENT - client_num);
	listen(s, 10);
	addrsize = sizeof(client);

	// ����� ���� wait
	while (1)
	{
		// Blocking ������� Clientf�� wait
		client_sock[seat] = accept(s, (sockaddr*)&client, &addrsize);

		// accept ��(�߿�, client_num �� accept() �Լ� ������ �� ���Ҽ� �����Ƿ�
		// MAX_CLIENT ���޽ö� ���� accept() �� ���� �߻� ���ɼ� ����
		if (client_num < MAX_CLIENT)		// Ŭ���̾�Ʈ�� ���� ���� �ʾ��� ���
		{
			if (client_sock[seat] != INVALID_SOCKET || client_sock[seat] != SOCKET_ERROR) {}
			_beginthread(recv_client, 0, &client_sock[seat]);
			Sleep(10);
			printf("%d�� ������ %s:%d ���� �����ϼ̽��ϴ�\n", seat, inet_ntoa(client.sin_addr), ntohs(client.sin_port));
		}
		else	// Ŭ���̾�Ʈ�� ���� á�� ���
		{
			addrsize = sizeof(client);
			if (client_sock[seat] == INVALID_SOCKET)
			{
				printf("accept() ����\n");
				closesocket(client_sock[seat]);
				closesocket(s);
				WSACleanup();
				return 1;
			}
			ret = send(client_sock[seat], welcome_full, sizeof(welcome_full), 0);
			closesocket(client_sock[seat]);
			// �޽��� ������ �ٷ� ���� ����.
		}
	}
	return 0;
}

void recv_client(void* ns)
{
	// ���������� �޾Ƶ��̸�, ������ ����
	// Ŭ���̾�Ʈ�� ���ڸ� �ø�
	WaitForSingleObject(hMutex, INFINITE);
	client_num++;				// Ŭ���̾�Ʈ �����ڼ� ����
	seat++;						// Ŭ���̾�Ʈ ��ȣ ����
	printf("-- '%d�ڸ�' ���ҽ��ϴ� --\n", MAX_CLIENT - client_num);

	ReleaseMutex(hMutex);

	char welcome[1024] = { 0 };
	char welcome1[1024] = { 0 };
	char buff[5000] = { 0 };
	int ret, i;
	int num1, num2;

	itoa(seat, welcome, 10);			// Ŭ���̾�Ʈ ��ȣ
	strcat(welcome, welcome_ok);		// ���� ȯ�� �޽��� ȯ��
	ret = send(*(SOCKET*)ns, welcome, sizeof(welcome), 0);		// ����
	while (ret != SOCKET_ERROR || ret != INVALID_SOCKET)
	{
		ret = recv(*(SOCKET*)ns, buff, 1024, 0);		// Ŭ���̾�Ʈ�� �޽����� ����
		for (i = 0; i < ALLOW; i++)
		{
			// ���� Ŭ���̾�Ʈ ������ �޸� �ּҿ� ������ Ŭ���̾�Ʈ ���� �޸� �ּҰ� �ٸ����� ����
			WaitForSingleObject(hMutex, INFINITE);
			if (((unsigned*)&client_sock[i] != (SOCKET*)ns))
			{
				send(client_sock[i], buff, strlen(buff), 0);
			}
			ReleaseMutex(hMutex);
		}
		// ���� �ܼ� â�� ���
		if (strlen(buff) != 0) {
			printf("%s", buff);

			memset(buff, 0, 1024);
			printf("> %d�� �������� ����� �Է��ϼ��� : ", seat);
			scanf("%d", &num1);

			switch (num1) {
			case 1:
				strcat(welcome1, bookList);
				send(*(SOCKET*)ns, welcome1, sizeof(welcome1), 0);
				memset(welcome1, 0, 1024);
				printf("==================================\n");
				printf("* ���������� �����Ͽ����ϴ�.\n");
				printf("==================================\n");
				break;
			case 2:
				printf("\n=============�������============\n1. �����\n2. �� �غ�\n3. ���� �Ӽ�\n4. ��� 1\n5. ������ ����\n6. �־��� �ʰ� ����ϰ�\n7. ��� 2\n8. �鼦\n9. �ϰ� ���� ��� ��Ƶ� ������\n10. ���� �����ΰ�\n11. ������ ����\n12. 1cm ���̺�\n13. Ǫ���ƺ� �ſ�����\n14. ö���� ���\n15. ������ ��\n16. ���� �غ��ؿ� ���\n17. Ÿ���� �ؼ�\n18. ���̾�\n19. ������ ���ڵǱ� ����\n20. �� ������ �ʶ�\n");
				printf("==================================\n");
				printf("> å ��ȣ �Է� : ");
				scanf("%d", &num2);

				switch (num2) {
				case 1:
					printf("���� : �����\n���� : �������丮\n�߰��� : 1943.04.06\n���� : 10\n");
					printf("==================================\n");
					printf("* ���������� �����Ͽ����ϴ�.\n");
					printf("==================================\n");
					strcat(welcome1, array1);
					send(*(SOCKET*)ns, welcome1, sizeof(welcome1), 0);
					memset(welcome1, 0, sizeof(char) * 1024);
					break;
				case 2:
					printf("���� : ���غ�\n���� : �̼���\n�߰��� : 2020.03.01\n���� : 9.22\n");
					printf("==================================\n");
					printf("* ���������� �����Ͽ����ϴ�.\n");
					printf("==================================\n");
					strcat(welcome1, array2);
					send(*(SOCKET*)ns, welcome1, sizeof(welcome1), 0);
					memset(welcome1, 0, sizeof(char) * 1024);
					break;
				case 3:
					printf("���� : ���� �Ӽ�\n���� : ���ȣ\n�߰��� : 2020.06.15\n���� : 9.2\n");
					printf("==================================\n");
					printf("* ���������� �����Ͽ����ϴ�.\n");
					printf("==================================\n");
					strcat(welcome1, array3);
					send(*(SOCKET*)ns, welcome1, sizeof(welcome1), 0);
					memset(welcome1, 0, sizeof(char) * 1024);
					break;
				case 4:
					printf("���� : ���1\n���� : ����������������\n�߰��� : 2020.05.30\n���� : 9.6\n");
					printf("==================================\n");
					printf("* ���������� �����Ͽ����ϴ�.\n");
					printf("==================================\n");
					strcat(welcome1, array4);
					send(*(SOCKET*)ns, welcome1, sizeof(welcome1), 0);
					memset(welcome1, 0, sizeof(char) * 1024);
					break;
				case 5:
					printf("���� : ������ ����\n���� : ���̳�\n�߰��� : 2020.05.27\n���� : 9.5\n");
					printf("==================================\n");
					printf("* ���������� �����Ͽ����ϴ�.\n");
					printf("==================================\n");
					strcat(welcome1, array5);
					send(*(SOCKET*)ns, welcome1, sizeof(welcome1), 0);
					memset(welcome1, 0, sizeof(char) * 1024);
					break;
				case 6:
					printf("���� : �־��� �ʰ� ����ϰ�\n���� : �����\n�߰��� : 2020.05.15\n���� : 9.25\n");
					printf("==================================\n");
					printf("* ���������� �����Ͽ����ϴ�.\n");
					printf("==================================\n");
					strcat(welcome1, array6);
					send(*(SOCKET*)ns, welcome1, sizeof(welcome1), 0);
					memset(welcome1, 0, sizeof(char) * 1024);
					break;
				case 7:
					printf("���� : ���2\n���� : ����������������\n�߰��� : 2020.05.30\n���� : 9.67\n");
					printf("==================================\n");
					printf("* ���������� �����Ͽ����ϴ�.\n");
					printf("==================================\n");
					strcat(welcome1, array7);
					send(*(SOCKET*)ns, welcome1, sizeof(welcome1), 0);
					memset(welcome1, 0, sizeof(char) * 1024);
					break;
				case 8:
					printf("���� : �鼦\n���� : ���� ��Į\n�߰��� : 2020.04.27\n���� : 9.9\n");
					printf("==================================\n");
					printf("* ���������� �����Ͽ����ϴ�.\n");
					printf("==================================\n");
					strcat(welcome1, array8);
					send(*(SOCKET*)ns, welcome1, sizeof(welcome1), 0);
					memset(welcome1, 0, sizeof(char) * 1024);
					break;
				case 9:
					printf("���� : �ϰ� ������� ��Ƶ� ������\n���� : ������\n�߰��� : 2019.11.04\n���� : 7.71\n");
					printf("==================================\n");
					printf("* ���������� �����Ͽ����ϴ�.\n");
					printf("==================================\n");
					strcat(welcome1, array9);
					send(*(SOCKET*)ns, welcome1, sizeof(welcome1), 0);
					memset(welcome1, 0, sizeof(char) * 1024);
					break;
				case 10:
					printf("���� : ���� �����ΰ�\n���� : �ּ���\n�߰��� : 2020.06.08\n���� : 7.8\n");
					printf("==================================\n");
					printf("* ���������� �����Ͽ����ϴ�.\n");
					printf("==================================\n");
					strcat(welcome1, array10);
					send(*(SOCKET*)ns, welcome1, sizeof(welcome1), 0);
					memset(welcome1, 0, sizeof(char) * 1024);
					break;
				case 11:
					printf("���� : ������ ����\n���� : õ��\n�߰��� : 20.06.20\n���� : 8.5\n");
					printf("==================================\n");
					printf("* ���������� �����Ͽ����ϴ�.\n");
					printf("==================================\n");
					strcat(welcome1, array11);
					send(*(SOCKET*)ns, welcome1, sizeof(welcome1), 0);
					memset(welcome1, 0, sizeof(char) * 1024);
					break;
				case 12:
					printf("���� : 1cm���̺�\n���� : �¼�\n�߰��� : 20.01.21\n���� : 8.5\n");
					printf("==================================\n");
					printf("* ���������� �����Ͽ����ϴ�.\n");
					printf("==================================\n");
					strcat(welcome1, array12);
					send(*(SOCKET*)ns, welcome1, sizeof(welcome1), 0);
					memset(welcome1, 0, sizeof(char) * 1024);
					break;
				case 13:
					printf("���� : Ǫ���ƺ� �ſ�����\n���� : �����\n�߰��� : 20.06.13\n���� : 8.1\n");
					printf("==================================\n");
					printf("* ���������� �����Ͽ����ϴ�.\n");
					printf("==================================\n");
					strcat(welcome1, array13);
					send(*(SOCKET*)ns, welcome1, sizeof(welcome1), 0);
					memset(welcome1, 0, sizeof(char) * 1024);
					break;
				case 14:
					printf("���� : ö�������\n���� : Ȳ����\n�߰��� : 20.05.15\n���� : 10\n");
					printf("==================================\n");
					printf("* ���������� �����Ͽ����ϴ�.\n");
					printf("==================================\n");
					strcat(welcome1, array14);
					send(*(SOCKET*)ns, welcome1, sizeof(welcome1), 0);
					memset(welcome1, 0, sizeof(char) * 1024);
					break;
				case 15:
					printf("���� :  ������ ��\n���� : �� ���� \n�߰��� : 16.08.10\n���� : 8.21\n");
					printf("==================================\n");
					printf("* ���������� �����Ͽ����ϴ�.\n");
					printf("==================================\n");
					strcat(welcome1, array15);
					send(*(SOCKET*)ns, welcome1, sizeof(welcome1), 0);
					memset(welcome1, 0, sizeof(char) * 1024);
					break;
				case 16:
					printf("���� : ���� �غ��ؿ� ���\n���� : �迵��\n�߰��� : 20.04.29\n���� : 8.67\n");
					printf("==================================\n");
					printf("* ���������� �����Ͽ����ϴ�.\n");
					printf("==================================\n");
					strcat(welcome1, array16);
					send(*(SOCKET*)ns, welcome1, sizeof(welcome1), 0);
					memset(welcome1, 0, sizeof(char) * 1024);
					break;
				case 17:
					printf("���� : Ÿ���� �ؼ� \n���� : ���� �۷����� \n�߰��� : 20.03.20\n���� : 9.57\n");
					printf("==================================\n");
					printf("* ���������� �����Ͽ����ϴ�.\n");
					printf("==================================\n");
					strcat(welcome1, array17);
					send(*(SOCKET*)ns, welcome1, sizeof(welcome1), 0);
					memset(welcome1, 0, sizeof(char) * 1024);
					break;
				case 18:
					printf("���� : ���̾� \n���� : �츣�� �켼 \n�߰��� : 17.10.30 \n���� : 9\n");
					printf("==================================\n");
					printf("* ���������� �����Ͽ����ϴ�.\n");
					printf("==================================\n");
					strcat(welcome1, array18);
					send(*(SOCKET*)ns, welcome1, sizeof(welcome1), 0);
					memset(welcome1, 0, sizeof(char) * 1024);
					break;
				case 19:
					printf("���� : ������ ���ڵǱ� ����\n ���� : �� ��\n�߰��� : 20.01.15\n���� : 8.33\n");
					printf("==================================\n");
					printf("* ���������� �����Ͽ����ϴ�.\n");
					printf("==================================\n");
					strcat(welcome1, array19);
					send(*(SOCKET*)ns, welcome1, sizeof(welcome1), 0);
					memset(welcome1, 0, sizeof(char) * 1024);
					break;
				case 20:
					printf("���� : �� ������ �ʶ� \n���� : ������ \n�߰��� : 18.12.05 \n���� : 9.25 \n");
					printf("==================================\n");
					printf("* ���������� �����Ͽ����ϴ�.\n");
					printf("==================================\n");
					strcat(welcome1, array20);
					send(*(SOCKET*)ns, welcome1, sizeof(welcome1), 0);
					memset(welcome1, 0, sizeof(char) * 1024);
					break;
				default:
					printf("�ش� flag�� �������� �ʽ��ϴ�.\n");
					break;
				}
				break;
			default:
				break;
			}
		}
	}

	// ���ӵ� ������ ������ ���� ��������
	WaitForSingleObject(hMutex, INFINITE);
	client_num--;
	printf("%d�� ���� ���� >> '%d�ڸ�' ���ҽ��ϴ�\n", seat, MAX_CLIENT - client_num);
	ReleaseMutex(hMutex);

	closesocket(*(int*)ns);

	return;
}
