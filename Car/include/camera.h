#ifndef CAMERA_H
#define CAMERA_H



#define FALSE 0
#define TRUE 1

#define OFF 0		//开
#define ON 1		//关

//摄像头状态的枚举（关、开、运行）
enum Camera{Off, On, Run};

typedef struct
{
	uint8_t* start;
	size_t length;
} buffer_t;

typedef struct 
{
	int fd;
	uint32_t width;
	uint32_t height;
	size_t buffer_count;
	buffer_t* buffers;
	buffer_t head;
} camera_t;

int lcd_draw_jpg(unsigned int x,unsigned int y,const char *pjpg_path,char *pjpg_buf,unsigned int jpg_buf_size,unsigned int jpg_half);
int xioctl(int fd, int request, void* arg);
void camera_check(camera_t* camera);
camera_t* camera_open(const char * device, uint32_t width, uint32_t height);
void camera_init(camera_t* camera);
void camera_start(camera_t* camera);
void camera_stop(camera_t* camera);
void camera_finish(camera_t* camera);
void camera_close(camera_t* camera);
int camera_capture(camera_t* camera);
int camera_frame(camera_t* camera, struct timeval timeout);
void jpeg(FILE* dest, uint8_t* rgb, uint32_t width, uint32_t height, int quality);
int minmax(int min, int v, int max);
uint8_t* yuyv2rgb1(uint8_t* yuyv, uint32_t width, uint32_t height);
uint8_t* yuyv2rgb2(uint8_t* yuyv, uint32_t width, uint32_t height);
int yuyv2rgb(int y, int u, int v);
int yuyv2rgb0(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height);
void init_camera(void);
void close_Camera(void);




#endif