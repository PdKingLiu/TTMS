#include "Studio_UI.h"

#include "./Main_Menu.h"
#include "../Common/list.h"
#include "../Service/Studio.h"
#include "../Service/Seat.h"
#include "../Service/EntityKey.h"
#include "Seat_UI.h"
#include "conio.h"

extern account_t gl_CurUser;

static const int STUDIO_PAGE_SIZE = 5;

#include <stdio.h>

	void Studio_UI_MgtEntry(account_type_t type) {
	int i, id;
	char choice;
	
	studio_list_t head;
	studio_node_t *pos;
	Pagination_t paging;
	List_Init(head, studio_node_t);
	paging.offset = 0;
	paging.pageSize = STUDIO_PAGE_SIZE;
	//载入数据
	paging.totalRecords = Studio_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);
	if (type==9) {
		do {
			system("cls");
			printf("\n\n\t\t\t                        演出厅清单\n");
			printf("\t\t\t=========================================================\n\n");
			printf("\t\t\t%-5s  %-14s  %-12s  %-12s  %-12s\n", "ID", "    名称", "行数",
				"列数", "座位数");
			
			//显示数据
			for (i = 0, pos = (studio_node_t *)(paging.curPos);
				pos != head && i < paging.pageSize; i++) {
				printf("\t\t\t%-5d  %-14s    %-12d  %-12d  %-12d\n", pos->data.id,
					pos->data.name, pos->data.rowsCount, pos->data.colsCount,
					pos->data.seatsCount);
				pos = pos->next;
			}
			printf("\n\t\t\t  总数:%2d \t\t\t\t\t%d/%d \n",
				paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));
			printf("\t\t\t=========================================================\n\n");
			printf("\t\t\t1.上一页\t\t\t\t2.下一页\n\t\t\t3.添加\t\t\t\t\t4.删除\n\t\t\t5.修改演出厅信息\t\t\t6.设置演出厅座位\n\t\t\t7.返回");
			
			printf("\n\n\t\t\t:");
			choice = getche();
			switch (choice)
			{ 
			case '3':
				if (Studio_UI_Add()) //新添加成功，跳到最后一页显示
				{
					paging.totalRecords = Studio_Srv_FetchAll(head);
					Paging_Locate_LastPage(head, paging, studio_node_t);
				}
				break; 
			case '4':
				printf("\n");
				printf("\t\t\tID:");
				scanf("%d", &id);
				getchar();
				if (Studio_UI_Delete(id)) {	//从新载入数据
					paging.totalRecords = Studio_Srv_FetchAll(head);
					List_Paging(head, paging, studio_node_t);
				}
				break; 
			case '5':
				printf("\n");
				printf("\t\t\tID:");
				scanf("%d", &id);
				if (Studio_UI_Modify(id)) {	//从新载入数据
					paging.totalRecords = Studio_Srv_FetchAll(head);
					List_Paging(head, paging, studio_node_t);
				}
				break; 
			case '6':
				printf("\n");
				printf("\t\t\tID:");
				scanf("%d", &id);
				getchar();
				Seat_UI_MgtEntry(id);
				paging.totalRecords = Studio_Srv_FetchAll(head);
				List_Paging(head, paging, studio_node_t);
				break; 
			case '1':
				if (1 < Pageing_CurPage(paging)) {
					Paging_Locate_OffsetPage(head, paging, -1, studio_node_t);
				}
				break; 
			case '2':
				if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
					Paging_Locate_OffsetPage(head, paging, 1, studio_node_t);
				}
				break;
			}
		} while (choice != '7');
	}
	else
	{
		do {
			system("cls");
			printf("\n\n\t\t\t                        演出厅列表\n");
			printf("\t\t\t=========================================================\n\n");
			printf("\t\t\t%-5s  %-14s  %-12s  %-12s  %-12s\n\n", "ID", "    名称", "行数",
				"列数", "座位数");
			//显示数据
			for (i = 0, pos = (studio_node_t *)(paging.curPos);
				pos != head && i < paging.pageSize; i++) {
				printf("\t\t\t%-5d  %-14s    %-12d  %-12d  %-12d\n", pos->data.id,
					pos->data.name, pos->data.rowsCount, pos->data.colsCount,
					pos->data.seatsCount);
				pos = pos->next;
			}
			printf("\n\t\t\t总数:%2d\t\t\t\t\t\t    %d/%d \n",
				paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));

			printf("\n\n\t\t\t\t\t  1.上页  2.下页  3.返回");

			printf("\n\n\t\t\t:");
			choice = getche();
			switch (choice)
			{
			case '1':

				if (1 < Pageing_CurPage(paging)) {
					Paging_Locate_OffsetPage(head, paging, -1, studio_node_t);
				}
				break;
			case '2':

				if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
					Paging_Locate_OffsetPage(head, paging, 1, studio_node_t);
				}
				break;
			}
		} while (choice != '3');
	}
	//释放链表空间
	List_Destroy(head, studio_node_t);
}

int Studio_UI_Add(void) {
	studio_t rec;
	int newRecCount = 0;
	char choice;
	do {
		/*system("cls");*/
			printf("\n\n\t\t\t                        增加演出厅\n");
			printf("\t\t\t=========================================================\n");

		printf("\t\t\t演出厅名称:");
		//getchar();
		gets(rec.name);
		printf("\t\t\t行数:");
		scanf("\t\t\t%d", &rec.rowsCount);
		printf("\t\t\t列数:");
		scanf("%d", &rec.colsCount);
		getchar();
		rec.seatsCount = 0;
		printf("\t\t\t=========================================================\n");

		//获取主键
		rec.id = EntKey_Srv_CompNewKey("Studio");  //主键的获取有问题，已经解决

		if (Studio_Srv_Add(&rec)) {
			newRecCount += 1;
			printf("\t\t\t添加成功\n");
		}
		else
			printf("\t\t\t添加失败\n");
		printf("\t\t\t-------------------------------------------------------\n");
		printf("\t\t\t1.继续添加\t2.返回");
		choice = getche();
	} while ('1' == choice);
	return newRecCount;
}

int Studio_UI_Modify(int id) {
	studio_t rec;
	int rtn = 0;
	int newrow, newcolumn;
	seat_list_t list;
	int seatcount;

	/*Load record*/
	if (!Studio_Srv_FetchByID(id, &rec)) {
		printf("\t\t\t演出厅不存在,按任意键返回");
		getch();
		return 0;
	}

			printf("\n\n\t\t\t                        修改演出厅\n");
			printf("\t\t\t=========================================================\n");

	printf("\t\t\t演出厅ID:%d\n", rec.id);
	printf("\t\t\t演出厅名称:", rec.name);
	getchar(); 
	gets(rec.name);

	List_Init(list, seat_node_t);
	seatcount = Seat_Srv_FetchByRoomID(list, rec.id);
	if (seatcount) {
		do {			//如果座位文件中已有座位信息，则更新的行列必须比以前大，否则不允许更改
			printf("\t\t\t行数(需大于 %d):", rec.rowsCount);
			scanf("\t\t\t%d", &(newrow));
			printf("\t\t\t列数(需大于 %d):", rec.colsCount);
			scanf("%d", &(newcolumn));
		} while (newrow<rec.rowsCount || newcolumn<rec.colsCount);
		getchar();
		rec.rowsCount = newrow;
		rec.colsCount = newcolumn;
		rec.seatsCount = seatcount;
	}
	else {
		printf("\t\t\t行数:");
		scanf("%d", &rec.rowsCount);
		printf("\t\t\t列数:");
		scanf("%d", &rec.colsCount);
		rec.seatsCount = 0;
	}

	printf("\t\t\t-------------------------------------------------------\n");

	if (Studio_Srv_Modify(&rec)) {
		rtn = 1;
		printf("\t\t\t修改成功,按任意键退出");
	}
	else
		printf("\t\t\t修改失败,按任意键退出");
	getch();
	return rtn;
}

int Studio_UI_Delete(int id) {

	int rtn = 0;

	if (Studio_Srv_DeleteByID(id)) {
		//在删除放映厅时，同时根据放映厅id删除座位文件中的座位
		if (Seat_Srv_DeleteAllByRoomID(id))
			printf("\t\t\t删除成功,按任意键退出\n");
		rtn = 1;
	}
	else {
		printf("\t\t\t演出厅不存在,按任意键退出\n");
	}
	printf("\t\t\t");
	getch();
	return rtn;
}
