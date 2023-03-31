#pragma once
#include<iostream>
#include<string>
#include<vector>
using namespace std;

class DeviceControl;
class dct_Dev;
class coct_Dev;
class chct_Dev;
class sdt_Dev;
class block_Dev;
class Resource;
class Resource_control;
class Res_Schedule;

class DeviceControl
{
public:
	sdt_Dev* sdt;//设备
	sdt_Dev* first;//设备列表的头结点
	dct_Dev* dct;//设备控制
	coct_Dev* coct;//设备控制器链表
	chct_Dev* chct1, * chct2;//通道1，通道2

	int DeviceNum = 7;//设备数，初始为7
	int ControllerNum = 4;//控制器数，初始为4
	int Controller[30];//控制器与通道对应表
	int Controller_busy[30];//控制器是否被占用的表

	int countId = 1;

	void initDC();//设备初始化
	void coct_chct();//控制器与通道的对应列表
	void coct_busy();//控制器是否被占用
	int add();//增加设备
	int del();//删除设备
	int apply(int);//请求设备
	int MenuPrint();//主菜单打印
	void RunDeviceControl(int choose);
	void ShowDev();
	void printfortest(string);//测试用输出
	void printall();
	void DeviceInit(string name, int id, char type, sdt_Dev*& sdt, int CName, int PName);

};

class dct_Dev //设备控制表
{
public:
	int pid;
	char type;//设备类型
	int deviceid;//设备标识符
	int state;//设备状态：等待/不等待  忙/闲0
	sdt_Dev* sdt;//系统设备
	coct_Dev* coct;//指向控制器表的指针
	//block_Dev* blockdct;//设备请求失败造成的阻塞队列******&&
	dct_Dev* first;//设备队列的队首指针
};

class coct_Dev//控制器列表
{
public:
	int name;//控制器名称
	int state;//控制器状态
	int chct;//设备通道
	//block_Dev* blockco;//控制器被占用造成的阻塞队列****&&
};

class chct_Dev //通道
{
public:
	int channelid;//通道标识符
	char name; //通道名称（1,2两个通道）
	int state; //通道状态
	//block_Dev* blockch;//指向通道被占用造成的阻塞队列********&&
	chct_Dev* first;//通道队首指针
	chct_Dev* last;//通道队尾指针
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

class Res_Schedule:public DeviceControl //资源调度列表
{
public:
	vector<int> pid_list;//排队进程标识
	int pid_occupy;
	int rtype;//资源类型
	int rid;//资源id
	int state;//资源状态
	//DeviceControl DevCon;
	//static vector<Res_Schedule> ResList;
	//Res_Schedule* ResList;
	Res_Schedule();
	void ResInit(DeviceControl*& Dev);
	int ResApply(int pid,int rid);
	void printBlock();
};

class Resource {
public:
	//临界资源
	int RID;
	int PID;
	int status;
	int Rtype;
	//RType---0:无交互功能的资源，占用后直接进入运行态，
	//1:有交互功能的设备功能，需要阻塞等待用户输入，
	Resource() {
		RID = 0;
		Rtype = 0;
		PID = 0;
		status = 0;
	}
};

class Resource_control {
public:
	int RID_counter;									//RID计数
	vector<Resource> apply_list;
	//请求list，记得用完了pop_front()把表清空
	vector<Resource> update_list;						//更新list，用于将设备模块的更新接入
	vector<Resource> resource_list;						//资源列表
	void create_resource(int Rtype);				//创建新的Recource对象
	void init_list();
	Resource_control() {
		RID_counter = 0;
	}
};
