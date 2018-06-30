#include "Account_UI.h"
#include "../Common/list.h"
#include "../Service/Account.h"
#include "../Service/EntityKey.h"
#include "Account_UI.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <time.h> 
#include "conio.h"
#include <unistd.h>
#include <ctype.h>
#include <windows.h> 
extern account_t gl_CurUser;
void gotoxy(int x,int y)   
{  
    COORD c;  
    c.X=x-1;  
    c.Y=y-1;  
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),c);  
}   
char *usr_type(int i)
{
    switch(i)
    {
        case 1:
            return "��ƱԱ";
            break;
            
        case 2:
            return "����";
            break;
        
        case 9:
            return "ϵͳ����Ա";
            break;
    
        case 0:
            return "�����û�";
            break;
     
    }
}

//�������뺯�� 
int GetPassword(char password[])
{
    char ch;
    int i;

        for(i=0;i<20;i++){
            ch=getch();
            if(ch==8){
                if((i-1)>=0){
                    i-=2;
                    printf("\b\b  \b\b");
                }
                else
                    i--;
                continue;
            }
                if(ch=='\r')
                break;
            password[i]=ch;
            printf("��");
        }
        password[i]='\0';
	return i; 
}

//��¼��������ʾ�û������û��������룬��¼�ɹ�return 1��������ʾ���µ�¼������3�Σ���¼ʧ��
int SysLogin() {
	Account_Srv_InitSys();
	char usrname[20];
	char passwd[20];
	int i=0,j;
	system("cls"); 
	
	for (i = 0; i < 3; i++) {
		printf("\n\n\n\n\n\n\n\n\t\t\t\t      ��ӭ��������¥Ʊ�����ϵͳ\n");
		printf("\n\n\t\t\t\t\t���û���: ");
		gets(usrname);
		printf("\n\t\t\t\t\t������: ");
		GetPassword(passwd);
		if (Account_Srv_Verify(usrname, passwd))
		{
			
			
		printf("\n\n\n\t\t\t\t     ������������������������������������������������������\n");
		printf("\t\t\t\t     ��                         ��\n");
		printf("\t\t\t\t     ������������������������������������������������������"); 
			printf("\n\n\n\n\t\t\t\t      ");
			gotoxy(40,18);
			for (j = 0; j < 12; j++)
			{
				Sleep(100);
				printf("��");
			}

			return 1;
		}
		printf("\n\n\n\t\t\t\t     ������������������������������������������������������\n");
		printf("\t\t\t\t     ��                         ��\n");
		printf("\t\t\t\t     ������������������������������������������������������"); 
		printf("\n\n\n\n\t\t\t\t      ");
		gotoxy(40,18);
		for (j = 0; j < 12; j++)
		{
			Sleep(100);
			printf("��");
		}
		printf("\n\n\t\t\t\t      �˺Ż���������,��������˳�\n");
		getch(); 
		system("cls");
	}
	return 0;
}
//����ϵͳ�û�������ں�������ʾ�û��˺Ź���˵�
void Account_UI_MgtEntry() {
	int i;
	char choice;
	account_t usr;

	account_list_t head;
	account_node_t *pos;
	Pagination_t paging;

	List_Init(head, account_node_t);
	paging.offset = 0;
	paging.pageSize = ACCOUNT_PAGE_SIZE;

	//��������
	paging.totalRecords = Account_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);
	if (gl_CurUser.type == 9)
	{
		do {
			system("cls");
			printf("\n\t\t\t\t\t       �û�����\n\n\n");
			printf("\t\t\t%-6s  %-20s  %-16s  %-16s   \n\n", "ID", "  �û�����", "�û���", "  ����");
			
			//��ʾ����
			for (i = 0, pos = (account_node_t *)(paging.curPos);
				pos != head && i < paging.pageSize; i++) {
				printf("\t\t\t%-6d  %-20s  %-16s  %-16s  \n",pos->data.id,usr_type(pos->data.type), pos->data.username,"********");
				pos = pos->next;
			}
			printf("\n\t\t\t����:%2d\t\t\t\t\t\t    %d/%d \n",
				paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));

			printf("\n\n\t\t\t  1.��һҳ 2.��һҳ 3.��� 4.ɾ�� 5.�޸� 6.��ѯ 7.����");
			printf("\n\n\n\t\t\t:");
			fflush(stdin);
			choice = getche();
			choice = toupper(choice);
			fflush(stdin);
			switch (choice) {
			case '3':
				if (Account_UI_Add(head)) //����ӳɹ����������һҳ��ʾ
				{
					paging.totalRecords = Account_Srv_FetchAll(head);
					Paging_Locate_LastPage(head, paging, account_node_t);
				}
				break;
			case '4':
				//getchar();
				printf("\n\n\t\t\t�����û���:");
				gets(usr.username);
				if (Account_UI_Delete(head, usr.username)) {	//������������
					paging.totalRecords = Account_Srv_FetchAll(head);
					List_Paging(head, paging, account_node_t);
				}
				break;
			case '5':
				//getchar();
				printf("\n\n\t\t\t�����û���:");
				gets(usr.username);
				if (Account_UI_Modify(head, usr.username)) {	//������������
					paging.totalRecords = Account_Srv_FetchAll(head);
					List_Paging(head, paging, account_node_t);
				}
				break;
			case '6':
				//getchar();
				printf("\n\n\t\t\t�����û���:");
				gets(usr.username);
				if (Account_UI_Query(head, usr.username)) {
					//������������
					paging.totalRecords = Account_Srv_FetchAll(head);
					List_Paging(head, paging, account_node_t);
					printf("\n\n\t\t\t��������˳�");
					getch(); 
				}
				break;
			case '1':
				if (1 < Pageing_CurPage(paging)) {
					Paging_Locate_OffsetPage(head, paging, -1, account_node_t);
				}
				break;
			case '2':
				if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
					Paging_Locate_OffsetPage(head, paging, 1, account_node_t);
				}
				break;
			}
		} while (choice != '7');
	}
	else
	{
		do {
			system("cls");
			printf("\n\n\t\t\t                        �޸�����\n");
			printf("\t\t\t=========================================================\n");

			printf("\t\t\t\t1.�޸�\t\t\t\t2.����");
			printf("\n\n\t\t\t:");
			fflush(stdin);
			choice = getche();
			//getchar();
			fflush(stdin);
			switch (choice)
			{
			case '1':
				//getchar();
				strcpy(usr.username , gl_CurUser.username);
				if (Account_UI_Modify(head, usr.username)) {	//������������
					paging.totalRecords = Account_Srv_FetchAll(head);
					List_Paging(head, paging, account_node_t);
				}
				break;
			}
		} while (choice != '2');

	}
	//getchar();
	//�ͷ�����ռ�
	List_Destroy(head, account_node_t);
}




//���һ���û��˺���Ϣ������˺������ڣ���ʾ������Ϣ
int Account_UI_Add(account_list_t list) {
	account_t usr;
	int newUsrCount = 0;
	char choice;
	char passwd[20];
	int l;
    List_Init(list,account_node_t);
    Account_Srv_FetchAll(list);
	do
	{
	printf("\n\n\t\t\t                        ����˻�\n");
	printf("\t\t\t=========================================================\n");

	printf("\n\t\t\t�û���:");
	gets(usr.username);
	printf("\n\t\t\t����:");
	GetPassword(usr.password);
	l = strlen(usr.password);
	while (l < 6)
	{
		printf("\n\t\t\t���Ȳ�����λ,����������\n");
		printf("\n\t\t\t����:");
		GetPassword(usr.password);
		l = strlen(usr.password);
	}
	printf("\n\t\t\t�ٴ���������:");
	GetPassword(passwd);
	while (strcmp(usr.password, passwd) != 0)
	{
		printf("\n\t\t\t�������벻һ�£�����������\n");
		printf("\n\t\t\t����:");
		GetPassword(usr.password);
		l = strlen(usr.password);
		while (l < 6)
		{
			printf("\n\t\t\t���Ȳ�����λ,����������\n");
			printf("\n\t\t\t����:");
			GetPassword(usr.password);
			l = strlen(usr.password);
		}
		printf("\n\t\t\t�ٴ���������:");
		GetPassword(passwd);
	}
	printf("\n\t\t\t�û�����(0�����û� 1��ƱԱ 2���� 9ϵͳ����Ա):");
	scanf("%d",&usr.type);
	getchar();
	if (NULL!=Account_Srv_FindByUsrName(list, usr.username))
	{
		printf("\n\t\t\t���û��Ѿ�����\n");
        printf("\t\t\t��������˳�");
        getch();
		return 0;
	}
	else {
		//��ȡ����
		usr.id = EntKey_Srv_CompNewKey("Account");
		if (Account_Srv_Add(&usr))
		{
			newUsrCount += 1;
			printf("\t\t\t��ӳɹ�\n");
		}
		else
			printf("\t\t\t���ʧ��\n");
	    }
		printf("\t\t\t-------------------------------------------------------\n");
		printf("\t\t\t1.�������    2.����");
		choice = getche();
		//getchar();
	} while ('1' == choice);
	return newUsrCount;
}
	
//�����û��˺����޸��û��˺����룬����������û��˺�������ʾ������Ϣ
int Account_UI_Modify(account_list_t list ,char usrName[]) 
{
	account_t usr;
	int rtn = 0;
	int l;
	char passwd[20];
	if (!Account_UI_Query(list, usrName))
		return 0;
	Account_Srv_FetchByName(usrName, &usr);

	printf("\n\n\t\t\t                       �޸��û���Ϣ\n");
	printf("\t\t\t=========================================================\n");

	printf("\t\t\t������:");
	GetPassword(usr.password);
	l = strlen(usr.password);
	while (l < 6)
	{
		printf("\n\t\t\t���Ȳ�����λ,����������\n");
		printf("\n\t\t\t������:");
		GetPassword(usr.password);
		l = strlen(usr.password);
	}
	printf("\n\t\t\t�ٴ�����������:");
	GetPassword(passwd);
	while (strcmp(usr.password, passwd) != 0)
	{
		printf("\n\t\t\t�������벻һ��,����������\n");
		printf("\n\t\t\t������:");
		GetPassword(usr.password);
		l = strlen(usr.password);
		while (l < 6)
		{
			printf("\n\t\t\t���Ȳ�����λ,����������\n");
			printf("\n\t\t\t������:");
			GetPassword(usr.password);
			l = strlen(usr.password);
		}
		printf("\n\t\t\t�ٴ�����������:");
		GetPassword(passwd);
	}
	if (Account_Srv_Modify(&usr)) {
		rtn = 1;
		printf("\n\n\t\t\t�޸ĳɹ�\n\t\t\t��������˳�\n\t\t\t");
	}
	else
		printf("\n\n\t\t\t�޸�ʧ��\n\t\t\t��������˳�\n\t\t\t");
	getchar();
	return rtn;
}

//�����û��˺���ɾ��һ���Ѿ����ڵ��û��˺���Ϣ���������������û��˺�������ʾ������Ϣ
int Account_UI_Delete(account_list_t list ,char usrName[]) {
	account_t usr;
	int rtn = 0;

	if (!Account_UI_Query(list, usrName))
		return 0;
	Account_Srv_FetchByName(usrName, &usr);
	if (Account_Srv_DeleteByID(usr.id))
	{
			rtn = 1;
			printf("\n\t\t\tɾ���ɹ�!\n\t\t\t��������˳�\n\t\t\t");
	}
	else
		    printf("\n\t\t\tɾ��ʧ��!\n\t\t\t��������˳�\n\t\t\t");
	getch();
	return rtn;

}

//�����û��˺������Ҹ��û��˺��Ƿ���ڣ����ڷ���1�����򷵻�0������ʾ������Ϣ
int Account_UI_Query(account_list_t  list ,char usrName[]) {
	account_t usr;

	if (!Account_Srv_FindByUsrName(list, usrName))
	{
		printf("\t\t\t����û�������,��������˳�");
		getch();
		return 0;
	}
	Account_Srv_FetchByName(usrName, &usr);
	
	printf("\n\n\t\t\t                       �û���Ϣ����\n");
	printf("\t\t\t=========================================================\n");
	printf("\t\t\t�û�ID: %d\n",usr.id);
	printf("\t\t\t�û�����:  %d\n", usr.type);
	printf("\t\t\t�û�����: ");
	puts(usr.username);
	printf("\t\t\t�û�����: ");
	puts(usr.password);
	printf("\t\t\t=========================================================\n");

	return 1;
}






