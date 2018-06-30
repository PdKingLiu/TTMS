#include "Play_UI.h"
#include "Schedule_UI.h"
#include "../Common/list.h"
#include "../Service/Play.h"
#include "../Service/EntityKey.h"
#include <stdio.h>
#include <stdlib.h>
#include "Studio_UI.h"

extern  gl_CurUser;

	/*
	 * Function:    Play_UI_ShowList
	 * Function ID:	TTMS_SCU_Play_UI_Show
	 * Description: 以列表模式显示剧目信息
	 * Input:       list剧目信息链表的头指针，paging分页设置参数
	 * Output:      所有剧目的信息
	 * Return:      返回查找到的记录数目
	 */
void Play_UI_ShowList(play_list_t list, Pagination_t paging) {
	int i;
	play_node_t *pos;
	printf("\n\t\t\t剧目类型:1为电影  2为歌剧  3为音乐会\n\t\t\t演出级别:1适合儿童观看  2适合青少年观看  3适合成人观看\n");
	printf("\t\t\t=========================================================\n");
	printf("\n\t\t\t                        剧目列表\n");
	printf("\n\t\n");
	printf("\t%-5s  %-16s  %-4s  %8s  %8s %8s  %10s  %10s  %5s\n\n",
		"ID", "    名称", "类型", "地区", "演出级别", "时长", "上映时间", "下架时间", "价格");
	
	//显示数据
	for (i = 0, pos = (play_node_t *)(paging.curPos);
		pos != list && i < paging.pageSize; i++) {
		printf("\t%-5d  %-16s  %-4s  %8s   %8s  %5d   %4d-%02d-%02d   %4d-%02d-%02d   %-5d\n", pos->data.id,
			pos->data.name, type(pos->data.type), pos->data.area,
			rating(pos->data.rating), pos->data.duration, pos->data.start_date.year, pos->data.start_date.month, pos->data.start_date.day,
			pos->data.end_date.year, pos->data.end_date.month, pos->data.end_date.day, pos->data.price);
		pos = pos->next;
	}
	//剧目类型定义，1表示电影，2表示歌剧，3表示音乐会
	//演出级别类型定义，1表示儿童可观看，2表示青少年可观看，3表示成人可观看

}

/*
 * Function:    Play_UI_MgtEntry
 * Function ID:	TTMS_SCU_Play_UI_MgtEnt
 * Description: 剧目信息管理界面
 * Input:       flag为0，进入管理页面，flag为1进入查询界面
 * Output:      剧目管理界面
 * Return:      无
 */
void  Play_UI_MgtEntry(int flag){  
	int i, id;
	char choice;
	play_list_t head;
	play_node_t *pos;
	Pagination_t paging;
	List_Init(head, play_node_t);
	paging.offset = 0;
	paging.pageSize = PLAY_PAGE_SIZE;
	//载入数据
	paging.totalRecords = Play_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);

	if (flag)
	{
		do {
			system("cls");
			Play_UI_ShowList(head, paging);
			printf("\n\t\t\t总数:%2d\t\t\t\t\t\t    %d/%d \n",
				paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));

			printf("\n\n\t\t\t\t 1.上页    2.下页   3.查询   4.返回");
			printf("\n\t=======================================================================================\n");
			printf("\n\t\t\t:");
			choice = getche();
			switch (choice)
			{
			case '3':
				printf("\n");
				printf("\n\t\t\tID:");
				scanf("%d", &id);
				getchar();
				if (Play_UI_Query(id)) {	//从新载入数据
					paging.totalRecords = Play_Srv_FetchAll(head);
					List_Paging(head, paging, play_node_t);
				}
				break;
			case '1':
				if (1 < Pageing_CurPage(paging)) {
					Paging_Locate_OffsetPage(head, paging, -1, play_node_t);
				}
				break;
			
			case '2':
				if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
					Paging_Locate_OffsetPage(head, paging, 1, play_node_t);
				}
				break;
			}
		} while (choice != '4');
	}
	else
	{
		do {
			system("cls");
			Play_UI_ShowList(head, paging);
			printf("\n\t\t\t总数:%2d\t\t\t\t\t\t    %d/%d \n",
				paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));
			printf("\n\n\t\t\t1.上页  2.下页  3.增加  4.删除  5.修改  6.查询  7.返回");
			printf("\n");
			printf("\n\t\t\t:");
			choice = getche();
			switch (choice)
			{
			case '3':
				if (Play_UI_Add()) //新添加成功，跳到最后一页显示
				{
					paging.totalRecords = Play_Srv_FetchAll(head);
					Paging_Locate_LastPage(head, paging, play_node_t);
				}
				break;
			case '4':
				printf("\n");
				printf("\n\t\t\tID:");
				scanf("%d", &id);
				getchar();
				if (Play_UI_Delete(id)) {	//从新载入数据
					paging.totalRecords = Play_Srv_FetchAll(head);
					List_Paging(head, paging, play_node_t);
				}
				break;
			case '5':
				printf("\n");
				printf("\n\t\t\tID:");
				scanf("%d", &id);
				getchar();
				if (Play_UI_Modify(id)) {	//从新载入数据
					paging.totalRecords = Play_Srv_FetchAll(head);
					List_Paging(head, paging, play_node_t);
				}
				break;

			case '6':
				printf("\n");
				printf("\n\t\t\tID:");
				scanf("%d", &id);
				getchar();
				if (Play_UI_Query(id)) {	//从新载入数据
					paging.totalRecords = Play_Srv_FetchAll(head);
					List_Paging(head, paging, play_node_t);
				}
				break;

			case '1':
				if (1 < Pageing_CurPage(paging)) {
					Paging_Locate_OffsetPage(head, paging, -1, play_node_t);
				}
				break;
			case '2':

				if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
					Paging_Locate_OffsetPage(head, paging, 1, play_node_t);
				}
				break;
			}
		} while (choice != '7');
	}
}


/*
 * Function:    Play_UI_Add
 * Function ID:	TTMS_SCU_Play_UI_Add
 * Description: 添加一条剧目信息
 * Input:       无
 * Output:      输入信息时的各种提示
 * Return:      添加的记录数
 */
int Play_UI_Add(void){
	
	play_t rec;
	char choice;
	int newRecCount = 0;

	do {
		printf("\n\n\t\t\t                        增添剧目\n");
		printf("\t\t\t=========================================================\n");

		printf("\n\t\t\t剧目名称:");
		fflush(stdin);
		gets(rec.name);
		printf("\t\t\t剧目类型(1/2/3):");
		scanf("%d", &rec.type);//剧目类型定义，1表示电影，2表示歌剧，3表示音乐会
		printf("\t\t\t地区:");
		getchar();
		gets(rec.area);
		printf("\t\t\t剧目级别类型(1/2/3):");
		scanf("%d", &rec.rating);//演出级别类型定义，1表示儿童可观看，2表示青少年可观看，3表示成人可观看
		printf("\t\t\t剧目时长:");
		scanf("%d",&rec.duration);
		printf("\t\t\t上映时间(年 月 日以空格间隔):");
		scanf("%d%d%d", &rec.start_date.year, &rec.start_date.month, &rec.start_date.day);
		printf("\t\t\t下架时间(年 月 日以空格间隔):");
		scanf("%d%d%d",&rec.end_date.year, &rec.end_date.month, &rec.end_date.day);
		printf("\t\t\t价格:");
		scanf("%d", &rec.price);
		getchar();
		printf("\t\t\t=======================================================\n");

		//获取主键
		rec.id = EntKey_Srv_CompNewKey("Play");

		if (Play_Srv_Add(&rec)) {
			newRecCount += 1;
			printf("\t\t\t增添成功\n");
		}
		else
			printf("\t\t\t增添失败\n");
		printf("\t\t\t-------------------------------------------------------\n");
		printf("\t\t\t1.继续添加    2.返回");
		fflush(stdin);
		choice = getche();
	}while ('1' == choice);
	return newRecCount;
}

/*
 * Function:    Play_UI_Modify
 * Function ID:	TTMS_SCU_Play_UI_Mod
 * Description: 更新剧目信息
 * Input:       待更新的剧目ID号
 * Output:      输入信息时的各种提示
 * Return:      更新的剧目信息数，0表示未找到，1表示找到并更新
 */
int Play_UI_Modify(int id){
	play_t rec;
	int rtn = 0;

	if (!Play_Srv_FetchByID(id, &rec)) {
		printf("\t\t\t剧目不存在,按任意键返回\n");
		getch();
		return 0;
	}
	//界面	
	printf("\n\n\t\t\t                        修改剧目\n");
	printf("\t\t\t=========================================================\n\n");
	printf("\t\t\tID:%d\n", rec.id);
	printf("\t\t\t剧目名称: [%s]\n", rec.name);
	printf("\t\t\t剧目类型:%s\n", type(rec.type));//剧目类型定义，1表示电影，2表示歌剧，3表示音乐会
	printf("\t\t\t地区:%s\n", rec.area);
	printf("\t\t\t剧目演出级别:%s\n", rating(rec.rating));//演出级别类型定义，1表示儿童可观看，2表示青少年可观看，3表示成人可观看
	printf("\t\t\t剧目时长:%d\n", rec.duration);
	printf("\t\t\t上映日期:%d-%d-%d\n", rec.start_date.year,rec.start_date.month,rec.start_date.day);
	printf("\t\t\t下架日期:%d-%d-%d\n", rec.end_date.year, rec.end_date.month, rec.end_date.day);
	printf("\t\t\t价格:%d\n", rec.price);
	
	printf("\t\t\t输入修改后的信息:\n");//如果只改某一项，这就比较麻烦，后期需要优化
	fflush(stdin);
	printf("\t\t\t剧目名称:");
	gets(rec.name);
	printf("\t\t\t剧目类型(1/2/3):");
	scanf("%d", &rec.type);//剧目类型定义，1表示电影，2表示歌剧，3表示音乐会
	getchar();
	printf("\t\t\t地区:");
	gets(rec.area);
	printf("\t\t\t剧目演出级别(1/2/3):");
	scanf("%d", &rec.rating);//演出级别类型定义，1表示儿童可观看，2表示青少年可观看，3表示成人可观看
	printf("\t\t\t剧目时长:");
	scanf("%d", &rec.duration);
	printf("\t\t\t上映日期(年 月 日以空格间隔):");
	scanf("%d%d%d", &rec.start_date.year, &rec.start_date.month, &rec.start_date.day);
	printf("\t\t\t下架日期(年 月 日以空格间隔):");
	scanf("%d%d%d", &rec.end_date.year, &rec.end_date.month, &rec.end_date.day);
	printf("\t\t\t价格:");
	scanf("%d", &rec.price);
	
	printf("\t\t\t-------------------------------------------------------\n");

	if (Play_Srv_Modify(&rec)) {
		rtn = 1;
		printf(
			"\t\t\t修改成功,按任意键退出");
	}
	else
		printf("\t\t\t修改失败,按任意键退出");
	getch();
	return rtn;
}

/*
 * Function:    Play_UI_Delete
 * Function ID:	TTMS_SCU_Play_UI_Del
 * Description: 按照ID号删除剧目信息
 * Input:       待删除的剧目ID号
 * Output:      提示删除是否成功
 * Return:      0表示删除失败，1表示删除成功
 */
int Play_UI_Delete(int id){

	int rtn = 0;

	if (Play_Srv_DeleteByID(id)) {
		printf("\t\t\t删除成功,按任意键退出");
		rtn = 1;
	}
	else {
		printf("\t\t\t删除失败,按任意键退出");
	}
	getch();
	return rtn;
}

/*
 * Function:    Play_UI_Query
 * Function ID:	TTMS_SCU_Play_UI_Qry
 * Description: 按照ID号查询剧目信息
 * Input:       待查找的剧目ID号
 * Output:      查找到的剧目信息
 * Return:      0表示未找到，1表示找到了
 */
int Play_UI_Query(int id){
	int rtn = 0;
	play_t rec;
	if (!Play_Srv_FetchByID(id, &rec)) {
		printf("\t\t\t没有此剧目,按任意键退出");
		getch();
		return 0;
	}
	//界面	
	printf("\n\n\t\t\t                        剧目信息\n");
	printf("\t\t\t=========================================================\n");

	printf("\t\t\tID:%d\n", rec.id);
	printf("\t\t\t名称: %s\n", rec.name);
	printf("\t\t\t类型:%s\n", type(rec.type));//剧目类型定义，1表示电影，2表示歌剧，3表示音乐会
	printf("\t\t\t地区:%s\n", rec.area);
	printf("\t\t\t演出级别:%s\n", rating(rec.rating));//演出级别类型定义，1表示儿童可观看，2表示青少年可观看，3表示成人可观看
	printf("\t\t\t时长:%d\n", rec.duration);
	printf("\t\t\t上映时间:%d-%d-%d\n", rec.start_date.year, rec.start_date.month, rec.start_date.day);
	printf("\t\t\t下架时间:%d-%d-%d\n", rec.end_date.year, rec.end_date.month, rec.end_date.day);
	printf("\t\t\t价格:%d\n", rec.price);
	printf("\t\t\t按任意键退出");
	getch();
	return 1;
}

char *type(int type)
{
	if (type == 1)
	{
		return "电影";
	}
	else if (type == 2)
	{
		return "歌剧";
	}
	else if (type == 3)
	{
		return  "音乐";
	}
	else
		return "其他";
}
char *rating(int rating)
{
	if (rating == 1)
	{
		return "儿童";
	}
	else if (rating == 2)
	{
		return "青年";
	}
	else if (rating == 3)
	{
		return "成人";
	}
	else
		return "其它";
}


