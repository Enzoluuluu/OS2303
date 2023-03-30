#pragma once
#include<iostream>
#include<string>
using namespace std;

class DeviceControl;
class dct_Dev;
class coct_Dev;
class chct_Dev;
class sdt_Dev;
class block_Dev;
class Resource;

class DeviceControl
{
public:
	void initDC();//设备初始化
	void coct_chct();//控制器与通道的对应列表
	void coct_busy();//控制器是否被占用
	//void addpre();//我也不知道为什么要加这个，这个就是个传参的，问就是我垃圾吧
	int add(string newname, int newdevid, char newtype);//增加设备
	int del();//删除设备
	int apply(int);//请求设备
	int MenuPrint();//主菜单打印
	void RunDeviceControl(int choose);
	void ShowDev();
	//void showstate();//设备状态查看
	//int DeviceController(char c, char name, char type, int pid = -1);
	//void DevicePint();//打印设备状态
	void printfortest(string);//测试用输出
	void printall();
};
class dct_Dev //设备控制表
{
public:
	int pid;
	char type;							//设备类型
	int deviceid;						//设备标识符
	int state;							//设备状态：等待/不等待  忙/闲0
	sdt_Dev* sdt;					//系统设备
	coct_Dev* coct;				//指向控制器表的指针
	block_Dev* blockdct;		//设备请求失败造成的阻塞队列
	dct_Dev* first;					//设备队列的队首指针
};

class coct_Dev//控制器列表
{
public:
	int name;//控制器名称
	int state;//控制器状态
	int chct;//设备通道
	block_Dev* blockco;//控制器被占用造成的阻塞队列
};

class chct_Dev //通道
{
public:
	int channelid;//通道标识符
	char name; //通道名称（1,2两个通道）
	int state; //通道状态
	struct block_Dev* blockch;//指向通道被占用造成的阻塞队列
	struct chct_Dev* first;//通道队首指针
	struct chct_Dev* last;//通道队尾指针
};

class sdt_Dev //系统设备表
{
public:
	string name; //设备名称
	//char type; //设备类型
	int deviceid;//设备标识符
	//struct sdt_Dev* first;//保存队首指针
	sdt_Dev* next; //指向下一个设备的指针
	dct_Dev* dct;

	sdt_Dev(){
		name = "";
		deviceid = -1;
		//first = nullptr;
		next = nullptr;
		dct = nullptr;
	}
	sdt_Dev(string _name):name(_name) {
		deviceid = -1;
		//first = nullptr;
		next = nullptr;
		dct = nullptr;
	}
};

class block_Dev //阻塞设备列表
{
public:
	int pid;//进程标识（PCB里面包含进程的唯一pid）
	//其实还有一个global.h，里面定义了pid，但是这里不需要调用这个文件，需要自己再写一个pid
	string block_name;//阻塞的设备名
	//dct_Dev* dct;
	block_Dev* next;//指向阻塞列表下一个被阻塞的设备
};

class Resource
{
public:
	int RID;
	int ues_PID;
	int state;
};

//void DevicePint();//打印设备状态
//int DeviceControler(char c,char name,char type,int pid=-1);