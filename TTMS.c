#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <io.h>
#include "./View/Main_Menu.h"
#include "./View/Account_UI.h"
//����ȫ�ֱ��������ڴ洢��½�û���Ϣ
account_t gl_CurUser = { 0, USR_ANOMY, "Anonymous","" };
int main(void)
{
    system("cls");
    system("color 0E");
    system("mode con cols=103 lines=35");
	setvbuf(stdout, NULL, _IONBF, 0);
	if (!SysLogin()) {
		printf("\n\t\t\t\t\t�����������,�����˳�ϵͳ\n");
		Sleep(3000);
		return EXIT_SUCCESS;
	}
	Main_Menu(gl_CurUser);
	system("pause"); 
	return EXIT_SUCCESS;
}
	

