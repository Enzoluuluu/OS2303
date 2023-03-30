#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<string>
#include"DeviceControl.h"
using namespace std;

//�豸����
sdt_Dev* sdt;//�豸
sdt_Dev* first;//�豸�б��ͷ���
dct_Dev* dct;//�豸����
coct_Dev* coct;//�豸����������
chct_Dev* chct1, *chct2;//ͨ��1��ͨ��2
block_Dev* blockdev;//�������豸
int DeviceNum=3;//�豸������ʼΪ3
int ControllerNum=4;//������������ʼΪ4
int Controller[30];//��������ͨ����Ӧ��
int Controller_busy[30];//�������Ƿ�ռ�õı�

//devlist:�豸id���豸�������豸���͡��豸״̬����������ͨ��

int main()
{
	DeviceControl* Dev = new DeviceControl();
	Dev->coct_chct();
	Dev->initDC();

	while (true) {
		int choice = Dev->MenuPrint();
		Dev->RunDeviceControl(choice);
		if (choice == 5)
			break;
	}
	
	//ͨ�ò���
	/*string name;
	printf("���������ѯ�����ƣ�");
	getline(cin, name);
	printfortest(name);
	printall();*/
	
	//add����
	/*int flag = add();
	if (flag == 1) {
		cout << "����豸�ɹ���" << endl;
		printfortest("Mouse", first);
	}
	else
		cout << "����豸ʧ�ܣ�" << endl;*/
	
	//apply����
	/*apply(1);//����id�����д�ģ�����д��֮����Ͻ������޸�
	apply(1);
	printfortest("Screen");*/

	//delete����
	/*del();
	printall();
	printfortest("Screen");*/
	
	return 0;
}

//�豸��ʼ��
void DeviceControl::initDC()
{
	blockdev = new (block_Dev);//������������

	//��ʼ��ͨ��
	chct1 = new (chct_Dev);
	chct1->name = '1';
	chct1->state = 0;//��ʼ״̬����
	chct2 = new (chct_Dev);
	chct2->name = '2';
	chct2->state = '0';

	//��ʼ�����������������nameָ���ǿ�����x��������Ҫ��Ӿ���β���
	first= new (sdt_Dev);
	//sdt = first;
	//sdt = sdt->next;
	//ͷָ�벻���豸���豸�ӵڶ����ռ俪ʼ��
	sdt = new (sdt_Dev);
	first->next = sdt;

	//��ʼ״̬�°������̡���ӡ�������豸
	// û�г�ʼ������
	sdt->name = "KeyBoard";
	sdt->deviceid = 1;
	sdt->dct = new (dct_Dev);
	sdt->dct->blockdct = new(block_Dev);
	sdt->dct->blockdct = blockdev;
	sdt->dct->type = 'I';
	
	//sdt = sdt->next;
	sdt_Dev* sdt2 = new (sdt_Dev);
	
	sdt2->name = "Screen";
	sdt2->deviceid = 2;
	sdt2->dct = new (dct_Dev);
	sdt2->dct->blockdct = new(block_Dev);
	sdt2->dct->blockdct = blockdev;
	sdt2->dct->type = 'O';
	sdt->next = sdt2;
	sdt = sdt->next;

	sdt_Dev* sdt3 = new (sdt_Dev);

	sdt3->name = "Mouse";
	sdt3->deviceid = 3;
	sdt3->dct = new (dct_Dev);
	sdt3->dct->blockdct = new(block_Dev);
	sdt3->dct->blockdct = blockdev;
	sdt3->dct->type = 'I';
	sdt->next = sdt3;

	sdt = first->next;//sdt�˻ص���һ���豸��ʼѭ����ʼ��
	while (sdt != NULL)
	{
		sdt->dct->state = 0;
		sdt->dct->coct = new (coct_Dev);
		sdt->dct->coct->state = 0;
		//sdt->dct->coct->chct = new (chct_Dev);
		//sdt->dct->coct->chct->state = 0;
		sdt = sdt->next;
	}
	sdt =first->next;//sdt�˻ص���һ���豸����ͨ��
	//sdt = sdt->next;
	//printf("%s\n", sdt->name.c_str());
	sdt->dct->coct->name = 1;//KeyBoardʹ�ÿ�����1��ͨ��1
	sdt->dct->coct->chct = 1;
	sdt->next->dct->coct->name = 3;//Screenʹ�ÿ�����3��ͨ��2
	sdt->next->dct->coct->chct = 2;
	sdt = sdt->next;
	//sdt->next->dct->state = 1;//�����豸��ռ�ã�apply�����ã�
	sdt->next->dct->coct->name = 2;//Mouseʹ�ÿ�����2��ͨ��1
	sdt->next->dct->coct->state = 0;//�����������ռ�ã�apply�����ã�
	Controller_busy[2] = 0;
	sdt->next->dct->coct->chct = 1;

	//β����ÿ�
	sdt = sdt->next;
	sdt->next = new (sdt_Dev);
	sdt->next = NULL;
}

void DeviceControl::coct_busy()
{
	for (int i = 0; i < 30; i++)
		Controller_busy[i] = 0;//��ʼ״̬�¿��������ǿ���
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
		printf("�豸�����ڣ�\n");
	}
	else {
		printf("�豸���ƣ�%s\n", Devname.c_str());
		printf("�豸���ͣ�%c\t�豸״̬��%d\tʹ�ÿ�����:������%d\tʹ��ͨ����ͨ��%d\n",
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

//����豸,�����豸���ơ��豸���ͣ�β��
int DeviceControl::add()
{
	string newname;
	int newid;
	char newtype;
	getchar();
	printf("����������豸���ƣ�");
	getline(cin, newname);
	printf("����������豸��id��");
	cin >> newid;
	printf("����������豸���ͣ�");
	cin >> newtype;
	//getchar();
	cout << "������ӡ���" << endl;
	sdt_Dev* temp = new(sdt_Dev);
	temp = first;
	int count = 0;
	while (count <= DeviceNum) {
		//cout <<"��ǰ�豸����"+ temp->name << endl;
		if (temp->name == newname) {
			cout << "�豸�Ѵ���" << endl;
			return 0;
		}
		count++;
	}
	sdt_Dev* NewDev=new (sdt_Dev);
	NewDev->name = newname;
	NewDev->deviceid = newid;
	NewDev->dct = new (dct_Dev);
	NewDev->dct->type = newtype;
	NewDev->dct->state = 0;//��ʼ�豸Ĭ��״̬Ϊ0
	NewDev->dct->coct = new (coct_Dev);
	char judge;//�ж�����
	cout << "�Ƿ���Ҫ����豸������ (y/n)" << endl;
	cin >> judge;
	if (judge == 'y') {
		cout << "������ӵ��豸���������(������20)" << endl;
		int devnum_new;
		cin >> devnum_new;
		while (devnum_new <= DeviceNum) {
			cout << "�ÿ������Ѵ��ڣ�����������" << endl;
			cin >> devnum_new;
		}
		int chct_new;
		cout << "�����������ռ��ͨ�� (1/2)" << endl;
		cin >> chct_new;
		NewDev->dct->coct->name = devnum_new;
		NewDev->dct->coct->chct = chct_new;
		ControllerNum++;
		Controller[devnum_new] = chct_new;//��ӵ�ͨ����������Ӧ��
		cout << "��ӿ������ɹ�  ^_^" << endl;
	}
	if (judge == 'n') {
		cout << "��ǰ���ڵĿ�������" << endl;
		for (int i = 0; i < ControllerNum; i++) {
			printf("������%d  ", i+1);
		}
		printf("\n");
		cout << "������ѡ��Ŀ�����" << endl;
		int coct_select;
		cin >> coct_select;
		NewDev->dct->coct->name = coct_select;
		NewDev->dct->coct->chct = Controller[coct_select];
	}
	NewDev->dct->blockdct = new(block_Dev);
	NewDev->dct->blockdct = blockdev;
	while (temp->next != NULL){
		temp = temp->next;
	}
	temp->next = NewDev;
	DeviceNum++;
	cout << "�豸��ӳɹ���" << endl;
	return 1;
}

//��������ͨ���Ķ�Ӧ��
void DeviceControl::coct_chct()
{
	for (int i = 0; i < 30; i++)
	{
		Controller[i] = 0;
	}
	Controller[1] = 1;//����������1ʹ��ͨ��1
	Controller[2] = 1;
	Controller[3] = 2;
	Controller[4] = 2;
}

//����ʹ���豸������ʹ���е��豸�����豸�����б���ע����id
int DeviceControl::apply(int pid_apply)
{
	block_Dev* tempblock;
	getchar();
	cout << "����ʹ�õ��豸����" << endl;
	string name_apply;
	getline(cin, name_apply);

	sdt_Dev* temp = new (sdt_Dev);
	temp = first;
	temp = temp->next;
	int count = 0;
	while (count<=DeviceNum)
	{
		if (temp->name == name_apply) {
			break;
		}
		count++;
		temp = temp->next;
	}
	if (count >DeviceNum) {
		cout << "�����豸�����ڣ�" << endl;
		return 0;
	}
	else {
		if (temp->dct->state == 0&&Controller_busy[temp->dct->coct->name] == 0) {
				//�豸�������豸�������������
				temp->dct->pid = pid_apply;
				temp->dct->state = 1;
				Controller_busy[temp->dct->coct->name] = 1;
				
				block_Dev* block_temp = new (block_Dev);//�豸�����б��½���
				block_temp->block_name = name_apply;
				block_temp->pid = pid_apply;
				block_temp->next = new (block_Dev);
				blockdev->next = block_temp;//�����̼��������б��豸����ʱ���̹���

				cout << "�豸����ɹ�  " << endl;
			}
			else if (temp->dct->state == 1) {
				cout << "�豸����ʧ�ܣ��豸��ռ�á�ռ�ý���id��" << temp->dct->pid<<endl;
				return 0;
			}
			else if (temp->dct->state == 0 && Controller_busy[temp->dct->coct->name] == 1) {
				cout << "�豸����ʧ�ܣ�������" << temp->dct->coct->name << "��ռ�ã�" << endl;
				return 0;
			}
			else {
				cout << "����ԭ�����豸����ʧ��" << endl;
				return 0;
			}
	}
	return 1;
}

//ɾ���豸
int DeviceControl::del()
{
	string name_del;
	getchar();
	cout << "������Ҫɾ�����豸����" << endl;
	getline(cin, name_del);
	sdt_Dev* temp = new (sdt_Dev);
	temp = first;
	temp = temp->next;
	int count = 0;
	while (count<=DeviceNum) {
		if (temp->name == name_del)
			break;
		temp = temp->next;
		count++;
	}
	if (count > DeviceNum){
		cout << "Ҫɾ�����豸������" << endl;
		return 0;
	}
	else {
		if (temp->dct->state == 1) {
			cout << "�豸��ռ�ã�ɾ��ʧ�ܡ�ռ���豸�Ľ���idΪ" << temp->dct->pid << endl;
			return 0;
		}
		else {
			sdt_Dev* devdel = new (sdt_Dev);//�豸ɾ���ĸ����ڵ�
			devdel = first;
			Controller_busy[temp->dct->coct->name] = 0;//�ͷſ�����
			cout << "�Ƿ�ɾ��������  (y/n)" << endl;
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
			cout << "ɾ���豸�ɹ���" << endl;
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
	cout << "������������������������ϵͳ�豸��SDT������������������������" << endl;
	cout << "�豸����\t�豸id\t�豸״̬\tʹ�ÿ�����\tʹ��ͨ��\n";
	while (temp != NULL) {
		printf("%-16s", temp->name.c_str());
		printf("%-8d", temp->deviceid);
		string state0 = "����";
		string state1 = "ռ��";
		//string con = "������";
		//string chc = "ͨ��";
		if (temp->dct->state == 0)
			printf("%-16s", state0.c_str());
		else
			printf("%-16s", state1.c_str());
		printf("������");
		printf("%-10d", temp->dct->coct->name);
		printf("ͨ��");
		printf("%-9d\n", temp->dct->coct->chct);
		//cout << temp->name <<"\t\t"<<temp->dct->state<<"\t\t"
		//<<temp->dct->coct->name<<"\t\t"<<temp->dct->coct->chct<<"\n";
		temp = temp->next;
	}
}
int DeviceControl::MenuPrint()
{
	cout << "******************�豸����***********************" << endl;
	cout << "\t\t1-��ʾ�豸�б�" << endl;
	cout << "\t\t2-����豸" << endl;
	cout << "\t\t3-����ʹ���豸" << endl;
	cout << "\t\t4-�����豸" << endl;
	cout << "\t\t5-�˳��豸����" << endl;
	cout << "***************************************************" << endl;

	cout << "\n\n" << endl;
	cout << "��������Ҫ�Ĺ��ܣ���������1-5��" << endl;
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
		cout << "�����������豸���̵�id" << endl;
		int id_aplly;
		cin >> id_aplly;
		apply(id_aplly);
		break;
	case 4:
		del();
		break;
	case 5:
		cout << "���˳��豸����" << endl;
		break;
	default:
		cout << "������󣬳������˳�" << endl;
		return;
	}
}