#include "main.h"    //存放所有头文件

int cardid = 0, car_num = 0;
int car_arry[20]={0};

pid_t tid;              //存放子进程ID
int touch_fd = -1;      //触摸屏的文件描述符 
int lcd_fd = -1;		//LCD文件描述符
int *mmap_p;		    //LCD内存映射

int Shot =  OFF;			//拍照的标准：默认没有按下拍照
int Camera_state = On;		//摄像头状态，默认打开状态（打开 ！= 运行）
struct timeval timeout;		//设置超时时间
camera_t* camera;			//摄像头结构体指针变量
unsigned char* rgb; 		//开辟存放RGB颜色数据内存：640*480*3

bool cardOn = false;		//card打开或者关闭标志位
int tty_fd;        			//串口的文件描述符

//音频线程
// void *music(void *arg) 
// { 
//     system("madplay in.mp3 -a 18");
// } 

//车牌识别系统的主功能函数
int car_system(void)
{
	int x = -1, y = -1, type;       //存放触摸屏坐标, 用户的滑动类型

	Camera_state = Run;		//运行摄像头，获取摄像头的画面并且显示到开发板上
	//显示开屏界面
	printf("\n*******[车牌识别系统主功能界面]*******\n");
    //显示功能按键
    show_bmp_any("menu.bmp", 0, 0);		//系统界面
    system("madplay welcome.mp3 -a 18");
    usleep(500000);
    while (1)
    {
       //获取触摸屏的坐标(滑屏算法)
        type = get_slide_xy(&x, &y);
        if (x >= 0 && x < 800 && y >= 0 && y < 480)     //确保坐标准确性
        {
            printf("【点击】(%d,%d)\n",x,y);  
        }

		if (x > 640 && x < 789 && y > 7 && y < 122)		//入库按键
		{
			printf("【车辆入库】\n");
            //音乐线程
                // pthread_t music_tid;
                // pthread_create(&music_tid, NULL, music, NULL);
            system("madplay in.mp3 -a 18");
			//车辆入库的具体功能代码
            while(1)
            {	
                printf("waiting card......\n");
                //检测附近是否有卡片
                request_card(tty_fd);
                //获取卡号
                cardid = get_id(tty_fd);
                // 忽略非法卡号
                if(cardid == 0 || cardid == 0xFFFFFFFF)continue;

                printf("正在入库，RFID卡号: %x\n", cardid);
                usleep(500000);	//延时0.5秒
                
                //查找数据组中有无该车辆卡号
                int length = sizeof(car_arry) / sizeof(car_arry[0]);  // 计算数组长度
                int found = 0;  // 标记是否找到目标整数
                for(int i = 0; i < length; i++) 
                {
                    if(car_arry[i] ==  cardid) 
                    {
                    found = 1;
                    break;
                    }
                }

                
                if(found == 1)
                {
                    printf("库中有该id卡号，失败\n");
                    usleep(500000);
                    system("madplay infailure.mp3 -a 18");
                    break;
                }
                usleep(500000);
                //入库数据
                if(cardid != 0 && found == 0) 
                {
                    //存储id数据
                    car_num++;
                    car_arry[car_num-1] = cardid;
                    printf("车辆加一，当前车id为：%x,车数量：%d\n",car_arry[car_num-1],car_num);
                    usleep(500000);
                    //抓拍车牌数据
                    Shot = ON;
                    if (Shot)	//获取一帧画面保存为jpg图片
                    {
                        FILE* out = fopen("car.png","w");
                        //将RGB转为jpg
                        jpeg(out, rgb,camera->width, camera->height, 100);
                        fclose(out);
                        Shot = OFF;
                        system("sz car.png");		//将获取到图片下载Windows中
                    }
                    printf("车辆入库中");
                    kill(tid, SIGUSR1);	    //发送入库车牌识别的信号
                    break;
                }                                              
            }

		}else if (x > 640 && x < 789 && y > 192 && y < 310)		//出库按键
		{
            system("madplay out.mp3 -a 18");
			printf("【车辆出库】\n");
			//车辆出库的具体功能代码
            while(1)
            {	
                printf("waiting card......\n");
                //4、检测附近是否有卡片
                request_card(tty_fd);
                //5、获取卡号
                cardid = get_id(tty_fd);
                // 忽略非法卡号
                if(cardid == 0 || cardid == 0xFFFFFFFF)continue;

                printf("正在出库，RFID卡号: %x\n", cardid);
                usleep(500000);	//延时0.5秒

                //查找数据组中有无该车辆卡号
                int length = sizeof(car_arry) / sizeof(car_arry[0]);  // 计算数组长度
                int found = 0;  // 标记是否找到目标整数
                for(int i = 0; i < length; i++) 
                {
                    if(car_arry[i] ==  cardid) 
                    {
                    found = 1;
                    break;
                    }
                }

                if(found == 0)
                {
                    printf("库里无该卡号，错误\n");
                    usleep(500000);
                    system("madplay outfailure.mp3 -a 18");
                    break;
                }
                if(cardid != 0 && found == 1) 
                {
                    //id数据
                    car_arry[car_num-1] = 0;
                    car_num--;
                    printf("车辆减一，当前车id为：%x,车数量：%d\n",cardid,car_num);
                    usleep(500000);
                    //车牌数据处理
                    Shot = ON;
                    if (Shot)	//获取一帧画面保存为jpg图片
                    {
                        FILE* out = fopen("car.png","w");
                        //将RGB转为jpg
                        jpeg(out, rgb,camera->width, camera->height, 100);
                        fclose(out);
                        Shot = OFF;
                        system("sz car.png");		//将获取到图片下载Windows中
                    }
                    printf("车辆出库中");
                    kill(tid, SIGUSR2);	    //发送入库车牌识别的信号
                    break;
                }                                              
            }
		}else if (x > 640 && x < 785 && y > 382 && y < 470)		//锁屏按键
		{
			printf("【锁屏】\n");
			break;
		}	
	  
	}
	
    Camera_state = On;		//停止获取摄像头画面，
	usleep(500000);		    //延时500000微妙
	return cardid;
}


//摄像头显示线程的执行函数
void *show_camera(void *arg)
{
    while (Camera_state)    
    {	
		//表示摄像头的执行（显示画面）
		if (Camera_state == Run)    //
		{
			//取一帧数据
			camera_frame(camera, timeout);
			//将YUV转化为RGB
			yuyv2rgb0(camera->head.start, rgb, camera->width, camera->height);
            
            
            // if (Shot)	//获取一帧画面保存为jpg图片
            // {
            //     FILE* out = fopen("car.png","w");
            //     //将RGB转为jpg
            //     jpeg(out, rgb,camera->width, camera->height, 100);
            //     fclose(out);
            //     Shot = OFF;
            //     system("sz car.png");		//将获取到图片下载Windows中
            // }

			//映射显示
			for(int y=0; y<camera->height; y++)  //480
			{
				for(int x=0; x<camera->width; x++)  //0~640
				{
					*(mmap_p+ y*800 + x)= rgb[3*y*camera->width +3*x ]<<16 | rgb[3*y*camera->width +3*x +1]<<8 | rgb[3*y*camera->width +3*x +2]; 
				}
				
			}
		
		}
	
    }

    pthread_exit("1");      //退出当前线程
}


//主函数
int main(int argc, char const *argv[])
{
    
    //初始化LCD
	lcd_init();
	//初始化触摸屏
    init_touch();
	//初始化摄像头
	init_camera();
	//打开、并初始化串口
	init_tty();
    
    //创建一个新的线程，不断的读取管道文件中的数据（车牌数据）
	pthread_t sqlite_tid;
	pthread_create(&sqlite_tid, NULL, fun_sqlite, NULL);

    pthread_t camera_tid;      //显示摄像头画面线程的ID号
    //创建一个线程并且执行conut函数
    pthread_create(&camera_tid, NULL, show_camera, NULL);

    
    

    //使用fork函数创建新的进程执行车牌识别的函数
    tid = fork();
    if(tid == 0)        //子进程（执行车牌识别的程序）
    {
        printf("正在启动车牌识别程序....\n");
		execl("./alpr", "alpr", NULL);          //执行alpr车牌设备程序
		printf("启动车牌识别失败! ! !\n");
		exit(0);
    }

    int x = -1, y = -1, type;       //存放触摸屏坐标, 用户的滑动类型

    printf("\n******[欢迎使用车牌识别系统]*******\n");

    system("mplayer -quiet -zoom -x 800 -y 480 -speed 3 begin.avi ");

    //在任意位置显示任意大小的BMP图片：图片名、x坐标、y坐标
	show_bmp_any("suoping.bmp", 0, 0);		//锁屏界面	

    while (1)       //卡住程序不让退出
    {
        //获取触摸屏的坐标(滑屏算法)
        type = get_slide_xy(&x, &y);
        if (x >= 0 && x < 800 && y >= 0 && y < 480)     //确保坐标准确性
        {
            printf("【点击】(%d,%d)\n",x,y);  
			continue;
        }
        
        if (type == UP)     //上滑
        {
            printf("【上滑】: [进入车牌识别系统主功能界面]\n");
            
			//RFID系统的主功能函数
			car_system();
        }else if (type == DOWN)     //下滑
        {
            printf("【下滑】: [退出程序]\n");
            Camera_state = Off;		    //关闭摄像头
			break;
        }else
		{
			continue;
		}

		//在任意位置显示任意大小的BMP图片：图片名、x坐标、y坐标
		show_bmp_any("suoping.bmp", 0, 0);		//锁屏界面	
    }


    //关闭LCD显示屏、内存映射、触摸屏、串口\摄像头
    pthread_join(camera_tid, NULL);
    close_lcd();
    close(touch_fd);
	close(tty_fd);
    close_Camera();
    
    
    return 0;
}
