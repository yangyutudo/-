#include "main.h"

extern int lcd_fd;		//LCD文件描述符
extern int *mmap_p;		//LCD内存映射

//LCD显示屏初始化
int lcd_init(void)
{
	//打开LCD显示屏屏幕
	lcd_fd = open("/dev/fb0",O_RDWR);
	if(lcd_fd == -1)
	{
		printf("open lcd failure\n");
		return  -1;
	}
	//屏幕内存映射
	mmap_p = (unsigned int *)mmap(NULL, 800*480*4,PROT_READ| PROT_WRITE,MAP_SHARED,lcd_fd,0);
	if(mmap_p == NULL)
	{
		
		printf("mmap failure\n");
		return -1;
	}
	return 0;
	
}

//LCD显示屏的关闭
void close_lcd(void)
{
	close(lcd_fd);
	munmap(mmap_p,800*480*4);
}

//在任意位置显示任意大小的BMP图片：BMP图片名称、x坐标，y坐标
void show_bmp_any(char *bmp_name, int r_x, int r_y)
{
    // （1）打开BMP图片文件：open
    int bmp_fd = open(bmp_name, O_RDWR);       //以可读可写的方式去打开BMP图片
    if (bmp_fd < 0)
    {
        printf("open BMP error\n");
        return;
    }

    // （2）读取BMP图片的颜色数据：read
    char head[54] = {0};            //存储数据头信息

    //读取数据头信息
    read(bmp_fd, head, sizeof(head));

    //拿到BMP图片的宽度（18~21）和高度（22~25）
    int width = head[18] | head[19] << 8 | head[20] << 16 | head[21] << 24;      //拿到BMP图片的宽度
    int height = head[22] | head[23] << 8 | head[24] << 16 | head[25] << 24;      //拿到BMP图片的高度

    printf("BMP图片的宽度: %d, 高度: %d\n", width, height);

    if (r_x + width > 800 || r_y + height > 480 || width * 3 % 4 != 0)      //简单的判断BMP图片是否出错
    {
        printf("BMP error\n");
        return;
    }
    
    char buf[width*height*3];      //存放bmp图片颜色数据
        //读取BGR的颜色数据
    read(bmp_fd, buf, sizeof(buf));

    // （3）将处理好的颜色数据写入到LCD显示屏中：write
    int color[width*height];       //存放拼接好的颜色数据
    for (int i = 0; i < width*height; i++)
    {
        color[i] = buf[3*i] | buf[3*i+1] << 8 | buf[3*i+2] << 16;
                //     B        G << 8              R << 16
    }

    //将颜色数据进行翻转并使用内存映射写入到虚拟内存（写入到LCD显示屏）
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            //进行数据的翻转并写入到虚拟内存
            mmap_p[(y+r_y)*800 + x + r_x] = color[(height-1-y)*width+x];
        }
    }

    //关闭BMP图片
    close(bmp_fd);

}




