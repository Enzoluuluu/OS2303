#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<string>
#include <algorithm> 
#include<windows.h>
#include"DeviceControl.h"
using namespace std;

#define INITDEVICENUM 7
#define RES_LENTH 30
#define RES_OCCUPIED 1
#define RES_IDLE 0
#define RES_ERROR 2
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0) 

int signal =0;

//devlist:设备id、设备名、设备类型、设备状态、控制器、通道;
vector<Res_Schedule> ResList;
DeviceControl* Dev = new DeviceControl();

int main()
{
	Dev->coct_chct();
	Dev->initDC();
	//设备管理测试
	/*while (true) {
		int choice = Dev->MenuPrint();
		Dev->RunDeviceControl(choice);
		if (choice == 6)
			break;
	}*/
	
	//Dev->ShowDev();
	//资源管理测试
	Res_Schedule* Res = new Res_Schedule();
	//vector<Res_Schedule> ResList = new Res_Schedule;
	Res->ResInit(Dev);

	//申请资源测试
	//Res->ResApply(5, 1);
	//Res->printBlock();
	Res->ResApply(6, 2);
	cout << "啊？" << endl;
	Res->printBlock();
	Res->ResApply(7, 2);
	cout << "为什么" << endl;
	Res->printBlock();
	Res->ResApply(8, 3);
	Res->printBlock();
	Res->ResApply(123, 5);
	Res->ResApply(456, 4);
	Res->ResApply(408, 4);
	Res->ResApply(24, 6);
	Res->ResApply(489, 5);
	Res->ResApply(59, 5);
	Res->ResApply(951, 103);
	Res->ResApply(798, 103);
	Res->ResApply(37, 103);
	Res->ResApply(159, 101);
	Res->ResApply(111, 101);

	Res->printBlock();

	//释放资源测试
	int a = 0;
	printf("a=%d\n", Res->ResRelease(24, 6));
	printf("a=%d\n", Res->ResRelease(123, 5));
	printf("a=%d\n",Res->ResRelease(489, 5) );
	printf("a=%d\n",Res->ResRelease(59, 5) );
	printf("a=%d\n",Res->ResRelease(456, 4) );
	printf("a=%d\n", Res->ResRelease(159, 101));
	printf("a=%d\n", Res->ResRelease(951, 103));
	
	Res->printBlock();
	Dev->ShowDev();
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
int DeviceControl::apply(int pid_apply,int id)
{
	//getchar();
	//cout << "申请使用的设备ID" << endl;
	//int id;
	//cin >> id;

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

//设备释放
int DeviceControl::Release(int rid)
{
	sdt_Dev* temp = new (sdt_Dev);
	temp = first;
	temp = temp->next;
	int count = 0;
	while (count <= DeviceNum)
	{
		if (temp->deviceid == rid) {
			break;
		}
		count++;
		temp = temp->next;
	}
	if (count > DeviceNum) {
		cout << "要释放的设备不存在！" << endl;
		return 0;
	}
	else if (temp->dct->state == 0) {
		cout << "设备已经是空闲状态，不能再释放资源" << endl;
		return 0;
	}
	else {
		temp->dct->state = 0;
		cout << "释放成功！" << endl;
		return 1;
	}
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

//打印设备信息
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

//打印菜单
int DeviceControl::MenuPrint()
{
	cout << "******************设备管理***********************" << endl;
	cout << "\t\t1-显示设备列表" << endl;
	cout << "\t\t2-添加设备" << endl;
	cout << "\t\t3-申请使用设备" << endl;
	cout << "\t\t4-释放设备" << endl;
	cout << "\t\t5-回收设备" << endl;
	cout << "\t\t6-退出设备管理" << endl;
	cout << "***************************************************" << endl;

	cout << "\n\n" << endl;
	cout << "请输入需要的功能（输入数字1-6）" << endl;
	int choose;
	cin >> choose;

	return choose;
}

//执行菜单分支
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
		cout << "请输入申请设备进程的id：" << endl;
		int id_aplly;
		cin >> id_aplly;
		cout << "请输入申请的设备id：" << endl;
		int rid_apply;
		cin >> rid_apply;
		apply(id_aplly,rid_apply);
		break;
	case 4:
		cout << "请输入要释放的设备id：" << endl;
		int id_rel;
		cin >> id_rel;
		Release(id_rel);
		break;
	case 5:
		del();
		break;
	case 6:
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

//申请使用资源
//键盘-1，鼠标-2，USB-3——为可交互设备
//rid在4-100是普通设备，100+是普通临界资源
int Res_Schedule::ResApply(int pid_Apply,int rid_Apply)
{
	int count = 0;
	for (int i = 0; i < RES_LENTH; i++)
	{
		if (ResList[i].rid == rid_Apply) {
			count = i;
			break;
		}
	}
	if (rid_Apply > 3) {
	//查找rid,返回位置
		if (count >= RES_LENTH) {
			cout << "所查找资源不存在！" << endl;
			return RES_ERROR;
		}
		else if(ResList[count].state==0) {
			//找到资源且资源空闲，资源状态更改为1，
			ResList[count].state = 1;
			ResList[count].pid_occupy = pid_Apply;
			if (rid_Apply < 100) {
				Dev->apply(pid_Apply,rid_Apply);
			}
			//ResList[count].pid_list.push_back(pid_Apply);
			return RES_IDLE;
		}
		else if (ResList[count].state == 1) {
			//找到资源但资源被占用，需要排队
			if (rid_Apply < 100) {
				Dev->apply(pid_Apply, rid_Apply);
			}
			ResList[count].pid_list.push_back(pid_Apply);
			return RES_OCCUPIED;
		}
	}
	else if (rid_Apply == 1 || rid_Apply == 2 || rid_Apply == 3) {//申请可交互设备
		if (ResList[count].state == 0) {
			//找到资源且资源空闲，资源状态更改为1，
			ResList[count].state = 1;
			ResList[count].pid_occupy = pid_Apply;
			InteractDev(pid_Apply, rid_Apply);
			//ResList[count].pid_list.push_back(pid_Apply);
			return RES_IDLE;
		}
		else if (ResList[count].state == 1) {
			//找到资源但资源被占用，需要排队
	     	Dev->apply(pid_Apply, rid_Apply);
			ResList[count].pid_list.push_back(pid_Apply);
			return RES_OCCUPIED;
	}
		
	}
	else {
		cout << "申请资源不存在" << endl;
		return RES_ERROR;
	}
}

//释放该进程占有的资源，把资源分配给下一个等待进程
int Res_Schedule::ResRelease(int pid_Rel, int rid_Rel)
{
	//查找rid,返回位置
	int count = 0;
	for (int i = 0; i < RES_LENTH; i++)
	{
		if (ResList[i].rid == rid_Rel) {
			count = i;
			break;
		}
	}
	if (count >= RES_LENTH) {
		cout << "要给我的资源大概是打错了，系统里面没有这个资源" << endl;
		return RES_ERROR;
	}
	else {
		//cout << "rid怎么会错啊！:" << rid_Rel << endl;
		if (rid_Rel > 99) {
			//占用资源的进程使用完资源
			if (ResList[count].pid_occupy == pid_Rel) {
				if (Search_pidlist(pid_Rel, count) == 2) {//等待队列为空
					ResList[count].state = 0;//释放资源，资源空闲
					cout << "已成功释放资源！" << endl;
					//ResList[count].pid_occupy = 0;
				}
				else if (Search_pidlist(pid_Rel, count) == 0) {//等待队列不为空
					ResList[count].pid_occupy = ResList[count].pid_list.front();//占用设备的进程修改为队列首进程
					vector<int> ::iterator temp;
					temp = find(ResList[count].pid_list.begin(), ResList[count].pid_list.end()-1, pid_Rel);
					if (temp != ResList[count].pid_list.begin()) {
						temp = ResList[count].pid_list.erase(temp-1);//在该资源的进程等待队列中删除该进程
					}
					else {
						ResList[count].pid_list.erase(ResList[count].pid_list.begin());
					}
				}
			}
			else {
				if (Search_pidlist(pid_Rel, count) == 1) {//进程尚未使用该资源，且该进程在阻塞队列里面
					cout << "该进程尚未使用此资源，已将该进程从阻塞队列中移除，返回值为当前占用该资源的进程" << endl;
				}
				else {
					cout << "该进程未使用此资源，且不在阻塞队列中，请检查进程输入是否正确" << endl;
				}
			}
		}
		else if (rid_Rel >= 4 && rid_Rel <= 99){
			if (ResList[count].pid_occupy == pid_Rel) {
				if (Search_pidlist(pid_Rel, count) == 2) {//等待队列为空
					ResList[count].state = 0;//释放资源，资源空闲
					Dev->Release(rid_Rel);//释放设备，设备空闲
					cout << "已成功释放设备！" << endl;
					//ResList[count].pid_occupy = 0;
				}
				else if (Search_pidlist(pid_Rel, count) == 0) {//等待队列不为空
					ResList[count].pid_occupy = ResList[count].pid_list.front();//占用设备的进程修改为队列首进程
					vector<int> ::iterator temp;
					temp = find(ResList[count].pid_list.begin(), ResList[count].pid_list.end()-1 , pid_Rel);
					if (temp != ResList[count].pid_list.begin()) {
						temp = ResList[count].pid_list.erase(temp - 1);//在该资源的进程等待队列中删除该进程
					}
					else {
						ResList[count].pid_list.erase(ResList[count].pid_list.begin());
					}
				}
					
			}
			else {
				if (Search_pidlist(pid_Rel, count) == 1) {//进程尚未使用该资源，且该进程在阻塞队列里面
					cout << "该进程尚未使用此设备，已将该进程从阻塞队列中移除，返回值为当前占用该资源的进程" << endl;
				}
				else {
					cout << "该进程未使用此设备，且不在阻塞队列中，请检查进程输入是否正确" << endl;
				}
			}
		}
		else{
			cout << "Rid错啦" << endl;
			return RES_ERROR;
		}
		return ResList[count].pid_occupy;//返回当前占用进程的pid，没有进程使用就返回0
	}
}

//查找阻塞链表中是否存在该进程
int Res_Schedule::Search_pidlist(int pid,int count)
{
	vector<int> ::iterator temp;
	if (!ResList[count].pid_list.empty()) {//进程等待队列不为空
		temp = find(ResList[count].pid_list.begin(), ResList[count].pid_list.end(), pid);
		if (temp != ResList[count].pid_list.end()) {//存在该进程
			//STL里的所有容器类中的erase实现都会返回一个迭代器，这个迭代器指向了“当前删除元素的后继元素，或是end()”
			//不写temp=的话就会报vector iterators incompatible
			//天呐，改了好久
			temp=ResList[count].pid_list.erase(temp);//在该资源的进程等待队列中删除该进程
			return 1;
		}
		else {//等待队列没有该进程
			//cout << "等待队列不含进程" << pid << endl;
			return 0;
		}
	}
	else {//等待队列为空
		ResList[count].pid_occupy = 0;//没有进程占用这个资源
		//cout << "该资源的进程等待队列为空" << endl;
		return 2;
	}
}

Res_Schedule::Res_Schedule() {
	this->pid_list = vector<int>(0, 0);
}

//可交互设备
void Res_Schedule::InteractDev(int pid_Apply, int rid_Apply)
{
	if (rid_Apply == 1) {
		Listen_KeyBoard(pid_Apply);
		cout << signal << endl;
	}
	if (rid_Apply == 2) {
		Listen_Mouse(pid_Apply);
		cout << signal << endl;
	}
	if (rid_Apply == 3) {
		Listen_USB(pid_Apply);
		cout << signal << endl;
	}
}

void Res_Schedule::Listen_KeyBoard(int pid_Apply)
{
	char i;
	cout << "输入‘#’后按下回车结束输入" << endl;
	while (1) {
		i = getchar();
		if (i == '#')
			break;
	}
	if (ResList[0].pid_occupy == pid_Apply) {
		if (Search_pidlist(pid_Apply, 0) == 2) {//等待队列为空
			ResList[0].state = 0;//释放资源，资源空闲
			Dev->Release(1);//释放设备，设备空闲
			cout << "已成功释放设备！" << endl;
			//ResList[count].pid_occupy = 0;
		}
		else if (Search_pidlist(pid_Apply, 0) == 0) {//等待队列不为空
			ResList[1].pid_occupy = ResList[0].pid_list.front();//占用设备的进程修改为队列首进程
			vector<int> ::iterator temp;
			temp = find(ResList[0].pid_list.begin(), ResList[0].pid_list.end() - 1, pid_Apply);
			if (temp != ResList[0].pid_list.begin()) {
				temp = ResList[0].pid_list.erase(temp - 1);//在该资源的进程等待队列中删除该进程
			}
			else {
				ResList[0].pid_list.erase(ResList[0].pid_list.begin());
			}
		}

	}
	else {
		if (Search_pidlist(pid_Apply, 0) == 1) {//进程尚未使用该资源，且该进程在阻塞队列里面
			cout << "该进程尚未使用此设备，已将该进程从阻塞队列中移除，返回值为当前占用该资源的进程" << endl;
		}
		else {
			cout << "该进程未使用此设备，且不在阻塞队列中，请检查进程输入是否正确" << endl;
		}
	}
	signal = 1;
}

void Res_Schedule::Listen_Mouse(int pid_Apply)
{
	char i;
	cout << "模拟监听鼠标事件，输入‘#’后按下回车结束输入" << endl;
	while (1) {
		i = getchar();
		if (i == '#')
			break;
	}
	if (ResList[1].pid_occupy == pid_Apply) {
		if (Search_pidlist(pid_Apply, 1) == 2) {//等待队列为空
			ResList[1].state = 0;//释放资源，资源空闲
			Dev->Release(2);//释放设备，设备空闲
			cout << "已成功释放设备！" << endl;
			//ResList[count].pid_occupy = 0;
		}
		else if (Search_pidlist(pid_Apply, 1) == 0) {//等待队列不为空
			ResList[1].pid_occupy = ResList[1].pid_list.front();//占用设备的进程修改为队列首进程
			vector<int> ::iterator temp;
			temp = find(ResList[1].pid_list.begin(), ResList[1].pid_list.end() - 1, pid_Apply);
			if (temp != ResList[1].pid_list.begin()) {
				temp = ResList[1].pid_list.erase(temp - 1);//在该资源的进程等待队列中删除该进程
			}
			else {
				ResList[1].pid_list.erase(ResList[1].pid_list.begin());
			}
		}

	}
	else {
		if (Search_pidlist(pid_Apply, 1) == 1) {//进程尚未使用该资源，且该进程在阻塞队列里面
			cout << "该进程尚未使用此设备，已将该进程从阻塞队列中移除，返回值为当前占用该资源的进程" << endl;
		}
		else {
			cout << "pid:" << pid_Apply << endl;
			cout << "该进程未使用此设备，且不在阻塞队列中，请检查进程输入是否正确" << endl;
		}
	}
	signal = 2;
}

void Res_Schedule::Listen_USB(int pid_Apply)
{
	char i;
	cout << "模拟插入USB，输入‘#’后按下回车就插完了" << endl;
	while (1) {
		i = getchar();
		if (i == '#')
			break;
	}
	if (ResList[2].pid_occupy == pid_Apply) {
		if (Search_pidlist(pid_Apply, 2) == 2) {//等待队列为空
			ResList[2].state = 0;//释放资源，资源空闲
			Dev->Release(3);//释放设备，设备空闲
			cout << "已成功释放设备！" << endl;
			//ResList[count].pid_occupy = 0;
		}
		else if (Search_pidlist(pid_Apply,2) == 0) {//等待队列不为空
			ResList[2].pid_occupy = ResList[2].pid_list.front();//占用设备的进程修改为队列首进程
			vector<int> ::iterator temp;
			temp = find(ResList[2].pid_list.begin(), ResList[2].pid_list.end() - 1, pid_Apply);
			if (temp != ResList[2].pid_list.begin()) {
				temp = ResList[2].pid_list.erase(temp - 1);//在该资源的进程等待队列中删除该进程
			}
			else {
				ResList[2].pid_list.erase(ResList[2].pid_list.begin());
			}
		}

	}
	else {
		if (Search_pidlist(pid_Apply, 2) == 1) {//进程尚未使用该资源，且该进程在阻塞队列里面
			cout << "该进程尚未使用此设备，已将该进程从阻塞队列中移除，返回值为当前占用该资源的进程" << endl;
		}
		else {
			cout << "该进程未使用此设备，且不在阻塞队列中，请检查进程输入是否正确" << endl;
		}
	}
	signal = 3;
}

//打印阻塞队列
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