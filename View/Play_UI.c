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
	 * Description: ���б�ģʽ��ʾ��Ŀ��Ϣ
	 * Input:       list��Ŀ��Ϣ�����ͷָ�룬paging��ҳ���ò���
	 * Output:      ���о�Ŀ����Ϣ
	 * Return:      ���ز��ҵ��ļ�¼��Ŀ
	 */
void Play_UI_ShowList(play_list_t list, Pagination_t paging) {
	int i;
	play_node_t *pos;
	printf("\n\t\t\t��Ŀ����:1Ϊ��Ӱ  2Ϊ���  3Ϊ���ֻ�\n\t\t\t�ݳ�����:1�ʺ϶�ͯ�ۿ�  2�ʺ�������ۿ�  3�ʺϳ��˹ۿ�\n");
	printf("\t\t\t=========================================================\n");
	printf("\n\t\t\t                        ��Ŀ�б�\n");
	printf("\n\t\n");
	printf("\t%-5s  %-16s  %-4s  %8s  %8s %8s  %10s  %10s  %5s\n\n",
		"ID", "    ����", "����", "����", "�ݳ�����", "ʱ��", "��ӳʱ��", "�¼�ʱ��", "�۸�");
	
	//��ʾ����
	for (i = 0, pos = (play_node_t *)(paging.curPos);
		pos != list && i < paging.pageSize; i++) {
		printf("\t%-5d  %-16s  %-4s  %8s   %8s  %5d   %4d-%02d-%02d   %4d-%02d-%02d   %-5d\n", pos->data.id,
			pos->data.name, type(pos->data.type), pos->data.area,
			rating(pos->data.rating), pos->data.duration, pos->data.start_date.year, pos->data.start_date.month, pos->data.start_date.day,
			pos->data.end_date.year, pos->data.end_date.month, pos->data.end_date.day, pos->data.price);
		pos = pos->next;
	}
	//��Ŀ���Ͷ��壬1��ʾ��Ӱ��2��ʾ��磬3��ʾ���ֻ�
	//�ݳ��������Ͷ��壬1��ʾ��ͯ�ɹۿ���2��ʾ������ɹۿ���3��ʾ���˿ɹۿ�

}

/*
 * Function:    Play_UI_MgtEntry
 * Function ID:	TTMS_SCU_Play_UI_MgtEnt
 * Description: ��Ŀ��Ϣ�������
 * Input:       flagΪ0���������ҳ�棬flagΪ1�����ѯ����
 * Output:      ��Ŀ�������
 * Return:      ��
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
	//��������
	paging.totalRecords = Play_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);

	if (flag)
	{
		do {
			system("cls");
			Play_UI_ShowList(head, paging);
			printf("\n\t\t\t����:%2d\t\t\t\t\t\t    %d/%d \n",
				paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));

			printf("\n\n\t\t\t\t 1.��ҳ    2.��ҳ   3.��ѯ   4.����");
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
				if (Play_UI_Query(id)) {	//������������
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
			printf("\n\t\t\t����:%2d\t\t\t\t\t\t    %d/%d \n",
				paging.totalRecords, Pageing_CurPage(paging), Pageing_TotalPages(paging));
			printf("\n\n\t\t\t1.��ҳ  2.��ҳ  3.����  4.ɾ��  5.�޸�  6.��ѯ  7.����");
			printf("\n");
			printf("\n\t\t\t:");
			choice = getche();
			switch (choice)
			{
			case '3':
				if (Play_UI_Add()) //����ӳɹ����������һҳ��ʾ
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
				if (Play_UI_Delete(id)) {	//������������
					paging.totalRecords = Play_Srv_FetchAll(head);
					List_Paging(head, paging, play_node_t);
				}
				break;
			case '5':
				printf("\n");
				printf("\n\t\t\tID:");
				scanf("%d", &id);
				getchar();
				if (Play_UI_Modify(id)) {	//������������
					paging.totalRecords = Play_Srv_FetchAll(head);
					List_Paging(head, paging, play_node_t);
				}
				break;

			case '6':
				printf("\n");
				printf("\n\t\t\tID:");
				scanf("%d", &id);
				getchar();
				if (Play_UI_Query(id)) {	//������������
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
 * Description: ���һ����Ŀ��Ϣ
 * Input:       ��
 * Output:      ������Ϣʱ�ĸ�����ʾ
 * Return:      ��ӵļ�¼��
 */
int Play_UI_Add(void){
	
	play_t rec;
	char choice;
	int newRecCount = 0;

	do {
		printf("\n\n\t\t\t                        �����Ŀ\n");
		printf("\t\t\t=========================================================\n");

		printf("\n\t\t\t��Ŀ����:");
		fflush(stdin);
		gets(rec.name);
		printf("\t\t\t��Ŀ����(1/2/3):");
		scanf("%d", &rec.type);//��Ŀ���Ͷ��壬1��ʾ��Ӱ��2��ʾ��磬3��ʾ���ֻ�
		printf("\t\t\t����:");
		getchar();
		gets(rec.area);
		printf("\t\t\t��Ŀ��������(1/2/3):");
		scanf("%d", &rec.rating);//�ݳ��������Ͷ��壬1��ʾ��ͯ�ɹۿ���2��ʾ������ɹۿ���3��ʾ���˿ɹۿ�
		printf("\t\t\t��Ŀʱ��:");
		scanf("%d",&rec.duration);
		printf("\t\t\t��ӳʱ��(�� �� ���Կո���):");
		scanf("%d%d%d", &rec.start_date.year, &rec.start_date.month, &rec.start_date.day);
		printf("\t\t\t�¼�ʱ��(�� �� ���Կո���):");
		scanf("%d%d%d",&rec.end_date.year, &rec.end_date.month, &rec.end_date.day);
		printf("\t\t\t�۸�:");
		scanf("%d", &rec.price);
		getchar();
		printf("\t\t\t=======================================================\n");

		//��ȡ����
		rec.id = EntKey_Srv_CompNewKey("Play");

		if (Play_Srv_Add(&rec)) {
			newRecCount += 1;
			printf("\t\t\t����ɹ�\n");
		}
		else
			printf("\t\t\t����ʧ��\n");
		printf("\t\t\t-------------------------------------------------------\n");
		printf("\t\t\t1.�������    2.����");
		fflush(stdin);
		choice = getche();
	}while ('1' == choice);
	return newRecCount;
}

/*
 * Function:    Play_UI_Modify
 * Function ID:	TTMS_SCU_Play_UI_Mod
 * Description: ���¾�Ŀ��Ϣ
 * Input:       �����µľ�ĿID��
 * Output:      ������Ϣʱ�ĸ�����ʾ
 * Return:      ���µľ�Ŀ��Ϣ����0��ʾδ�ҵ���1��ʾ�ҵ�������
 */
int Play_UI_Modify(int id){
	play_t rec;
	int rtn = 0;

	if (!Play_Srv_FetchByID(id, &rec)) {
		printf("\t\t\t��Ŀ������,�����������\n");
		getch();
		return 0;
	}
	//����	
	printf("\n\n\t\t\t                        �޸ľ�Ŀ\n");
	printf("\t\t\t=========================================================\n\n");
	printf("\t\t\tID:%d\n", rec.id);
	printf("\t\t\t��Ŀ����: [%s]\n", rec.name);
	printf("\t\t\t��Ŀ����:%s\n", type(rec.type));//��Ŀ���Ͷ��壬1��ʾ��Ӱ��2��ʾ��磬3��ʾ���ֻ�
	printf("\t\t\t����:%s\n", rec.area);
	printf("\t\t\t��Ŀ�ݳ�����:%s\n", rating(rec.rating));//�ݳ��������Ͷ��壬1��ʾ��ͯ�ɹۿ���2��ʾ������ɹۿ���3��ʾ���˿ɹۿ�
	printf("\t\t\t��Ŀʱ��:%d\n", rec.duration);
	printf("\t\t\t��ӳ����:%d-%d-%d\n", rec.start_date.year,rec.start_date.month,rec.start_date.day);
	printf("\t\t\t�¼�����:%d-%d-%d\n", rec.end_date.year, rec.end_date.month, rec.end_date.day);
	printf("\t\t\t�۸�:%d\n", rec.price);
	
	printf("\t\t\t�����޸ĺ����Ϣ:\n");//���ֻ��ĳһ���ͱȽ��鷳��������Ҫ�Ż�
	fflush(stdin);
	printf("\t\t\t��Ŀ����:");
	gets(rec.name);
	printf("\t\t\t��Ŀ����(1/2/3):");
	scanf("%d", &rec.type);//��Ŀ���Ͷ��壬1��ʾ��Ӱ��2��ʾ��磬3��ʾ���ֻ�
	getchar();
	printf("\t\t\t����:");
	gets(rec.area);
	printf("\t\t\t��Ŀ�ݳ�����(1/2/3):");
	scanf("%d", &rec.rating);//�ݳ��������Ͷ��壬1��ʾ��ͯ�ɹۿ���2��ʾ������ɹۿ���3��ʾ���˿ɹۿ�
	printf("\t\t\t��Ŀʱ��:");
	scanf("%d", &rec.duration);
	printf("\t\t\t��ӳ����(�� �� ���Կո���):");
	scanf("%d%d%d", &rec.start_date.year, &rec.start_date.month, &rec.start_date.day);
	printf("\t\t\t�¼�����(�� �� ���Կո���):");
	scanf("%d%d%d", &rec.end_date.year, &rec.end_date.month, &rec.end_date.day);
	printf("\t\t\t�۸�:");
	scanf("%d", &rec.price);
	
	printf("\t\t\t-------------------------------------------------------\n");

	if (Play_Srv_Modify(&rec)) {
		rtn = 1;
		printf(
			"\t\t\t�޸ĳɹ�,��������˳�");
	}
	else
		printf("\t\t\t�޸�ʧ��,��������˳�");
	getch();
	return rtn;
}

/*
 * Function:    Play_UI_Delete
 * Function ID:	TTMS_SCU_Play_UI_Del
 * Description: ����ID��ɾ����Ŀ��Ϣ
 * Input:       ��ɾ���ľ�ĿID��
 * Output:      ��ʾɾ���Ƿ�ɹ�
 * Return:      0��ʾɾ��ʧ�ܣ�1��ʾɾ���ɹ�
 */
int Play_UI_Delete(int id){

	int rtn = 0;

	if (Play_Srv_DeleteByID(id)) {
		printf("\t\t\tɾ���ɹ�,��������˳�");
		rtn = 1;
	}
	else {
		printf("\t\t\tɾ��ʧ��,��������˳�");
	}
	getch();
	return rtn;
}

/*
 * Function:    Play_UI_Query
 * Function ID:	TTMS_SCU_Play_UI_Qry
 * Description: ����ID�Ų�ѯ��Ŀ��Ϣ
 * Input:       �����ҵľ�ĿID��
 * Output:      ���ҵ��ľ�Ŀ��Ϣ
 * Return:      0��ʾδ�ҵ���1��ʾ�ҵ���
 */
int Play_UI_Query(int id){
	int rtn = 0;
	play_t rec;
	if (!Play_Srv_FetchByID(id, &rec)) {
		printf("\t\t\tû�д˾�Ŀ,��������˳�");
		getch();
		return 0;
	}
	//����	
	printf("\n\n\t\t\t                        ��Ŀ��Ϣ\n");
	printf("\t\t\t=========================================================\n");

	printf("\t\t\tID:%d\n", rec.id);
	printf("\t\t\t����: %s\n", rec.name);
	printf("\t\t\t����:%s\n", type(rec.type));//��Ŀ���Ͷ��壬1��ʾ��Ӱ��2��ʾ��磬3��ʾ���ֻ�
	printf("\t\t\t����:%s\n", rec.area);
	printf("\t\t\t�ݳ�����:%s\n", rating(rec.rating));//�ݳ��������Ͷ��壬1��ʾ��ͯ�ɹۿ���2��ʾ������ɹۿ���3��ʾ���˿ɹۿ�
	printf("\t\t\tʱ��:%d\n", rec.duration);
	printf("\t\t\t��ӳʱ��:%d-%d-%d\n", rec.start_date.year, rec.start_date.month, rec.start_date.day);
	printf("\t\t\t�¼�ʱ��:%d-%d-%d\n", rec.end_date.year, rec.end_date.month, rec.end_date.day);
	printf("\t\t\t�۸�:%d\n", rec.price);
	printf("\t\t\t��������˳�");
	getch();
	return 1;
}

char *type(int type)
{
	if (type == 1)
	{
		return "��Ӱ";
	}
	else if (type == 2)
	{
		return "���";
	}
	else if (type == 3)
	{
		return  "����";
	}
	else
		return "����";
}
char *rating(int rating)
{
	if (rating == 1)
	{
		return "��ͯ";
	}
	else if (rating == 2)
	{
		return "����";
	}
	else if (rating == 3)
	{
		return "����";
	}
	else
		return "����";
}


