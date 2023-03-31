#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<string>
#include"DeviceControl.h"
using namespace std;

#define INITDEVICENUM 7
#define RES_LENTH 30
#define RES_OCCUPIED 1
#define RES_IDLE 0
#define RES_NOTEXIST 2

//devlist:设备id、设备名、设备类型、设备状态、控制器、通道;
vector<Res_Schedule> ResList;
DeviceControl* Dev = new DeviceControl();

int main()
{
	Dev->coct_chct();
	Dev->initDC();
	/*//设备管理测试
	while (true) {
		int choice = Dev->MenuPrint();
		Dev->RunDeviceControl(choice);
		if (choice == 5)
			break;
	}*/
	
	Dev->ShowDev();
	//资源管理测试
	Res_Schedule* Res = new Res_Schedule();
	//vector<Res_Schedule> ResList = new Res_Schedule;
	Res->ResInit(Dev);
	Res->ResApply(123, 5);
	Res->ResApply(456, 4);
	Res->ResApply(789, 5);
	Res->ResApply(59, 5);
	Res->printBlock();
	
	//通用测试
	/*string name;
	printf("请输入设查询备名称：");
	getline(cin, name);
	printfortest(name);
	printall();*/
	
	//add测试
	/*int flag = add();
	if (flag == 1) {
		cout << "添加设备成功！" << endl;
		printfortest("Mouse", first);
	}
	else
		cout << "添加设备失败！" << endl;*/
	
	//apply测试
	/*apply(1);//进程id是随便写的，进程写完之后配合进程再修改
	apply(1);
	printfortest("Screen");*/

	//delete测试
	/*del();
	printall();
	printfortest("Screen");*/
	
	return 0;
}
//设备初始化
void DeviceControl::initDC()
{
	//blockdev = new (block_Dev);//定义阻塞队列****************
	
	sdt_Dev* newNode = nullptr;

	//初始化通道
	chct1 = new (chct_Dev);
	chct1->name = '1';
	chct1->state = 0;//初始状态空闲
	chct2 = new (chct_Dev);
	chct2->name = '2';
	chct2->state = '0';

	//初始化控制器链表，链表的name指的是控制器x，后期需要添加就用尾插加
	first= new (sdt_Dev);
	//sdt = first;
	//sdt = sdt->next;
	//头指针不存设备，设备从第二个空间开始存
	sdt = new (sdt_Dev);
	first->next = sdt;
	
	DeviceInit("KeyBoard", countId++, 'I', sdt, 1, 1);
	newNode = new(sdt_Dev);
	sdt->next = newNode;
	sdt = sdt->next;

	DeviceInit("Mouse", countId++, 'I', sdt, 2, 1);
	newNode = new(sdt_Dev);
	sdt->next = newNode;
	sdt = sdt->next;

	DeviceInit("USB", countId++, 'O', sdt, 3, 2);
	newNode = new(sdt_Dev);
	sdt->next = newNode;
	sdt = sdt->next;

	DeviceInit("Screen2", countId++, 'O', sdt, 1, 1);
	newNode = new(sdt_Dev);
	sdt->next = newNode;
	sdt = sdt->next;

	DeviceInit("Screen1", countId++, 'O', sdt, 2, 1);
	newNode = new(sdt_Dev);
	sdt->next = newNode;
	sdt = sdt->next;

	DeviceInit("Speaker", countId++, 'O', sdt, 4, 2);
	newNode = new(sdt_Dev);
	sdt->next = newNode;
	sdt = sdt->next;

	DeviceInit("Microphone", countId++, 'O', sdt, 4, 2);
	sdt->next = nullptr;
}

void DeviceControl::DeviceInit(string name, int id, char type, sdt_Dev * &sdt, int CName, int PName) {
	sdt->name = name;
	sdt->deviceid = id;
	sdt->dct = new (dct_Dev);
	//sdt->dct->blockdct = new(block_Dev);**********
	sdt->dct->type = type;
	sdt->dct->state = 0;
	sdt->dct->pid = 0;
	sdt->dct->coct = new (coct_Dev);
	sdt->dct->coct->state = 0;
	sdt->dct->coct->name = CName;
	sdt->dct->coct->chct = PName;
}

void DeviceControl::coct_busy()
{
	for (int i = 0; i < 30; i++)
		Controller_busy[i] = 0;//初始状态下控制器都是空闲
}

void DeviceControl::printfortest(string Devname)
{
	int i = 0;
	sdt_Dev *temp = new (sdt_Dev);
	temp = first;
	while (i<=DeviceNum)
	{
		//cout << temp->name << endl;
		if (temp->name == Devname)
			break;
		//cout << i << endl;
		temp = temp->next;
		i++;
	}
	if (i > DeviceNum) {
		printf("设备不存在！\n");
	}
	else {
		printf("设备名称：%s\n", Devname.c_str());
		printf("设备类型：%c\t设备状态：%d\t使用控制器:控制器%d\t使用通道：通道%d\n",
			temp->dct->type,temp->dct->state,temp->dct->coct->name,temp->dct->coct->chct);
	}
}

void DeviceControl::printall()
{
	sdt_Dev* temp = new (sdt_Dev);
	temp = first;
	while (temp != NULL) {
		cout << temp->name << endl;
		temp = temp->next;
	}
}

//添加设备,传入设备名称、设备类型，尾插
int DeviceControl::add()
{
	string newname;
	int newid;
	char newtype;
	getchar();
	printf("请输入添加设备名称：");
	getline(cin, newname);
	//printf("请输入添加设备的id：");
	//cin >> newid;
	printf("请输入添加设备类型：");
	cin >> newtype;
	//getchar();
	cout << "正在添加……" << endl;
	sdt_Dev* temp = new(sdt_Dev);
	temp = first;
	int count = 0;
	while (count <= DeviceNum) {
		//cout <<"当前设备名称"+ temp->name << endl;
		if (temp->name == newname) {
			cout << "设备已存在" << endl;
			return 0;
		}
		count++;
	}
	sdt_Dev* NewDev=new (sdt_Dev);
	NewDev->name = newname;
	NewDev->deviceid = countId++;
	NewDev->dct = new (dct_Dev);
	NewDev->dct->type = newtype;
	NewDev->dct->state = 0;//初始设备默认状态为0
	NewDev->dct->coct = new (coct_Dev);
	char judge;//判断输入
	cout << "是否需要添加设备控制器 (y/n)" << endl;
	cin >> judge;
	if (judge == 'y') {
		cout << "输入添加的设备控制器编号(不超过20)" << endl;
		int devnum_new;
		cin >> devnum_new;
		while (devnum_new <= DeviceNum) {
			cout << "该控制器已存在，请重新输入" << endl;
			cin >> devnum_new;
		}
		int chct_new;
		cout << "请输入控制器占用通道 (1/2)" << endl;
		cin >> chct_new;
		NewDev->dct->coct->name = devnum_new;
		NewDev->dct->coct->chct = chct_new;
		ControllerNum++;
		Controller[devnum_new] = chct_new;//添加到通道控制器对应表
		cout << "添加控制器成功  ^_^" << endl;
	}
	if (judge == 'n') {
		cout << "当前存在的控制器有" << endl;
		for (int i = 0; i < ControllerNum; i++) {
			printf("控制器%d  ", i+1);
		}
		printf("\n");
		cout << "请输入选择的控制器" << endl;
		int coct_select;
		cin >> coct_select;
		NewDev->dct->coct->name = coct_select;
		NewDev->dct->coct->chct = Controller[coct_select];
	}
	//NewDev->dct->blockdct = new(block_Dev);***********
	//NewDev->dct->blockdct = blockdev;**************
	while (temp->next != NULL){
		temp = temp->next;
	}
	temp->next = NewDev;
	DeviceNum++;
	cout << "设备添加成功！" << endl;
	return 1;
}

//控制器与通道的对应表
void DeviceControl::coct_chct()
{
	for (int i = 0; i < 30; i++)
	{
		Controller[i] = 0;
	}
	Controller[1] = 1;//例，控制器1使用通道1
	Controller[2] = 1;
	Controller[3] = 2;
	Controller[4] = 2;
}

//申请使用设备，并将使用中的设备加入设备阻塞列表，标注进程id
int DeviceControl::apply(int pid_apply)
{
	//block_Dev* tempblock;
	getchar();
	cout << "申请使用的设备ID" << endl;
	//string name_apply;
	//getline(cin, name_apply);
	int id;
	cin >> id;

	sdt_Dev* temp = new (sdt_Dev);
	temp = first;
	temp = temp->next;
	int count = 0;
	while (count<=DeviceNum)
	{
		if (temp->deviceid == id) {
			break;
		}
		count++;
		temp = temp->next;
	}
	if (count >DeviceNum) {
		cout << "申请设备不存在！" << endl;
		return 0;
	}
	else {
		if(temp->dct->state == 0){
		//if (temp->dct->state == 0&&Controller_busy[temp->dct->coct->name] == 0) {//暂时不考虑控制器冲突问题
				//设备空闲且设备所需控制器空闲
				temp->dct->pid = pid_apply;
				temp->dct->state = 1;
				Controller_busy[temp->dct->coct->name] = 1;
				
				//******************
				//block_Dev* block_temp = new (block_Dev);//设备阻塞列表新建项
				//block_temp->block_name = temp->name;
				//block_temp->pid = pid_apply;
				//block_temp->next = new (block_Dev);
				//blockdev->next = block_temp;//将进程加入阻塞列表，设备运行时进程挂起

				cout << "设备申请成功  " << endl;
			}
			else if (temp->dct->state == 1) {
				cout << "设备申请失败！设备被占用。占用进程id：" << temp->dct->pid<<endl;
				return 0;
			}
			/*else if (temp->dct->state == 0 && Controller_busy[temp->dct->coct->name] == 1) {
				cout << "设备申请失败！控制器" << temp->dct->coct->name << "被占用！" << endl;
				return 0;
			}*/
			else {
				cout << "因不明原因导致设备申请失败" << endl;
				return 0;
			}
	}
	return 1;
}

//删除设备
int DeviceControl::del()
{
	string name_del;
	getchar();
	cout << "请输入要删除的设备名称" << endl;
	getline(cin, name_del);
	sdt_Dev* temp = new (sdt_Dev);
	temp = first;
	temp = temp->next;
	int count = 0;
	while (count<DeviceNum) {
		if (temp->name == name_del)
			break;
		temp = temp->next;
		count++;
	}
	if (count >= DeviceNum){
		cout << "要删除的设备不存在" << endl;
		return 0;
	}
	else {
		if (temp->dct->state == 1) {
			cout << "设备被占用，删除失败。占用设备的进程id为" << temp->dct->pid << endl;
			return 0;
		}
		else {
			sdt_Dev* devdel = new (sdt_Dev);//设备删除的辅助节点
			devdel = first;
			Controller_busy[temp->dct->coct->name] = 0;//释放控制器
			cout << "是否删除控制器  (y/n)" << endl;
			char judge;
			cin >> judge;
			if (judge == 'y') {
				ControllerNum--;
			}
			Controller_busy[temp->dct->coct->name] = 0;
			while (devdel->next != temp) {
				devdel = devdel->next;
			}
			devdel->next = temp->next;
			delete(temp);
			cout << "删除设备成功！" << endl;
			return 1;
		}
	}
	return 0;
}

void DeviceControl::ShowDev()
{
	sdt_Dev* temp = new (sdt_Dev);
	temp = first;
	temp = temp->next;
	cout << "————————————系统设备表SDT————————————" << endl;
	cout << "设备名称\t设备id\t设备状态\t使用控制器\t使用通道\n";
	while (temp != NULL) {
		printf("%-16s", temp->name.c_str());
		printf("%-8d", temp->deviceid);
		string state0 = "空闲";
		string state1 = "占用";
		if (temp->dct->state == 0)
			printf("%-16s", state0.c_str());
		else
			printf("%-16s", state1.c_str());
		printf("控制器");
		printf("%-10d", temp->dct->coct->name);
		printf("通道");
		printf("%-9d\n", temp->dct->coct->chct);
		//cout << temp->name <<"\t\t"<<temp->dct->state<<"\t\t"
		//<<temp->dct->coct->name<<"\t\t"<<temp->dct->coct->chct<<"\n";
		temp = temp->next;
	}
}

int DeviceControl::MenuPrint()
{
	cout << "******************设备管理***********************" << endl;
	cout << "\t\t1-显示设备列表" << endl;
	cout << "\t\t2-添加设备" << endl;
	cout << "\t\t3-申请使用设备" << endl;
	cout << "\t\t4-回收设备" << endl;
	cout << "\t\t5-退出设备管理" << endl;
	cout << "***************************************************" << endl;

	cout << "\n\n" << endl;
	cout << "请输入需要的功能（输入数字1-5）" << endl;
	int choose;
	cin >> choose;

	return choose;
}

void DeviceControl::RunDeviceControl(int choose)
{
	switch (choose)
	{
	case 1:
		ShowDev();
		break;
	case 2:
		add();
		break;
	case 3:
		cout << "请输入申请设备进程的id" << endl;
		int id_aplly;
		cin >> id_aplly;
		apply(id_aplly);
		break;
	case 4:
		del();
		break;
	case 5:
		cout << "已退出设备管理" << endl;
		break;
	default:
		cout << "输入错误，程序已退出" << endl;
		return;
	}
}

//初始化资源队列
void Res_Schedule::ResInit(DeviceControl *&Dev)
{
	//vector<Res_Schedule> ResList;
	int countRid = 100;
	sdt_Dev* temp =Dev->first->next;
	//初始化设备资源队列
	for (int i = 0; i < Dev->DeviceNum; i++)
	{
		Res_Schedule tmp;
		tmp.rid = temp->deviceid;
		tmp.pid_occupy = temp->dct->pid;
		tmp.state = temp->dct->state;
		//countRid++;
		temp = temp->next;
		ResList.push_back(tmp);
	}
	for (int i = Dev->DeviceNum; i < RES_LENTH; i++)
	{
		Res_Schedule tmp2;
		tmp2.rid = countRid;
		tmp2.pid_occupy = 0;
		tmp2.state = 0;
		countRid++;
		ResList.push_back(tmp2);
	}
	//初始化其他临界资源队列
	
}

//申请使用、改变
//键盘-1，鼠标-2，USB-3——为可交互设备
//rid在4-100是普通设备，100+是普通临界资源
int Res_Schedule::ResApply(int pid_Apply,int rid_Apply)
{
	//查找rid,返回位置
	int count = 0;
	for (int i = 0; i < RES_LENTH; i++)
	{
		if (ResList[i].rid == rid_Apply) {
			count = i;
			break;
		}
	}
	if (count >= RES_LENTH) {
		cout << "所查找资源不存在！" << endl;
		return RES_NOTEXIST;
	}
	else if(ResList[count].state==0) {
		//找到资源且资源空闲，资源状态更改为1，
		ResList[count].state = 1;
		ResList[count].pid_occupy = pid_Apply;
		//ResList[count].pid_list.push_back(pid_Apply);
		return RES_IDLE;
	}
	else if (ResList[count].state == 1) {
		//找到资源但资源被占用，需要排队
		ResList[count].pid_list.push_back(pid_Apply);
		return RES_OCCUPIED;
	}
}

//打印阻塞队列，用于测试
void Res_Schedule::printBlock()
{
	cout << "\n\n————————设备资源列表————————" << endl;
	cout << "资源id\t资源状态  占用进程id   等待进程列表\t" << endl;
	for (int i = 0; i < 30; i++)
	{
		printf("%-9d", ResList[i].rid);
		printf("%-10d", ResList[i].state);
		printf("pid:%d\t", ResList[i].pid_occupy);
		for (auto j = ResList[i].pid_list.begin(); j != ResList[i].pid_list.end(); j++)
		{
			std::cout << *j << ' ';
		}
		printf("\n");
	}
}

Res_Schedule::Res_Schedule() {
	this->pid_list = vector<int>(0, 0);
}