#ifndef __SNAKE_H__
#define __SNAKE_H__

//蛇的数据结构
typedef struct SNAKE
{
	int x;     //行
	int y;     //列
	//int len;   //当前长度
	struct SNAKE* next;
}*pSnake;

//食物的数据结构
typedef struct FOOD
{
	int x;
	int y;
}*pFood;

//蛇的状态
enum STATE
{
	TOUCH_SELF, //咬到自己
	TOUCH_WALL, //碰到墙壁
	NORMAL,     //正常状态
	SUCCESS     //通关状态
};

#endif // __SNAKE_H__
