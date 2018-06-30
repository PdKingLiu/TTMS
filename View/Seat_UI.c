#include "Seat_UI.h"

#include "../Service/Seat.h"
#include "../Service/Studio.h"
#include "../Service/EntityKey.h"
#include "../Common/list.h"
#include <stdio.h>
#include "conio.h"

//����״̬���ض�Ӧ��ʾ״̬����
char Seat_UI_Status2Char(seat_status_t status) {

	char statusChar;
	switch (status) {
	case SEAT_GOOD:		//����λ
		statusChar = '@';
		break;
	case SEAT_BROKEN:	//�𻵵���λ 
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

//����״̬���ŷ�����λ״̬
seat_status_t Seat_UI_Char2Status(char statusChar) {
	seat_status_t status;
	switch (statusChar) {
	case '@':		//����λ
		status = SEAT_GOOD;
		break; 
	case 'O':	//�𻵵���λ
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
	
	if (!Studio_Srv_FetchByID(roomID, &studioRec)) {  //��ö�Ӧid��ӳ������Ϣ
		printf("\t\t\t�ݳ���������,�����������");
		getch();
		return;
	}
	seat_list_t list;
	seat_node_t *p;

	List_Init(list, seat_node_t);
	//ѡ���ӳ����������λ
	seatCount = Seat_Srv_FetchByRoomID(list, roomID);

	if (!seatCount) {		//����ӳ����û��������λ�����Զ�������λ
		seatCount = Seat_Srv_RoomInit(list, roomID, studioRec.rowsCount,
				studioRec.colsCount);
		//�޸��ݳ��������λ����
		studioRec.seatsCount = seatCount;
		Studio_Srv_Modify(&studioRec);
	}
  do {
  		system("cls");
		printf("\n\n\n\n\n\t\t\t                   %d ���ݳ�����λһ��ͼ\n",roomID);
		printf("\t\t\t=========================================================");
		printf("\n\t\t\t\t\t\t��\tĻ");
		printf("\n\n\n\n");
		
		printf("\t\t\t\t\t%4c ", ' ');
		for (i = 1; i <= studioRec.colsCount; i++) {
			printf("%3d", i);
		}
		printf("\n\n\n");
		//��ʾ����
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
		printf("\n\t\t\t    ([@]��ʾ��λ���� [O]��ʾ��λ�� [ ]�հױ�ʾ����λ)\n\n");
		printf("\n\n\n\t\t\t\t     1.���  2.ɾ��  3.�޸�  4.����   ");
		printf("\n\n\t\t\t\t     :");
		choice = getche();
		switch (choice) { 
		case '1':
			changedCount = Seat_UI_Add(list, roomID, studioRec.rowsCount,
					studioRec.colsCount);
			if (changedCount > 0) {
				seatCount += changedCount;
				//�޸��ݳ��������λ����
				studioRec.seatsCount = seatCount;
				Studio_Srv_Modify(&studioRec);
			}
			break; 
		case '2':
			changedCount = Seat_UI_Delete(list, studioRec.rowsCount,
					studioRec.colsCount);
			if (changedCount > 0) {
				seatCount -= changedCount;
				//�޸��ݳ��������λ����
				studioRec.seatsCount = seatCount;
				Studio_Srv_Modify(&studioRec);
			}
			break; 
		case '3':
			Seat_UI_Modify(list, studioRec.rowsCount, studioRec.colsCount);
			break;
		}
	} while (choice != '4');
	//�ͷ�����ռ�
	List_Destroy(list, seat_node_t);
}

int Seat_UI_Add(seat_list_t list, int roomID, int row, int column) {  //����һ����λ
	seat_t rec;
	seat_node_t *p;
	int newRecCount = 0;
	char choice;
	do {
		printf("\n\n\t\t\t                        �����λ\n");
		printf("\t\t\t=========================================================");

		do {
			printf("\n\t\t\t����(С�ڵ��� %d):",row);
			scanf("%d", &(rec.row));
			printf("\t\t\t����(С�ڵ��� %d):", column);
			scanf("%d", &(rec.column));
			fflush(stdin);
		} while (rec.row > row || rec.column > column);

		p = Seat_Srv_FindByRowCol(list, rec.row, rec.column);
		if(p!=NULL) {						//����������к�����Ӧ����λ�Ѵ��ڣ����ܲ���
			printf("\t\t\t���ʧ��,��λ�Ѵ���\n");
			
			printf("\t\t\t1.�������   2.����   ");
			fflush(stdin);
			choice = getche();
			continue;
		}

		rec.id = EntKey_Srv_CompNewKey("Seat");		//������λid
		rec.roomID = roomID;
		rec.status = SEAT_GOOD;    //���������λ��״̬Ĭ��Ϊ����λ
		

		if (Seat_Srv_Add(&rec)) {
			newRecCount++;
			printf("\t\t\t��ӳɹ�\n");
			p = (seat_node_t*) malloc(sizeof(seat_node_t));
			p->data = rec;
			Seat_Srv_AddToSoftedList(list, p); //������������λ�������list
		} else
			printf("\t\t\t���ʧ��\n");
		
		printf("\t\t\t1.�������   2.����  ");
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

	printf("\n\n\t\t\t                        �޸���λ\n");
	printf("\t\t\t=========================================================");
	
	do {
		do {				//���µ���λ������Ϣ���ܳ�����ӳ����������
			printf("\n\t\t\t����(С�ڵ��� %d):",row);
			scanf("%d", &newrow);
			printf("\t\t\t����(С�ڵ��� %d):", column);
			scanf("%d", &newcolumn);
			getchar();
		} while (newrow > row || newcolumn > column);

		p = Seat_Srv_FindByRowCol(list, newrow, newcolumn);
		if (p) {
			printf("\t\t\t����Ϊ(����@��ʾ��λ���� ��д��ĸO��ʾ��λ�� ����������ʾ����λ):", newrow, newcolumn,Seat_UI_Status2Char(p->data.status));
	
			p->data.status = Seat_UI_Char2Status(getchar());
			
			if (Seat_Srv_Modify(&(p->data))) {
				rtn = 1;
				printf("\t\t\t�޸ĳɹ�\n");
			} else
				printf("\t\t\t�޸�ʧ��\n");
		} else
			printf("\t\t\t��λ������\n");
		
		printf("\t\t\t1.�����޸�   2.����  ");
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
		printf("\n\n\t\t\t                        ɾ����λ\n");
		printf("\t\t\t=========================================================\n");

		do {
			fflush(stdin);
			printf("\t\t\t����(С�ڵ��� %d):",row);
			scanf("%d", &(newrow));
			printf("\t\t\t����(С�ڵ��� %d):",column);
			scanf("%d", &(newcolumn));
			fflush(stdin);
		} while (newrow > row || newcolumn > column);

		p = Seat_Srv_FindByRowCol(list, newrow, newcolumn);
		if (p) {

			if (Seat_Srv_DeleteByID(p->data.id)) {
				printf("\t\t\tɾ���ɹ�\n");

				delSeatCount++;
				List_FreeNode(p);	//�ͷŽ����λ���p
			}
		} else {
			printf("\t\t\t��λ������\n");
		}

		printf("\t\t\t------------------------------------------------------------------\n");
		printf("\t\t\t1.����ɾ��\t2.����");
		choice = getche();
	} while ('1' == choice);
	return delSeatCount;
}

