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
#define max 36		//������������

/*
ʵ��˼·�벽�裺
1.����ʵ������ƶ��뷢�䣬�ýṹ�崦��,����
2.��ʼ�����Բ�ĵ�ת��
3.������ֱ𱣴��䷢��������ĺ�������
4.�������ͬʱ�ж��Ƿ�ײ��������룬�������Ƿ�ɹ�
5.ʵ�ֻ��֣�ʱ�䵹��ʱϵͳ��ʱ����Ϊ10-15�����ң���������ʾ���֣���������ҳ��,���Կ��������ɫ�ı仯��
7.��ʼ�������ҳ��*/
 
/*������ת������ʵ���������ת��˼·�����ǿ���һ���ж���״̬�ı�־������
�������û��޹صĸ����м�¼ÿ����Ӧ״̬�����֮꣬���䴫�͵���ʾ����ȥ��ʾˢ��
*/

IMAGE img_bk0;		//��ʼҳ��ͼ
IMAGE img_bk1;		//��ͣҳ��ͼ
IMAGE img_bk2;		//��Ϸҳ��ͼ
IMAGE img_bk3;		//����ҳ��ͼ
IMAGE img_bk4;		//��Ϸ˵��ҳ��
IMAGE img_gj11, img_gj12;
IMAGE img_gj21, img_gj22;

//��������Ϸ��Ҫ����

//ʱ���ʱ��ʼ�����
clock_t start, end;
double timesum;		//��ʱ��
double timeuse;		//������ʱ��
double timeleft;	//ʣ�µ�ʱ��

struct gameplayer {
	//Ϊ����ı���
	double radius;	//��İ뾶
	double ball_x, ball_y;	//С��ĺ�������
	double ball_vx, ball_vy;	//С���ƶ����ٶ�
	int score;

	//����ת�ı���
	double ballnext_x[100], ballnext_y[100];		//��ת��ʾ����
	double linebegin_x[100], linebegin_y[100];	//����׶�����
	double lineend_x[100], lineend_y[100];	//���β������
	int flag[100];		//������ʶ�ڼ�����Ļ�����ʾ,0Ϊ����ʾ��1Ϊ��ʾ
	double position_x[100];
	double position_y[100];	  //�������λ�����꣬������Ƿ���ɹ�
	double angle[100];	//��ת�ĽǶ�,��ͬ�����Ӧͬһʱ��εĲ�ͬ����
	int number_vy;		//Բ��������ʾ���ֵ��ƶ�
	double	number1_x, number1_y;		//��count1|2<10���ֵ�λ��
	double number2_x, number2_y;	//��count1|2>=10���ֵ�λ��

}gameplayer1, gameplayer2;


enum state {
	movesuccess, movestill		//�ֱ��ж�����״̬��disappearΪ��ת������
}move1, move2;


int i, j;
int count1;		//���һ����
int count2;		//��Ҷ�����
int length;	//	����
int center_x, center_y, radius;		//�м��Բ
char s1[50], st[50], s2[50], s3[50], s4[50];
char s5[50], s6[50];	//���һ����Ҷ��������ֵ���ʾ
char stime[50];		//ʱ�����ʾ
int gameStatus = 0;		// ��Ϸ״̬��0Ϊ��ʼ�˵����棬1Ϊ������Ϸ��2Ϊ������Ϸ״̬��3Ϊ��Ϸ��ͣ,4Ϊ��Ϸ˵��ҳ��

						//����
void startMenu();		//��Ϸ��ʼ����
void pauseMenu();		//��Ϸ��ͣ����
void endMenu();		//��Ϸ����ҳ��
void gameTips();	//��Ϸ˵��ҳ��
void startup();		//��ʼ��Ϸ
void countdown();	//��ʱ
void updateWithoutInput();	//�������޹صĸ���
void updateWithInput();		//�������йصĸ���
void gameover();		//��Ϸ����

						//��Ϸ˼·:
						//��Ϸ�������ֽ��У�һ����ѭ�����֣���һ����������һ������ʾĿ��

void startMenu()
{
	putimage(0, 0, &img_bk0);		//��ʾ��ʼ�Ľ���ͼƬ

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
	putimage(0, 0, &img_bk1);	// ��ʾ����

	LOGFONT f2;
	gettextstyle(&f2);                     // ��ȡ��ǰ��������
	f2.lfHeight = 23;                      // ��������߶�Ϊ 23
	_tcscpy_s(f2.lfFaceName, _T("����"));    // ��������Ϊ�����顱
	f2.lfQuality = ANTIALIASED_QUALITY;    // �������Ч��Ϊ�����  
	settextstyle(&f2);		// ����������ʽ
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(25, 0, _T("����"));
	outtextxy(180, 200, _T("1. ������Ϸ"));
	settextcolor(LIGHTBLUE);
	outtextxy(180, 230, _T("2. �˳�"));
	//��Ϸ�淨˵��
	settextcolor(BLUE);
	settextstyle(25, 0, _T("����"));
	outtextxy(180, 420, _T("�ո�����"));
	outtextxy(180, 450, _T("ESC����ͣ��Ϸ"));

	FlushBatchDraw();
	Sleep(2);

	char input;
	if (_kbhit())  // �ж��Ƿ�������
	{
		input = _getch();  // �����û��Ĳ�ͬ�������ƶ�����������س�

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
	gettextstyle(&f2);                     // ��ȡ��ǰ��������
	f2.lfHeight = 50;                      // ��������߶�Ϊ 23
	_tcscpy_s(f2.lfFaceName, _T("����"));    // ��������Ϊ�����顱
	f2.lfQuality = ANTIALIASED_QUALITY;    // �������Ч��Ϊ�����  
	settextstyle(&f2);		// ����������ʽ
	setbkmode(TRANSPARENT);
	settextcolor(LIGHTBLUE);
	outtextxy(156, 100, "��Ϸ˵��");

	//��Ϸ˵��
	LOGFONT f3;
	gettextstyle(&f3);                     // ��ȡ��ǰ��������
	f3.lfHeight = 30;                      // ��������߶�Ϊ 23
	_tcscpy_s(f3.lfFaceName, _T("����"));    // ��������Ϊ�����顱
	f3.lfQuality = ANTIALIASED_QUALITY;    // �������Ч��Ϊ�����  
	settextstyle(&f3);		// ����������ʽ
	setbkmode(TRANSPARENT);
	settextcolor(LIGHTBLUE);
	outtextxy(150, 250, "���һ�ո���");
	outtextxy(155, 280, "��Ҷ�s������");
	outtextxy(110, 310, "˫����������,�ȽϷ���");
	outtextxy(110, 340, "���������˳���Ϸ");
	outtextxy(176, 600, "��1������Ϸ");

	FlushBatchDraw();
	Sleep(2);

	char input;

	if (_kbhit())  // �ж��Ƿ�������
	{
		input = _getch();  // �����û��Ĳ�ͬ�������ƶ�����������س�

		if (input == '1')
			gameStatus = 0;

	}

}

void endMenu()
{
	putimage(0, 0, &img_bk3);	//��ʾ���ͼ

	if (gameplayer1.score > gameplayer2.score)
	{
		LOGFONT f2;
		gettextstyle(&f2);                     // ��ȡ��ǰ��������
		f2.lfHeight = 50;                      // ��������߶�Ϊ 23
		_tcscpy_s(f2.lfFaceName, _T("����"));    // ��������Ϊ�����顱
		f2.lfQuality = ANTIALIASED_QUALITY;    // �������Ч��Ϊ�����  
		settextstyle(&f2);		// ����������ʽ
		setbkmode(TRANSPARENT);
		settextcolor(RGB(75, 176, 229));
		outtextxy(150, 110, "���һʤ��");


	}

	else if (gameplayer1.score < gameplayer2.score)
	{
		LOGFONT f2;
		gettextstyle(&f2);                     // ��ȡ��ǰ��������
		f2.lfHeight = 50;                      // ��������߶�Ϊ 23
		_tcscpy_s(f2.lfFaceName, _T("����"));    // ��������Ϊ�����顱
		f2.lfQuality = ANTIALIASED_QUALITY;    // �������Ч��Ϊ�����  
		settextstyle(&f2);		// ����������ʽ
		setbkmode(TRANSPARENT);
		settextcolor(RGB(75, 176, 229));
		outtextxy(150, 110, "��Ҷ�ʤ��");

	}

	else if (gameplayer1.score == gameplayer2.score)
	{
		LOGFONT f2;
		gettextstyle(&f2);                     // ��ȡ��ǰ��������
		f2.lfHeight = 50;                      // ��������߶�Ϊ 23
		_tcscpy_s(f2.lfFaceName, _T("����"));    // ��������Ϊ�����顱
		f2.lfQuality = ANTIALIASED_QUALITY;    // �������Ч��Ϊ�����  
		settextstyle(&f2);		// ����������ʽ
		setbkmode(TRANSPARENT);
		settextcolor(RGB(75, 176, 229));
		outtextxy(142, 110, "����,ƽ����");

	}

	FlushBatchDraw();
	Sleep(2);

	char input;

	if (_kbhit())  // �ж��Ƿ�������
	{
		input = _getch();  // �����û��Ĳ�ͬ�������ƶ�����������س�

		exit(0);		//��������˳�
	}

}

void startup()
{
	initgraph(500, 700);	//����Ŀ����߶�

							//�������ֵĲ���
	mciSendString("open D:\\game_music.mp3 alias bkmusic", NULL, 0, NULL);//�򿪱�������
	mciSendString("play bkmusic repeat", NULL, 0, NULL);  // ѭ������

	HWND hwnd = GetHWnd();		//��ȡ���ھ��	
	SetWindowText(hwnd, _T("������Ϸ1.0"));	// ���ô��ڱ�������

	loadimage(&img_bk0, _T("D:\\C program__Game\\�������\\������Ϸ�ز�\\������Ϸ�ز�\\startgame.jpg"));
	loadimage(&img_bk1, _T("D:\\C program__Game\\�������\\������Ϸ�ز�\\������Ϸ�ز�\\background.jpg"));
	loadimage(&img_bk2, _T("D:\\C program__Game\\�������\\������Ϸ�ز�\\������Ϸ�ز�\\background.jpg"));
	loadimage(&img_bk3, _T("D:\\C program__Game\\�������\\������Ϸ�ز�\\������Ϸ�ز�\\gameover.jpg"));
	loadimage(&img_bk4, _T("D:\\C program__Game\\�������\\������Ϸ�ز�\\������Ϸ�ز�\\background.jpg"));
	loadimage(&img_gj11, _T("D:\\C program__Game\\�������\\������Ϸ�ز�\\������Ϸ�ز�\\gj1.jpg"));
	loadimage(&img_gj12, _T("D:\\C program__Game\\�������\\������Ϸ�ز�\\������Ϸ�ز�\\gj2.jpg"));
	loadimage(&img_gj21, _T("D:\\C program__Game\\�������\\������Ϸ�ز�\\������Ϸ�ز�\\gj3.jpg"));
	loadimage(&img_gj22, _T("D:\\C program__Game\\�������\\������Ϸ�ز�\\������Ϸ�ز�\\gj4.jpg"));

	//���һ��ĳ�ʼ��
	gameplayer1.radius = 10;
	gameplayer1.ball_x = 245;
	gameplayer1.ball_y = 685;
	gameplayer1.ball_vx = 1;
	gameplayer1.ball_vy = 1;
	move1 = movestill;	//��ʼĬ�ϲ����䣬��ֹ
	gameplayer1.score = 0;
	gameplayer1.number_vy = 1;		//���һ�������ֵ��ƶ��ٶȳ�ʼ��
	gameplayer1.number1_x = 240;	//��һ������������ֵĺ�����
	gameplayer1.number1_y = 675;	//��һ������������ֵ�������
	gameplayer1.number2_x = 236;	//�ڶ�������������ֵĺ�����
	gameplayer1.number2_y = 675;	//�ڶ�������������ֵ�������

									//��Ҷ���ĳ�ʼ��
	gameplayer2.radius = 10;
	gameplayer2.ball_x = 245;
	gameplayer2.ball_y = 15;
	gameplayer2.ball_vx = 1;
	gameplayer2.ball_vy = 1;
	move2 = movestill;	//��ʼĬ�ϲ����䣬��ֹ
	gameplayer2.score = 0;
	gameplayer2.number_vy = 1;		//��Ҷ��������ֵ��ƶ��ٶȳ�ʼ��
	gameplayer2.number1_x = 240;		//��һ������������ֵĺ�����
	gameplayer2.number1_y = 5;		//��һ������������ֵ�������
	gameplayer2.number2_x = 236;	//�ڶ�������������ֵĺ�����
	gameplayer2.number2_y = 5;	//�ڶ�������������ֵ�������

	memset(gameplayer1.angle, 0, sizeof(gameplayer1.angle));
	memset(gameplayer2.angle, 0, sizeof(gameplayer2.angle));
	length = 152;


	//���һ��ĳ�ʼ��
	//������ʼ����
	count1 = 0;
	count2 = 0;

	gameplayer1.radius = 10;
	gameplayer2.radius = 10;
	center_x = 245;			//�м�Բ�ĳ�ʼ��
	center_y = 345;
	radius = 60;

	int i;

	//�����������ʼ��
	for (i = 0; i<max; i++)
	{
		//���һ
		gameplayer1.ballnext_x[i] = 245;
		gameplayer1.ballnext_y[i] = center_y + radius + 80 + gameplayer1.radius;
		gameplayer1.lineend_x[i] = gameplayer1.ballnext_x[i];
		gameplayer1.lineend_y[i] = gameplayer1.ballnext_y[i] - gameplayer1.radius;
		gameplayer1.linebegin_x[i] = gameplayer1.ballnext_x[i];
		gameplayer1.linebegin_y[i] = center_y + radius;

		gameplayer1.flag[i] = 0;

		gameplayer1.position_x[i] = 0;
		gameplayer1.position_y[i] = 0;

		//��Ҷ�
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
		startMenu();	//��ʼ�˵�ҳ��
	}

	while (gameStatus == 4)
	{
		gameTips();
	}

}

//��ʱ��ʼ
void countdown()
{

	start = clock();
	timesum = 18;	//��ʱ���ʼ��
	timeuse = 0;	//������ʱ�����
	timeleft = 0;	//ʣ��ʱ���ʼ��

}

void show()		//�����漰�뷢����ƶ����
{
	while (gameStatus == 3)
	{
		pauseMenu();	//��esc������ʾ�������
	}

	putimage(0, 0, &img_bk2);
	putimage(185, -5, &img_gj21, NOTSRCERASE);
	putimage(185, -5, &img_gj22, SRCINVERT);	//��һ����ҹ�
	putimage(185, 580, &img_gj11, NOTSRCERASE);
	putimage(185, 580, &img_gj12, SRCINVERT);	//�ڶ�����ҹ�

												//���м��Բ
	setcolor(RGB(46, 110, 168));
	setfillcolor(RGB(46, 110, 168));	// Բ�ڲ�λ��ɫ���
	fillcircle(center_x, center_y, radius);

	//�������һ�����ʽ
	LOGFONT f;
	gettextstyle(&f);                     // ��ȡ��ǰ��������
	f.lfHeight = 23;                      // ��������߶�Ϊ 23
	_tcscpy_s(f.lfFaceName, _T("����"));    // ��������Ϊ�����顱
	f.lfQuality = ANTIALIASED_QUALITY;    // �������Ч��Ϊ�����  
	settextstyle(&f);		// ����������ʽ
	setbkmode(TRANSPARENT);
	//����
	outtextxy(400, 670, _T("����:"));
	sprintf_s(s1, "%d", count1);
	outtextxy(460, 670, s1);
	//�÷ֵ���ʾ
	outtextxy(10, 670, _T("�÷�:"));
	sprintf_s(s2, "%d", gameplayer1.score);
	outtextxy(68, 670, s2);


	//������Ҷ�����ĸ�ʽ
	LOGFONT f1;
	gettextstyle(&f1);                     // ��ȡ��ǰ��������
	f1.lfHeight = 23;                      // ��������߶�Ϊ 23
	_tcscpy_s(f1.lfFaceName, _T("����"));    // ��������Ϊ�����顱
	f1.lfQuality = ANTIALIASED_QUALITY;    // �������Ч��Ϊ�����  
	settextstyle(&f1);		// ����������ʽ
	setbkmode(TRANSPARENT);
	//����
	outtextxy(400, 10, _T("����:"));
	sprintf_s(s3, "%d", count2);
	outtextxy(460, 10, s3);
	//�÷ֵ���ʾ
	outtextxy(10, 10, _T("�÷�:"));
	sprintf_s(s4, "%d", gameplayer2.score);
	outtextxy(68, 10, s4);
	//ʱ�����ʾ
	settextcolor(WHITE);
	outtextxy(213, 312, "����ʱ");
	end = clock();  		//ֹͣ��ʱ
	timeuse = ((double)end - start) / CLK_TCK;
	timeleft = timesum - timeuse;
	sprintf_s(stime, "%.2lf", timeleft);
	if (timeleft >= 10)
		outtextxy(215, 338, stime);
	else
		outtextxy(222, 338, stime);		//��������ʱ������ʾ��λ��

										//���һ��Բ
	setcolor(LIGHTBLUE);
	setfillcolor(LIGHTBLUE);
	fillcircle(gameplayer1.ball_x, gameplayer1.ball_y, gameplayer1.radius);

	//���һԲ�ϱ�ŵ���ʾ
	if (count1 < 10)
	{
		settextcolor(WHITE);
		settextstyle(20, 0, __T("����"));
		sprintf_s(s5, "%d", count1 + 1);
		outtextxy(gameplayer1.number1_x, gameplayer1.number1_y, s5);
	}

	if (count1 >= 10)
	{
		settextcolor(WHITE);
		settextstyle(20, 0, _T("����"));
		sprintf_s(s5, "%d", count1 + 1);
		outtextxy(gameplayer1.number2_x, gameplayer1.number2_y, s5);
	}


	//��Ҷ���Բ
	setcolor(BLUE);
	setfillcolor(BLUE);
	fillcircle(gameplayer2.ball_x, gameplayer2.ball_y, gameplayer2.radius);

	//��Ҷ�Բ�ϱ�ŵ���ʾ
	if (count2 < 10)
	{
		settextcolor(WHITE);
		settextstyle(20, 0, _T("����"));
		sprintf_s(s6, "%d", count2 + 1);
		outtextxy(gameplayer2.number1_x, gameplayer2.number1_y, s6);
	}

	if (count2 >= 10)
	{
		settextcolor(WHITE);
		settextstyle(20, 0, _T("����"));
		sprintf_s(s6, "%d", count2 + 1);
		outtextxy(gameplayer2.number2_x, gameplayer2.number2_y, s6);
	}

	//����flag�ı�ʶ����ʾ�����һ���״̬

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

	//����flag�ı�ʶ����ʾ����Ҷ����״̬

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


void updateWithoutInput()		//������ķ������û��޹صĸ���
{

	//���һ����
	double gameplayer1position_xmin;
	double gameplayer1position_xmax;
	double gameplayer1position_ymin;
	double gameplayer1position_ymax;	//�ж����Ƿ�ײ���ı�Ҫ����
	double a;	//�Ƕ�
	double sina;
	double cosa;

	//�Ƕ�
	sina = sin(gameplayer1.radius / (80 + gameplayer1.radius));
	cosa = sqrt(1 - sina * sina);
	//����ķ�Χ
	gameplayer1position_xmin = gameplayer1.ballnext_x[count1 - 1] - 2 * gameplayer1.radius * cosa;
	gameplayer1position_xmax = gameplayer1.ballnext_x[count1 - 1] + 2 * gameplayer1.radius * cosa;
	gameplayer1position_ymin = gameplayer1.ballnext_y[count1 - 1] - 2 * gameplayer1.radius * sina;
	gameplayer1position_ymax = gameplayer1.ballnext_y[count1 - 1] + 2 * gameplayer1.radius * sina;


	//��Ҷ�����
	double gameplayer2position_xmin;
	double gameplayer2position_xmax;
	double gameplayer2position_ymin;
	double gameplayer2position_ymax;	//�ж����Ƿ�ײ���ı�Ҫ����
	double b;	//�Ƕ�
	double sinb;
	double cosb;

	//�Ƕ�
	sinb = sin(gameplayer2.radius / (80 + gameplayer2.radius));
	cosb = sqrt(1 - sinb * sinb);
	//����ķ�Χ
	gameplayer2position_xmin = gameplayer2.ballnext_x[count2 - 1] - 2 * gameplayer2.radius * cosb;
	gameplayer2position_xmax = gameplayer2.ballnext_x[count2 - 1] + 2 * gameplayer2.radius * cosb;
	gameplayer2position_ymax = gameplayer2.ballnext_y[count2 - 1] + 2 * gameplayer2.radius * sinb;
	gameplayer2position_ymin = gameplayer2.ballnext_y[count2 - 1] - 2 * gameplayer2.radius * sinb;


	//���һ�����ƶ�
	if (move1 == movesuccess)
	{
		//���һ����ƶ�
		if (gameplayer1.ball_y > center_y + radius + length)	//������
		{
			//������ı仯
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
	//�������ֵı仯
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

	//�жϵ��벻�ƶ�ʱ������������³�ʼ��
	if (move1 == movestill)
	{
		gameplayer1.radius = 10;
		gameplayer1.ball_x = 245;
		gameplayer1.ball_y = 685;
		gameplayer1.ball_vx = 1;
		gameplayer1.ball_vy = 1;
		if (count1 < 10)
		{
			gameplayer1.number1_x = 240;	//��һ������������ֵĺ�����
			gameplayer1.number1_y = 675;	//��һ������������ֵ�������
		}
		else
		{
			gameplayer1.number2_x = 236;	//�ڶ�������������ֵĺ�����
			gameplayer1.number2_y = 675;	//�ڶ�������������ֵ�������
		}

	}


	//��Ҷ�����ƶ�
	if (move2 == movesuccess)
	{
		//��Ҷ�����ƶ�
		if (gameplayer2.ball_y < center_y - radius - length)	//������
		{
			//������ı仯
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

	//�������ֵı仯
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


	//�жϵ��벻�ƶ�ʱ������������³�ʼ��
	if (move2 == movestill)
	{

		gameplayer2.radius = 10;
		gameplayer2.ball_x = 245;
		gameplayer2.ball_y = 15;
		gameplayer2.ball_vx = 1;
		gameplayer2.ball_vy = 1;
		if (count2 < 10)
		{
			gameplayer2.number1_x = 240;		//��һ������������ֵĺ�����
			gameplayer2.number1_y = 5;		//��һ������������ֵ�������
		}
		else
		{
			gameplayer2.number2_x = 236;	//�ڶ�������������ֵĺ�����
			gameplayer2.number2_y = 5;	//�ڶ�������������ֵ�������
		}

	}

	//���һ����ת�Ĳ����仯
	//�ȸ���ǰcount1-1������
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

	//��Ҷ������ı仯
	//����ǰcount2-1������
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

	//�жϷ�������Ϊcount1-1���Ƿ�ײ��֮ǰ����


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
					//ײ��������������ʾ
					mciSendString(_T("stop failmusic"), NULL, 0, NULL);   // �Ȱ�ǰ��һ�ε�����ֹͣ
					mciSendString(_T("close failmusic"), NULL, 0, NULL); // �Ȱ�ǰ��һ�ε����ֹر�
					mciSendString(_T("open D:\\C program__Game\\�������\\������Ϸ�ز�\\������Ϸ�ز�\\9437.wav alias failmusic"), NULL, 0, NULL);//�򿪱�������
					mciSendString(_T("play failmusic"), NULL, 0, NULL);  // ������һ��

				}

			}

		}

		//�ж����һ�Ƿ�ײ����Ҷ�
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
					//ײ��������������ʾ
					mciSendString(_T("stop failmusic"), NULL, 0, NULL);   // �Ȱ�ǰ��һ�ε�����ֹͣ
					mciSendString(_T("close failmusic"), NULL, 0, NULL); // �Ȱ�ǰ��һ�ε����ֹر�
					mciSendString(_T("open D:\\C program__Game\\�������\\������Ϸ�ز�\\������Ϸ�ز�\\9437.wav alias failmusic"), NULL, 0, NULL);//�򿪱�������
					mciSendString(_T("play failmusic"), NULL, 0, NULL);  // ������һ��
				}
			}
		}
	}

	//�жϷ�������Ϊcount2-1���Ƿ�ײ��֮ǰ����

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
					//ײ��������������ʾ
					mciSendString(_T("stop failmusic"), NULL, 0, NULL);   // �Ȱ�ǰ��һ�ε�����ֹͣ
					mciSendString(_T("close failmusic"), NULL, 0, NULL); // �Ȱ�ǰ��һ�ε����ֹر�
					mciSendString(_T("open D:\\C program__Game\\�������\\������Ϸ�ز�\\������Ϸ�ز�\\9437.wav alias failmusic"), NULL, 0, NULL);//�򿪱�������
					mciSendString(_T("play failmusic"), NULL, 0, NULL);  // ������һ��
				}

			}

		}

		//�ж���Ҷ��Ƿ�ײ�����һ
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
					//ײ��������������ʾ
					mciSendString(_T("stop failmusic"), NULL, 0, NULL);   // �Ȱ�ǰ��һ�ε�����ֹͣ
					mciSendString(_T("close failmusic"), NULL, 0, NULL); // �Ȱ�ǰ��һ�ε����ֹر�
					mciSendString(_T("open D:\\C program__Game\\�������\\������Ϸ�ز�\\������Ϸ�ز�\\9437.wav alias failmusic"), NULL, 0, NULL);//�򿪱�������
					mciSendString(_T("play failmusic"), NULL, 0, NULL);  // ������һ��
				}
			}
		}
	}


	//�����count1-1����ûײ�ϣ�����������Ϸ����

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


	//�����Ϊcount2-1����ûײ�ϣ�����������Ϸ����

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
		endMenu();		//����ҳ��
	}

}

//�����ת����ײ�жϵ�ʵ�ִ���

void updateWithInput()
{
	char input;
	if (_kbhit())
	{
		input = _getch();
		//���һ�Ĳ���
		if (input == ' ')
		{
			move1 = movesuccess;
			count1++;		//��¼���1������ĸ���
			gameplayer1.score++;

			//���������
			mciSendString(_T("stop send1music"), NULL, 0, NULL);   // �Ȱ�ǰ��һ�ε�����ֹͣ
			mciSendString(_T("close send1music"), NULL, 0, NULL); // �Ȱ�ǰ��һ�ε����ֹر�
			mciSendString(_T("open D:\\C program__Game\\�������\\������Ϸ�ز�\\������Ϸ�ز�\\jump.mp3 alias send1music"), NULL, 0, NULL);//�򿪱�������
			mciSendString(_T("play send1music"), NULL, 0, NULL);  // ������һ��

		}
		//��Ϸ��ͣ����
		if (input == 27)
			gameStatus = 3;
		//��Ҷ��Ĳ���
		if (input == 's')
		{
			move2 = movesuccess;
			count2++;		//��¼��Ҷ�������ĸ���
			gameplayer2.score++;

			//���������
			mciSendString(_T("stop send2music"), NULL, 0, NULL);   // �Ȱ�ǰ��һ�ε�����ֹͣ
			mciSendString(_T("close send2music"), NULL, 0, NULL); // �Ȱ�ǰ��һ�ε����ֹر�
			mciSendString(_T("open D:\\C program__Game\\�������\\������Ϸ�ز�\\������Ϸ�ز�\\jump.mp3 alias send2music"), NULL, 0, NULL);//�򿪱�������
			mciSendString(_T("play send2music"), NULL, 0, NULL);  // ������һ��

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
