#include <iostream>
using namespace std;
 
/*循环队列的类型定义*/
const int Queue_Size=k;
 
typedef struct circlQueue
{
       char *elem;
       int rear;
       int front;
       int queueSize;
}circlQueue;
 
/*初始化*/
void initQueue_C(circlQueue &Q)
{
       Q.elem=new char[Queue_Size];
       Q.front=Q.rear=0;//首尾指针相等说明队列为空。
       Q.queueSize=Queue_Size;
}
/*销毁队列*/
void destroyQueue_C(circlQueue &Q)
{
       delete []Q.elem;
       Q.front=Q.rear=0;
       Q.queueSize=0;
}
 /*求队列的长度*/
int  lengthQueue_C(circlQueue &Q)
{
       int length;
       length=(Q.rear-Q.front+Q.queueSize)%Q.queueSize;/*一般情况下，rear在front的上方，此种算法是用于
  rear已到front的下方，即已出现假溢出的情况。*/
       return length;
}
 
 
/*入队列*/
void enterQueue_C(circlQueue &Q,char x)
{
       if(((Q.rear+1)%Q.queueSize)==Q.front)//判断栈满的情况
              cout<<"Queue OverFlow!";
       Q.elem[Q.rear]=x;
       Q.rear=(Q.rear+1)%Queue_Size;//尾指针应以此种方式加1，才会实现循环队列。
}
 
/*出队列*/
char outputQueue_C(circlQueue &Q)
{
       char e;
       if(Q.rear==Q.front)
              cout<<"Queue Empty";
       e=Q.elem[Q.front];
       Q.front=(Q.front+1)%Q.queueSize;;//头指针应以此种方式加1，才会实现循环队列。
       return e;
}
