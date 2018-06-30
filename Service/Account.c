#include "Account.h"
#include "EntityKey.h"
#include "../Common/list.h"
#include "../Persistence/Account_Persist.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

extern account_t gl_CurUser;

//����ϵͳ��ʼ���˺�admin
void Account_Srv_InitSys(){
	account_t data_admin;
	if (Account_Perst_CheckAccFile())
		return ;
	data_admin.id = EntKey_Srv_CompNewKey("account");

	printf("\n\n\n\n\n\n\n\n\n\t\t\t\t\t    ����ϵͳ����Ա\n\n");
	printf("\t\t\t\t    �û�����");
	gets(data_admin.username);
	printf("\n\t\t\t\t    ���룺");
	GetPassword(data_admin.password);
	data_admin.type = USR_ADMIN;
	Account_Srv_Add(&data_admin);
}

//��֤��¼�˺��Ƿ��Ѵ��ڣ����ڣ������¼�û���Ϣ��ȫ�ֱ���gl_CurUser��return 1������return 0
int Account_Srv_Verify(char usrName[], char pwd[]) {
	account_t usr;
	account_list_t head;
	List_Init(head, account_node_t);
	Account_Srv_FetchAll(head);
	Account_Perst_SelByName(usrName, &usr);
	if (!Account_Srv_FindByUsrName(head, usrName))
		return 0;
	if (strcmp(usr.password, pwd) == 0) {
		gl_CurUser = usr;
		return 1;
	}
	return 0;
}


//��֤�û����Ƿ���ڣ�����list�������ڣ����ض�Ӧ���ָ�룻���򣬷���null
account_node_t * Account_Srv_FindByUsrName(account_list_t list, char usrName[]) {
	account_node_t  *pos;
	List_ForEach(list, pos)
	if (strcmp(pos->data.username, usrName )== 0)
			return pos;
	return NULL;
}

//���һ���û��˺ţ�ͨ������Account_Perst_Insert(data)����ʵ��
int Account_Srv_Add(const account_t *data){
	 return Account_Perst_Insert(data);
}

//�޸�һ���û��˺ţ�ͨ������Account_Perst_Update(data)����ʵ��
int Account_Srv_Modify(const account_t *data){
	return Account_Perst_Update(data);
}

//ɾ��һ���û��˺ţ�ͨ������Account_Perst_DeleteByID(usrID)����ʵ��
int Account_Srv_DeleteByID(int usrID){
	return Account_Perst_DeleteByID(usrID);
}

//��ȡusrID��Ӧ���û��˺���Ϣ��ͨ������Account_Perst_SelectByID(usrID, buf)����ʵ��
int Account_Srv_FetchByID(int usrID, account_t *buf){
	return Account_Perst_SelectByID(usrID,buf);
}

//��ȡusrName��Ӧ���û��˺���Ϣ��ͨ������Account_Perst_SelByName(usrName, buf)����ʵ��
int Account_Srv_FetchByName(char usrName[], account_t *buf){
	return Account_Perst_SelByName(usrName, buf);
}

//��ȡ�����û��˺���Ϣ�����浽list�����У�ͨ������Account_Perst_SelectAll(list)����ʵ��
int Account_Srv_FetchAll(account_list_t list){
	return Account_Perst_SelectAll(list);
}


