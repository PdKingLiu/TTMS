#include <stdio.h>
#include "Main_Menu.h"
#include "Studio_UI.h"
#include "../Service/Account.h"	
#include "Queries_Menu.h"
#include "Query_Play_UI.h"
#include "Sale_UI.h"
#include "Play_UI.h"
#include "SalesAnalysis_UI.h"
#include "Play_UI.h"	
#include "conio.h"
#include "Ticket_UI.h" 
#include "../Persistence/Play_Persist.h" 

void Main_Menu(account_t usr)
{
	char choice;
	char ch[30];
	int sal_id;
	play_t buf;
	schedule_t buf2;
	play_list_t list;
	switch (usr.type)
	{
	case 1:
		do {
            system("cls");
			printf("\n\n\n\n\n\n\n\t\t\t\t\t售票员您好,请选择\n\n"); 
			printf("\t\t\t\t\t  1.售票与退票\n");
			printf("\t\t\t\t\t  2.查询演出票\n");
			printf("\t\t\t\t\t  3.查询演出\n");
			printf("\t\t\t\t\t  4.统计销售额\n");
			printf("\t\t\t\t\t  5.维护个人资料\n");
			printf("\t\t\t\t\t  6.退出系统\n");
			printf("\n\t\t\t\t\t:");
			choice = getche();
			switch (choice) {
			case '3':
				Schedule_UI_ListAll();
				break;
			case '5':
				Account_UI_MgtEntry();
				break;
			case '2':
				printf("\n\t\t\t\t\t演出计划ID:");
				scanf("%d", &sal_id);
				getchar();
				if (!Schedule_Srv_FetchByID(sal_id, &buf2))
				{
					printf("\t\t\t\t\t演出计划不存在,任意键返回");
					getch();
				}
				else
				{
					Sale_UI_ShowTicket2(sal_id);
				}
				break;
			case '1':
				Sale_UI_MgtEntry();
				break;
			case '4':
				SalesAanalysis_UI_MgtEntry();
				break;
			}
		} while ('6' != choice);
	
		break;
	case 2:
		do {
            system("cls");
			printf("\n\n\n\n\n\n\n\t\t\t\t\t经理您好,请选择\n\n"); 
			printf("\t\t\t\t\t  1.管理剧目\n");
			printf("\t\t\t\t\t  2.查询演出\n");
			printf("\t\t\t\t\t  3.统计销售额\n");
			printf("\t\t\t\t\t  4.维护个人资料\n");
			printf("\t\t\t\t\t  5.统计票房\n");
			printf("\t\t\t\t\t  6.查询演出票\n");
			printf("\t\t\t\t\t  7.安排演出\n");
			printf("\t\t\t\t\t  8.退出系统\n");
			printf("\n\t\t\t\t\t:");
			choice = getche();
			switch (choice) {
			case '5':
				SalesAnalysis_UI_BoxOffice();
				break; 
			case '4':
				Account_UI_MgtEntry();
				break;
			case '7':
				Schedule_UI_MgtEntry(); 
				break;
			case '1':
				Play_UI_MgtEntry(0);
				break;
			case '6':
				printf("\n\t\t\t\t\t演出计划ID:");
				scanf("%d", &sal_id);
				getchar();
				if (!Schedule_Srv_FetchByID(sal_id, &buf2))
				{
					printf("\t\t\t\t\t演出计划不存在,任意键返回");
					getch();
				}
				else
				{
					Sale_UI_ShowTicket2(sal_id);
				}
				break;
				break;
			case '2':
				Schedule_UI_ListAll();
				break;
			case '3':
				SalesAanalysis_UI_MgtEntry();
				break;
				
			}
		} while ('8' != choice);
		break;
	case 9:
		do {
            system("cls");
			printf("\n\n\n\n\n\n\n\t\t\t\t\t管理员您好,请选择\n\n"); 
			printf("\t\t\t\t\t  1.管理演出厅\n");
			printf("\t\t\t\t\t  2.管理系统用户\n");
			printf("\t\t\t\t\t  3.维护个人资料\n");
			printf("\t\t\t\t\t  4.退出系统\n");
			printf("\n\t\t\t\t\t:");
			choice = getche();
			switch (choice) {
			case '1':
				Studio_UI_MgtEntry(usr.type);
				break;
			case '2':
				Account_UI_MgtEntry();
				break;
			case '3':
				Account_UI_MgtEntry();
				break;
			}
		} while ('4' != choice);
		break;
	case 0:
	default:
		do {
            system("cls");
			printf("\n\n\n\n\n\n\n\t\t\t\t\t您好,请选择\n\n"); 
			printf("\t\t\t\t\t  1.信息查询\n");
			printf("\t\t\t\t\t  2.退出系统\n");
			printf("\n\t\t\t\t\t:");
			choice = getche();
			switch (choice) {
			case '1':
				Queries_Menu();
				break;
			}
		} while ('2' != choice);
		break;
	}
	printf("\n\t\t\t");
	getchar();
}

