#include "ondi.h"



void do_register(int sockfd, MSG *msg);
int do_login(int sockfd, MSG *msg);
int do_query(int sockfd, MSG *msg);
int do_history(int sockfd, MSG *msg);
void getpwd(char *pwd, int pwdlen);

int main(int argc, const char *argv[])
{
	int sockfd;
	struct sockaddr_in serv_addr;
	int n;
	MSG msg;


	if(argc != 3){
		printf("Usage:%s <serverip> <port>.\n", argv[0]);
		exit(0);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		perror("sockfd");
		exit(0);
	}

	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		perror("connect");
		exit(0);
	}

	while(1){
		printf("********************\n");
		printf("1.注册 2.登录 3.退出\n");
		printf("********************\n");
		printf("请选择:");
		scanf("%d", &n);
		getchar();

		switch(n) {
		case 1:
			do_register(sockfd, &msg);
			break;
		case 2:
			if(do_login(sockfd, &msg) == 1){
				goto NEXT;
			}
			break;
		case 3:
			close(sockfd);
			exit(1);
			break;
		default:
			printf("输入无效\n");
		}
	}

NEXT:
	while(1){
		printf("********************\n");
		printf("1.查询 2.历史 3.退出\n");
		printf("********************\n");
		printf("请选择:");
		scanf("%d", &n);
		getchar();

		switch(n) {
		case 1:
			do_query(sockfd, &msg);
			break;
		case 2:
			do_history(sockfd, &msg);
			break;
		case 3:
			close(sockfd);
			exit(1);
			break;
		default:
			printf("输入无效\n");
		}
	}


	return 0;
}


/*注册模块*/
void do_register(int sockfd, MSG *msg){
	msg->type = R;
	printf("输入用户名:");
	scanf("%s", msg->name);
	getchar();
	//输入密码不可见
	char *password = getpass("输入密码:");
	strcpy(msg->data, password);

	//将用户名和密码传递给服务端
	if(send(sockfd, msg, sizeof(MSG), 0) < 0) {
		perror("send");
		exit(0);
	}
	//接收服务端发来的反馈
	if(recv(sockfd, msg, sizeof(MSG), 0) < 0) {
		perror("recv");
		exit(0);
	}

	printf("%s\n", msg->data);

	return;
}

/*登录模块*/
int do_login(int sockfd, MSG *msg){
	msg->type = L;
	printf("输入用户名:");
	scanf("%s", msg->name);
	getchar();
	//使输入密码不可见
	char *password = getpass("输入密码:");
	strcpy(msg->data, password);

	//将用户名和密码传递给服务端
	if(send(sockfd, msg, sizeof(MSG), 0) < 0) {
		perror("send");
		return -1;
	}
	//接收服务端发来的反馈
	if(recv(sockfd, msg, sizeof(MSG), 0) < 0) {
		perror("recv");
		return -1;
	}
	if(strncmp(msg->data, "OK", 3) == 0){
		printf("登录成功！\n");
		return 1;
	}else{
		printf("%s\n", msg->data);
	}

	return 0;
}

int do_query(int sockfd, MSG *msg){
	msg->type = Q;

	printf("输入＃退出查询！\n");
	while(1){
		printf("输入查询的单词:");
		scanf("%s", msg->data);
		getchar();

		//输入#号返回上一级菜单
		if(strncmp(msg->data , "#", 1) == 0)
			break;

		//	printf("%d\n", msg->type);
		/*将要查询的单词发送至服务端*/
		if(send(sockfd, msg, sizeof(MSG), 0) < 0){
			perror("send");
			return -1;
		}

		/*等待接受服务器传递回来的单词信息*/
		if(recv(sockfd, msg, sizeof(MSG), 0) < 0){
			perror("recv");
			return -1;
		}

		/*打印接受到的单词信息*/
		printf("%s\n", msg->data);
	}
	return 0;
}

int do_history(int sockfd, MSG *msg){
	msg->type = H;

	/*发送消息类型给服务端*/
	if(send(sockfd, msg, sizeof(MSG), 0) < 0) {
		perror("send");
		return -1;
	}

	while(1){
		/*接收服务端发来的历史记录*/
		if(recv(sockfd, msg, sizeof(MSG), 0) < 0) {
			perror("recv");
			return -1;
		}

		if(msg->data[0] == '\0')
			break;

		/*输出历史记录信息*/
		printf("%s\n", msg->data);
	}
	return 0;
}
