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
 * Description: ��ʾ���е��ݳ��ƻ���Ϣ,���в�ѯ��
 * Input:       ��
 * Output:      ���о�Ŀ����Ϣ
 * Return:      ��
 */
void Schedule_UI_ListAll(void) {
	int i;
	char ch[50];
	play_t buf;
	schedule_list_t head;          //˫������Ķ���  
	schedule_node_t *pos;          //˫�������һ�����  
	char choice;
	Pagination_t paging;
	List_Init(head, schedule_node_t);        //����
	paging.offset = 0;       //��ʼ��  ��¼ 
	paging.pageSize = SCHEDULE_PAGE_SIZE;
	//��������
	paging.totalRecords = Schedule_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);
		//��ʾ����
		do {
			system("cls");
			printf("\n\n\t\t\t                        �ݳ��ƻ�\n");
			printf("\t\t\n");
			printf("\n");
			printf("\t\t=========================================================================== \n");
			printf("\t\t%8s %8s %10s %15s    %14s %12s\n", "�ƻ�ID��", "��ĿID��", "��ӳ��ID��", "�ݳ�����", "�ݳ�ʱ��", "��λ����");
			printf("\t\t===========================================================================\n\n");
			//��ʾ����
			for (i = 0, pos = (schedule_node_t *)(paging.curPos);
				pos != head && i < paging.pageSize; i++) {
				printf("\t\t%5d %8d %8d    %8d��%02d��%02d��   %8dʱ%02d��%02d��  %3d\n", pos->data.id,
					pos->data.play_id, pos->data.studio_id, pos->data.date.year, pos->data.date.month, pos->data.date.day,
					pos->data.time.hour, pos->data.time.minute, pos->data.time.second, pos->data.seat_count);
				pos = pos->next;
			}
			printf("\t\t=========================================================================== \n");
			printf("\n\t\t  ����:%2d \t\t\t\t\t\t\tҳ��:%d/%d \n\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
			printf("\n\n\t\t     1.��һҳ    2.��һҳ    3.��һҳ    4.���һҳ    5.��ѯ    6.�˳�\n");
			printf("\n\t\t  ѡ��:");
			fflush(stdin);
			choice = getche();
		    switch(choice)
			{ 
					case '5':
						printf("\n\n\t\t  �����Ŀ����:");
						scanf("%s",ch); 
						if(!Play_Perst_SelectByname(ch, &buf)){
							printf("\t\t\t\t\t�޴˾�Ŀ,�����������");
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
 * Description: ���һ���ݳ��ƻ���Ϣ
 * Input:       ��ؾ�Ŀ��ID��
 * Output:      ������Ϣʱ�ĸ�����ʾ
 * Return:      ��ӵļ�¼��
 */
int Schedule_UI_Add(void) {
	schedule_t rec;
	seat_list_t list;
	List_Init(list, seat_node_t);
	char choice;
	int newRecCount = 0;
	do {
		printf("\n\n\t\t\t                        �����ݳ��ƻ�\n");
		printf("\t\t\t=========================================================\n");		
		fflush(stdin);
		printf("\t\t\t��ĿID:");
		scanf("%d", &rec.play_id);
		printf("\t\t\t�ݳ���ID:");
		scanf("%d", &rec.studio_id);
		printf("\t\t\t����(�� �� ���Կո���):");
		scanf("%d%d%d", &rec.date.year, &rec.date.month, &rec.date.day);
		printf("\t\t\tʱ��(ʱ �� ���Կո���)::");
		scanf("%d%d%d", &rec.time.hour, &rec.time.minute, &rec.time.second);
		getchar();


		//��ȡ����
		rec.id = EntKey_Srv_CompNewKey("Schedule");
		rec.seat_count = Seat_Srv_FetchValidByRoomID(list, rec.studio_id);

		if (Schedule_Srv_Add(&rec)&&Ticket_Srv_AddBatch(rec.id,rec.studio_id)) {
			newRecCount += 1;
			printf("\t\t\t����ɹ�!\n");
		}
		else {
			printf("\t\t\t����ʧ��\n");
			Schedule_Srv_DeleteByID(rec.id);
		}
		printf("\t\t\t-------------------------------------------------------\n");
		printf("\t\t\t1.�������	2.����");
		fflush(stdin);
		choice = getche();
	} while ('1' == choice);
	return newRecCount;
}

/*
 * Function:    Schedule_UI_Modify
 * Function ID:	TTMS_SCU_Schedule_UI_Mod
 * Description: ����ID�Ÿ����ݳ��ƻ���Ϣ
 * Input:       �����µľ�ĿID��
 * Output:      ������Ϣʱ�ĸ�����ʾ
 * Return:      ���µľ�Ŀ��Ϣ����0��ʾδ�ҵ���1��ʾ�ҵ�������
 */
int Schedule_UI_Modify(int id){

	schedule_t rec;
	int rtn = 0;

	if (!Schedule_Srv_FetchByID(id, &rec)) {
		printf("\t\t\t�ݳ��ƻ�������,�����������\n\t\t\t");
		getch();
		return 0;
	}
	//����	
	printf("\n\n\t\t\t                        �޸��ݳ��ƻ�\n");
	printf("\t\t\t=========================================================\n");

	printf("\t\t\t��ĿID:");
	fflush(stdin);
	printf("%d\n", rec.play_id);
	printf("\t\t\t�ݳ���ID:");
	printf("%d\n", rec.studio_id);
	printf("\t\t\t�ݳ�ʱ��: ");
	printf("%d��%d��%d��\n", rec.date.year, rec.date.month, rec.date.day);
	printf("\t\t\t����ʱ��: ");
	printf("%dʱ%d��%d��\n", rec.time.hour, rec.time.minute, rec.time.second);
	Ticket_Srv_DeleteBatch(rec.id);

	printf("\n\t\t\t��ʼ�޸���Ϣ\n");//���ֻ��ĳһ���ͱȽ��鷳��������Ҫ�Ż�
	printf("\t\t\t��ĿID:");
	fflush(stdin);
	scanf("%d", &rec.play_id);
	printf("\t\t\t�ݳ���ID:");
	scanf("%d", &rec.studio_id);
	printf("\t\t\t�ݳ�ʱ��(�� �� �տո���):");
	scanf("%d%d%d", &rec.date.year, &rec.date.month, &rec.date.day);
	printf("\t\t\t�ݳ�����(ʱ �� ��ո���):");
	scanf("%d%d%d", &rec.time.hour, &rec.time.minute, &rec.time.second);

	printf("\t\t\t-------------------------------------------------------\n");
	rec.seat_count=Ticket_Srv_AddBatch(rec.id, rec.studio_id);
	if (Schedule_Srv_Modify(&rec)) {
		rtn = 1;
		printf(
			"\t\t\t�޸ĳɹ�,�����������");
	}
	else
		printf("\t\t\t�޸�ʧ��,�����������");
	getch();
	return rtn;
}


/*
 * Function:    Schedule_UI_Delete
 * Function ID:	TTMS_SCU_Schedule_UI_Del
 * Description: ����ID��ɾ���ݳ��ƻ���Ϣ
 * Input:       ��ɾ���ľ�ĿID��
 * Output:      ��ʾɾ���Ƿ�ɹ�
 * Return:      0��ʾɾ��ʧ�ܣ�1��ʾɾ���ɹ�
 */
int Schedule_UI_Delete(int id){
	int rtn = 0;
	schedule_t rec;

	if (!Schedule_Srv_FetchByID(id, &rec)) {
		printf("\t\t\t�����ڴ��ݳ��ƻ�,�����������\n\t\t\t");
		getch();
		return 0;
	}
	if (Schedule_Srv_DeleteByID(id)&& Ticket_Srv_DeleteBatch(rec.studio_id)) {
		printf("\t\t\tɾ���ɹ�,�����������\n\t\t\t");
		rtn = 1;
	}
	else {
		printf("\t\t\t�����ڴ��ݳ��ƻ�,�����������\n\t\t\t");
	}
	getch();
	return rtn;

	return 0;
}

/*
 * Function:    Schedule_UI_Query
 * Function ID:	TTMS_SCU_Schedule_UI_Qry
 * Description: ����ID�Ų�ѯ�ݳ��ƻ���Ϣ
 * Input:       �����ҵľ�ĿID��
 * Output:      ���ҵ��ľ�Ŀ��Ϣ
 * Return:      0��ʾδ�ҵ���1��ʾ�ҵ���
 */
int Schedule_UI_Query(int id){
	int rtn = 1;
	schedule_t  buf;

	if (Schedule_Srv_FetchByID(id, &buf)) {
		system("cls");
		printf("\n\n\n");
		printf("\n\t\t\t                        �ݳ��ƻ�\n");
		printf("\n\n\n\t\t\t");
		rtn = 1;
		printf("= = = = = = = = = = = = = = = = = = = = = = = = = = = = = =  \n");
		printf("\t\t\t�ƻ�ID: %d\n", buf.id);
		printf("\t\t\t��ĿID: %d\n", buf.play_id);
		printf("\t\t\t��ӳ��ID: %d\n", buf.studio_id);
		printf("\t\t\t�ݳ�����:%d��%d��%d��\n", buf.date.year, buf.date.month, buf.date.day);
		printf("\t\t\t�ݳ�ʱ��:%dʱ%d��%d��\n", buf.time.hour, buf.time.minute, buf.time.second);
		printf("\t\t\t��λ����:%d\n", buf.seat_count);
		printf("\t\t\t= = = = = = = = = = = = = = = = = = = = = = = = = = = = = =  \n");
		printf("\n\n\t\t\t�����������");
		getch();
	}
	else
	{
		printf("\t\t\t���ݳ��ƻ�,�����������");
		getch();
	}
	return rtn;
}

/*
 * Function:    Schedule_UI_ListByPlay
 * Function ID:	TTMS_SCU_Schedule_UI_ListByPlay
 * Description: ���б�ģʽ��ʾ������Ŀ���ݳ��ƻ���Ϣ
 * Input:       listΪ���ҵ����ݳ��ƻ���Ϣ�����ͷָ�룬playΪ��Ŀ��Ϣ��pagingΪ��ʾ��ҳ���ò���
 * Output:      ��
 * Return:      ���ز��ҵ��ļ�¼��Ŀ
 */
void Schedule_UI_ListByPlay(const play_t *play, schedule_list_t list, Pagination_t paging){
}

/*
 * Function:    Schedule_UI_MgtEntry
 * Function ID:	TTMS_SCU_Schedule_UI_MgtEnt
 * Description: �ݳ��ƻ�������棬�ݳ��ƻ����վ�Ŀ���й����Ծ�ĿID��Ϊ����
 * Input:       ��ؾ�ĿID��
 * Output:      �ݳ��ƻ���Ϣ
 * Return:      ��
 */
void Schedule_UI_MgtEntry(void){
	int id;
	printf("\n\t\t\t\t\t��ĿID:");
	scanf("%d",&id);
	Sale_UI_ShowScheduler3(id); 

}                     
