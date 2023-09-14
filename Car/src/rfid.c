﻿#include "main.h"

#define DEV_PATH   "/dev/ttySAC2"      	//串口设备定义

extern bool cardOn;					//card打开或者关闭标志位
extern int tty_fd;        				//串口的文件描述符

//初始化串口
void init_tty(void)
{    
	//1、打开开发板串口
	tty_fd = open(DEV_PATH, O_RDWR | O_NOCTTY);
	if(tty_fd == -1)
	{
		printf("open %s error\n", DEV_PATH);
		return;
	}
	//声明设置串口的结构体
	struct termios config;
	bzero(&config, sizeof(config));

	// 设置无奇偶校验
	// 设置数据位为8位
	// 设置为非规范模式（对比与控制终端）
	cfmakeraw(&config);

	//设置波特率
	cfsetispeed(&config, B9600);
	cfsetospeed(&config, B9600);

	// CLOCAL和CREAD分别用于本地连接和接受使能
	// 首先要通过位掩码的方式激活这两个选项。    
	config.c_cflag |= CLOCAL | CREAD;

	// 一位停止位
	config.c_cflag &= ~CSTOPB;

	// 可设置接收字符和等待时间，无特殊要求可以将其设置为0
	config.c_cc[VTIME] = 0;
	config.c_cc[VMIN] = 1;

	// 用于清空输入/输出缓冲区
	tcflush (tty_fd, TCIFLUSH);
	tcflush (tty_fd, TCOFLUSH);

	//完成配置后，可以使用以下函数激活串口设置
	if(tcsetattr(tty_fd, TCSANOW, &config) != 0)
	{
		perror("设置串口失败");
		exit(0);
	}
}

//不断发送A指令（请求RFID卡），一旦探测到卡片就退出
void request_card(int fd)
{
	init_REQUEST();
	char recvinfo[128];
	while(1)
	{
		// 向串口发送指令
		tcflush(fd, TCIFLUSH);

		//发送请求指令
		write(fd, PiccRequest_IDLE, PiccRequest_IDLE[0]);

		usleep(50*1000);

		bzero(recvinfo, 128);
		if(read(fd, recvinfo, 128) == -1)
			continue;

		//应答帧状态部分为0 则请求成功
		if(recvinfo[2] == 0x00)	
		{
			cardOn = true;
			break;
		}
		cardOn = false;
	}
}

//获取RFID卡号
int get_id(int fd)
{
	// 刷新串口缓冲区
	tcflush (fd, TCIFLUSH);

	// 初始化获取ID指令并发送给读卡器
	init_ANTICOLL();

	//发送防碰撞（一级）指令
	write(fd, PiccAnticoll1, PiccAnticoll1[0]);

	usleep(50*1000);

	// 获取读卡器的返回值
	char info[256];
	bzero(info, 256);
	read(fd, info, 128);

	// 应答帧状态部分为0 则成功
	uint32_t id = 0;
	if(info[2] == 0x00) 
	{
		memcpy(&id, &info[4], info[3]);
		if(id == 0)		
		{
			return -1;
		}
	}else
	{
		return -1;
	}
	return id;
}



// int rfid_into(void)
// {
// 	int x = -1, y = -1, cardid;        //存放xy的坐标，函数返回值，注册的rfid卡号
// 	while(1)
// 	{	

// 		printf("waiting card......\n");
// 		//4、检测附近是否有卡片
// 		request_card(tty_fd);
// 		//5、获取卡号
// 		cardid = get_id(tty_fd);
// 		// 忽略非法卡号
// 		if(cardid == 0 || cardid == 0xFFFFFFFF)
// 			continue;

// 		printf("RFID卡号: %x\n", cardid);
// 		usleep(500000);	//延时0.5秒

// 		if(cardid != 0) 
// 			break;
        		
// 	}
// 	close(tty_fd);
// 	return cardid;
// }
