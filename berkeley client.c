#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define BUFLEN 100

static int readn(int sock,char *msgp,unsigned msgsize){
   int rlen,temp;
   rlen=0;
   while(rlen<msgsize){
      temp=read(sock,msgp+rlen,msgsize-rlen);
      if(temp<0)
         if(errno==EINTR)   continue;
         else   return 0;
      else if(temp==0)   break;
      rlen+=temp;
   }
   return rlen == msgsize;
}

int main(int argc, char *argv[]){
   int sockfd,i,recvLen;
   struct sockaddr_in addr;
   char buf1[BUFLEN],buf2[BUFLEN];

   if(argc<3){
      fprintf(stderr,"USAGE: %s ip_addr port.\n",argv[0]); // ip address port number
      exit(-1);
   }
   if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0){
      perror("소켓을 가져올 수 없습니다");
      exit(1);
   }
   
   memset(&addr,0,sizeof(struct sockaddr_in));
   addr.sin_family = AF_INET;
   addr.sin_port = htons(atoi(argv[2]));
   inet_pton(AF_INET,argv[1],&addr.sin_addr);

   if(connect(sockfd,(struct sockaddr *)&addr,sizeof(addr))<0){ //connect
      perror("connect");
      exit(1);
   }

   i=100;
   printf("[minu] %x\n",i);
   write(sockfd,&i,sizeof(i));

   switch(fork()){	//다중연결처리를 위해서

      case -1:
         perror("fork error");
         exit(0);
      case 0:      
         while(1){
            if((recvLen=recv(sockfd, buf1, BUFLEN, 0)) == -1){
               printf("recv error");
               exit(1);
            }
            if(recvLen == 0){
               printf("서버 연결 해제\n");
               break;
            }
            buf1[recvLen] = '\0';

            printf("Server: %s\n", buf1);
         }
         break;
      default:
         while(1){
            fgets(buf2, BUFLEN, stdin);
            
            if(send(sockfd, buf2, strlen(buf2), 0) != strlen(buf2)){
               perror("send error");
               exit(1);
            }
         }
      
   }
   close(sockfd);
   exit(0);
}
