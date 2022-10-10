#ifndef __ONLIN__
#define __ONLIN__

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <signal.h>
#include <sqlite3.h>
#include <string.h>
#include <time.h>
#include <curses.h>

#define N 64

#define R 1 	//注册
#define L 2 	//登录
#define Q 3 	//查询
#define H 4 	//历史记录

typedef struct{
	int type; 		//数据类型
	char name[N]; 	//姓名
	char data[256]; //密码或者查询的单词
}MSG; 				//消息结构体


#endif




