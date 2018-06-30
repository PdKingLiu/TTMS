/*
 * SalesAnalysis_UI.c
 *
 *  Created on: 2015年5月8日
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

//外部变量gl_CurUser 用于存储登录用户信息
extern account_t gl_CurUser;

//定义分页机制中页面大小为5
static const int SALESANALYSIS_PAGE_SIZE = 5;

//剧院销售排行榜函数,降序显示截止目前剧院电影票房排行榜
void SalesAnalysis_UI_BoxOffice() {
	salesanalysis_list_t  head;
	salesanalysis_node_t  *pos;
	int i;
	char choice;

	
	Pagination_t paging;
	paging.offset = 0;
	
	List_Init(head, salesanalysis_node_t);
	
	Paging_Locate_FirstPage(head, paging);
	//设置分页大小
	paging.totalRecords = Salesanalysis_Srv_StaticSale(head);
	SalesAnalysis_Srv_SortBySale(head);
	/*设置分页显示页面大小*/
	paging.pageSize = SALESANALYSIS_PAGE_SIZE;
	do {
		system("cls");
		printf("\n\n\t\t\t                        统计票房\n");
		printf("\t      ============================================================================\n");
		printf("\t      %-4s %-4s    %-12s%-8s%-8s  %-6s    %-14s%-10s\n","ID","排名","影名","地区","总票数","票房","上线时间","下线时间");
		//显示数据
		int icount = 1;
		for (i = 0, pos = head->next;
			pos != head && i < paging.pageSize; i++)
		{
			printf("\t      %-4d  %-4d%    -14s  %-8s  %-8d%-8d%4d-%02d-%02d    %4d-%02d-%02d\n",pos->data.play_id,icount,pos->data.name,pos->data.area,pos->data.totaltickets,pos->data.sales,pos->data.start_date.year,pos->data.start_date.month,pos->data.start_date.day,pos->data.end_date.year,pos->data.end_date.month,pos->data.end_date.day);
			pos = pos->next;
			icount++;
		}
		printf("\t      ============================================================================\n");
		printf("\n\t\t\t1.上一页");
		printf("\t\t    2.下一页");
		printf("\t\t    3.返回\n");
		printf("\n\t\t\t：");
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

//显示售票员在给定日期区间的售票情况
void SalesAnalysis_UI_StatSale(int usrID, user_date_t stDate, user_date_t endDate){
	int rtn;
	account_t buf;
	Account_Srv_FetchByID(usrID, &buf);
	rtn = Salesanalysis_Srv_CompSaleVal(usrID, stDate,endDate);
	system("cls");
	printf("\n\n\t\t\t                        售票员信息\n");
	printf("\t\t\t=========================================================\n");
	printf("\t\t\t\t%-10s %-20s %-15s\n", "ID", "用户名", "总数");
	printf("\t\t\t\t%-10d %-20s %-15d\n", buf.id, buf.username, rtn);
	printf("\n\n\t\t\t按任意键返回");
	getch(); 
	return ;
}

//销售分析入口函数，显示菜单，菜单包含"降序显示截止目前剧院电影票房排行榜"，“显示或查询当日售票员售票情况”，
//“查询给定日期区间某售票员售票情况”
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
			printf("\n\n\t\t\t                        销售统计与分析\n");
			printf("\t\t\t=========================================================\n");
			printf("\t\t\t	1.售票员销售情况(按时间段)	2.返回");
			printf("\n\n\t\t\t:");
			choice = getche();
			switch (choice)
			{ 

			case '1':
				printf("\n\t\t\t请输入售票员的用户名:");
				scanf("%s", ch);
				Account_Srv_FetchByName(ch, &buf);
				printf("\t\t\t请输入开始查询日期(xxxx-xx-xx):");
				scanf("%d-%d-%d", &stDate.year, &stDate.month, &stDate.day);
				printf("\t\t\t请输入结束查询日期(xxxx-xx-xx):");
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
			printf("\n\n\t\t\t                      销售数据管理\n");
			printf("\t\t\t=========================================================\n");

			printf("\t\t\t     1.查询自己今月售出   2.查询自己今日售出   3.返回");
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



