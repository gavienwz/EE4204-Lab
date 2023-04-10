#include "headsock.h"

void str_ser(int sockfd, struct sockaddr *addr, int addrlen); 

int main(void)
{
	int sockfd, ret;
	struct sockaddr_in my_addr;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);  
	if (sockfd <0)
	{
		printf("error in socket!");
		exit(1);
	}
	
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(MYUDP_PORT);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(my_addr.sin_zero), 8);
	ret = bind(sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr));                //bind socket
	if (ret <0)
	{
		printf("error in binding");
		exit(1);
	}
	
	//while (1)
	//{
		printf("waiting for data\n");
		str_ser(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr_in));

	//}
	close(sockfd);
	exit(0);
}

void str_ser(int sockfd, struct sockaddr *addr, int addrlen)
{
	char buf[BUFSIZE];
	FILE *fp;
	char recvs[3 * DATALEN];
	struct ack_so ack;
	int end = 0, n = 0;
	long lseek=0;
	
	printf("receiving data!\n");

	while(!end)
	{
		if ((n= recvfrom(sockfd, &recvs, DATALEN, 0, addr, (socklen_t *)&addrlen)) == -1)
		{
			printf("error when receiving\n");
			exit(1);
		}
		if (recvs[n-1] == '\0')									//if it is the end of the file
		{
			end = 1;
			n --;
		}
		memcpy((buf+lseek), recvs, n);
		lseek += n;
		for (int i = 1; i <= 3; i++) {
			for (int j = 1; j <= i; j++) {
				ack.num = 1;
				ack.len = 0;
				if ((n = sendto(sockfd, &ack, 2, 0, addr, addrlen)) == -1) {
					printf("send error!\n");
					exit(1);
				}
			}
		}
	}
	if ((fp = fopen ("myUDPreceive.txt","wt")) == NULL)
	{
		printf("File doesn't exit\n");
		exit(0);
	}
	fwrite (buf , 1 , lseek , fp);					//write data into file
	fclose(fp);
	printf("a file has been successfully received!\nthe total data received is %d bytes\n", (int)lseek);
}
