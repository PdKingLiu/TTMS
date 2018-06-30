#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <io.h>
#include "./View/Main_Menu.h"
#include "./View/Account_UI.h"
//定义全局变量，用于存储登陆用户信息
account_t gl_CurUser = { 0, USR_ANOMY, "Anonymous","" };
int main(void)
{
    system("cls");
    system("color 0E");
    system("mode con cols=103 lines=35");
	setvbuf(stdout, NULL, _IONBF, 0);
	if (!SysLogin()) {
		printf("\n\t\t\t\t\t错误次数超限,即将退出系统\n");
		Sleep(3000);
		return EXIT_SUCCESS;
	}
	Main_Menu(gl_CurUser);
	system("pause"); 
	return EXIT_SUCCESS;
}
	

