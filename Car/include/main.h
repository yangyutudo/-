#ifndef MAIN_H
#define MAIN_H

//存放所有头文件

//系统头文件
#include <time.h>
#include <wait.h>
#include <math.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <linux/fb.h>
#include <linux/input.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <setjmp.h>
#include <assert.h>
#include <termios.h> 
#include <sys/select.h>
#include <netdb.h>
#include <pthread.h>

//自定义或者引用头文件
#include "ISO14443A.h"
#include "jpeglib.h"
#include "jconfig.h"
#include "mySQLite.h"
#include "touch.h"
#include "camera.h"
#include "rfid.h"
#include "lcd.h"

#endif
