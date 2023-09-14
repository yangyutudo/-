#include "include/Pipeline.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>

using namespace std;
int in_fd;		//入库管道文件描述符
int out_fd;		//出库管道文件描述符

//入库执行函数
void in_car(int sig)
{
	cout<<"【收到入库识别信号】"<<endl;
	    // 加载数据模型
	pr::PipelinePR prc("model/cascade.xml",
	"model/HorizonalFinemapping.prototxt",
	"model/HorizonalFinemapping.caffemodel",
	"model/Segmentation.prototxt",
    "model/Segmentation.caffemodel",
    "model/CharacterRecognization.prototxt",
	"model/CharacterRecognization.caffemodel",
	"model/SegmenationFree-Inception.prototxt",
	"model/SegmenationFree-Inception.caffemodel");
	cout<<"【加载车牌识别数据模型完成】"<<endl;

	string pn;
	// 读取一张图片（支持BMP、JPG、PNG等）
	cv::Mat image = cv::imread("car.jpg");
	std::vector<pr::PlateInfo> res;

	// 尝试识别图片中的车牌信息
	// 若图片中不含车牌信息，接口会抛出异常
	try{
		res = prc.RunPiplineAsImage(image,pr::SEGMENTATION_FREE_METHOD);
	}
	catch(...)
	{
		cout << "【未检测到车牌】" << endl;
		return ;
	}
	for(auto st:res)
	{
		pn = st.getPlateName();
		if(pn.length() == 9)
		{
			cout << "检测到车牌: " << pn.data();
			cout << "，确信率: "   << st.confidence << endl;
			
			if(st.confidence > 0.8)
			{
				write(in_fd, pn.data(), 9);
				cout << "【入库车牌发送完成】" << endl;
				return ;
			}
		}
	}
	write(in_fd, "err", 3);
}

//出库执行函数
void out_car(int sig)
{
	cout<<"【收到出库识别信号】"<<endl;

	// 加载数据模型
	pr::PipelinePR prc("model/cascade.xml",
	"model/HorizonalFinemapping.prototxt",
	"model/HorizonalFinemapping.caffemodel",
	"model/Segmentation.prototxt",
    "model/Segmentation.caffemodel",
    "model/CharacterRecognization.prototxt",
	"model/CharacterRecognization.caffemodel",
	"model/SegmenationFree-Inception.prototxt",
	"model/SegmenationFree-Inception.caffemodel");
	cout<<"【加载车牌识别数据模型完成】"<<endl;
	string pn;
	
	// 读取一张图片（支持BMP、JPG、PNG等）
	cv::Mat image = cv::imread("car.jpg");
	std::vector<pr::PlateInfo> res;

	// 尝试识别图片中的车牌信息
	// 若图片中不含车牌信息，接口会抛出异常
	try{
		res = prc.RunPiplineAsImage(image,pr::SEGMENTATION_FREE_METHOD);
	}
	catch(...)
	{
		cout << "【未检测到车牌】" << endl;
		return ;
	}
	for(auto st:res)
	{
		pn = st.getPlateName();
		if(pn.length() == 9)
		{
			cout << "检测到车牌: " << pn.data();
			cout << "，确信率: "   << st.confidence << endl;
			
			if(st.confidence > 0.8)
			{
				write(out_fd, pn.data(), 9);
				cout << "【出库车牌发送完成】" << endl;
				return ;
			}
		}
	}
	write(out_fd, "err", 3);
}

//车牌识别主功能函数
int main(int argc,char **argv)
{
	printf("[车牌识别程序ALPR启动成功】......\n");
	
	//注册信号，每次收到SIGUSR1信号，就调用一次in_car函数
	signal(SIGUSR1, in_car);
	//注册信号，每次收到SIGUSR2信号，就调用一次out_car函数
	signal(SIGUSR2, out_car);

	//打开入库管道文件
	in_fd = open("/tmp/LPR2SQLitIn", O_RDWR);
	if(in_fd == -1)
	{
		printf("open /tmp/LPR2SQLitIn err!\n");
	}
	
	//打开出库管道文件
	out_fd = open("/tmp/LPR2SQLitOut", O_RDWR);
	if(out_fd == -1)
	{
		printf("open /tmp/LPR2SQLitOut err!\n");
	}

	while(1)		//卡住程序不让其退出
	{
		sleep(1);
	}

	return 0;
}
