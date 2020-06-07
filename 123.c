#include <stdio.h>
#include <malloc.h>
#include <string.h> 

typedef struct _Node Node;  
typedef struct _Node * Link;   //Link는 노드의 위치 정보


struct _Node
{
    int num;                 //수
    int seat;                 //10의 seat승
    Link after;              //다음 노드의 위치 정보
    Link before;           //이전 노드의 위치 정보
};

 

typedef struct _List List;  
struct _List
{
    Link head;              //리스트의 맨 처음 노드 - 여기서는 더미 노드임
    Link tail;                 //리스트의 맨 마지막 노드 - 여기서는 더미 노드임
};


Link NewNode(int num,int seat)  //노드 생성 함수
{
    Link now = (Link)malloc(sizeof(Node));  //노드를 할당
    now->num = num;                                //num 속성 대입
    now->seat = seat;                               //seat 속성 대입
    now->after = now->before = NULL;        //새로 할당한 노드의 이전 노드와 다음 노드의 위치 정보를 NULL로 설정
    return now;                                         //생성한 노드 위치정보 반환
}


void DeleteNode(Link now)                       //노드 소멸 함수
{
    free(now);                                          //할당한 메모리 해제
}

 

//실제 유효한 노드는 head와 tail사이에 존재하게 됨

List *NewList()                                        //리스트 생성 함수(더미 노드 있는 이중 연결 리스트)
{
    List *now = (List *)malloc(sizeof(List));  //리스트를 할당
    now->head = NewNode(0,-1);               //더미 노드를 생성하여 head에 대입
    now->tail = NewNode(0,-1);                  //더미 노드를 생성하여 tail에 대입
    now->head->after = now->tail;             //head의 다음 노드는 tail로 설정
    now->tail->before = now->head;          //tail의 이전 노드는 head로 설정

    return now;                                         //생성한 리스트 반환
}


void DeleteList(List *now)                        //리스트 해제 함수
{
    while(now->head != now->tail)            //head가 tail이 아니라면
    {
        now->head = now->head->after;      //head는 다음 노드 위치로 바꾸고
        DeleteNode(now->head->before);     //head 이전 노드를 제거
    }
    DeleteNode(now->head);                     //head를 제거(tail이기도 함)
    free(now);                                          //now 해제
}

 

void InsertNode(Link seat,Link now)    //seat 노드 앞에 now를 매다는 함수
{
    now->before = seat->before;          
    now->after = seat;
    seat->before->after = now;
    seat->before = now;
}


void Push(List *list,Link now)           //now를 list 맨 뒤에 매다는 함수
{
    InsertNode(list->tail,now); 
}


//잘못된 입력이 오지 않는다는 가정에서 동작함 - 좀 더 신뢰성 있게 수정해 보세요.

List *MakeList(const char *strnum) //"12345"와 같이 숫자 문자열이 오면 이를 리스트로 만들어 반환하는 함수
{
    int seat = strlen(strnum);            //문자열 길이를 구함 - 자리수와 동일함
    List *now = NewList();               //리스트를 생성

    while(seat)                               //마지막 자리까지 반복
    {
        seat--;                                 //자리수를 감소함 n번째 자리는 10의 n-1승이므로
        if(*strnum-'0')                       //숫자 문자 '0'이 아니라면
        {
            Push(now,NewNode(*strnum-'0',seat)); //노드를 생성하여 now리스트 맨 뒤에 매단다.
        }
        strnum++;                             //다음 문자 위치로 이동
    }
    return now;                              //리스트를 반환
}

 

Link FindSeat(List *f,int seat)         //리스트 f 에서 입력인자 seat과 같거나 작은 링크 찾음
{
    Link now = f->head->after;       //head는 더미노드이므로 head->after부터 찾음
    while((now!=f->tail) &&(now->seat >seat)) //맨 뒤에 왔거나 아직 now->seat>seat이라면 
    {
        now = now->after;  //now를 다음 링크로 변경
    }
    return now; //now를 반환, now는 자리가 seat이거나 tail혹은 seat보다 작음
}

 

void AddNode(List *list,int num,int seat) //list에 seat을 찾아 num을 더하는 함수 
{
    Link find = FindSeat(list,seat);  //list에서 seat과 같은 링크 찾음
    if((find == list->tail)||(find->seat != seat)) //만약 못 찾았다면
    {
        InsertNode(find,NewNode(num,seat)); //노드를 생성해서 find앞에 매단다. - seat순으로 매달게 됨
    }
    else  //찾았다면
    {
        int carry = 0;
        carry = (find->num + num)/10;       //노드의 수와 입력 인자의 수를 10으로 나눈 값은 carry임

        if(carry)                                      //carry가 있다면
        {
            AddNode(list,carry,seat+1);      //list의 seat+1자리에 carry를 더함
        }
        find->num = (find->num + num)%10;  //find->num을 입력 인자를 반영함
    }
}

 

List *AddList(List *a,List *b)  //리스트 a와 리스트 b를 더한 리스트를 생성하여 반환하는 함수
{
    List *list = NewList();              //새로운 리스트를 생성한다.
    Link nowa = a->tail->before;   //뒷 자리부터 더할 것임 즉, nowa 를 tail의 before로 배정(tail은 더미노드)
    Link nowb = b->tail->before;   //뒷 자리부터 더할 것임 즉, nowa 를 tail의 before로 배정(tail은 더미노드)
       

    while((nowa != a->head)||(nowb != b->head))  //두 리스트의 모든 노드에 대한 연산이 끝나지 않았다면
    {
        if(nowa->seat>nowb->seat)        //만약 nowa의 자리가 nowb의 자리보다 크다면
        {
            AddNode(list,nowa->num,nowa->seat);  // nowa를 list에 더해라
            nowa = nowa->before;                           //nowa를 변경
        }
        else if(nowa->seat<nowb->seat) //만약 nowb의 자리가 nowa의 자리보다 크다면
        {
            AddNode(list,nowb->num,nowb->seat);  //nowb를 list에 더해라
            nowb = nowb->before;                          //nowb를 변경
        }
        else //nowa와 nowb의 자리가 같다면
        {
            AddNode(list,nowa->num,nowa->seat);   //리스트에 nowa를 더해라
            AddNode(list,nowb->num,nowb->seat);   //리스트에 nowb를 더해라
            nowa = nowa->before;                           //nowa를 변경
            nowb = nowb->before;                           //nowb를 변경
        }
    }

    return list;                                                     //더한 결과 list를 반환
}


List *MulList(List *a,List *b)     //리스트 a와 리스트 b를 곱한 리스트를 생성하여 반환하는 함수
{
    List *list = NewList();          //새로운 리스트를 생성
    Link nowa = a->head->after;   //곱셈은 앞 자리부터 곱해나가는 것으로 해보자. 즉, nowa 를 head의 after로 배정
    Link nowb = b->head->after;   //곱셈은 앞 자리부터 곱해나가는 것으로 해보자. 즉, nowb 를 head의 after로 배정
    int seat = 0;
    int val = 0;
    int carry = 0;
 
    while(nowa != a->tail)              //nowa가 끝을 만나기 전까지 반복
    {
        carry = 0;
        nowb = b->head->after;       //곱셈은 앞 자리부터 곱해나가는 것으로 해보자. 즉, nowb 를 head의 after로 배정
        while(nowb != b->tail)         //nowb가 끝을 만나기 전까지 반복
        {
            val = nowa->num * nowb->num;   //nowa와 nowb의 두 수를 곱합
            carry = val / 10;                           //곱한 결과를 10으로 나눈 몫은 carry임
            val = val % 10;                             //곱한 결과를 10으로 나눈 나머지는 val임
            seat = nowa->seat + nowb->seat ; //nowa와 nowb의 자리를 더함(10의 a승 * 10의 b승은 10의 a+b승)

            if(carry)                                       //carry가 있다면
            {
                AddNode(list,carry,seat+1);        //list에 carry를 seat+1자리에 더함
            }

            AddNode(list,val,seat);                  //list에 val을 seat자라에 더함
            nowb = nowb->after;                    //nowb는 다음 링크로 이동
        }
        nowa = nowa->after;                        //nowa는 다음 링크로 이동
    }
    return list;                                           //결과 리스트 반환
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
