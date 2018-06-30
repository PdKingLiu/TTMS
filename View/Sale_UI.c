#include "Play_UI.h"
#include "Schedule_UI.h"
#include "Sale_UI.h"
#include "../Common/list.h"
#include "../Service/Ticket.h"
#include "../Service/Seat.h"
#include "../Service/Studio.h"
#include "../Service/EntityKey.h"
#include "../Service/Sale.h"
#include "../Service/Schedule.h"
#define PAGESIZE 5

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "conio.h"

//引用登陆用户的全局数据结构
extern account_t gl_CurUser;

//根据计划ID，显示演出票
void Sale_UI_ShowTicket(int schID)
{
	int pageSize = 10, i;
	int listSize;
	int ticket_id;
	char choice;
	ticket_list_t list, s;
	seat_t buf;
	Pagination_t paging;
	List_Init(list, ticket_node_t);
	listSize = Ticket_Srv_FetchBySchID(list, schID);
	paging.pageSize = pageSize;
	paging.totalRecords = listSize;
	Paging_Locate_FirstPage(list, paging);
	do
	{
		system("cls");
		printf("\n\t\t\t                      票务系统\n");
				printf("\t\t\t===================================================\n");
		printf("\t\t\t %-4s %10s   %-8s%4s %4s    %-6s%-8s\n\n", "ID", "演出计划ID", "座位ID", "行", "列", "状态","票价");
		
		for (i = 0, s = (ticket_node_t *)(paging.curPos); s != list && i < paging.pageSize; i++, s = s->next)
		{
			Seat_Srv_FetchByID(s->data.seat_id, &buf);
			if (buf.status == 1)
				printf("\t\t\t %-4d       %-6d   %-6d%4d %4d    %-6s  %-8d\n", s->data.id,
				 s->data.schedule_id, s->data.seat_id, buf.row + 1, buf.column + 1, Status(s->data.status),s->data.price);
		}
		
		printf("\n\t\t\t\t总数:%2d\t\t\t\t%d/%d \n", paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));
		printf("\t\t\t===================================================\n");
		printf("\t\t\t      1.上一页　　    　　      2.下一页　       \n");
		printf("\t\t\t      3.退票　　　              4.订票　　　   　\n");
		printf("\t\t\t      5.返回  　　            　       \n");
		printf("\t\t\t===================================================\n");
		printf("\n\t\t\t:");
		choice = getche();
		switch (choice)
		{ 
		case '4':
			printf("\n\t\t\t票的ID：");
			scanf("%d",&ticket_id);
            getchar();
            Sale_UI_SellTicket(ticket_id);
			paging.totalRecords = Ticket_Srv_FetchBySchID(list, schID);
			List_Paging(list, paging, ticket_node_t);
			break; 
		case '5':
			break; 
		case '3':
			printf("\n\t\t\t票的ID：");
			scanf("%d", &ticket_id);
			getchar();
			Sale_UI_ReturnTicket(ticket_id);
			paging.totalRecords = Ticket_Srv_FetchBySchID(list, schID);
			List_Paging(list, paging, ticket_node_t);
			break; 
		case '1':
			if (1 < Pageing_CurPage(paging))
			{
				Paging_Locate_OffsetPage(list, paging, -1, ticket_node_t);
			}
			break; 
		case '2':
			if (Pageing_TotalPages(paging) > Pageing_CurPage(paging))
			{
				Paging_Locate_OffsetPage(list, paging, 1, ticket_node_t);
			}
			break;
		}
	} while (choice != '5');
}


void Sale_UI_ReturnTicket(int ticket_id)
{
	ticket_t buf, data;
	sale_t a;
	if (Ticket_Srv_FetchByID(ticket_id, &buf))
	{
		if (buf.status == 0)
		{
			printf("\t\t\t此票未售出,任意键返回");
			getch();
		}
		else
		{
			//根据票的id获得票售票的基本信息
			Select_Price_By_Playid(buf.id, &a);
			
			data.id = buf.id;
			data.schedule_id = buf.schedule_id;
			data.seat_id = buf.seat_id;
			data.price = buf.price;
			data.status = 0;
			
			if (Ticket_Srv_Modify(&data)&& Sale_Srv_DeleteByID(a.id))
			{
				printf("\t\t\t退订成功,任意键返回");
				getch();
			}
			else
			{
				printf("\t\t\t退订失败,任意键返回");
				getch();
			}
		}
	}
	else
	{
		printf("\t\t\t无此票,任意键返回");
		getchar();
	}
}

int Sale_UI_SellTicket(int ticket_id)
{
	ticket_t buf, data;
	sale_t sale;
	system("cls");
	printf("\n\n\n\n\n") ;
	if (Ticket_Srv_FetchByID(ticket_id, &buf))
	{
		if (buf.status == 1)
		{
			printf("\n\t\t\t\t        此票已售,任意键返回");
			getch();
		}
		else
		{
			sale.id = EntKey_Srv_CompNewKey("Sale");
			sale.user_id = gl_CurUser.id;
			sale.date = DateNow();
			sale.time = TimeNow();
			sale.ticket_id = buf.id;
			data.id = buf.id;
			data.schedule_id = buf.schedule_id;
			data.seat_id = buf.seat_id;
			sale.value=data.price = buf.price;
			data.status = 1;
			if (Ticket_Srv_Modify(&data) && Sale_Srv_Add(&sale))
			{
				printf("\n\t\t\t\t    订单时间:%d年-%02d月-%02d日 %d时-%02d分-%02d秒\n", sale.date.year,sale.date.month,sale.date.day,sale.time.hour,sale.time.minute,sale.time.second);
				printf("\t\t\t\t    售票员ID: %d\n",sale.user_id);
				Ticket_Info(&data);
				printf("\t\t\t\t    订购成功,任意键返回");
				getch();
			}
			else
			{
				printf("\t\t\t\t    订购失败,任意键返回");
				getch();
			}
		}
	}
	else
	{
		printf("\t\t\t\t        无此票,任意键返回");
		getch();
	}
    return 0;
}

void Ticket_Info(ticket_t *data)
{
	schedule_t p;
	play_t buf;
	seat_t s;
	studio_t studio;
	
	Seat_Srv_FetchByID(data->seat_id, &s);
	Schedule_Srv_FetchByID(data->schedule_id, &p);
	if (Play_Srv_FetchByID(p.play_id, &buf))
	{
		Studio_Srv_FetchByID(p.studio_id,&studio);
		printf("\n\n\t\t\t\t\t\t    影票\n");
		printf("\t\t\t\t    =====================================\n");
		printf("\t\t\t\t    剧目名称: %s\n", buf.name);
		printf("\t\t\t\t    类型：%s\n", type(buf.type));
		printf("\t\t\t\t    出品地区：%s\n", buf.area);
		printf("\t\t\t\t    剧目等级：%s\n", rating(buf.rating));
		printf("\t\t\t\t    时长: %d 分钟\n", buf.duration);
		
		printf("\t\t\t\t    放映时间: %4d年%02d月%02d日 ",p.date.year,p.date.month,p.date.day);
		printf("%2d时%02d分%02d秒\n", p.time.hour, p.time.minute, p.time.second);
		
		printf("\t\t\t\t    票价：%d 元\n", buf.price);
		printf("\t\t\t\t    演出厅:%s\n", studio.name);
		printf("\t\t\t\t    位置: %d行%d列\n", s.row + 1, s.column + 1);
		printf("\t\t\t\t    =====================================\n");

	}
}
//根据剧目ID显示演出计划
void Sale_UI_ShowScheduler(int playID)
{
	play_t buf;
	if (Play_Srv_FetchByID(playID, &buf))
	{
		int listSize, sal_id, i;
		char choice;
		Pagination_t paging;
		schedule_list_t list, pos,p;
		schedule_t buf;
		studio_t buf3;
		play_t buf2;
		List_Init(list, schedule_node_t);
		listSize = Schedule_Srv_FetchByPlay(list, playID);
		paging.pageSize = PAGESIZE;
		paging.totalRecords = listSize;
		Paging_Locate_FirstPage(list, paging);
		do {
			system("cls");
			printf("\n\t\t\t\t\t\t演出计划\n\n");
			printf("\t\t  %-4s%-14s%-12s  %-10s    %-10s%-8s\n\n", "ID", "剧目ID", "演出厅ID", "放映日期", "放映时间", "座位数");
			
			//Paging_ViewPage_ForEach(list, paging, schedule_node_t, pos, i)
			for (i = 0, pos = (schedule_node_t *)(paging.curPos); pos != list && i < paging.pageSize; i++, pos = pos->next)
			{
				Play_Srv_FetchByID(pos->data.play_id, &buf2);
				Studio_Perst_SelectByID(pos->data.studio_id, &buf3);
				printf("\t\t  %-4d%-14s%-12s%6d-%02d-%02d    %2d:%02d:%02d  %4d\n", pos->data.id,buf2.name, buf3.name, pos->data.date.year, pos->data.date.month, pos->data.date.day, pos->data.time.hour, pos->data.time.minute, pos->data.time.second, pos->data.seat_count);
			}
			
			printf("\n\t\t\t  总数:%d\t\t\t\t\t%d/%d \n\n", paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));
			
			printf("\n\t\t\t1.上一页    2.下一页    3.进入售退票系统    4.返回 \n");
			
			printf("\n\t\t\t:");
			fflush(stdin);
			choice = getche();
			switch (choice)
			{
			case '3':
				printf("\n\t\t\t演出计划ID:");
				scanf("%d", &sal_id);
				getchar();
				if (!Schedule_Srv_FetchByID(sal_id, &buf))
				{
					printf("\t\t\t演出计划不存在,任意键返回");
					getch();
				}
				else
				{
					Sale_UI_ShowTicket(sal_id);
				}
				paging.totalRecords = Schedule_Srv_FetchByPlay(list, playID);
				List_Paging(list, paging, schedule_node_t);
				break; 
			case '4':
				break; 
			case '1':
				if (1 < Pageing_CurPage(paging))
				{
					Paging_Locate_OffsetPage(list, paging, -1, studio_node_t);
				}
				choice = '3';
				break; 
			case '2':
				if (Pageing_TotalPages(paging) > Pageing_CurPage(paging))
				{
					Paging_Locate_OffsetPage(list, paging, 1, studio_node_t);
				}
				choice = '3';
				break;
			}
		} while (choice != '4');
	}
	else
	{
		printf("      没有此剧目,按任意键退出");
		getch();
	}
}

void Sale_UI_MgtEntry()
{

	int listSize, i, id;
	char choice;
	char name[31];
	Pagination_t paging;
	play_list_t list, p, data, pos;
	List_Init(data, play_node_t);
	List_Init(list, play_node_t);
	listSize = Play_Srv_FetchAll(list);
	paging.pageSize = PAGESIZE;
	paging.totalRecords = listSize;
	Paging_Locate_FirstPage(list, paging);
	do {
		system("cls");
		printf("\n\t\t\t                      售票与退票\n\n\n");
		
		printf("      %-5s  %-16s  %-4s  %8s  %8s %8s  %10s  %10s  %5s\n\n",
			"ID", "    影名", "类型", "地区", "演出级别", "时长", "上映日期", "结束日期", "价格");
		
		for (i = 0, p = (play_node_t *)(paging.curPos); p != list && i < paging.pageSize; i++, p = p->next)
		{
			printf("      %-5d  %-16s  %-4s  %8s   %8s  %5d   %4d-%02d-%02d   %4d-%02d-%02d   %-5d\n", p->data.id, p->data.name, type(p->data.type),
				p->data.area,rating(p->data.rating), p->data.duration, p->data.start_date.year, p->data.start_date.month, p->data.start_date.day, p->data.end_date.year, p->data.end_date.month, p->data.end_date.day, p->data.price);
		}
		
		printf("\n\t总数:%d\t\t\t\t\t\t\t\t\t\t%d/%d \n", paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));
		printf("      =========================================================================================\n");
		printf("\t\t1.上一页    2.下一页    3.查询剧目信息    4.显示剧目演出计划     5.返回  \n");
		
		printf("\n\n      :");
		fflush(stdin);
		choice = getche();
		switch (choice)
		{
		case '4':
			printf("\n      ID:");
			scanf("%d", &id);
			getchar();
			Sale_UI_ShowScheduler(id);
			paging.totalRecords = Play_Srv_FetchAll(list);
			List_Paging(list, paging, play_node_t);
			break;
		case '3': 
			printf("\n      剧目名称:");
			scanf("%s", name);
			getchar();
			if (Play_Srv_FetchByName(data, name))
			{
				Print_Play(data);
			}
			else
			{
				printf("\n      无此剧目,按任意键退出");
				getch();
			}
			paging.totalRecords = Play_Srv_FetchAll(list);
			List_Paging(list, paging, play_node_t);
			break;
		case '5': 
			break;
		case '1':
			if (1 < Pageing_CurPage(paging))
			{
				Paging_Locate_OffsetPage(list, paging, -1, studio_node_t);
			}
			break;
		case '2': 
			if (Pageing_TotalPages(paging) > Pageing_CurPage(paging))
			{
				Paging_Locate_OffsetPage(list, paging, 1, studio_node_t);
			}
			break;
		}
	} while (choice != '5');
	List_Destroy(list, play_node_t);//销毁链表 
}

void Print_Play(play_list_t list)
{
	play_list_t p;
	p = list->next;
	do
	{
		printf("\n\t\t\t========================剧目信息=====================\n");
		printf("\t\t\t剧目id: %d\n", p->data.id);
		printf("\t\t\t剧目名称: %s\n", p->data.name);
		printf("\t\t\t类型：%s\n", type(p->data.type));
		printf("\t\t\t出品地区：%s\n", p->data.area);
		printf("\t\t\t剧目等级：%s\n",rating(p->data.rating));
		printf("\t\t\t时长: %d 分钟\n", p->data.duration);
		printf("\t\t\t上映时间: %d年%d月 %d日\n", p->data.start_date.year, p->data.start_date.month, p->data.start_date.day);
		printf("\t\t\t结束放映时间：%d年%d月%d日\n", p->data.end_date.year, p->data.end_date.month, p->data.end_date.day);
		printf("\t\t\t票价：%d 元\n", p->data.price);
		printf("\t\t\t======================================================\n");
		p = p->next;
	} while (p != list);
	printf("\t\t\t按任意键退出");
	getch();
}


char * Status(int i)
{
	if (i == 1)
	{
		return "已售";
	}
	else if (i == 0)
	{
		return "未售";
	}
	else if (i == -1)
	{
		return "损坏";
	}
	else
	{
		return "其他";
	}
}
void Sale_UI_ShowScheduler2(int playID)
{
	play_t buf;
	if (Play_Srv_FetchByID(playID, &buf))
	{
		int listSize, sal_id, i;
		char choice;
		Pagination_t paging;
		schedule_list_t list, pos,p;
		schedule_t buf;
		studio_t buf3;
		play_t buf2;
		List_Init(list, schedule_node_t);
		listSize = Schedule_Srv_FetchByPlay(list, playID);
		paging.pageSize = PAGESIZE;
		paging.totalRecords = listSize;
		Paging_Locate_FirstPage(list, paging);
		do {
			system("cls");
			printf("\n\t\t\t\t\t\t演出计划\n\n");
			printf("\t\t  %-4s%-14s%-12s  %-10s    %-10s%-8s\n\n", "ID", "剧目ID", "演出厅ID", "放映日期", "放映时间", "座位数");
			
			//Paging_ViewPage_ForEach(list, paging, schedule_node_t, pos, i)
			for (i = 0, pos = (schedule_node_t *)(paging.curPos); pos != list && i < paging.pageSize; i++, pos = pos->next)
			{
				Play_Srv_FetchByID(pos->data.play_id, &buf2);
				Studio_Perst_SelectByID(pos->data.studio_id, &buf3);
				printf("\t\t  %-4d%-14s%-12s%6d-%02d-%02d    %2d:%02d:%02d  %4d\n", pos->data.id,buf2.name, buf3.name, pos->data.date.year, pos->data.date.month, pos->data.date.day, pos->data.time.hour, pos->data.time.minute, pos->data.time.second, pos->data.seat_count);
			}
			
			printf("\n\t\t\t  总数:%d\t\t\t\t\t%d/%d \n\n", paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));
			
			printf("\n\t\t\t1.上一页            2.下一页            3.返回 \n");
			
			printf("\n\t\t\t:");
			fflush(stdin);
			choice = getche();
			switch (choice)
			{
			case '3':
				break; 
			case '1':
				if (1 < Pageing_CurPage(paging))
				{
					Paging_Locate_OffsetPage(list, paging, -1, studio_node_t);
				}
				break; 
			case '2':
				if (Pageing_TotalPages(paging) > Pageing_CurPage(paging))
				{
					Paging_Locate_OffsetPage(list, paging, 1, studio_node_t);
				}
				break;
			}
		} while (choice != '3');
	}
	else
	{
		printf("      没有此剧目,按任意键退出");
		getch();
	}
} 
			
void Sale_UI_ShowScheduler3(int playID)
{
	play_t buf;
	int id;
	if (Play_Srv_FetchByID(playID, &buf))
	{
		char ch;
		schedule_list_t head;
		int listSize, sal_id, i;
		char choice;
		List_Init(head, schedule_node_t);
		Pagination_t paging;
		schedule_list_t list, pos,p;
		schedule_t buf;
		studio_t buf3;
		play_t buf2;
		List_Init(list, schedule_node_t);
		listSize = Schedule_Srv_FetchByPlay(list, playID);
		paging.pageSize = PAGESIZE;
		paging.totalRecords = listSize;
		Paging_Locate_FirstPage(list, paging);
		do {
			system("cls");
			printf("\n\t\t\t\t\t\t演出计划\n\n");
			printf("\t\t  %-4s  %-14s%-12s  %-10s      %-10s  %-8s\n\n", "ID", "剧目ID", "演出厅ID", "放映日期", "放映时间", "座位数");
			
			//Paging_ViewPage_ForEach(list, paging, schedule_node_t, pos, i)
			for (i = 0, pos = (schedule_node_t *)(paging.curPos); pos != list && i < paging.pageSize; i++, pos = pos->next)
			{
				Play_Srv_FetchByID(pos->data.play_id, &buf2);
				Studio_Perst_SelectByID(pos->data.studio_id, &buf3);
				printf("\t\t  %-4d  %-14s%-12s%6d-%02d-%02d      %2d:%02d:%02d    %4d\n", pos->data.id,buf2.name, buf3.name, pos->data.date.year, pos->data.date.month, pos->data.date.day, pos->data.time.hour, pos->data.time.minute, pos->data.time.second, pos->data.seat_count);
			}
			
			printf("\n\t\t\t总数:%d\t\t\t\t\t  %d/%d \n\n", paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));
			
			printf("\n\t\t\t1.上一页      2.下一页      4.返回      3.添加演出计划  \n\n\t\t\t5.修改演出计划    7.查找演出计划    6.删除演出计划\n");
			printf("\n\n\t\t\t:");
			fflush(stdin);
			choice = getche();
			switch (choice)
			{
			case '6':
			//删除演出计划
			printf("\n\t\t\tID:");
			while (scanf("%d", &id) != 1)
			{
				printf("\n\t\t\tID:");
	     		printf("\t\t\t输入错误,重新输入\n");

			}
			fflush(stdin);
			if (Schedule_UI_Delete(id)) {	//从新载入数据
					paging.totalRecords = Schedule_Srv_FetchByPlay(list, playID);
					List_Paging(list, paging, schedule_node_t);
			}
			break; 
			case '7':
				printf("\n\t\t\t输入要查找的ID:");
				while (scanf("%d", &id) != 1)
				{
					printf("\n\t\t\t输入错误"); 
					printf("\n\t\t\t输入要查找的ID:");
					while ((ch = getchar()) != '\n')
						continue;
				}
				fflush(stdin);
				Schedule_UI_Query(id);
				break; 
			case '5':
					//修改演出计划
					printf("\n\t\t\tID:");
					while (scanf("%d", &id) != 1)
					{
						printf("\t\t\t输入错误请重新输入\n");
						while ((ch = getchar()) != '\n')
							continue;
					}
					fflush(stdin);
					if (Schedule_UI_Modify(id)) {	//从新载入数据
						paging.totalRecords = Schedule_Srv_FetchByPlay(list, playID);
						List_Paging(list, paging, schedule_node_t);
					}
					break; 
			case '3':

					//添加演出计划
				if (Schedule_UI_Add())     //新添加成功，跳到最后一页显示
				{
					paging.totalRecords = Schedule_Srv_FetchByPlay(list, playID);
					List_Paging(list, paging, schedule_node_t);
				}
				break; 

			case '4':
				break; 
			case '1':
				if (1 < Pageing_CurPage(paging))
				{
					Paging_Locate_OffsetPage(list, paging, -1, studio_node_t);
				}
				choice = '3';
				break; 
			case '2':
				if (Pageing_TotalPages(paging) > Pageing_CurPage(paging))
				{
					Paging_Locate_OffsetPage(list, paging, 1, studio_node_t);
				}
				choice = '3';
				break;
			}
		} while (choice != '4');
	}
	else
	{
		printf("\t\t\t\t\t没有此剧目,按任意键退出");
		getch();
	}
}

void Sale_UI_ShowTicket2(int schID)
{
	int pageSize = 10, i;
	int listSize;
	int ticket_id;
	char choice;
	ticket_list_t list, s;
	seat_t buf;
	Pagination_t paging;
	List_Init(list, ticket_node_t);
	listSize = Ticket_Srv_FetchBySchID(list, schID);
	paging.pageSize = pageSize;
	paging.totalRecords = listSize;
	Paging_Locate_FirstPage(list, paging);
	do
	{
		system("cls");
		printf("\n\t\t\t                      查询演出票\n");
				printf("\t\t\t===================================================\n");
		printf("\t\t\t %-4s %10s   %-8s%4s %4s    %-6s%-8s\n\n", "ID", "演出计划ID", "座位ID", "行", "列", "状态","票价");
		
		for (i = 0, s = (ticket_node_t *)(paging.curPos); s != list && i < paging.pageSize; i++, s = s->next)
		{
			Seat_Srv_FetchByID(s->data.seat_id, &buf);
			if (buf.status == 1)
				printf("\t\t\t %-4d       %-6d   %-6d%4d %4d    %-6s  %-8d\n", s->data.id,
				 s->data.schedule_id, s->data.seat_id, buf.row + 1, buf.column + 1, Status(s->data.status),s->data.price);
		}
		
		printf("\n\t\t\t\t总数:%2d\t\t\t\t%d/%d \n", paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));
		printf("\t\t\t===================================================\n");
		printf("\t\t\t      1.上一页　　    　　      2.下一页　       \n");
		printf("\t\t\t      3.返回  　　            　       \n");
		printf("\t\t\t===================================================\n");
		printf("\n\t\t\t:");
		choice = getche();
		switch (choice)
		{ 

		case '1':
			if (1 < Pageing_CurPage(paging))
			{
				Paging_Locate_OffsetPage(list, paging, -1, ticket_node_t);
			}
			break; 
		case '2':
			if (Pageing_TotalPages(paging) > Pageing_CurPage(paging))
			{
				Paging_Locate_OffsetPage(list, paging, 1, ticket_node_t);
			}
			break;
		}
	} while (choice != '3'); 
}

