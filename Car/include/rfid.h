#ifndef RFID_H
#define RFID_H

//初始化串口
void init_tty(void);
//不断发送A指令（请求RFID卡），一旦探测到卡片就退出
void request_card(int fd);
//获取RFID卡号
int get_id(int fd);



#endif