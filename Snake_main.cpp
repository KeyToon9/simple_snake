/*
	The Snake by keytoon
	2019/4/24
	a small exercise
*/

#include "snake.h"
#include <cstdio>
#include <Windows.h>
#include <conio.h>
#include <time.h>

//全局设置
#define MAP_HEIGHT 20  //地图行数
#define MAP_WIDTH  40  //地图列数
#define UP         'w'
#define DOWN       's'
#define LEFT       'a'
#define RIGHT      'd'

char   keyHit = 'w';    //键盘输入
int    SCORE  = 0;      //分数
int    SPEED  = 500;    //速度
STATE  state  = NORMAL; //状态
pFood  food   = NULL;   //食物
pSnake head   = NULL;   //蛇头

void gotoPos(int, int);    //控制台光标控制
void gotoDraw(int, int);   //绘制元素
void gotoDelete(int, int); //删除元素(填充空白)
int keyboard_IN();         //键盘信息输入
void creatMap();           //创建地图
void creatFood();          //刷新食物
void printSnake();         //打印蛇
int initSnake();           //初始化蛇
int IsTouchWall();         //是否撞到墙
int IsTouchSelf(pSnake);   //是否咬到自己
int IsFood(pSnake);        //是否吃到食物
void moveSnake();          //蛇的移动
void StartGame();          //开始游戏 初始化
void RunGame();            //游戏运行
void endGame();            //结束游戏

int main()
{
	StartGame();
	RunGame();

	//initSnake();

	return 0;
}

//控制台光标控制
void gotoPos(int x, int y)
{
	COORD pos;
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	pos.X = x;
	pos.Y = y;

	SetConsoleCursorPosition(hOut, pos);
}

//绘制元素
void gotoDraw(int x, int y)
{
	gotoPos(x, y);
	printf("■");
}

//删除元素(填充空白)
void gotoDelete(int x, int y)
{
	gotoPos(x, y);
	printf(" ");
}

//键盘信息输入
int keyboard_IN()
{
	if (state != NORMAL) return 0;
	if (_kbhit())
	{
		keyHit = _getch();
	}
	return 1;
}

//初始化地图
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

//刷新食物
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

//打印蛇
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

//初始化蛇
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

//是否撞到墙
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

//是否咬到自己
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

//是否吃到食物
int IsFood(pSnake p)
{
	if (p == NULL || food == NULL) return -1;
	if (p->x == food->x && p->y == food->y) return 1;
	return 0;
}


//蛇的移动
void moveSnake()
{
	pSnake temp = head;
	pSnake newHead = (pSnake)malloc(sizeof(SNAKE));

	if (head == NULL || newHead == NULL) return;

	//刷新
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
	//事件
	if (IsFood(newHead))
	{
		head = newHead;
		printSnake();
		SCORE++;
		SPEED = SPEED == 100 ? 100 : 500 - (SCORE / 5) * 100;
		gotoPos(22, 21);
		printf("%d", SCORE);
		//食物刷新
		free(food);
		food = NULL;
		creatFood();
		//如果分数大于一定值，游戏通关

	}
	else
	{
		if (!IsTouchWall() && !IsTouchSelf(newHead))
		{
			temp = NULL;
			head = newHead;
			temp = head;
			//删除蛇尾并打印
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

//开始游戏 初始化
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
		printf("操作 上 w 下 s 左 a 右 d (小写)");
	}
	else
	{
		printf("ERROR!");
	}
}

//游戏运行
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

//结束游戏
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
	printf("菜");
	gotoPos(0, 22);
	Sleep(2000);
}
