/*
 * SalesAnalysis_UI.c
 *
 *  Created on: 2015��5��8��
 *      Author: Administrator
 */
#include "SalesAnalysis_UI.h"
#include "../Common/list.h"
#include "../Common/common.h"
#include "../Service/SalesAnalysis.h"
#include "conio.h"
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

//�ⲿ����gl_CurUser ���ڴ洢��¼�û���Ϣ
extern account_t gl_CurUser;

//�����ҳ������ҳ���СΪ5
static const int SALESANALYSIS_PAGE_SIZE = 5;

//��Ժ�������а���,������ʾ��ֹĿǰ��Ժ��ӰƱ�����а�
void SalesAnalysis_UI_BoxOffice() {
	salesanalysis_list_t  head;
	salesanalysis_node_t  *pos;
	int i;
	char choice;

	
	Pagination_t paging;
	paging.offset = 0;
	
	List_Init(head, salesanalysis_node_t);
	
	Paging_Locate_FirstPage(head, paging);
	//���÷�ҳ��С
	paging.totalRecords = Salesanalysis_Srv_StaticSale(head);
	SalesAnalysis_Srv_SortBySale(head);
	/*���÷�ҳ��ʾҳ���С*/
	paging.pageSize = SALESANALYSIS_PAGE_SIZE;
	do {
		system("cls");
		printf("\n\n\t\t\t                        ͳ��Ʊ��\n");
		printf("\t      ============================================================================\n");
		printf("\t      %-4s %-4s    %-12s%-8s%-8s  %-6s    %-14s%-10s\n","ID","����","Ӱ��","����","��Ʊ��","Ʊ��","����ʱ��","����ʱ��");
		//��ʾ����
		int icount = 1;
		for (i = 0, pos = head->next;
			pos != head && i < paging.pageSize; i++)
		{
			printf("\t      %-4d  %-4d%    -14s  %-8s  %-8d%-8d%4d-%02d-%02d    %4d-%02d-%02d\n",pos->data.play_id,icount,pos->data.name,pos->data.area,pos->data.totaltickets,pos->data.sales,pos->data.start_date.year,pos->data.start_date.month,pos->data.start_date.day,pos->data.end_date.year,pos->data.end_date.month,pos->data.end_date.day);
			pos = pos->next;
			icount++;
		}
		printf("\t      ============================================================================\n");
		printf("\n\t\t\t1.��һҳ");
		printf("\t\t    2.��һҳ");
		printf("\t\t    3.����\n");
		printf("\n\t\t\t��");
		fflush(stdin);
 		choice = getche();
		switch (choice)
		{
		case '1':
			if (1 < Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, salesanalysis_node_t);
			}
			break;
		case '2':
			if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, salesanalysis_node_t);
			}
			break;
		}
	} while (choice != '3');
}

//��ʾ��ƱԱ�ڸ��������������Ʊ���
void SalesAnalysis_UI_StatSale(int usrID, user_date_t stDate, user_date_t endDate){
	int rtn;
	account_t buf;
	Account_Srv_FetchByID(usrID, &buf);
	rtn = Salesanalysis_Srv_CompSaleVal(usrID, stDate,endDate);
	system("cls");
	printf("\n\n\t\t\t                        ��ƱԱ��Ϣ\n");
	printf("\t\t\t=========================================================\n");
	printf("\t\t\t\t%-10s %-20s %-15s\n", "ID", "�û���", "����");
	printf("\t\t\t\t%-10d %-20s %-15d\n", buf.id, buf.username, rtn);
	printf("\n\n\t\t\t�����������");
	getch(); 
	return ;
}

//���۷�����ں�������ʾ�˵����˵�����"������ʾ��ֹĿǰ��Ժ��ӰƱ�����а�"������ʾ���ѯ������ƱԱ��Ʊ�������
//����ѯ������������ĳ��ƱԱ��Ʊ�����
void SalesAanalysis_UI_MgtEntry() {
	char choice;
	char ch[20] ;
	int id;
	account_t buf;
	user_date_t stDate;
	user_date_t endDate;
	if (gl_CurUser.type == 2) {
		do
		{
			printf("\n\n\t\t\t                        ����ͳ�������\n");
			printf("\t\t\t=========================================================\n");
			printf("\t\t\t	1.��ƱԱ�������(��ʱ���)	2.����");
			printf("\n\n\t\t\t:");
			choice = getche();
			switch (choice)
			{ 

			case '1':
				printf("\n\t\t\t��������ƱԱ���û���:");
				scanf("%s", ch);
				Account_Srv_FetchByName(ch, &buf);
				printf("\t\t\t�����뿪ʼ��ѯ����(xxxx-xx-xx):");
				scanf("%d-%d-%d", &stDate.year, &stDate.month, &stDate.day);
				printf("\t\t\t�����������ѯ����(xxxx-xx-xx):");
				scanf("%d-%d-%d", &endDate.year, &endDate.month, &endDate.day);
				getchar();
				SalesAnalysis_UI_StatSale(buf.id, stDate, endDate);
				break; 

			}
		} while (choice != '2');
	}
	else {
		do
		{
			system("cls");
			printf("\n\n\t\t\t                      �������ݹ���\n");
			printf("\t\t\t=========================================================\n");

			printf("\t\t\t     1.��ѯ�Լ������۳�   2.��ѯ�Լ������۳�   3.����");
			printf("\n\t\t\t=========================================================\n");
			printf("\n\t\t\t:");
			choice = getche();
			switch (choice)
			{
			case '1':
				id = gl_CurUser.id;
				stDate = DateNow();
				stDate.month= stDate.month-1; 
				endDate = DateNow();
				SalesAnalysis_UI_StatSale(id, stDate, endDate);
				break;
			case '2':
				id = gl_CurUser.id;
				stDate = DateNow();
				endDate = DateNow();
				SalesAnalysis_UI_StatSale(id, stDate, endDate);
				break;
			}
		} while (choice != '3');

	}
}



