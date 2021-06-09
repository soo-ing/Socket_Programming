#include <winsock.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <process.h>

#pragma warning(disable:4996)
#pragma comment(lib, "wsock32.lib")

#define _CRT_SECURE_NO_WARNINGS
#define PORT 9291			// 사용포트는 9291
#define MAX_CLIENT 5		// 최대 허용 인원 수 5개
#define ALLOW 65535			// 최대 생성 가능 소켓 번호 65535
#define MAXBUFSIZE 4000

void recv_client(void* ns);	// 스레드 함수 프로토 타입
int client_num = 0;			//클라이언트 접속자수
int seat = 0;				// 클라이언트 번호
char welcome_ok[] = "번 클라이언트, 환영합니다.\n====================\n1.도서목록\n2.도서검색\n3.도서추가\n====================\n";	// Welcome 정상 초기 글
char welcome_full[] = "가득찼습니다.\n";
char bookList[1000] = "\n========도서목록=======\n1. 어린왕자\n2. 더 해빙\n3. 돈의 속성\n4. 기억 1\n5. 보통의 언어들\n6. 애쓰지 않고 편안하게\n7. 기억 2\n8. 룬샷\n9. 하고 싶은 대로 살아도 괜찮아\n10. 나는 누구인가\n11. 통찰과 역설\n12. 1cm 다이빙\n13. 푸름아빠 거울육아\n14. 철도원 삼대\n15. 지리의 힘\n16. 오래 준비해온 대답\n17. 타인의 해석\n18. 데미안\n19. 존리의 부자되기 습관\n20. 참 소중한 너라서\n";
char array1[] = "\n====================\n도서 [어린왕자] 입니다\n--------------------\n제목 : 어린왕자\n저자 : 생텍쥐페리\n발간일 : 1943.04.06\n평점 : 10\n====================\n";
char array2[] = "\n====================\n도서 [더 해빙] 입니다\n--------------------\n제목 : 더 해빙\n저자 : 이서윤\n발간일 : 2020.03.01\n평점 : 9.22\n====================\n";
char array3[] = "\n====================\n도서 [돈의 속성] 입니다\n--------------------\n제목 : 돈의 속성\n저자 : 기승호\n발간일 : 2020.06.15\n평점 : 9.2\n====================\n";
char array4[] = "\n====================\n도서 [기억 1] 입니다\n--------------------\n제목 : 기억1\n저자 : 베르나르베르베르\n발간일 : 2020.05.30\n평점 : 9.6\n====================\n";
char array5[] = "\n====================\n도서 [보통의 언어들] 입니다\n--------------------\n제목 : 보통의 언어들\n저자 : 김이나\n발간일 : 2020.05.27\n평점 : 9.5\n====================\n";
char array6[] = "\n====================\n도서 [애쓰지 않고 편안하게] 입니다\n--------------------\n제목 : 애쓰지 않고 편안하게\n저자 : 김수현\n발간일 : 2020.05.15\n평점 : 9.25\n====================\n";
char array7[] = "\n====================\n도서 [기억 2] 입니다\n--------------------\n제목 : 기억2\n저자 : 베르나르베르베르\n발간일 : 2020.05.30\n평점 : 9.67\n====================\n";
char array8[] = "\n====================\n도서 [룬샷] 입니다\n--------------------\n제목 : 룬샷\n저자 : 사피 바칼\n발간일 : 2020.04.27\n평점 : 9.9\n====================\n";
char array9[] = "\n====================\n도서 [하고 싶은대로 살아도 괜찮아] 입니다\n--------------------\n제목 : 하고 싶은대로 살아도 괜찮아\n저자 : 윤정은\n발간일 : 2019.11.04\n평점 : 7.71\n====================\n";
char array10[] = "\n====================\n도서 [나는 누구인가] 입니다\n--------------------\n제목 : 나는 누구인가\n저자 : 최서원\n발간일 : 2020.06.08\n평점 : 7.8\n====================\n";
char array11[] = "\n====================\n도서 [통찰과 역설] 입니다\n--------------------\n제목 : 통찰과 역설\n저자 : 천공\n발간일 : 20.06.20\n평점 : 8.5\n====================\n";
char array12[] = "\n====================\n도서 [1cm 다이빙] 입니다\n--------------------\n제목 : 1cm다이빙\n저자 : 태수\n발간일 : 20.01.21\n평점 : 8.5\n====================\n";
char array13[] = "\n====================\n도서 [푸름아빠 거울육아] 입니다\n--------------------\n제목 : 푸름아빠 거울육아\n저자 : 최희수\n발간일 : 20.06.13\n평점 : 8.1\n====================\n";
char array14[] = "\n====================\n도서 [철도원 삼대] 입니다\n--------------------\n제목 : 철도원삼대\n저자 : 황석영\n발간일 : 20.05.15\n평점 : 10\n====================\n";
char array15[] = "\n====================\n도서 [지리의 힘] 입니다\n--------------------\n제목 :  지리의 힘\n저자 : 팀 마샬 \n발간일 : 16.08.10\n평점 : 8.21\n====================\n";
char array16[] = "\n====================\n도서 [오래 준비해온 대답] 입니다\n--------------------\n제목 : 오래 준비해온 대답\n저자 : 김영하\n발간일 : 20.04.29\n평점 : 8.67\n====================\n";
char array17[] = "\n====================\n도서 [타인의 해석] 입니다\n--------------------\n제목 : 타인의 해석 \n저자 : 말콤 글래드웰 \n발간일 : 20.03.20\n평점 : 9.57\n====================\n";
char array18[] = "\n====================\n도서 [데미안] 입니다\n--------------------\n제목 : 데미안 \n저자 : 헤르만 헤세 \n발간일 : 17.10.30 \n평점 : 9\n====================\n";
char array19[] = "\n====================\n도서 [존리의 부자되기 습관] 입니다\n--------------------\n제목 : 존리의 부자되기 습관\n 저자 : 존 리\n발간일 : 20.01.15\n평점 : 8.33\n====================\n";
char array20[] = "\n====================\n도서 [참 소중한 너라서] 입니다\n--------------------\n제목 : 참 소중한 너라서 \n저자 : 김지훈 \n발간일 : 18.12.05 \n평점 : 9.25 \n====================\n";

int client_sock[ALLOW];		// client_sock (클라이언트 Welcome Socket)
HANDLE hMutex = 0;			// 뮤택스

int main()
{
	printf(" ---------------------------------\n");
	printf("|\t네트워크 프로그래밍       |\n");
	printf("|\t                          |\n");
	printf("|\t도서 검색 프로그램        |\n");
	printf("|\t                          |\n");
	printf("|\t2016154007 김민우         |\n");
	printf("|\t2016154033 차수빈         |\n");
	printf("|\t2016154034 천세빈         |\n");
	printf("|\t                          |\n");
	printf(" ---------------------------------\n");

	// 뮤택스 생성
	hMutex = CreateMutex(NULL, FALSE, NULL);	// 생성 실패시 오류
	if (!hMutex)
	{
		printf("Mutex 오류\n");
		CloseHandle(hMutex);
		return 1;
	}

	// winsock 초기화
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(1, 1), &wsd) != 0)	// 사용 소켓 버전은 1.1
	{
		printf("Winsock 오류\n");
		WSACleanup();
		return 1;
	}

	// Listening socket 생성
	int s, addrsize, ret;
	sockaddr_in server, client;

	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == SOCKET_ERROR)
	{
		printf("socket() 오류\n");
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
		printf("bind() 오류\n");
		closesocket(s);
		WSACleanup();
		return 1;

	}

	printf("*접속자 대기중..* '%d자리' 남음\n", MAX_CLIENT - client_num);
	listen(s, 10);
	addrsize = sizeof(client);

	// 사용자 접속 wait
	while (1)
	{
		// Blocking 방식으로 Clientf를 wait
		client_sock[seat] = accept(s, (sockaddr*)&client, &addrsize);

		// accept 시(중요, client_num 가 accept() 함수 수행중 에 변할수 있으므로
		// MAX_CLIENT 도달시랑 따로 accept() 시 문제 발생 가능성 있음
		if (client_num < MAX_CLIENT)		// 클라이언트가 가득 차지 않았을 경우
		{
			if (client_sock[seat] != INVALID_SOCKET || client_sock[seat] != SOCKET_ERROR) {}
			_beginthread(recv_client, 0, &client_sock[seat]);
			Sleep(10);
			printf("%d번 접속자 %s:%d 에서 접속하셨습니다\n", seat, inet_ntoa(client.sin_addr), ntohs(client.sin_port));
		}
		else	// 클라이언트가 가득 찼을 경우
		{
			addrsize = sizeof(client);
			if (client_sock[seat] == INVALID_SOCKET)
			{
				printf("accept() 오류\n");
				closesocket(client_sock[seat]);
				closesocket(s);
				WSACleanup();
				return 1;
			}
			ret = send(client_sock[seat], welcome_full, sizeof(welcome_full), 0);
			closesocket(client_sock[seat]);
			// 메시지 보내고 바로 연결 종료.
		}
	}
	return 0;
}

void recv_client(void* ns)
{
	// 정상적으로 받아들이면, 스레드 실행
	// 클라이언트의 숫자를 늘림
	WaitForSingleObject(hMutex, INFINITE);
	client_num++;				// 클라이언트 접속자수 증가
	seat++;						// 클라이언트 번호 증가
	printf("-- '%d자리' 남았습니다 --\n", MAX_CLIENT - client_num);

	ReleaseMutex(hMutex);

	char welcome[1024] = { 0 };
	char welcome1[1024] = { 0 };
	char buff[5000] = { 0 };
	int ret, i;
	int num1, num2;

	itoa(seat, welcome, 10);			// 클라이언트 번호
	strcat(welcome, welcome_ok);		// 정상 환영 메시지 환영
	ret = send(*(SOCKET*)ns, welcome, sizeof(welcome), 0);		// 전송
	while (ret != SOCKET_ERROR || ret != INVALID_SOCKET)
	{
		ret = recv(*(SOCKET*)ns, buff, 1024, 0);		// 클라이언트의 메시지를 받음
		for (i = 0; i < ALLOW; i++)
		{
			// 받은 클라이언트 소켓의 메모리 주소와 보내는 클라이언트 소켓 메모리 주소가 다를때만 전송
			WaitForSingleObject(hMutex, INFINITE);
			if (((unsigned*)&client_sock[i] != (SOCKET*)ns))
			{
				send(client_sock[i], buff, strlen(buff), 0);
			}
			ReleaseMutex(hMutex);
		}
		// 서버 콘솔 창에 출력
		if (strlen(buff) != 0) {
			printf("%s", buff);

			memset(buff, 0, 1024);
			printf("> %d번 접속자의 명령을 입력하세요 : ", seat);
			scanf("%d", &num1);

			switch (num1) {
			case 1:
				strcat(welcome1, bookList);
				send(*(SOCKET*)ns, welcome1, sizeof(welcome1), 0);
				memset(welcome1, 0, 1024);
				printf("==================================\n");
				printf("* 성공적으로 전송하였습니다.\n");
				printf("==================================\n");
				break;
			case 2:
				printf("\n=============도서목록============\n1. 어린왕자\n2. 더 해빙\n3. 돈의 속성\n4. 기억 1\n5. 보통의 언어들\n6. 애쓰지 않고 편안하게\n7. 기억 2\n8. 룬샷\n9. 하고 싶은 대로 살아도 괜찮아\n10. 나는 누구인가\n11. 통찰과 역설\n12. 1cm 다이빙\n13. 푸름아빠 거울육아\n14. 철도원 삼대\n15. 지리의 힘\n16. 오래 준비해온 대답\n17. 타인의 해석\n18. 데미안\n19. 존리의 부자되기 습관\n20. 참 소중한 너라서\n");
				printf("==================================\n");
				printf("> 책 번호 입력 : ");
				scanf("%d", &num2);

				switch (num2) {
				case 1:
					printf("제목 : 어린왕자\n저자 : 생텍쥐페리\n발간일 : 1943.04.06\n평점 : 10\n");
					printf("==================================\n");
					printf("* 성공적으로 전송하였습니다.\n");
					printf("==================================\n");
					strcat(welcome1, array1);
					send(*(SOCKET*)ns, welcome1, sizeof(welcome1), 0);
					memset(welcome1, 0, sizeof(char) * 1024);
					break;
				case 2:
					printf("제목 : 더해빙\n저자 : 이서윤\n발간일 : 2020.03.01\n평점 : 9.22\n");
					printf("==================================\n");
					printf("* 성공적으로 전송하였습니다.\n");
					printf("==================================\n");
					strcat(welcome1, array2);
					send(*(SOCKET*)ns, welcome1, sizeof(welcome1), 0);
					memset(welcome1, 0, sizeof(char) * 1024);
					break;
				case 3:
					printf("제목 : 돈의 속성\n저자 : 기승호\n발간일 : 2020.06.15\n평점 : 9.2\n");
					printf("==================================\n");
					printf("* 성공적으로 전송하였습니다.\n");
					printf("==================================\n");
					strcat(welcome1, array3);
					send(*(SOCKET*)ns, welcome1, sizeof(welcome1), 0);
					memset(welcome1, 0, sizeof(char) * 1024);
					break;
				case 4:
					printf("제목 : 기억1\n저자 : 베르나르베르베르\n발간일 : 2020.05.30\n평점 : 9.6\n");
					printf("==================================\n");
					printf("* 성공적으로 전송하였습니다.\n");
					printf("==================================\n");
					strcat(welcome1, array4);
					send(*(SOCKET*)ns, welcome1, sizeof(welcome1), 0);
					memset(welcome1, 0, sizeof(char) * 1024);
					break;
				case 5:
					printf("제목 : 보통의 언어들\n저자 : 김이나\n발간일 : 2020.05.27\n평점 : 9.5\n");
					printf("==================================\n");
					printf("* 성공적으로 전송하였습니다.\n");
					printf("==================================\n");
					strcat(welcome1, array5);
					send(*(SOCKET*)ns, welcome1, sizeof(welcome1), 0);
					memset(welcome1, 0, sizeof(char) * 1024);
					break;
				case 6:
					printf("제목 : 애쓰지 않고 편안하게\n저자 : 김수현\n발간일 : 2020.05.15\n평점 : 9.25\n");
					printf("==================================\n");
					printf("* 성공적으로 전송하였습니다.\n");
					printf("==================================\n");
					strcat(welcome1, array6);
					send(*(SOCKET*)ns, welcome1, sizeof(welcome1), 0);
					memset(welcome1, 0, sizeof(char) * 1024);
					break;
				case 7:
					printf("제목 : 기억2\n저자 : 베르나르베르베르\n발간일 : 2020.05.30\n평점 : 9.67\n");
					printf("==================================\n");
					printf("* 성공적으로 전송하였습니다.\n");
					printf("==================================\n");
					strcat(welcome1, array7);
					send(*(SOCKET*)ns, welcome1, sizeof(welcome1), 0);
					memset(welcome1, 0, sizeof(char) * 1024);
					break;
				case 8:
					printf("제목 : 룬샷\n저자 : 사피 바칼\n발간일 : 2020.04.27\n평점 : 9.9\n");
					printf("==================================\n");
					printf("* 성공적으로 전송하였습니다.\n");
					printf("==================================\n");
					strcat(welcome1, array8);
					send(*(SOCKET*)ns, welcome1, sizeof(welcome1), 0);
					memset(welcome1, 0, sizeof(char) * 1024);
					break;
				case 9:
					printf("제목 : 하고 싶은대로 살아도 괜찮아\n저자 : 윤정은\n발간일 : 2019.11.04\n평점 : 7.71\n");
					printf("==================================\n");
					printf("* 성공적으로 전송하였습니다.\n");
					printf("==================================\n");
					strcat(welcome1, array9);
					send(*(SOCKET*)ns, welcome1, sizeof(welcome1), 0);
					memset(welcome1, 0, sizeof(char) * 1024);
					break;
				case 10:
					printf("제목 : 나는 누구인가\n저자 : 최서원\n발간일 : 2020.06.08\n평점 : 7.8\n");
					printf("==================================\n");
					printf("* 성공적으로 전송하였습니다.\n");
					printf("==================================\n");
					strcat(welcome1, array10);
					send(*(SOCKET*)ns, welcome1, sizeof(welcome1), 0);
					memset(welcome1, 0, sizeof(char) * 1024);
					break;
				case 11:
					printf("제목 : 통찰과 역설\n저자 : 천공\n발간일 : 20.06.20\n평점 : 8.5\n");
					printf("==================================\n");
					printf("* 성공적으로 전송하였습니다.\n");
					printf("==================================\n");
					strcat(welcome1, array11);
					send(*(SOCKET*)ns, welcome1, sizeof(welcome1), 0);
					memset(welcome1, 0, sizeof(char) * 1024);
					break;
				case 12:
					printf("제목 : 1cm다이빙\n저자 : 태수\n발간일 : 20.01.21\n평점 : 8.5\n");
					printf("==================================\n");
					printf("* 성공적으로 전송하였습니다.\n");
					printf("==================================\n");
					strcat(welcome1, array12);
					send(*(SOCKET*)ns, welcome1, sizeof(welcome1), 0);
					memset(welcome1, 0, sizeof(char) * 1024);
					break;
				case 13:
					printf("제목 : 푸름아빠 거울육아\n저자 : 최희수\n발간일 : 20.06.13\n평점 : 8.1\n");
					printf("==================================\n");
					printf("* 성공적으로 전송하였습니다.\n");
					printf("==================================\n");
					strcat(welcome1, array13);
					send(*(SOCKET*)ns, welcome1, sizeof(welcome1), 0);
					memset(welcome1, 0, sizeof(char) * 1024);
					break;
				case 14:
					printf("제목 : 철도원삼대\n저자 : 황석영\n발간일 : 20.05.15\n평점 : 10\n");
					printf("==================================\n");
					printf("* 성공적으로 전송하였습니다.\n");
					printf("==================================\n");
					strcat(welcome1, array14);
					send(*(SOCKET*)ns, welcome1, sizeof(welcome1), 0);
					memset(welcome1, 0, sizeof(char) * 1024);
					break;
				case 15:
					printf("제목 :  지리의 힘\n저자 : 팀 마샬 \n발간일 : 16.08.10\n평점 : 8.21\n");
					printf("==================================\n");
					printf("* 성공적으로 전송하였습니다.\n");
					printf("==================================\n");
					strcat(welcome1, array15);
					send(*(SOCKET*)ns, welcome1, sizeof(welcome1), 0);
					memset(welcome1, 0, sizeof(char) * 1024);
					break;
				case 16:
					printf("제목 : 오래 준비해온 대답\n저자 : 김영하\n발간일 : 20.04.29\n평점 : 8.67\n");
					printf("==================================\n");
					printf("* 성공적으로 전송하였습니다.\n");
					printf("==================================\n");
					strcat(welcome1, array16);
					send(*(SOCKET*)ns, welcome1, sizeof(welcome1), 0);
					memset(welcome1, 0, sizeof(char) * 1024);
					break;
				case 17:
					printf("제목 : 타인의 해석 \n저자 : 말콤 글래드웰 \n발간일 : 20.03.20\n평점 : 9.57\n");
					printf("==================================\n");
					printf("* 성공적으로 전송하였습니다.\n");
					printf("==================================\n");
					strcat(welcome1, array17);
					send(*(SOCKET*)ns, welcome1, sizeof(welcome1), 0);
					memset(welcome1, 0, sizeof(char) * 1024);
					break;
				case 18:
					printf("제목 : 데미안 \n저자 : 헤르만 헤세 \n발간일 : 17.10.30 \n평점 : 9\n");
					printf("==================================\n");
					printf("* 성공적으로 전송하였습니다.\n");
					printf("==================================\n");
					strcat(welcome1, array18);
					send(*(SOCKET*)ns, welcome1, sizeof(welcome1), 0);
					memset(welcome1, 0, sizeof(char) * 1024);
					break;
				case 19:
					printf("제목 : 존리의 부자되기 습관\n 저자 : 존 리\n발간일 : 20.01.15\n평점 : 8.33\n");
					printf("==================================\n");
					printf("* 성공적으로 전송하였습니다.\n");
					printf("==================================\n");
					strcat(welcome1, array19);
					send(*(SOCKET*)ns, welcome1, sizeof(welcome1), 0);
					memset(welcome1, 0, sizeof(char) * 1024);
					break;
				case 20:
					printf("제목 : 참 소중한 너라서 \n저자 : 김지훈 \n발간일 : 18.12.05 \n평점 : 9.25 \n");
					printf("==================================\n");
					printf("* 성공적으로 전송하였습니다.\n");
					printf("==================================\n");
					strcat(welcome1, array20);
					send(*(SOCKET*)ns, welcome1, sizeof(welcome1), 0);
					memset(welcome1, 0, sizeof(char) * 1024);
					break;
				default:
					printf("해당 flag는 존재하지 않습니다.\n");
					break;
				}
				break;
			default:
				break;
			}
		}
	}

	// 접속된 소켓이 연결을 해제 시켰을때
	WaitForSingleObject(hMutex, INFINITE);
	client_num--;
	printf("%d번 접속 해제 >> '%d자리' 남았습니다\n", seat, MAX_CLIENT - client_num);
	ReleaseMutex(hMutex);

	closesocket(*(int*)ns);

	return;
}
