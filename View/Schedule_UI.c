/*
* File name:  Schedule_UI.c
* File ID:	  TTMS_SSF_Schedule_UI_Sour
* Author:     Huang Ru
* Version:    V1.0
* Date: 	  2015-04-25
*/

#include "Schedule_UI.h"
#include "Play_UI.h"
#include "../Common/list.h"
#include "../Service/Schedule.h"
#include "../Service/Play.h"
#include "../Service/Studio.h"
#include "../Service/EntityKey.h"
#include "../Service/Seat.h"
#include "../Service/Ticket.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Function:    Schedule_UI_ListAll
 * Function ID:	TTMS_SCU_Schedule_UI_List
 * Description: 显示所有的演出计划信息,进行查询。
 * Input:       无
 * Output:      所有剧目的信息
 * Return:      无
 */
void Schedule_UI_ListAll(void) {
	int i;
	char ch[50];
	play_t buf;
	schedule_list_t head;          //双向链表的定义  
	schedule_node_t *pos;          //双向链表的一个结点  
	char choice;
	Pagination_t paging;
	List_Init(head, schedule_node_t);        //链表
	paging.offset = 0;       //初始化  记录 
	paging.pageSize = SCHEDULE_PAGE_SIZE;
	//载入数据
	paging.totalRecords = Schedule_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);
		//显示数据
		do {
			system("cls");
			printf("\n\n\t\t\t                        演出计划\n");
			printf("\t\t\n");
			printf("\n");
			printf("\t\t=========================================================================== \n");
			printf("\t\t%8s %8s %10s %15s    %14s %12s\n", "计划ID号", "剧目ID号", "放映厅ID号", "演出日期", "演出时间", "座位数量");
			printf("\t\t===========================================================================\n\n");
			//显示数据
			for (i = 0, pos = (schedule_node_t *)(paging.curPos);
				pos != head && i < paging.pageSize; i++) {
				printf("\t\t%5d %8d %8d    %8d年%02d月%02d日   %8d时%02d分%02d秒  %3d\n", pos->data.id,
					pos->data.play_id, pos->data.studio_id, pos->data.date.year, pos->data.date.month, pos->data.date.day,
					pos->data.time.hour, pos->data.time.minute, pos->data.time.second, pos->data.seat_count);
				pos = pos->next;
			}
			printf("\t\t=========================================================================== \n");
			printf("\n\t\t  总数:%2d \t\t\t\t\t\t\t页数:%d/%d \n\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
			printf("\n\n\t\t     1.上一页    2.下一页    3.第一页    4.最后一页    5.查询    6.退出\n");
			printf("\n\t\t  选择:");
			fflush(stdin);
			choice = getche();
		    switch(choice)
			{ 
					case '5':
						printf("\n\n\t\t  输入剧目名称:");
						scanf("%s",ch); 
						if(!Play_Perst_SelectByname(ch, &buf)){
							printf("\t\t\t\t\t无此剧目,按任意键返回");
							getch();
							break;
						}
						Sale_UI_ShowScheduler2(buf.id);
						break;
					case '3':
					Paging_Locate_FirstPage(head,paging);
					break;
					case '1': 
			        if(1<Pageing_CurPage(paging))
					{
			   		    Paging_Locate_OffsetPage(head,paging,-1, schedule_node_t);
			    	 }
			    	break;	   
					case '2':
						if(Pageing_TotalPages(paging)>Pageing_CurPage(paging))
						{
							Paging_Locate_OffsetPage(head,paging,1, schedule_node_t);
						}
						break; 
					case '4':
					Paging_Locate_LastPage(head,paging, schedule_node_t);
						break;
				}
		} while (choice != '6');
		//getchar();
}

/*
 * Function:    Schedule_UI_Add
 * Function ID:	TTMS_SCU_Schedule_UI_Add
 * Description: 添加一条演出计划信息
 * Input:       相关剧目的ID号
 * Output:      输入信息时的各种提示
 * Return:      添加的记录数
 */
int Schedule_UI_Add(void) {
	schedule_t rec;
	seat_list_t list;
	List_Init(list, seat_node_t);
	char choice;
	int newRecCount = 0;
	do {
		printf("\n\n\t\t\t                        增添演出计划\n");
		printf("\t\t\t=========================================================\n");		
		fflush(stdin);
		printf("\t\t\t剧目ID:");
		scanf("%d", &rec.play_id);
		printf("\t\t\t演出厅ID:");
		scanf("%d", &rec.studio_id);
		printf("\t\t\t日期(年 月 日以空格间隔):");
		scanf("%d%d%d", &rec.date.year, &rec.date.month, &rec.date.day);
		printf("\t\t\t时间(时 分 秒以空格间隔)::");
		scanf("%d%d%d", &rec.time.hour, &rec.time.minute, &rec.time.second);
		getchar();


		//获取主键
		rec.id = EntKey_Srv_CompNewKey("Schedule");
		rec.seat_count = Seat_Srv_FetchValidByRoomID(list, rec.studio_id);

		if (Schedule_Srv_Add(&rec)&&Ticket_Srv_AddBatch(rec.id,rec.studio_id)) {
			newRecCount += 1;
			printf("\t\t\t增添成功!\n");
		}
		else {
			printf("\t\t\t增添失败\n");
			Schedule_Srv_DeleteByID(rec.id);
		}
		printf("\t\t\t-------------------------------------------------------\n");
		printf("\t\t\t1.继续添加	2.返回");
		fflush(stdin);
		choice = getche();
	} while ('1' == choice);
	return newRecCount;
}

/*
 * Function:    Schedule_UI_Modify
 * Function ID:	TTMS_SCU_Schedule_UI_Mod
 * Description: 按照ID号更新演出计划信息
 * Input:       待更新的剧目ID号
 * Output:      输入信息时的各种提示
 * Return:      更新的剧目信息数，0表示未找到，1表示找到并更新
 */
int Schedule_UI_Modify(int id){

	schedule_t rec;
	int rtn = 0;

	if (!Schedule_Srv_FetchByID(id, &rec)) {
		printf("\t\t\t演出计划不存在,按任意键返回\n\t\t\t");
		getch();
		return 0;
	}
	//界面	
	printf("\n\n\t\t\t                        修改演出计划\n");
	printf("\t\t\t=========================================================\n");

	printf("\t\t\t剧目ID:");
	fflush(stdin);
	printf("%d\n", rec.play_id);
	printf("\t\t\t演出厅ID:");
	printf("%d\n", rec.studio_id);
	printf("\t\t\t演出时间: ");
	printf("%d年%d月%d日\n", rec.date.year, rec.date.month, rec.date.day);
	printf("\t\t\t具体时间: ");
	printf("%d时%d分%d秒\n", rec.time.hour, rec.time.minute, rec.time.second);
	Ticket_Srv_DeleteBatch(rec.id);

	printf("\n\t\t\t开始修改信息\n");//如果只改某一项，这就比较麻烦，后期需要优化
	printf("\t\t\t剧目ID:");
	fflush(stdin);
	scanf("%d", &rec.play_id);
	printf("\t\t\t演出厅ID:");
	scanf("%d", &rec.studio_id);
	printf("\t\t\t演出时间(年 月 日空格间隔):");
	scanf("%d%d%d", &rec.date.year, &rec.date.month, &rec.date.day);
	printf("\t\t\t演出日期(时 分 秒空格间隔):");
	scanf("%d%d%d", &rec.time.hour, &rec.time.minute, &rec.time.second);

	printf("\t\t\t-------------------------------------------------------\n");
	rec.seat_count=Ticket_Srv_AddBatch(rec.id, rec.studio_id);
	if (Schedule_Srv_Modify(&rec)) {
		rtn = 1;
		printf(
			"\t\t\t修改成功,按任意键返回");
	}
	else
		printf("\t\t\t修改失败,按任意键返回");
	getch();
	return rtn;
}


/*
 * Function:    Schedule_UI_Delete
 * Function ID:	TTMS_SCU_Schedule_UI_Del
 * Description: 按照ID号删除演出计划信息
 * Input:       待删除的剧目ID号
 * Output:      提示删除是否成功
 * Return:      0表示删除失败，1表示删除成功
 */
int Schedule_UI_Delete(int id){
	int rtn = 0;
	schedule_t rec;

	if (!Schedule_Srv_FetchByID(id, &rec)) {
		printf("\t\t\t不存在此演出计划,按任意键返回\n\t\t\t");
		getch();
		return 0;
	}
	if (Schedule_Srv_DeleteByID(id)&& Ticket_Srv_DeleteBatch(rec.studio_id)) {
		printf("\t\t\t删除成功,按任意键返回\n\t\t\t");
		rtn = 1;
	}
	else {
		printf("\t\t\t不存在此演出计划,按任意键返回\n\t\t\t");
	}
	getch();
	return rtn;

	return 0;
}

/*
 * Function:    Schedule_UI_Query
 * Function ID:	TTMS_SCU_Schedule_UI_Qry
 * Description: 按照ID号查询演出计划信息
 * Input:       待查找的剧目ID号
 * Output:      查找到的剧目信息
 * Return:      0表示未找到，1表示找到了
 */
int Schedule_UI_Query(int id){
	int rtn = 1;
	schedule_t  buf;

	if (Schedule_Srv_FetchByID(id, &buf)) {
		system("cls");
		printf("\n\n\n");
		printf("\n\t\t\t                        演出计划\n");
		printf("\n\n\n\t\t\t");
		rtn = 1;
		printf("= = = = = = = = = = = = = = = = = = = = = = = = = = = = = =  \n");
		printf("\t\t\t计划ID: %d\n", buf.id);
		printf("\t\t\t剧目ID: %d\n", buf.play_id);
		printf("\t\t\t放映厅ID: %d\n", buf.studio_id);
		printf("\t\t\t演出日期:%d年%d月%d日\n", buf.date.year, buf.date.month, buf.date.day);
		printf("\t\t\t演出时间:%d时%d分%d秒\n", buf.time.hour, buf.time.minute, buf.time.second);
		printf("\t\t\t座位数量:%d\n", buf.seat_count);
		printf("\t\t\t= = = = = = = = = = = = = = = = = = = = = = = = = = = = = =  \n");
		printf("\n\n\t\t\t按任意键返回");
		getch();
	}
	else
	{
		printf("\t\t\t无演出计划,按任意键返回");
		getch();
	}
	return rtn;
}

/*
 * Function:    Schedule_UI_ListByPlay
 * Function ID:	TTMS_SCU_Schedule_UI_ListByPlay
 * Description: 以列表模式显示给定剧目的演出计划信息
 * Input:       list为查找到的演出计划信息链表的头指针，play为剧目信息，paging为显示分页设置参数
 * Output:      无
 * Return:      返回查找到的记录数目
 */
void Schedule_UI_ListByPlay(const play_t *play, schedule_list_t list, Pagination_t paging){
}

/*
 * Function:    Schedule_UI_MgtEntry
 * Function ID:	TTMS_SCU_Schedule_UI_MgtEnt
 * Description: 演出计划管理界面，演出计划按照剧目进行管理，以剧目ID号为输入
 * Input:       相关剧目ID号
 * Output:      演出计划信息
 * Return:      无
 */
void Schedule_UI_MgtEntry(void){
	int id;
	printf("\n\t\t\t\t\t剧目ID:");
	scanf("%d",&id);
	Sale_UI_ShowScheduler3(id); 

}                     
