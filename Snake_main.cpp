/*
	The Snake by keytoon
	2019/4/24
	a small exercise
*/

#include "sanke.h"
#include <cstdio>
#include <Windows.h>
#include <conio.h>
#include <time.h>

//ȫ������
#define MAP_HEIGHT 20  //��ͼ����
#define MAP_WIDTH  40  //��ͼ����
#define UP         'w'
#define DOWN       's'
#define LEFT       'a'
#define RIGHT      'd'

char   keyHit = 'w';    //��������
int    SCORE  = 0;      //����
int    SPEED  = 500;    //�ٶ�
STATE  state  = NORMAL; //״̬
pFood  food   = NULL;   //ʳ��
pSnake head   = NULL;   //��ͷ

void gotoPos(int, int);    //����̨������
void gotoDraw(int, int);   //����Ԫ��
void gotoDelete(int, int); //ɾ��Ԫ��(���հ�)
int keyboard_IN();         //������Ϣ����
void creatMap();           //������ͼ
void creatFood();          //ˢ��ʳ��
void printSnake();         //��ӡ��
int initSnake();           //��ʼ����
int IsTouchWall();         //�Ƿ�ײ��ǽ
int IsTouchSelf(pSnake);   //�Ƿ�ҧ���Լ�
int IsFood(pSnake);        //�Ƿ�Ե�ʳ��
void moveSnake();          //�ߵ��ƶ�
void StartGame();          //��ʼ��Ϸ ��ʼ��
void RunGame();            //��Ϸ����
void endGame();            //������Ϸ

int main()
{
	StartGame();
	RunGame();

	//initSnake();

	return 0;
}

//����̨������
void gotoPos(int x, int y)
{
	COORD pos;
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	pos.X = x;
	pos.Y = y;

	SetConsoleCursorPosition(hOut, pos);
}

//����Ԫ��
void gotoDraw(int x, int y)
{
	gotoPos(x, y);
	printf("��");
}

//ɾ��Ԫ��(���հ�)
void gotoDelete(int x, int y)
{
	gotoPos(x, y);
	printf(" ");
}

//������Ϣ����
int keyboard_IN()
{
	if (state != NORMAL) return 0;
	if (_kbhit())
	{
		keyHit = _getch();
	}
	return 1;
}

//��ʼ����ͼ
void creatMap()
{
	int i = 0;
	for (i = 0; i < MAP_WIDTH; i += 2)
	{
		gotoDraw(i, 0);
		gotoDraw(i, MAP_HEIGHT - 1);
	}
	for (i = 0; i < MAP_HEIGHT; i++)
	{
		gotoDraw(0, i);
		gotoDraw(MAP_WIDTH - 2, i);
	}
}

//ˢ��ʳ��
void creatFood()
{
	pSnake temp = head;
	food = (pFood)malloc(sizeof(FOOD));
	if (head == NULL || food == NULL) return;
	srand((unsigned)time(NULL));
	food->x = rand() % (MAP_WIDTH - 4 - 2 + 1) + 2;
	food->y = rand() % (MAP_HEIGHT - 3 - 2 + 2) + 2;
	if (food->x % 2 != 0)
	{
		food->x = food->x + 1;
	}

	while (temp != NULL)
	{
		if (temp->x == food->x && temp->y == food->y)
		{
			free(food);
			food = NULL;
			creatFood();
			return;
		}
		temp = temp->next;
	}

	gotoPos(food->x, food->y);
	printf("@");
}

//��ӡ��
void printSnake()
{
	pSnake temp = head;
	if (temp == NULL) return;
	while (temp)
	{
		gotoDraw(temp->x, temp->y);
		temp = temp->next;
	}
}

//��ʼ����
int initSnake()
{
	int initNum = 3;
	pSnake temp;

	head = (pSnake)malloc(sizeof(SNAKE));
	if (head == NULL) return 0;
	head->x = 20;
	head->y = 10;
	head->next = NULL;

	temp = head;
	for (int i = 1; i < initNum; i++)
	{
		pSnake newNode = (pSnake)malloc(sizeof(SNAKE));
		if (newNode == NULL) return 0;
		newNode->x = 20;
		newNode->y = 10 + i;
		newNode->next = NULL;

		temp->next = newNode;
		temp = temp->next;
	}

	printSnake();
	return 1;
}

//�Ƿ�ײ��ǽ
int IsTouchWall()
{
	if (head->x <= 1 || head->x >= MAP_WIDTH - 2 ||
		head->y <= 1 || head->y >= MAP_HEIGHT - 1)
	{
		state = TOUCH_WALL;
		return 1;
	}
	return 0;
}

//�Ƿ�ҧ���Լ�
int IsTouchSelf(pSnake p)
{
	pSnake temp = head;
	if (p == NULL || temp == NULL)
	{
		return -1;
	}
	while (temp)
	{
		if (temp->x == p->x && temp->y == p->y)
		{
			state = TOUCH_SELF;
			return 1;
		}
		temp = temp->next;
	}
	return 0;
}

//�Ƿ�Ե�ʳ��
int IsFood(pSnake p)
{
	if (p == NULL || food == NULL) return -1;
	if (p->x == food->x && p->y == food->y) return 1;
	return 0;
}


//�ߵ��ƶ�
void moveSnake()
{
	pSnake temp = head;
	pSnake newHead = (pSnake)malloc(sizeof(SNAKE));

	if (head == NULL || newHead == NULL) return;

	//ˢ��
	while (temp->next)
	{
		gotoDelete(temp->x, temp->y);
		temp = temp->next;
	}

	switch (keyHit)
	{
	case UP:
		newHead->y = head->y - 1;
		newHead->x = head->x;
		newHead->next = head;
		break;
	case DOWN:
		newHead->y = head->y + 1;
		newHead->x = head->x;
		newHead->next = head;
		break;
	case LEFT:
		newHead->x = head->x - 2;
		newHead->y = head->y;
		newHead->next = head;
		break;
	case RIGHT:
		newHead->x = head->x + 2;
		newHead->y = head->y;
		newHead->next = head;
		break;
	default:
		newHead->y = head->y - 1;
		newHead->x = head->x;
		newHead->next = head;
		break;
	}
	//�¼�
	if (IsFood(newHead))
	{
		head = newHead;
		printSnake();
		SCORE++;
		SPEED = SPEED == 100 ? 100 : 500 - (SCORE / 5) * 100;
		gotoPos(22, 21);
		printf("%d", SCORE);
		//ʳ��ˢ��
		free(food);
		food = NULL;
		creatFood();
		//�����������һ��ֵ����Ϸͨ��

	}
	else
	{
		if (!IsTouchWall() && !IsTouchSelf(newHead))
		{
			temp = NULL;
			head = newHead;
			temp = head;
			//ɾ����β����ӡ
			while (temp->next->next != NULL)
			{
				gotoDraw(temp->x, temp->y);
				temp = temp->next;
			}
			gotoDraw(temp->x, temp->y);
			gotoDelete(temp->next->x, temp->next->y);
			free(temp->next);
			temp->next = NULL;
		}
		else
		{
			free(newHead);
			newHead = NULL;
		}
	}
}

//��ʼ��Ϸ ��ʼ��
void StartGame()
{
	if (initSnake())
	{
		system("color 07");
		creatMap();
		creatFood();
		gotoPos(14, 21);
		printf("Socre:");
		gotoPos(22, 21);
		printf("%d", SCORE);
		gotoPos(8, 22);
		printf("���� �� w �� s �� a �� d (Сд)");
	}
	else
	{
		printf("ERROR!");
	}
}

//��Ϸ����
void RunGame()
{
	while (1)
	{
		keyboard_IN();
		moveSnake();
		if (state != NORMAL)
		{
			endGame();
			break;
		}
		Sleep(SPEED);
	}
}

//������Ϸ
void endGame()
{
	pSnake temp = head;
	while (temp)
	{
		pSnake del = temp;
		temp = temp->next;
		free(del);
		del = NULL;
	}
	if (food != NULL)
	{
		free(food);
		food = NULL;
	}
	SCORE = 0;
	SPEED = 500;
	system("color C4");
	gotoPos(18, 10);
	printf("��");
	gotoPos(0, 22);
	Sleep(2000);
}
