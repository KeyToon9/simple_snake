#ifndef __SNAKE_H__
#define __SNAKE_H__

//�ߵ����ݽṹ
typedef struct SNAKE
{
	int x;     //��
	int y;     //��
	//int len;   //��ǰ����
	struct SNAKE* next;
}*pSnake;

//ʳ������ݽṹ
typedef struct FOOD
{
	int x;
	int y;
}*pFood;

//�ߵ�״̬
enum STATE
{
	TOUCH_SELF, //ҧ���Լ�
	TOUCH_WALL, //����ǽ��
	NORMAL,     //����״̬
	SUCCESS     //ͨ��״̬
};

#endif // __SNAKE_H__
