#include "Seat_UI.h"

#include "../Service/Seat.h"
#include "../Service/Studio.h"
#include "../Service/EntityKey.h"
#include "../Common/list.h"
#include <stdio.h>
#include "conio.h"

//根据状态返回对应表示状态符号
char Seat_UI_Status2Char(seat_status_t status) {

	char statusChar;
	switch (status) {
	case SEAT_GOOD:		//有座位
		statusChar = '@';
		break;
	case SEAT_BROKEN:	//损坏的座位 
		statusChar = 'O';
		break;
	case SEAT_NONE:
		statusChar = ' ';
		break;
	default:
		break;
	}
	return statusChar;
}

//根据状态符号返回桌位状态
seat_status_t Seat_UI_Char2Status(char statusChar) {
	seat_status_t status;
	switch (statusChar) {
	case '@':		//有座位
		status = SEAT_GOOD;
		break; 
	case 'O':	//损坏的座位
		status = SEAT_BROKEN;
		break;
	default:
		status = SEAT_NONE;
		break;
	}
	return status;
}

void Seat_UI_MgtEntry(int roomID) {
	int i, j;
	char choice;
	int seatCount;
	int changedCount = 0;
	studio_t studioRec;
	
	if (!Studio_Srv_FetchByID(roomID, &studioRec)) {  //获得对应id放映厅的信息
		printf("\t\t\t演出厅不存在,按任意键返回");
		getch();
		return;
	}
	seat_list_t list;
	seat_node_t *p;

	List_Init(list, seat_node_t);
	//选择放映厅的所有座位
	seatCount = Seat_Srv_FetchByRoomID(list, roomID);

	if (!seatCount) {		//若放映厅还没有设置座位，则自动生成座位
		seatCount = Seat_Srv_RoomInit(list, roomID, studioRec.rowsCount,
				studioRec.colsCount);
		//修改演出厅里的座位数量
		studioRec.seatsCount = seatCount;
		Studio_Srv_Modify(&studioRec);
	}
  do {
  		system("cls");
		printf("\n\n\n\n\n\t\t\t                   %d 号演出厅座位一览图\n",roomID);
		printf("\t\t\t=========================================================");
		printf("\n\t\t\t\t\t\t巨\t幕");
		printf("\n\n\n\n");
		
		printf("\t\t\t\t\t%4c ", ' ');
		for (i = 1; i <= studioRec.colsCount; i++) {
			printf("%3d", i);
		}
		printf("\n\n\n");
		//显示数据
		for (i = 1; i <= studioRec.rowsCount; i++) {
			j = 1;
			printf("\t\t\t\t\t%2d  %c", i, ' ');
			List_ForEach(list,p)
			{
				if (p->data.row == i) {
					while (p->data.column != j) {
						printf("%3c", ' ');
						j++;
					}
					printf("%3c", Seat_UI_Status2Char(p->data.status));
					j++;
				}
			}
			printf("\n");
		}
		printf("\n\t\t\t    ([@]表示座位正常 [O]表示座位损坏 [ ]空白表示无座位)\n\n");
		printf("\n\n\n\t\t\t\t     1.添加  2.删除  3.修改  4.返回   ");
		printf("\n\n\t\t\t\t     :");
		choice = getche();
		switch (choice) { 
		case '1':
			changedCount = Seat_UI_Add(list, roomID, studioRec.rowsCount,
					studioRec.colsCount);
			if (changedCount > 0) {
				seatCount += changedCount;
				//修改演出厅里的座位数量
				studioRec.seatsCount = seatCount;
				Studio_Srv_Modify(&studioRec);
			}
			break; 
		case '2':
			changedCount = Seat_UI_Delete(list, studioRec.rowsCount,
					studioRec.colsCount);
			if (changedCount > 0) {
				seatCount -= changedCount;
				//修改演出厅里的座位数量
				studioRec.seatsCount = seatCount;
				Studio_Srv_Modify(&studioRec);
			}
			break; 
		case '3':
			Seat_UI_Modify(list, studioRec.rowsCount, studioRec.colsCount);
			break;
		}
	} while (choice != '4');
	//释放链表空间
	List_Destroy(list, seat_node_t);
}

int Seat_UI_Add(seat_list_t list, int roomID, int row, int column) {  //输入一个座位
	seat_t rec;
	seat_node_t *p;
	int newRecCount = 0;
	char choice;
	do {
		printf("\n\n\t\t\t                        添加座位\n");
		printf("\t\t\t=========================================================");

		do {
			printf("\n\t\t\t行数(小于等于 %d):",row);
			scanf("%d", &(rec.row));
			printf("\t\t\t列数(小于等于 %d):", column);
			scanf("%d", &(rec.column));
			fflush(stdin);
		} while (rec.row > row || rec.column > column);

		p = Seat_Srv_FindByRowCol(list, rec.row, rec.column);
		if(p!=NULL) {						//若输入的行列号所对应的座位已存在，则不能插入
			printf("\t\t\t添加失败,座位已存在\n");
			
			printf("\t\t\t1.继续添加   2.返回   ");
			fflush(stdin);
			choice = getche();
			continue;
		}

		rec.id = EntKey_Srv_CompNewKey("Seat");		//设置座位id
		rec.roomID = roomID;
		rec.status = SEAT_GOOD;    //插入的新座位的状态默认为好座位
		

		if (Seat_Srv_Add(&rec)) {
			newRecCount++;
			printf("\t\t\t添加成功\n");
			p = (seat_node_t*) malloc(sizeof(seat_node_t));
			p->data = rec;
			Seat_Srv_AddToSoftedList(list, p); //若增加了新座位，需更新list
		} else
			printf("\t\t\t添加失败\n");
		
		printf("\t\t\t1.继续添加   2.返回  ");
		fflush(stdin);
		choice = getche();
	} while ('1' == choice);
	return newRecCount;
}

int Seat_UI_Modify(seat_list_t list, int row, int column) {
	int rtn = 0;
	int newrow, newcolumn;
	char choice;
	seat_node_t *p;

	printf("\n\n\t\t\t                        修改座位\n");
	printf("\t\t\t=========================================================");
	
	do {
		do {				//更新的座位行列信息不能超出放映厅的行列数
			printf("\n\t\t\t行数(小于等于 %d):",row);
			scanf("%d", &newrow);
			printf("\t\t\t列数(小于等于 %d):", column);
			scanf("%d", &newcolumn);
			getchar();
		} while (newrow > row || newcolumn > column);

		p = Seat_Srv_FindByRowCol(list, newrow, newcolumn);
		if (p) {
			printf("\t\t\t更改为(符号@表示座位正常 大写字母O表示座位损坏 输入其他表示无座位):", newrow, newcolumn,Seat_UI_Status2Char(p->data.status));
	
			p->data.status = Seat_UI_Char2Status(getchar());
			
			if (Seat_Srv_Modify(&(p->data))) {
				rtn = 1;
				printf("\t\t\t修改成功\n");
			} else
				printf("\t\t\t修改失败\n");
		} else
			printf("\t\t\t座位不存在\n");
		
		printf("\t\t\t1.继续修改   2.返回  ");
		choice = getche();
	} while ('1' == choice);
	return rtn;
}

int Seat_UI_Delete(seat_list_t list, int row, int column) {

	int delSeatCount = 0;
	int newrow, newcolumn;
	seat_node_t *p;
	char choice;

	do {
		/*system("cls");*/
		printf("\n\n\t\t\t                        删除座位\n");
		printf("\t\t\t=========================================================\n");

		do {
			fflush(stdin);
			printf("\t\t\t行数(小于等于 %d):",row);
			scanf("%d", &(newrow));
			printf("\t\t\t列数(小于等于 %d):",column);
			scanf("%d", &(newcolumn));
			fflush(stdin);
		} while (newrow > row || newcolumn > column);

		p = Seat_Srv_FindByRowCol(list, newrow, newcolumn);
		if (p) {

			if (Seat_Srv_DeleteByID(p->data.id)) {
				printf("\t\t\t删除成功\n");

				delSeatCount++;
				List_FreeNode(p);	//释放结点座位结点p
			}
		} else {
			printf("\t\t\t座位不存在\n");
		}

		printf("\t\t\t------------------------------------------------------------------\n");
		printf("\t\t\t1.继续删除\t2.返回");
		choice = getche();
	} while ('1' == choice);
	return delSeatCount;
}

