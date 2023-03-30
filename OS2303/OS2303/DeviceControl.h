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
	void initDC();//�豸��ʼ��
	void coct_chct();//��������ͨ���Ķ�Ӧ�б�
	void coct_busy();//�������Ƿ�ռ��
	//void addpre();//��Ҳ��֪��ΪʲôҪ�������������Ǹ����εģ��ʾ�����������
	int add(string newname, int newdevid, char newtype);//�����豸
	int del();//ɾ���豸
	int apply(int);//�����豸
	int MenuPrint();//���˵���ӡ
	void RunDeviceControl(int choose);
	void ShowDev();
	//void showstate();//�豸״̬�鿴
	//int DeviceController(char c, char name, char type, int pid = -1);
	//void DevicePint();//��ӡ�豸״̬
	void printfortest(string);//���������
	void printall();
};
class dct_Dev //�豸���Ʊ�
{
public:
	int pid;
	char type;							//�豸����
	int deviceid;						//�豸��ʶ��
	int state;							//�豸״̬���ȴ�/���ȴ�  æ/��0
	sdt_Dev* sdt;					//ϵͳ�豸
	coct_Dev* coct;				//ָ����������ָ��
	block_Dev* blockdct;		//�豸����ʧ����ɵ���������
	dct_Dev* first;					//�豸���еĶ���ָ��
};

class coct_Dev//�������б�
{
public:
	int name;//����������
	int state;//������״̬
	int chct;//�豸ͨ��
	block_Dev* blockco;//��������ռ����ɵ���������
};

class chct_Dev //ͨ��
{
public:
	int channelid;//ͨ����ʶ��
	char name; //ͨ�����ƣ�1,2����ͨ����
	int state; //ͨ��״̬
	struct block_Dev* blockch;//ָ��ͨ����ռ����ɵ���������
	struct chct_Dev* first;//ͨ������ָ��
	struct chct_Dev* last;//ͨ����βָ��
};

class sdt_Dev //ϵͳ�豸��
{
public:
	string name; //�豸����
	//char type; //�豸����
	int deviceid;//�豸��ʶ��
	//struct sdt_Dev* first;//�������ָ��
	sdt_Dev* next; //ָ����һ���豸��ָ��
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

class block_Dev //�����豸�б�
{
public:
	int pid;//���̱�ʶ��PCB����������̵�Ψһpid��
	//��ʵ����һ��global.h�����涨����pid���������ﲻ��Ҫ��������ļ�����Ҫ�Լ���дһ��pid
	string block_name;//�������豸��
	//dct_Dev* dct;
	block_Dev* next;//ָ�������б���һ�����������豸
};

class Resource
{
public:
	int RID;
	int ues_PID;
	int state;
};

//void DevicePint();//��ӡ�豸״̬
//int DeviceControler(char c,char name,char type,int pid=-1);