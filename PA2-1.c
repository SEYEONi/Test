#include<stdio.h>
#include<stdlib.h>

/*수정 불가*/
int block_length = 0; //레고 블록 문자열의 길이
int wall_length = 0; //성벽 문자열의 길이

char* block; //레고 블록의 문자열
char* wall; //성벽의 문자열

int answer_num = 0; //성벽을 구성하는 블럭의 최소 개수
int answer_height = 0; //성벽이 가질 수 있는 최소 높이


/*블록의 우선순위 저장*/
int* priority()
{
	int* pri = (int*)malloc(sizeof(int) * wall_length);

	for (int i = 0; i < wall_length; i++)
	{
		for (int j = 0; j < block_length; j++)
		{
			if (wall[i] == block[j])
			{
				pri[i] = j;
				break;
			}
		}
	}
	return pri;
}

/*블록 시작 위치를 저장*/
int* startmarking(int* pri)
{
	int* start = (int*)calloc(wall_length, sizeof(int)); //처음엔 0으로 초기화
	int max = block_length - 1; //가장 큰 우선순위 값
	//성벽에서 보이는 첫 문자의 위치는 1, 안보이는 첫 문자의 위치는 -1

	for (int i = 0; i < wall_length; i++)
	{
		if (pri[i] == 0) //첫 문자를 만나면 1 마킹
			start[i] = 1;
		else if (pri[i] == max) //마지막 문자를 만나면
		{
			if (start[i - block_length + 1] == 1)
				continue;
			else
				start[i - block_length + 1] = -1; //안보이므로 -1 마킹
		}
		else //첫문자도 아니고 마지막문자도 아니면
		{
			if (start[i - pri[i]] == 0)
				start[i - pri[i]] = -1;
			else
				continue;
		}
	}

	return start;
}

/*레고 개수 계산*/
int calNum(int* start)
{
	int count = 0;
	for (int i = 0; i < wall_length; i++)
	{
		if (start[i] != 0)
			count++;
		else
			continue;
	}
	return count;
}

/*성벽 높이 계산*/
int calHeight(int* start)
{
	int floor_now = 0; //현재 위치
	int ground_height = 0, under_height = 0; //시작은 모두 0

	int* ground[100] = { NULL, }; // 성벽높이<100
	int* under[100] = { NULL, }; //성벽높이<100

	//ground, under 모두 최대99층까지, 일단 0으로 초기화
	for (int i = 0; i < 100; i++)
		ground[i] = (int*)calloc(wall_length, sizeof(int));

	under[0] = ground[0];

	for (int i = 1; i < 100; i++)
		under[i] = (int*)calloc(wall_length, sizeof(int));

	//레고 쌓기
	for (int i = 0; i < wall_length; i++)
	{
		if (start[i] == 1)
		{
			if (floor_now < 0)
			{
				if (under[-floor_now][i] == 0) //현재 층의 해당 위치가 비어있으면 레고 쌓을 수 있음
				{
					for (int j = i; j < i + block_length; j++)
						under[-floor_now][j] = 1;
				}
				else
				{
					floor_now++; //비어있지 않으면 한 층 올라가서 블럭쌓기
					for (int j = i; j < i + block_length; j++)
						under[-floor_now][j] = 1;
				}
			}
			else //floor_now >= 0
			{
				if (ground[floor_now][i] == 0)
				{
					for (int j = i; j < i + block_length; j++)
						ground[floor_now][j] = 1;
				}
				else
				{
					floor_now++;
					for (int j = i; j < i + block_length; j++)
						ground[floor_now][j] = 1;

					if (floor_now > ground_height)
						ground_height = floor_now; //높이가 추가되는 경우
				}
			}
		}

		else if (start[i] == -1)
		{
			floor_now--;
			for (floor_now; floor_now >= 0; floor_now--)
			{
				if (ground[floor_now][i] == 0)
				{
					for (int j = i; j < i + block_length; j++)
						ground[floor_now][j] = 1;
					break;
				}

				if (floor_now == 0) //0인데 break못했다면
				{
					floor_now = -1; //under
					break;
				}
			}

			if (floor_now < 0)
			{
				for (floor_now; floor_now > -100; floor_now--)
				{
					if (under[-floor_now][i] == 0)
					{
						for (int j = i; j < i + block_length; j++)
							under[-floor_now][j] = 1;
						break;
					}
				}
				if ((-floor_now) > under_height)
					under_height = (-floor_now);
			}
		}
		else
			continue;
	}

	return under_height + (ground_height + 1);
}

void calculator() {
	//answer_num 과 answer_height를 구하는 코드를 작성해주세요
	int* pri = priority(); //우선순위 구하기
	int* start = startmarking(pri); //시작위치 표시

	answer_num = calNum(start);
	answer_height = calHeight(start);
}

/*수정 불가*/
int main()
{
	scanf("%d", &block_length);
	scanf("%d", &wall_length);

	block = malloc(sizeof(char) * block_length+1); //문자열을 가리키게 되니까 널문자까지
	wall = malloc(sizeof(char) * wall_length+1);
	scanf("%s", block);
	scanf("%s", wall);

	calculator();

	printf("%d\n", answer_num);
	printf("%d\n", answer_height);

	return 0;

}


