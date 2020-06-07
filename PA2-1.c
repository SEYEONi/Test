#include<stdio.h>
#include<stdlib.h>

/*���� �Ұ�*/
int block_length = 0; //���� ��� ���ڿ��� ����
int wall_length = 0; //���� ���ڿ��� ����

char* block; //���� ����� ���ڿ�
char* wall; //������ ���ڿ�

int answer_num = 0; //������ �����ϴ� ���� �ּ� ����
int answer_height = 0; //������ ���� �� �ִ� �ּ� ����


/*����� �켱���� ����*/
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

/*��� ���� ��ġ�� ����*/
int* startmarking(int* pri)
{
	int* start = (int*)calloc(wall_length, sizeof(int)); //ó���� 0���� �ʱ�ȭ
	int max = block_length - 1; //���� ū �켱���� ��
	//�������� ���̴� ù ������ ��ġ�� 1, �Ⱥ��̴� ù ������ ��ġ�� -1

	for (int i = 0; i < wall_length; i++)
	{
		if (pri[i] == 0) //ù ���ڸ� ������ 1 ��ŷ
			start[i] = 1;
		else if (pri[i] == max) //������ ���ڸ� ������
		{
			if (start[i - block_length + 1] == 1)
				continue;
			else
				start[i - block_length + 1] = -1; //�Ⱥ��̹Ƿ� -1 ��ŷ
		}
		else //ù���ڵ� �ƴϰ� ���������ڵ� �ƴϸ�
		{
			if (start[i - pri[i]] == 0)
				start[i - pri[i]] = -1;
			else
				continue;
		}
	}

	return start;
}

/*���� ���� ���*/
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

/*���� ���� ���*/
int calHeight(int* start)
{
	int floor_now = 0; //���� ��ġ
	int ground_height = 0, under_height = 0; //������ ��� 0

	int* ground[100] = { NULL, }; // ��������<100
	int* under[100] = { NULL, }; //��������<100

	//ground, under ��� �ִ�99������, �ϴ� 0���� �ʱ�ȭ
	for (int i = 0; i < 100; i++)
		ground[i] = (int*)calloc(wall_length, sizeof(int));

	under[0] = ground[0];

	for (int i = 1; i < 100; i++)
		under[i] = (int*)calloc(wall_length, sizeof(int));

	//���� �ױ�
	for (int i = 0; i < wall_length; i++)
	{
		if (start[i] == 1)
		{
			if (floor_now < 0)
			{
				if (under[-floor_now][i] == 0) //���� ���� �ش� ��ġ�� ��������� ���� ���� �� ����
				{
					for (int j = i; j < i + block_length; j++)
						under[-floor_now][j] = 1;
				}
				else
				{
					floor_now++; //������� ������ �� �� �ö󰡼� ���ױ�
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
						ground_height = floor_now; //���̰� �߰��Ǵ� ���
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

				if (floor_now == 0) //0�ε� break���ߴٸ�
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
	//answer_num �� answer_height�� ���ϴ� �ڵ带 �ۼ����ּ���
	int* pri = priority(); //�켱���� ���ϱ�
	int* start = startmarking(pri); //������ġ ǥ��

	answer_num = calNum(start);
	answer_height = calHeight(start);
}

/*���� �Ұ�*/
int main()
{
	scanf("%d", &block_length);
	scanf("%d", &wall_length);

	block = malloc(sizeof(char) * block_length+1); //���ڿ��� ����Ű�� �Ǵϱ� �ι��ڱ���
	wall = malloc(sizeof(char) * wall_length+1);
	scanf("%s", block);
	scanf("%s", wall);

	calculator();

	printf("%d\n", answer_num);
	printf("%d\n", answer_height);

	return 0;

}


