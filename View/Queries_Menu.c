#include <stdio.h>
#include <stdlib.h>

#include "Queries_Menu.h"
#include "Schedule_UI.h"
#include "Studio_UI.h"
#include "Play_UI.h"
#include "conio.h"

void Queries_Menu(void){
	char choice;
	do {
		system("cls");
		printf("\n\n\t\t\t                        ��Ϣ��ѯ\n");
		printf("\t\t\t=========================================================\n");
		printf("\t\t\t1.�ݳ�����ѯ\n");
		printf("\t\t\t2.��Ŀ��ѯ\n");
		printf("\t\t\t3.�ݳ��ƻ���ѯ\n");
		printf("\t\t\t4.�˳�\n");
		printf("\n\t\t\t=========================================================\n");
		printf("\n\t\t\t:");
		fflush(stdin);
		choice = getche();
		switch (choice) {
		case '1':
			Studio_UI_MgtEntry(2);
			break;
		case '2':
			Play_UI_MgtEntry(1);
			break;
		case '3':
			Schedule_UI_ListAll();
			break;
		}
	} while ('4' != choice);
	}
