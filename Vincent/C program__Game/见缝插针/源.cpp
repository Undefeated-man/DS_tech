#include<graphics.h>
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include<mmsystem.h>
#include<windows.h>


#define pi 3.1415926
#pragma comment(lib,"Winmm.lib")
#define max 36		//最大发射针的数量

/*
实现思路与步骤：
1.首先实现针的移动与发射，用结构体处理,数组
2.开始针跟随圆的的转动
3.用数组分别保存其发射所有针的横纵坐标
4.发射的针同时判断是否撞上其余的针，及积分是否成功
5.实现积分，时间倒计时系统（时间设为10-15秒左右）及球上显示数字（并美化其页面,可以考虑球的颜色的变化）
7.开始与结束的页面*/
 
/*就针旋转：关于实现针独立旋转的思路，我们开设一个判断其状态的标志量数组
并在与用户无关的更新中记录每个对应状态的坐标，之后将其传送到显示画面去显示刷新
*/

IMAGE img_bk0;		//开始页面图
IMAGE img_bk1;		//暂停页面图
IMAGE img_bk2;		//游戏页面图
IMAGE img_bk3;		//结束页面图
IMAGE img_bk4;		//游戏说明页面
IMAGE img_gj11, img_gj12;
IMAGE img_gj21, img_gj22;

//下面是游戏必要参数

//时间计时开始与结束
clock_t start, end;
double timesum;		//总时间
double timeuse;		//经过的时间
double timeleft;	//剩下的时间

struct gameplayer {
	//为球发射的变量
	double radius;	//针的半径
	double ball_x, ball_y;	//小球的横纵坐标
	double ball_vx, ball_vy;	//小球移动的速度
	int score;

	//求旋转的变量
	double ballnext_x[100], ballnext_y[100];		//旋转显示的球
	double linebegin_x[100], linebegin_y[100];	//针的首端坐标
	double lineend_x[100], lineend_y[100];	//针的尾端坐标
	int flag[100];		//用来标识第几个针的画面显示,0为不显示，1为显示
	double position_x[100];
	double position_y[100];	  //关于球的位置坐标，检测其是否发射成功
	double angle[100];	//旋转的角度,不同的针对应同一时间段的不同坐标
	int number_vy;		//圆（球）上显示数字的移动
	double	number1_x, number1_y;		//当count1|2<10数字的位置
	double number2_x, number2_y;	//当count1|2>=10数字的位置

}gameplayer1, gameplayer2;


enum state {
	movesuccess, movestill		//分别判断两种状态，disappear为旋转不出现
}move1, move2;


int i, j;
int count1;		//玩家一计数
int count2;		//玩家二计数
int length;	//	长度
int center_x, center_y, radius;		//中间的圆
char s1[50], st[50], s2[50], s3[50], s4[50];
char s5[50], s6[50];	//玩家一与玩家二球上数字的显示
char stime[50];		//时间的显示
int gameStatus = 0;		// 游戏状态，0为初始菜单界面，1为正常游戏，2为结束游戏状态，3为游戏暂停,4为游戏说明页面

						//函数
void startMenu();		//游戏开始界面
void pauseMenu();		//游戏暂停界面
void endMenu();		//游戏结束页面
void gameTips();	//游戏说明页面
void startup();		//开始游戏
void countdown();	//计时
void updateWithoutInput();	//与输入无关的更新
void updateWithInput();		//与输入有关的更新
void gameover();		//游戏结束

						//游戏思路:
						//游戏分两部分进行，一部分循环出现，另一部分用数组一个个显示目标

void startMenu()
{
	putimage(0, 0, &img_bk0);		//显示开始的界面图片

	FlushBatchDraw();
	Sleep(2);

	char input;
	if (_kbhit())
	{
		input = _getch();
		if (input == '1')
		{
			gameStatus = 1;

		}

		else if (input == '2')
		{
			gameStatus = 4;
		}

		else
		{
			gameStatus = 2;
			exit(0);
		}

	}

}

void pauseMenu()
{
	putimage(0, 0, &img_bk1);	// 显示背景

	LOGFONT f2;
	gettextstyle(&f2);                     // 获取当前字体设置
	f2.lfHeight = 23;                      // 设置字体高度为 23
	_tcscpy_s(f2.lfFaceName, _T("隶书"));    // 设置字体为“隶书”
	f2.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
	settextstyle(&f2);		// 设置字体样式
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(25, 0, _T("隶书"));
	outtextxy(180, 200, _T("1. 继续游戏"));
	settextcolor(LIGHTBLUE);
	outtextxy(180, 230, _T("2. 退出"));
	//游戏玩法说明
	settextcolor(BLUE);
	settextstyle(25, 0, _T("黑体"));
	outtextxy(180, 420, _T("空格发射针"));
	outtextxy(180, 450, _T("ESC键暂停游戏"));

	FlushBatchDraw();
	Sleep(2);

	char input;
	if (_kbhit())  // 判断是否有输入
	{
		input = _getch();  // 根据用户的不同输入来移动，不必输入回车

		if (input == '1')
			gameStatus = 1;

		else if (input == '2')
		{
			gameStatus = 2;
			exit(0);
		}
	}

}

void gameTips()
{
	putimage(0, 0, &img_bk4);

	LOGFONT f2;
	gettextstyle(&f2);                     // 获取当前字体设置
	f2.lfHeight = 50;                      // 设置字体高度为 23
	_tcscpy_s(f2.lfFaceName, _T("隶书"));    // 设置字体为“隶书”
	f2.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
	settextstyle(&f2);		// 设置字体样式
	setbkmode(TRANSPARENT);
	settextcolor(LIGHTBLUE);
	outtextxy(156, 100, "游戏说明");

	//游戏说明
	LOGFONT f3;
	gettextstyle(&f3);                     // 获取当前字体设置
	f3.lfHeight = 30;                      // 设置字体高度为 23
	_tcscpy_s(f3.lfFaceName, _T("隶书"));    // 设置字体为“隶书”
	f3.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
	settextstyle(&f3);		// 设置字体样式
	setbkmode(TRANSPARENT);
	settextcolor(LIGHTBLUE);
	outtextxy(150, 250, "玩家一空格发射");
	outtextxy(155, 280, "玩家二s键发射");
	outtextxy(110, 310, "双方轮流发射,比较分数");
	outtextxy(110, 340, "最后按任意键退出游戏");
	outtextxy(176, 600, "按1进入游戏");

	FlushBatchDraw();
	Sleep(2);

	char input;

	if (_kbhit())  // 判断是否有输入
	{
		input = _getch();  // 根据用户的不同输入来移动，不必输入回车

		if (input == '1')
			gameStatus = 0;

	}

}

void endMenu()
{
	putimage(0, 0, &img_bk3);	//显示结果图

	if (gameplayer1.score > gameplayer2.score)
	{
		LOGFONT f2;
		gettextstyle(&f2);                     // 获取当前字体设置
		f2.lfHeight = 50;                      // 设置字体高度为 23
		_tcscpy_s(f2.lfFaceName, _T("隶书"));    // 设置字体为“隶书”
		f2.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
		settextstyle(&f2);		// 设置字体样式
		setbkmode(TRANSPARENT);
		settextcolor(RGB(75, 176, 229));
		outtextxy(150, 110, "玩家一胜利");


	}

	else if (gameplayer1.score < gameplayer2.score)
	{
		LOGFONT f2;
		gettextstyle(&f2);                     // 获取当前字体设置
		f2.lfHeight = 50;                      // 设置字体高度为 23
		_tcscpy_s(f2.lfFaceName, _T("隶书"));    // 设置字体为“隶书”
		f2.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
		settextstyle(&f2);		// 设置字体样式
		setbkmode(TRANSPARENT);
		settextcolor(RGB(75, 176, 229));
		outtextxy(150, 110, "玩家二胜利");

	}

	else if (gameplayer1.score == gameplayer2.score)
	{
		LOGFONT f2;
		gettextstyle(&f2);                     // 获取当前字体设置
		f2.lfHeight = 50;                      // 设置字体高度为 23
		_tcscpy_s(f2.lfFaceName, _T("隶书"));    // 设置字体为“隶书”
		f2.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
		settextstyle(&f2);		// 设置字体样式
		setbkmode(TRANSPARENT);
		settextcolor(RGB(75, 176, 229));
		outtextxy(142, 110, "啊噢,平局了");

	}

	FlushBatchDraw();
	Sleep(2);

	char input;

	if (_kbhit())  // 判断是否有输入
	{
		input = _getch();  // 根据用户的不同输入来移动，不必输入回车

		exit(0);		//按任意键退出
	}

}

void startup()
{
	initgraph(500, 700);	//画面的宽度与高度

							//背景音乐的播放
	mciSendString("open D:\\game_music.mp3 alias bkmusic", NULL, 0, NULL);//打开背景音乐
	mciSendString("play bkmusic repeat", NULL, 0, NULL);  // 循环播放

	HWND hwnd = GetHWnd();		//获取窗口句柄	
	SetWindowText(hwnd, _T("插针游戏1.0"));	// 设置窗口标题文字

	loadimage(&img_bk0, _T("D:\\C program__Game\\见缝插针\\插针游戏素材\\插针游戏素材\\startgame.jpg"));
	loadimage(&img_bk1, _T("D:\\C program__Game\\见缝插针\\插针游戏素材\\插针游戏素材\\background.jpg"));
	loadimage(&img_bk2, _T("D:\\C program__Game\\见缝插针\\插针游戏素材\\插针游戏素材\\background.jpg"));
	loadimage(&img_bk3, _T("D:\\C program__Game\\见缝插针\\插针游戏素材\\插针游戏素材\\gameover.jpg"));
	loadimage(&img_bk4, _T("D:\\C program__Game\\见缝插针\\插针游戏素材\\插针游戏素材\\background.jpg"));
	loadimage(&img_gj11, _T("D:\\C program__Game\\见缝插针\\插针游戏素材\\插针游戏素材\\gj1.jpg"));
	loadimage(&img_gj12, _T("D:\\C program__Game\\见缝插针\\插针游戏素材\\插针游戏素材\\gj2.jpg"));
	loadimage(&img_gj21, _T("D:\\C program__Game\\见缝插针\\插针游戏素材\\插针游戏素材\\gj3.jpg"));
	loadimage(&img_gj22, _T("D:\\C program__Game\\见缝插针\\插针游戏素材\\插针游戏素材\\gj4.jpg"));

	//玩家一球的初始化
	gameplayer1.radius = 10;
	gameplayer1.ball_x = 245;
	gameplayer1.ball_y = 685;
	gameplayer1.ball_vx = 1;
	gameplayer1.ball_vy = 1;
	move1 = movestill;	//初始默认不发射，静止
	gameplayer1.score = 0;
	gameplayer1.number_vy = 1;		//玩家一球上数字的移动速度初始化
	gameplayer1.number1_x = 240;	//第一种情况球上数字的横坐标
	gameplayer1.number1_y = 675;	//第一种情况球上数字的纵坐标
	gameplayer1.number2_x = 236;	//第二种情况球上数字的横坐标
	gameplayer1.number2_y = 675;	//第二种情况球上数字的纵坐标

									//玩家二球的初始化
	gameplayer2.radius = 10;
	gameplayer2.ball_x = 245;
	gameplayer2.ball_y = 15;
	gameplayer2.ball_vx = 1;
	gameplayer2.ball_vy = 1;
	move2 = movestill;	//初始默认不发射，静止
	gameplayer2.score = 0;
	gameplayer2.number_vy = 1;		//玩家二球上数字的移动速度初始化
	gameplayer2.number1_x = 240;		//第一种情况球上数字的横坐标
	gameplayer2.number1_y = 5;		//第一种情况球上数字的纵坐标
	gameplayer2.number2_x = 236;	//第二种情况球上数字的横坐标
	gameplayer2.number2_y = 5;	//第二种情况球上数字的纵坐标

	memset(gameplayer1.angle, 0, sizeof(gameplayer1.angle));
	memset(gameplayer2.angle, 0, sizeof(gameplayer2.angle));
	length = 152;


	//玩家一针的初始化
	//计数初始二化
	count1 = 0;
	count2 = 0;

	gameplayer1.radius = 10;
	gameplayer2.radius = 10;
	center_x = 245;			//中间圆的初始化
	center_y = 345;
	radius = 60;

	int i;

	//将其最大根针初始化
	for (i = 0; i<max; i++)
	{
		//玩家一
		gameplayer1.ballnext_x[i] = 245;
		gameplayer1.ballnext_y[i] = center_y + radius + 80 + gameplayer1.radius;
		gameplayer1.lineend_x[i] = gameplayer1.ballnext_x[i];
		gameplayer1.lineend_y[i] = gameplayer1.ballnext_y[i] - gameplayer1.radius;
		gameplayer1.linebegin_x[i] = gameplayer1.ballnext_x[i];
		gameplayer1.linebegin_y[i] = center_y + radius;

		gameplayer1.flag[i] = 0;

		gameplayer1.position_x[i] = 0;
		gameplayer1.position_y[i] = 0;

		//玩家二
		gameplayer2.ballnext_x[i] = 245;
		gameplayer2.ballnext_y[i] = center_y - radius - 80 - gameplayer2.radius;
		gameplayer2.lineend_x[i] = gameplayer2.ballnext_x[i];
		gameplayer2.lineend_y[i] = gameplayer2.ballnext_y[i] + gameplayer2.radius;
		gameplayer2.linebegin_x[i] = gameplayer2.ballnext_x[i];
		gameplayer2.linebegin_y[i] = center_y - radius;

		gameplayer2.flag[i] = 0;

		gameplayer2.position_x[i] = 0;
		gameplayer2.position_y[i] = 0;

	}

	BeginBatchDraw();

	while (gameStatus == 0)
	{
		startMenu();	//初始菜单页面
	}

	while (gameStatus == 4)
	{
		gameTips();
	}

}

//计时开始
void countdown()
{

	start = clock();
	timesum = 18;	//总时间初始化
	timeuse = 0;	//经过的时间归零
	timeleft = 0;	//剩余时间初始化

}

void show()		//主画面及针发射的移动情况
{
	while (gameStatus == 3)
	{
		pauseMenu();	//按esc键，显示这个画面
	}

	putimage(0, 0, &img_bk2);
	putimage(185, -5, &img_gj21, NOTSRCERASE);
	putimage(185, -5, &img_gj22, SRCINVERT);	//第一个玩家弓
	putimage(185, 580, &img_gj11, NOTSRCERASE);
	putimage(185, 580, &img_gj12, SRCINVERT);	//第二个玩家弓

												//画中间的圆
	setcolor(RGB(46, 110, 168));
	setfillcolor(RGB(46, 110, 168));	// 圆内部位绿色填充
	fillcircle(center_x, center_y, radius);

	//设置玩家一字体格式
	LOGFONT f;
	gettextstyle(&f);                     // 获取当前字体设置
	f.lfHeight = 23;                      // 设置字体高度为 23
	_tcscpy_s(f.lfFaceName, _T("隶书"));    // 设置字体为“隶书”
	f.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
	settextstyle(&f);		// 设置字体样式
	setbkmode(TRANSPARENT);
	//计数
	outtextxy(400, 670, _T("针数:"));
	sprintf_s(s1, "%d", count1);
	outtextxy(460, 670, s1);
	//得分的显示
	outtextxy(10, 670, _T("得分:"));
	sprintf_s(s2, "%d", gameplayer1.score);
	outtextxy(68, 670, s2);


	//设置玩家二字体的格式
	LOGFONT f1;
	gettextstyle(&f1);                     // 获取当前字体设置
	f1.lfHeight = 23;                      // 设置字体高度为 23
	_tcscpy_s(f1.lfFaceName, _T("隶书"));    // 设置字体为“隶书”
	f1.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
	settextstyle(&f1);		// 设置字体样式
	setbkmode(TRANSPARENT);
	//计数
	outtextxy(400, 10, _T("针数:"));
	sprintf_s(s3, "%d", count2);
	outtextxy(460, 10, s3);
	//得分的显示
	outtextxy(10, 10, _T("得分:"));
	sprintf_s(s4, "%d", gameplayer2.score);
	outtextxy(68, 10, s4);
	//时间的显示
	settextcolor(WHITE);
	outtextxy(213, 312, "倒计时");
	end = clock();  		//停止计时
	timeuse = ((double)end - start) / CLK_TCK;
	timeleft = timesum - timeuse;
	sprintf_s(stime, "%.2lf", timeleft);
	if (timeleft >= 10)
		outtextxy(215, 338, stime);
	else
		outtextxy(222, 338, stime);		//调整倒计时数字显示的位置

										//玩家一的圆
	setcolor(LIGHTBLUE);
	setfillcolor(LIGHTBLUE);
	fillcircle(gameplayer1.ball_x, gameplayer1.ball_y, gameplayer1.radius);

	//玩家一圆上编号的显示
	if (count1 < 10)
	{
		settextcolor(WHITE);
		settextstyle(20, 0, __T("黑体"));
		sprintf_s(s5, "%d", count1 + 1);
		outtextxy(gameplayer1.number1_x, gameplayer1.number1_y, s5);
	}

	if (count1 >= 10)
	{
		settextcolor(WHITE);
		settextstyle(20, 0, _T("黑体"));
		sprintf_s(s5, "%d", count1 + 1);
		outtextxy(gameplayer1.number2_x, gameplayer1.number2_y, s5);
	}


	//玩家二的圆
	setcolor(BLUE);
	setfillcolor(BLUE);
	fillcircle(gameplayer2.ball_x, gameplayer2.ball_y, gameplayer2.radius);

	//玩家二圆上编号的显示
	if (count2 < 10)
	{
		settextcolor(WHITE);
		settextstyle(20, 0, _T("黑体"));
		sprintf_s(s6, "%d", count2 + 1);
		outtextxy(gameplayer2.number1_x, gameplayer2.number1_y, s6);
	}

	if (count2 >= 10)
	{
		settextcolor(WHITE);
		settextstyle(20, 0, _T("黑体"));
		sprintf_s(s6, "%d", count2 + 1);
		outtextxy(gameplayer2.number2_x, gameplayer2.number2_y, s6);
	}

	//根据flag的标识，显示其玩家一针的状态

	for (i = 0; i < count1; i++)
	{

		if (gameplayer1.flag[i] == 1)
		{
			setcolor(LIGHTBLUE);
			setfillcolor(LIGHTBLUE);
			fillcircle(gameplayer1.ballnext_x[i], gameplayer1.ballnext_y[i], gameplayer1.radius);
			setlinecolor(LIGHTBLUE);
			setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 3);
			line(gameplayer1.linebegin_x[i], gameplayer1.linebegin_y[i], gameplayer1.lineend_x[i], gameplayer1.lineend_y[i]);

		}

	}

	//根据flag的标识，显示其玩家二针的状态

	for (i = 0; i < count2; i++)
	{

		if (gameplayer2.flag[i] == 1)
		{
			setcolor(BLUE);
			setfillcolor(BLUE);
			fillcircle(gameplayer2.ballnext_x[i], gameplayer2.ballnext_y[i], gameplayer2.radius);
			setlinecolor(BLUE);
			setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 3);
			line(gameplayer2.linebegin_x[i], gameplayer2.linebegin_y[i], gameplayer2.lineend_x[i], gameplayer2.lineend_y[i]);

		}

	}

	FlushBatchDraw();

	Sleep(8);

}


void updateWithoutInput()		//负责针的发射与用户无关的更新
{

	//玩家一参数
	double gameplayer1position_xmin;
	double gameplayer1position_xmax;
	double gameplayer1position_ymin;
	double gameplayer1position_ymax;	//判断针是否撞到的必要参数
	double a;	//角度
	double sina;
	double cosa;

	//角度
	sina = sin(gameplayer1.radius / (80 + gameplayer1.radius));
	cosa = sqrt(1 - sina * sina);
	//坐标的范围
	gameplayer1position_xmin = gameplayer1.ballnext_x[count1 - 1] - 2 * gameplayer1.radius * cosa;
	gameplayer1position_xmax = gameplayer1.ballnext_x[count1 - 1] + 2 * gameplayer1.radius * cosa;
	gameplayer1position_ymin = gameplayer1.ballnext_y[count1 - 1] - 2 * gameplayer1.radius * sina;
	gameplayer1position_ymax = gameplayer1.ballnext_y[count1 - 1] + 2 * gameplayer1.radius * sina;


	//玩家二参数
	double gameplayer2position_xmin;
	double gameplayer2position_xmax;
	double gameplayer2position_ymin;
	double gameplayer2position_ymax;	//判断针是否撞到的必要参数
	double b;	//角度
	double sinb;
	double cosb;

	//角度
	sinb = sin(gameplayer2.radius / (80 + gameplayer2.radius));
	cosb = sqrt(1 - sinb * sinb);
	//坐标的范围
	gameplayer2position_xmin = gameplayer2.ballnext_x[count2 - 1] - 2 * gameplayer2.radius * cosb;
	gameplayer2position_xmax = gameplayer2.ballnext_x[count2 - 1] + 2 * gameplayer2.radius * cosb;
	gameplayer2position_ymax = gameplayer2.ballnext_y[count2 - 1] + 2 * gameplayer2.radius * sinb;
	gameplayer2position_ymin = gameplayer2.ballnext_y[count2 - 1] - 2 * gameplayer2.radius * sinb;


	//玩家一的球移动
	if (move1 == movesuccess)
	{
		//玩家一针的移动
		if (gameplayer1.ball_y > center_y + radius + length)	//发射针
		{
			//球坐标的变化
			gameplayer1.ball_y = gameplayer1.ball_y - gameplayer1.ball_vy;
			gameplayer1.ball_vx = 1;
			gameplayer1.ball_vy = 1;
			gameplayer1.ball_vy += 9.5;

		}
		else if (gameplayer1.ball_y <= center_y + radius + length)
		{
			move1 = movestill;
			gameplayer1.flag[count1 - 1] = 1;
		}
	}
	//球上数字的变化
	if (count1 < 10)
	{
		if (gameplayer1.number1_y > center_y + radius + length)
		{
			gameplayer1.number1_y = gameplayer1.number1_y - gameplayer1.number1_y;
			gameplayer1.number_vy = 1;
			gameplayer1.number_vy += 9.5;
		}
	}

	else
	{
		if (gameplayer1.number2_y > center_y + radius + length)
		{
			gameplayer1.number2_y = gameplayer1.number2_y - gameplayer1.number2_y;
			gameplayer1.number_vy = 1;
			gameplayer1.number_vy += 9.5;
		}

	}

	//判断当针不移动时对数组进行重新初始化
	if (move1 == movestill)
	{
		gameplayer1.radius = 10;
		gameplayer1.ball_x = 245;
		gameplayer1.ball_y = 685;
		gameplayer1.ball_vx = 1;
		gameplayer1.ball_vy = 1;
		if (count1 < 10)
		{
			gameplayer1.number1_x = 240;	//第一种情况球上数字的横坐标
			gameplayer1.number1_y = 675;	//第一种情况球上数字的纵坐标
		}
		else
		{
			gameplayer1.number2_x = 236;	//第二种情况球上数字的横坐标
			gameplayer1.number2_y = 675;	//第二种情况球上数字的纵坐标
		}

	}


	//玩家二球的移动
	if (move2 == movesuccess)
	{
		//玩家二针的移动
		if (gameplayer2.ball_y < center_y - radius - length)	//发射针
		{
			//球坐标的变化
			gameplayer2.ball_y = gameplayer2.ball_y + gameplayer2.ball_vy;
			gameplayer2.ball_vx = 1;
			gameplayer2.ball_vy = 1;
			gameplayer2.ball_vy += 9.5;


		}
		else if (gameplayer2.ball_y >= center_y - radius - length)
		{
			move2 = movestill;
			gameplayer2.flag[count2 - 1] = 1;
		}

	}

	//球上数字的变化
	if (count2 < 10)
	{
		if (gameplayer2.number1_y < center_y - radius - length)
		{
			gameplayer2.number1_y = gameplayer2.number1_y + gameplayer2.number1_y;
			gameplayer2.number_vy = 1;
			gameplayer2.number_vy += 9.5;
		}
	}

	else
	{
		if (gameplayer2.number2_y < center_y - radius - length)
		{
			gameplayer2.number2_y = gameplayer2.number2_y + gameplayer2.number2_y;
			gameplayer2.number_vy = 1;
			gameplayer2.number_vy += 9.5;
		}

	}


	//判断当针不移动时对数组进行重新初始化
	if (move2 == movestill)
	{

		gameplayer2.radius = 10;
		gameplayer2.ball_x = 245;
		gameplayer2.ball_y = 15;
		gameplayer2.ball_vx = 1;
		gameplayer2.ball_vy = 1;
		if (count2 < 10)
		{
			gameplayer2.number1_x = 240;		//第一种情况球上数字的横坐标
			gameplayer2.number1_y = 5;		//第一种情况球上数字的纵坐标
		}
		else
		{
			gameplayer2.number2_x = 236;	//第二种情况球上数字的横坐标
			gameplayer2.number2_y = 5;	//第二种情况球上数字的纵坐标
		}

	}

	//玩家一针旋转的参数变化
	//先更新前count1-1的数据
	for (i = 0; i < count1 - 1; i++)
	{
		if (gameplayer1.flag[i] == 1)
		{
			gameplayer1.ballnext_x[i] = center_x - length * sin(gameplayer1.angle[i]);
			gameplayer1.ballnext_y[i] = center_y + length * cos(gameplayer1.angle[i]);
			gameplayer1.lineend_x[i] = gameplayer1.ballnext_x[i] + gameplayer1.radius * sin(gameplayer1.angle[i]);
			gameplayer1.lineend_y[i] = gameplayer1.ballnext_y[i] - gameplayer1.radius * cos(gameplayer1.angle[i]);
			gameplayer1.linebegin_x[i] = gameplayer1.lineend_x[i] + 80 * sin(gameplayer1.angle[i]);
			gameplayer1.linebegin_y[i] = gameplayer1.lineend_y[i] - 80 * cos(gameplayer1.angle[i]);
			gameplayer1.angle[i] = gameplayer1.angle[i] + 2 * pi / 180;

			gameplayer1.position_x[i] = gameplayer1.ballnext_x[i];
			gameplayer1.position_y[i] = gameplayer1.ballnext_y[i];

		}

	}

	//玩家二参数的变化
	//更新前count2-1的数据
	for (i = 0; i < count2 - 1; i++)
	{
		if (gameplayer2.flag[i] == 1)
		{
			gameplayer2.ballnext_x[i] = center_x + length * sin(gameplayer2.angle[i]);
			gameplayer2.ballnext_y[i] = center_y - length * cos(gameplayer2.angle[i]);
			gameplayer2.lineend_x[i] = gameplayer2.ballnext_x[i] - gameplayer2.radius * sin(gameplayer2.angle[i]);
			gameplayer2.lineend_y[i] = gameplayer2.ballnext_y[i] + gameplayer2.radius * cos(gameplayer2.angle[i]);
			gameplayer2.linebegin_x[i] = gameplayer2.lineend_x[i] - 80 * sin(gameplayer2.angle[i]);
			gameplayer2.linebegin_y[i] = gameplayer2.lineend_y[i] + 80 * cos(gameplayer2.angle[i]);
			gameplayer2.angle[i] = gameplayer2.angle[i] + 2 * pi / 180;

			gameplayer2.position_x[i] = gameplayer2.ballnext_x[i];
			gameplayer2.position_y[i] = gameplayer2.ballnext_y[i];

		}

	}

	//判断发射的序号为count1-1针是否撞上之前的针


	if (gameplayer1.flag[count1 - 1] == 1)
	{
		for (j = 0; j < count1 - 1; j++)
		{
			if (gameplayer1.flag[j] == 1)
			{
				if ((gameplayer1.position_x[j] >= gameplayer1position_xmin
					&& gameplayer1.position_x[j] <= gameplayer1position_xmax)
					&& (gameplayer1.position_y[j] >= gameplayer1position_ymin
						&& gameplayer1.position_y[j] <= gameplayer1position_ymax))

				{
					gameplayer1.flag[count1 - 1] = 0;
					gameplayer1.score--;
					//撞上其他针音乐提示
					mciSendString(_T("stop failmusic"), NULL, 0, NULL);   // 先把前面一次的音乐停止
					mciSendString(_T("close failmusic"), NULL, 0, NULL); // 先把前面一次的音乐关闭
					mciSendString(_T("open D:\\C program__Game\\见缝插针\\插针游戏素材\\插针游戏素材\\9437.wav alias failmusic"), NULL, 0, NULL);//打开背景音乐
					mciSendString(_T("play failmusic"), NULL, 0, NULL);  // 仅播放一次

				}

			}

		}

		//判断玩家一是否撞上玩家二
		for (j = 0; j < count2 - 1; j++)
		{
			if (gameplayer2.flag[j] == 1)
			{
				if ((gameplayer2.position_x[j] >= gameplayer1position_xmin
					&& gameplayer2.position_x[j] <= gameplayer1position_xmax)
					&& (gameplayer2.position_y[j] >= gameplayer1position_ymin
						&& gameplayer2.position_y[j] <= gameplayer1position_ymax))
				{
					gameplayer1.flag[count1 - 1] = 0;
					gameplayer1.score--;
					//撞上其他针音乐提示
					mciSendString(_T("stop failmusic"), NULL, 0, NULL);   // 先把前面一次的音乐停止
					mciSendString(_T("close failmusic"), NULL, 0, NULL); // 先把前面一次的音乐关闭
					mciSendString(_T("open D:\\C program__Game\\见缝插针\\插针游戏素材\\插针游戏素材\\9437.wav alias failmusic"), NULL, 0, NULL);//打开背景音乐
					mciSendString(_T("play failmusic"), NULL, 0, NULL);  // 仅播放一次
				}
			}
		}
	}

	//判断发射的序号为count2-1针是否撞上之前的针

	if (gameplayer2.flag[count2 - 1] == 1)
	{
		for (j = 0; j < count2 - 1; j++)
		{
			if (gameplayer2.flag[j] == 1)
			{
				if ((gameplayer2.position_x[j] >= gameplayer2position_xmin &&
					gameplayer2.position_x[j] <= gameplayer2position_xmax)
					&& (gameplayer2.position_y[j] >= gameplayer2position_ymin &&
						gameplayer2.position_y[j] <= gameplayer2position_ymax))
				{
					gameplayer2.flag[count2 - 1] = 0;
					gameplayer2.score--;
					//撞上其他针音乐提示
					mciSendString(_T("stop failmusic"), NULL, 0, NULL);   // 先把前面一次的音乐停止
					mciSendString(_T("close failmusic"), NULL, 0, NULL); // 先把前面一次的音乐关闭
					mciSendString(_T("open D:\\C program__Game\\见缝插针\\插针游戏素材\\插针游戏素材\\9437.wav alias failmusic"), NULL, 0, NULL);//打开背景音乐
					mciSendString(_T("play failmusic"), NULL, 0, NULL);  // 仅播放一次
				}

			}

		}

		//判断玩家二是否撞上玩家一
		for (j = 0; j < count1 - 1; j++)
		{
			if (gameplayer1.flag[j] == 1)
			{
				if ((gameplayer1.position_x[j] >= gameplayer2position_xmin
					&& gameplayer1.position_x[j] <= gameplayer2position_xmax)
					&& (gameplayer1.position_y[j] >= gameplayer2position_ymin
						&& gameplayer1.position_y[j] <= gameplayer2position_ymax))
				{
					gameplayer2.flag[count2 - 1] = 0;
					gameplayer2.score--;
					//撞上其他针音乐提示
					mciSendString(_T("stop failmusic"), NULL, 0, NULL);   // 先把前面一次的音乐停止
					mciSendString(_T("close failmusic"), NULL, 0, NULL); // 先把前面一次的音乐关闭
					mciSendString(_T("open D:\\C program__Game\\见缝插针\\插针游戏素材\\插针游戏素材\\9437.wav alias failmusic"), NULL, 0, NULL);//打开背景音乐
					mciSendString(_T("play failmusic"), NULL, 0, NULL);  // 仅播放一次
				}
			}
		}
	}


	//若序号count1-1的针没撞上，及更新其游戏数据

	if (gameplayer1.flag[count1 - 1] == 1)
	{
		gameplayer1.ballnext_x[count1 - 1] = center_x - length * sin(gameplayer1.angle[count1 - 1]);
		gameplayer1.ballnext_y[count1 - 1] = center_y + length * cos(gameplayer1.angle[count1 - 1]);
		gameplayer1.lineend_x[count1 - 1] = gameplayer1.ballnext_x[count1 - 1] + gameplayer1.radius * sin(gameplayer1.angle[count1 - 1]);
		gameplayer1.lineend_y[count1 - 1] = gameplayer1.ballnext_y[count1 - 1] - gameplayer1.radius * cos(gameplayer1.angle[count1 - 1]);
		gameplayer1.linebegin_x[count1 - 1] = gameplayer1.lineend_x[count1 - 1] + 80 * sin(gameplayer1.angle[count1 - 1]);
		gameplayer1.linebegin_y[count1 - 1] = gameplayer1.lineend_y[count1 - 1] - 80 * cos(gameplayer1.angle[count1 - 1]);
		gameplayer1.angle[count1 - 1] = gameplayer1.angle[count1 - 1] + 2 * pi / 180;

		gameplayer1.position_x[count1 - 1] = gameplayer1.ballnext_x[count1 - 1];
		gameplayer1.position_y[count1 - 1] = gameplayer1.ballnext_y[count1 - 1];

	}


	//若序号为count2-1的针没撞上，及更新其游戏数据

	if (gameplayer2.flag[count2 - 1] == 1)
	{
		gameplayer2.ballnext_x[count2 - 1] = center_x + length * sin(gameplayer2.angle[count2 - 1]);
		gameplayer2.ballnext_y[count2 - 1] = center_y - length * cos(gameplayer2.angle[count2 - 1]);
		gameplayer2.lineend_x[count2 - 1] = gameplayer2.ballnext_x[count2 - 1] - gameplayer2.radius * sin(gameplayer2.angle[count2 - 1]);
		gameplayer2.lineend_y[count2 - 1] = gameplayer2.ballnext_y[count2 - 1] + gameplayer2.radius * cos(gameplayer2.angle[count2 - 1]);
		gameplayer2.linebegin_x[count2 - 1] = gameplayer2.lineend_x[count2 - 1] - 80 * sin(gameplayer2.angle[count2 - 1]);
		gameplayer2.linebegin_y[count2 - 1] = gameplayer2.lineend_y[count2 - 1] + 80 * cos(gameplayer2.angle[count2 - 1]);
		gameplayer2.angle[count2 - 1] = gameplayer2.angle[i] + 2 * pi / 180;

		gameplayer2.position_x[count2 - 1] = gameplayer2.ballnext_x[count2 - 1];
		gameplayer2.position_y[count2 - 1] = gameplayer2.ballnext_y[count2 - 1];

	}

	if (timeleft <= 0)
	{
		gameStatus = 2;
	}

	while (gameStatus == 2)
	{
		endMenu();		//结束页面
	}

}

//针的旋转及碰撞判断的实现代码

void updateWithInput()
{
	char input;
	if (_kbhit())
	{
		input = _getch();
		//玩家一的操作
		if (input == ' ')
		{
			move1 = movesuccess;
			count1++;		//记录玩家1发射针的个数
			gameplayer1.score++;

			//发针的音乐
			mciSendString(_T("stop send1music"), NULL, 0, NULL);   // 先把前面一次的音乐停止
			mciSendString(_T("close send1music"), NULL, 0, NULL); // 先把前面一次的音乐关闭
			mciSendString(_T("open D:\\C program__Game\\见缝插针\\插针游戏素材\\插针游戏素材\\jump.mp3 alias send1music"), NULL, 0, NULL);//打开背景音乐
			mciSendString(_T("play send1music"), NULL, 0, NULL);  // 仅播放一次

		}
		//游戏暂停界面
		if (input == 27)
			gameStatus = 3;
		//玩家二的操作
		if (input == 's')
		{
			move2 = movesuccess;
			count2++;		//记录玩家二发射针的个数
			gameplayer2.score++;

			//发针的音乐
			mciSendString(_T("stop send2music"), NULL, 0, NULL);   // 先把前面一次的音乐停止
			mciSendString(_T("close send2music"), NULL, 0, NULL); // 先把前面一次的音乐关闭
			mciSendString(_T("open D:\\C program__Game\\见缝插针\\插针游戏素材\\插针游戏素材\\jump.mp3 alias send2music"), NULL, 0, NULL);//打开背景音乐
			mciSendString(_T("play send2music"), NULL, 0, NULL);  // 仅播放一次

		}

	}

}

void gameover()
{
	EndBatchDraw();
	_getch();
	closegraph();

}


int main()
{
	startup();

	countdown();

	while (1)
	{

		show();

		updateWithoutInput();

		updateWithInput();

	}

	gameover();

	return 0;
}
