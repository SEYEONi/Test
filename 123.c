#include <stdio.h>
#include <malloc.h>
#include <string.h> 

typedef struct _Node Node;  
typedef struct _Node * Link;   //Link�� ����� ��ġ ����


struct _Node
{
    int num;                 //��
    int seat;                 //10�� seat��
    Link after;              //���� ����� ��ġ ����
    Link before;           //���� ����� ��ġ ����
};

 

typedef struct _List List;  
struct _List
{
    Link head;              //����Ʈ�� �� ó�� ��� - ���⼭�� ���� �����
    Link tail;                 //����Ʈ�� �� ������ ��� - ���⼭�� ���� �����
};


Link NewNode(int num,int seat)  //��� ���� �Լ�
{
    Link now = (Link)malloc(sizeof(Node));  //��带 �Ҵ�
    now->num = num;                                //num �Ӽ� ����
    now->seat = seat;                               //seat �Ӽ� ����
    now->after = now->before = NULL;        //���� �Ҵ��� ����� ���� ���� ���� ����� ��ġ ������ NULL�� ����
    return now;                                         //������ ��� ��ġ���� ��ȯ
}


void DeleteNode(Link now)                       //��� �Ҹ� �Լ�
{
    free(now);                                          //�Ҵ��� �޸� ����
}

 

//���� ��ȿ�� ���� head�� tail���̿� �����ϰ� ��

List *NewList()                                        //����Ʈ ���� �Լ�(���� ��� �ִ� ���� ���� ����Ʈ)
{
    List *now = (List *)malloc(sizeof(List));  //����Ʈ�� �Ҵ�
    now->head = NewNode(0,-1);               //���� ��带 �����Ͽ� head�� ����
    now->tail = NewNode(0,-1);                  //���� ��带 �����Ͽ� tail�� ����
    now->head->after = now->tail;             //head�� ���� ���� tail�� ����
    now->tail->before = now->head;          //tail�� ���� ���� head�� ����

    return now;                                         //������ ����Ʈ ��ȯ
}


void DeleteList(List *now)                        //����Ʈ ���� �Լ�
{
    while(now->head != now->tail)            //head�� tail�� �ƴ϶��
    {
        now->head = now->head->after;      //head�� ���� ��� ��ġ�� �ٲٰ�
        DeleteNode(now->head->before);     //head ���� ��带 ����
    }
    DeleteNode(now->head);                     //head�� ����(tail�̱⵵ ��)
    free(now);                                          //now ����
}

 

void InsertNode(Link seat,Link now)    //seat ��� �տ� now�� �Ŵٴ� �Լ�
{
    now->before = seat->before;          
    now->after = seat;
    seat->before->after = now;
    seat->before = now;
}


void Push(List *list,Link now)           //now�� list �� �ڿ� �Ŵٴ� �Լ�
{
    InsertNode(list->tail,now); 
}


//�߸��� �Է��� ���� �ʴ´ٴ� �������� ������ - �� �� �ŷڼ� �ְ� ������ ������.

List *MakeList(const char *strnum) //"12345"�� ���� ���� ���ڿ��� ���� �̸� ����Ʈ�� ����� ��ȯ�ϴ� �Լ�
{
    int seat = strlen(strnum);            //���ڿ� ���̸� ���� - �ڸ����� ������
    List *now = NewList();               //����Ʈ�� ����

    while(seat)                               //������ �ڸ����� �ݺ�
    {
        seat--;                                 //�ڸ����� ������ n��° �ڸ��� 10�� n-1���̹Ƿ�
        if(*strnum-'0')                       //���� ���� '0'�� �ƴ϶��
        {
            Push(now,NewNode(*strnum-'0',seat)); //��带 �����Ͽ� now����Ʈ �� �ڿ� �Ŵܴ�.
        }
        strnum++;                             //���� ���� ��ġ�� �̵�
    }
    return now;                              //����Ʈ�� ��ȯ
}

 

Link FindSeat(List *f,int seat)         //����Ʈ f ���� �Է����� seat�� ���ų� ���� ��ũ ã��
{
    Link now = f->head->after;       //head�� ���̳���̹Ƿ� head->after���� ã��
    while((now!=f->tail) &&(now->seat >seat)) //�� �ڿ� �԰ų� ���� now->seat>seat�̶�� 
    {
        now = now->after;  //now�� ���� ��ũ�� ����
    }
    return now; //now�� ��ȯ, now�� �ڸ��� seat�̰ų� tailȤ�� seat���� ����
}

 

void AddNode(List *list,int num,int seat) //list�� seat�� ã�� num�� ���ϴ� �Լ� 
{
    Link find = FindSeat(list,seat);  //list���� seat�� ���� ��ũ ã��
    if((find == list->tail)||(find->seat != seat)) //���� �� ã�Ҵٸ�
    {
        InsertNode(find,NewNode(num,seat)); //��带 �����ؼ� find�տ� �Ŵܴ�. - seat������ �Ŵް� ��
    }
    else  //ã�Ҵٸ�
    {
        int carry = 0;
        carry = (find->num + num)/10;       //����� ���� �Է� ������ ���� 10���� ���� ���� carry��

        if(carry)                                      //carry�� �ִٸ�
        {
            AddNode(list,carry,seat+1);      //list�� seat+1�ڸ��� carry�� ����
        }
        find->num = (find->num + num)%10;  //find->num�� �Է� ���ڸ� �ݿ���
    }
}

 

List *AddList(List *a,List *b)  //����Ʈ a�� ����Ʈ b�� ���� ����Ʈ�� �����Ͽ� ��ȯ�ϴ� �Լ�
{
    List *list = NewList();              //���ο� ����Ʈ�� �����Ѵ�.
    Link nowa = a->tail->before;   //�� �ڸ����� ���� ���� ��, nowa �� tail�� before�� ����(tail�� ���̳��)
    Link nowb = b->tail->before;   //�� �ڸ����� ���� ���� ��, nowa �� tail�� before�� ����(tail�� ���̳��)
       

    while((nowa != a->head)||(nowb != b->head))  //�� ����Ʈ�� ��� ��忡 ���� ������ ������ �ʾҴٸ�
    {
        if(nowa->seat>nowb->seat)        //���� nowa�� �ڸ��� nowb�� �ڸ����� ũ�ٸ�
        {
            AddNode(list,nowa->num,nowa->seat);  // nowa�� list�� ���ض�
            nowa = nowa->before;                           //nowa�� ����
        }
        else if(nowa->seat<nowb->seat) //���� nowb�� �ڸ��� nowa�� �ڸ����� ũ�ٸ�
        {
            AddNode(list,nowb->num,nowb->seat);  //nowb�� list�� ���ض�
            nowb = nowb->before;                          //nowb�� ����
        }
        else //nowa�� nowb�� �ڸ��� ���ٸ�
        {
            AddNode(list,nowa->num,nowa->seat);   //����Ʈ�� nowa�� ���ض�
            AddNode(list,nowb->num,nowb->seat);   //����Ʈ�� nowb�� ���ض�
            nowa = nowa->before;                           //nowa�� ����
            nowb = nowb->before;                           //nowb�� ����
        }
    }

    return list;                                                     //���� ��� list�� ��ȯ
}


List *MulList(List *a,List *b)     //����Ʈ a�� ����Ʈ b�� ���� ����Ʈ�� �����Ͽ� ��ȯ�ϴ� �Լ�
{
    List *list = NewList();          //���ο� ����Ʈ�� ����
    Link nowa = a->head->after;   //������ �� �ڸ����� ���س����� ������ �غ���. ��, nowa �� head�� after�� ����
    Link nowb = b->head->after;   //������ �� �ڸ����� ���س����� ������ �غ���. ��, nowb �� head�� after�� ����
    int seat = 0;
    int val = 0;
    int carry = 0;
 
    while(nowa != a->tail)              //nowa�� ���� ������ ������ �ݺ�
    {
        carry = 0;
        nowb = b->head->after;       //������ �� �ڸ����� ���س����� ������ �غ���. ��, nowb �� head�� after�� ����
        while(nowb != b->tail)         //nowb�� ���� ������ ������ �ݺ�
        {
            val = nowa->num * nowb->num;   //nowa�� nowb�� �� ���� ����
            carry = val / 10;                           //���� ����� 10���� ���� ���� carry��
            val = val % 10;                             //���� ����� 10���� ���� �������� val��
            seat = nowa->seat + nowb->seat ; //nowa�� nowb�� �ڸ��� ����(10�� a�� * 10�� b���� 10�� a+b��)

            if(carry)                                       //carry�� �ִٸ�
            {
                AddNode(list,carry,seat+1);        //list�� carry�� seat+1�ڸ��� ����
            }

            AddNode(list,val,seat);                  //list�� val�� seat�ڶ� ����
            nowb = nowb->after;                    //nowb�� ���� ��ũ�� �̵�
        }
        nowa = nowa->after;                        //nowa�� ���� ��ũ�� �̵�
    }
    return list;                                           //��� ����Ʈ ��ȯ
}


void Print(List *c)
{
    Link now = c->head->after;
    int gap;
 
    while(now!=c->tail)
    {
        putchar(now->num+'0');
        gap = now->seat - now->after->seat-1;
        while(gap)
        {
            putchar('0');
            gap--;

        }
        now = now->after;
    }
}

 

void DemoAddList(List *a,List *b)
{
    List *c = AddList(a,b);
    Print(a);
    putchar('+');
    Print(b);
    putchar('=');
    Print(c);
    printf("\n");
    DeleteList(c);
}
void DemoMulList(List *a,List *b)
{
    List *d = MulList(a,b);
    Print(a);
    putchar('*');
    Print(b);
    putchar('=');
    Print(d);
    printf("\n");
    DeleteList(d);
}


int main()
{
    List *a = MakeList("99999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999");
    List *b = MakeList("121212222222222222222222111111111111111111111111111111111111111111111111111111111111112222222222222111111111111111");

    DemoAddList(a,b);
    DemoMulList(a,b);
    DeleteList(a);
    DeleteList(b);

    return 0;
}
