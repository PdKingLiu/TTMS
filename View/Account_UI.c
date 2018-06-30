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
            return "售票员";
            break;
            
        case 2:
            return "经理";
            break;
        
        case 9:
            return "系统管理员";
            break;
    
        case 0:
            return "匿名用户";
            break;
     
    }
}

//密码输入函数 
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
            printf("嘤");
        }
        password[i]='\0';
	return i; 
}

//登录函数，提示用户输入用户名和密码，登录成功return 1，否则提示重新登录，超过3次，登录失败
int SysLogin() {
	Account_Srv_InitSys();
	char usrname[20];
	char passwd[20];
	int i=0,j;
	system("cls"); 
	
	for (i = 0; i < 3; i++) {
		printf("\n\n\n\n\n\n\n\n\t\t\t\t      欢迎来到掬月楼票务管理系统\n");
		printf("\n\n\t\t\t\t\t　用户名: ");
		gets(usrname);
		printf("\n\t\t\t\t\t　密码: ");
		GetPassword(passwd);
		if (Account_Srv_Verify(usrname, passwd))
		{
			
			
		printf("\n\n\n\t\t\t\t     ┏━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
		printf("\t\t\t\t     ┃                         ┃\n");
		printf("\t\t\t\t     ┗━━━━━━━━━━━━━━━━━━━━━━━━━┛"); 
			printf("\n\n\n\n\t\t\t\t      ");
			gotoxy(40,18);
			for (j = 0; j < 12; j++)
			{
				Sleep(100);
				printf("▉");
			}

			return 1;
		}
		printf("\n\n\n\t\t\t\t     ┏━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
		printf("\t\t\t\t     ┃                         ┃\n");
		printf("\t\t\t\t     ┗━━━━━━━━━━━━━━━━━━━━━━━━━┛"); 
		printf("\n\n\n\n\t\t\t\t      ");
		gotoxy(40,18);
		for (j = 0; j < 12; j++)
		{
			Sleep(100);
			printf("▉");
		}
		printf("\n\n\t\t\t\t      账号或密码有误,按任意键退出\n");
		getch(); 
		system("cls");
	}
	return 0;
}
//管理系统用户功能入口函数，显示用户账号管理菜单
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

	//载入数据
	paging.totalRecords = Account_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);
	if (gl_CurUser.type == 9)
	{
		do {
			system("cls");
			printf("\n\t\t\t\t\t       用户管理\n\n\n");
			printf("\t\t\t%-6s  %-20s  %-16s  %-16s   \n\n", "ID", "  用户类型", "用户名", "  密码");
			
			//显示数据
			for (i = 0, pos = (account_node_t *)(paging.curPos);
				pos != head && i < paging.pageSize; i++) {
				printf("\t\t\t%-6d  %-20s  %-16s  %-16s  \n",pos->data.id,usr_type(pos->data.type), pos->data.username,"********");
				pos = pos->next;
			}
			printf("\n\t\t\t总数:%2d\t\t\t\t\t\t    %d/%d \n",
				paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));

			printf("\n\n\t\t\t  1.上一页 2.下一页 3.添加 4.删除 5.修改 6.查询 7.返回");
			printf("\n\n\n\t\t\t:");
			fflush(stdin);
			choice = getche();
			choice = toupper(choice);
			fflush(stdin);
			switch (choice) {
			case '3':
				if (Account_UI_Add(head)) //新添加成功，跳到最后一页显示
				{
					paging.totalRecords = Account_Srv_FetchAll(head);
					Paging_Locate_LastPage(head, paging, account_node_t);
				}
				break;
			case '4':
				//getchar();
				printf("\n\n\t\t\t输入用户名:");
				gets(usr.username);
				if (Account_UI_Delete(head, usr.username)) {	//从新载入数据
					paging.totalRecords = Account_Srv_FetchAll(head);
					List_Paging(head, paging, account_node_t);
				}
				break;
			case '5':
				//getchar();
				printf("\n\n\t\t\t输入用户名:");
				gets(usr.username);
				if (Account_UI_Modify(head, usr.username)) {	//从新载入数据
					paging.totalRecords = Account_Srv_FetchAll(head);
					List_Paging(head, paging, account_node_t);
				}
				break;
			case '6':
				//getchar();
				printf("\n\n\t\t\t输入用户名:");
				gets(usr.username);
				if (Account_UI_Query(head, usr.username)) {
					//从新载入数据
					paging.totalRecords = Account_Srv_FetchAll(head);
					List_Paging(head, paging, account_node_t);
					printf("\n\n\t\t\t按任意键退出");
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
			printf("\n\n\t\t\t                        修改密码\n");
			printf("\t\t\t=========================================================\n");

			printf("\t\t\t\t1.修改\t\t\t\t2.返回");
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
				if (Account_UI_Modify(head, usr.username)) {	//从新载入数据
					paging.totalRecords = Account_Srv_FetchAll(head);
					List_Paging(head, paging, account_node_t);
				}
				break;
			}
		} while (choice != '2');

	}
	//getchar();
	//释放链表空间
	List_Destroy(head, account_node_t);
}




//添加一个用户账号信息，如果账号名存在，提示出错信息
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
	printf("\n\n\t\t\t                        添加账户\n");
	printf("\t\t\t=========================================================\n");

	printf("\n\t\t\t用户名:");
	gets(usr.username);
	printf("\n\t\t\t密码:");
	GetPassword(usr.password);
	l = strlen(usr.password);
	while (l < 6)
	{
		printf("\n\t\t\t长度不足六位,请重新输入\n");
		printf("\n\t\t\t密码:");
		GetPassword(usr.password);
		l = strlen(usr.password);
	}
	printf("\n\t\t\t再次输入密码:");
	GetPassword(passwd);
	while (strcmp(usr.password, passwd) != 0)
	{
		printf("\n\t\t\t两次密码不一致，请重新输入\n");
		printf("\n\t\t\t密码:");
		GetPassword(usr.password);
		l = strlen(usr.password);
		while (l < 6)
		{
			printf("\n\t\t\t长度不足六位,请重新输入\n");
			printf("\n\t\t\t密码:");
			GetPassword(usr.password);
			l = strlen(usr.password);
		}
		printf("\n\t\t\t再次输入密码:");
		GetPassword(passwd);
	}
	printf("\n\t\t\t用户类型(0匿名用户 1售票员 2经理 9系统管理员):");
	scanf("%d",&usr.type);
	getchar();
	if (NULL!=Account_Srv_FindByUsrName(list, usr.username))
	{
		printf("\n\t\t\t该用户已经存在\n");
        printf("\t\t\t按任意键退出");
        getch();
		return 0;
	}
	else {
		//获取主键
		usr.id = EntKey_Srv_CompNewKey("Account");
		if (Account_Srv_Add(&usr))
		{
			newUsrCount += 1;
			printf("\t\t\t添加成功\n");
		}
		else
			printf("\t\t\t添加失败\n");
	    }
		printf("\t\t\t-------------------------------------------------------\n");
		printf("\t\t\t1.继续添加    2.返回");
		choice = getche();
		//getchar();
	} while ('1' == choice);
	return newUsrCount;
}
	
//根据用户账号名修改用户账号密码，不存在这个用户账号名，提示出错信息
int Account_UI_Modify(account_list_t list ,char usrName[]) 
{
	account_t usr;
	int rtn = 0;
	int l;
	char passwd[20];
	if (!Account_UI_Query(list, usrName))
		return 0;
	Account_Srv_FetchByName(usrName, &usr);

	printf("\n\n\t\t\t                       修改用户信息\n");
	printf("\t\t\t=========================================================\n");

	printf("\t\t\t新密码:");
	GetPassword(usr.password);
	l = strlen(usr.password);
	while (l < 6)
	{
		printf("\n\t\t\t长度不足六位,请重新输入\n");
		printf("\n\t\t\t新密码:");
		GetPassword(usr.password);
		l = strlen(usr.password);
	}
	printf("\n\t\t\t再次输入新密码:");
	GetPassword(passwd);
	while (strcmp(usr.password, passwd) != 0)
	{
		printf("\n\t\t\t两次密码不一致,请重新输入\n");
		printf("\n\t\t\t新密码:");
		GetPassword(usr.password);
		l = strlen(usr.password);
		while (l < 6)
		{
			printf("\n\t\t\t长度不足六位,请重新输入\n");
			printf("\n\t\t\t新密码:");
			GetPassword(usr.password);
			l = strlen(usr.password);
		}
		printf("\n\t\t\t再次输入新密码:");
		GetPassword(passwd);
	}
	if (Account_Srv_Modify(&usr)) {
		rtn = 1;
		printf("\n\n\t\t\t修改成功\n\t\t\t按任意键退出\n\t\t\t");
	}
	else
		printf("\n\n\t\t\t修改失败\n\t\t\t按任意键退出\n\t\t\t");
	getchar();
	return rtn;
}

//根据用户账号名删除一个已经存在的用户账号信息，如果不存在这个用户账号名，提示出错信息
int Account_UI_Delete(account_list_t list ,char usrName[]) {
	account_t usr;
	int rtn = 0;

	if (!Account_UI_Query(list, usrName))
		return 0;
	Account_Srv_FetchByName(usrName, &usr);
	if (Account_Srv_DeleteByID(usr.id))
	{
			rtn = 1;
			printf("\n\t\t\t删除成功!\n\t\t\t按任意键退出\n\t\t\t");
	}
	else
		    printf("\n\t\t\t删除失败!\n\t\t\t按任意键退出\n\t\t\t");
	getch();
	return rtn;

}

//根据用户账号名查找该用户账号是否存在，存在返回1，否则返回0，并提示错误信息
int Account_UI_Query(account_list_t  list ,char usrName[]) {
	account_t usr;

	if (!Account_Srv_FindByUsrName(list, usrName))
	{
		printf("\t\t\t这个用户不存在,按任意键退出");
		getch();
		return 0;
	}
	Account_Srv_FetchByName(usrName, &usr);
	
	printf("\n\n\t\t\t                       用户信息如下\n");
	printf("\t\t\t=========================================================\n");
	printf("\t\t\t用户ID: %d\n",usr.id);
	printf("\t\t\t用户种类:  %d\n", usr.type);
	printf("\t\t\t用户名称: ");
	puts(usr.username);
	printf("\t\t\t用户密码: ");
	puts(usr.password);
	printf("\t\t\t=========================================================\n");

	return 1;
}






