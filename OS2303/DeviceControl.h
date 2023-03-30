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

class DeviceControl
{
public:
	void initDC();//�豸��ʼ��
	void coct_chct();//��������ͨ���Ķ�Ӧ�б�
	void coct_busy();//�������Ƿ�ռ��
	int add();//�����豸
	int del();//ɾ���豸
	int apply(int);//�����豸
	int MenuPrint();//���˵���ӡ
	void RunDeviceControl(int choose);
	void ShowDev();
	void printfortest(string);//���������
	void printall();
	void DeviceInit(string name, int id, char type, sdt_Dev*& sdt, int CName, int PName);


};

class dct_Dev //�豸���Ʊ�
{
public:
	int pid;
	char type;//�豸����
	int deviceid;//�豸��ʶ��
	int state;//�豸״̬���ȴ�/���ȴ�  æ/��0
	sdt_Dev* sdt;//ϵͳ�豸
	coct_Dev* coct;//ָ����������ָ��
	block_Dev* blockdct;//�豸����ʧ����ɵ���������
	dct_Dev* first;//�豸���еĶ���ָ��
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
	block_Dev* blockch;//ָ��ͨ����ռ����ɵ���������
	chct_Dev* first;//ͨ������ָ��
	chct_Dev* last;//ͨ����βָ��
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
	block_Dev* next;//ָ�������б���һ�����������豸
};

class Resource {
public:
	//�ٽ���Դ
	int RID;
	int PID;
	int status;
	int Rtype;
	//RType---0:�޽������ܵ���Դ��ռ�ú�ֱ�ӽ�������̬��1:�н������ܵ��豸���ܣ���Ҫ�����ȴ��û�����
	Resource() {
		RID = 0;
		Rtype = 0;
		PID = 0;
		status = 0;
	}
};

class Resource_control {
public:
	int RID_counter;									//RID����
	vector<Resource> apply_list;
	//����list���ǵ�������pop_front()�ѱ����
	vector<Resource> update_list;						//����list�����ڽ��豸ģ��ĸ��½���
	vector<Resource> resource_list;						//��Դ�б�
	void create_resource(int Rtype);				//�����µ�Recource����
	void init_list();
	Resource_control() {
		RID_counter = 0;
	}
};
