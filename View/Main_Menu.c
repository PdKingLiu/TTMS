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
			printf("\n\n\n\n\n\n\n\t\t\t\t\t��ƱԱ����,��ѡ��\n\n"); 
			printf("\t\t\t\t\t  1.��Ʊ����Ʊ\n");
			printf("\t\t\t\t\t  2.��ѯ�ݳ�Ʊ\n");
			printf("\t\t\t\t\t  3.��ѯ�ݳ�\n");
			printf("\t\t\t\t\t  4.ͳ�����۶�\n");
			printf("\t\t\t\t\t  5.ά����������\n");
			printf("\t\t\t\t\t  6.�˳�ϵͳ\n");
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
				printf("\n\t\t\t\t\t�ݳ��ƻ�ID:");
				scanf("%d", &sal_id);
				getchar();
				if (!Schedule_Srv_FetchByID(sal_id, &buf2))
				{
					printf("\t\t\t\t\t�ݳ��ƻ�������,���������");
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
			printf("\n\n\n\n\n\n\n\t\t\t\t\t��������,��ѡ��\n\n"); 
			printf("\t\t\t\t\t  1.�����Ŀ\n");
			printf("\t\t\t\t\t  2.��ѯ�ݳ�\n");
			printf("\t\t\t\t\t  3.ͳ�����۶�\n");
			printf("\t\t\t\t\t  4.ά����������\n");
			printf("\t\t\t\t\t  5.ͳ��Ʊ��\n");
			printf("\t\t\t\t\t  6.��ѯ�ݳ�Ʊ\n");
			printf("\t\t\t\t\t  7.�����ݳ�\n");
			printf("\t\t\t\t\t  8.�˳�ϵͳ\n");
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
				printf("\n\t\t\t\t\t�ݳ��ƻ�ID:");
				scanf("%d", &sal_id);
				getchar();
				if (!Schedule_Srv_FetchByID(sal_id, &buf2))
				{
					printf("\t\t\t\t\t�ݳ��ƻ�������,���������");
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
			printf("\n\n\n\n\n\n\n\t\t\t\t\t����Ա����,��ѡ��\n\n"); 
			printf("\t\t\t\t\t  1.�����ݳ���\n");
			printf("\t\t\t\t\t  2.����ϵͳ�û�\n");
			printf("\t\t\t\t\t  3.ά����������\n");
			printf("\t\t\t\t\t  4.�˳�ϵͳ\n");
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
			printf("\n\n\n\n\n\n\n\t\t\t\t\t����,��ѡ��\n\n"); 
			printf("\t\t\t\t\t  1.��Ϣ��ѯ\n");
			printf("\t\t\t\t\t  2.�˳�ϵͳ\n");
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

