#include<stdio.h>
#include<math.h>

void fcfs(int noq, int qu[10], int st)
{
	int i, s = 0;
	for (i = 0; i < noq; i++)
	{
		s = s + abs(st - qu[i]);
		st = qu[i];
	}
	printf("\n Total Head movements : %d", s);
	printf("\n Total Seek Time (Assuming Seek Rate 5ms) : %d", s*5);
}

void sstf(int noq, int qu[10], int st, int visit[10])
{
	int min, s = 0, p, i;
	while (1)
	{
		min = 999;
		for (i = 0; i < noq; i++)
			if (visit[i] == 0)
			{
				if (min > abs(st - qu[i]))
				{
					min = abs(st - qu[i]);
					p = i;
				}
			}
		if (min == 999)
			break;
		visit[p] = 1;
		s = s + min;
		st = qu[p];
	}
	printf("\n Total Head movements : %d", s);
	printf("\n Total Seek Time (Assuming Seek Rate 5ms) : %d", s * 5);
}

void scan(int noq, int qu[10], int st, int ch)
{
	int i, j, s = 0;
	for (i = 0; i < noq; i++)
	{
		if (st < qu[i])
		{
			for (j = i - 1; j >= 0; j--)
			{
				s = s + abs(st - qu[j]);
				st = qu[j];
			}
			if (ch == 3)
			{
				s = s + abs(st - 0);
				st = 0;
			}
			for (j = 1; j < noq; j++)
			{
				s = s + abs(st - qu[j]);
				st = qu[j];
			}
			break;
		}
	}
	printf("\n Total Head movements : %d", s);
	printf("\n Total Seek Time (Assuming Seek Rate 5ms) : %d", s * 5);
}

int main()
{
	int n, qu[20], st, i, j, t, noq, ch, visit[20];
	printf("\n Enter the maximum number of cylinders : ");
	scanf_s("%d", &n);
	printf("Enter number of queue elements : ");
	scanf_s("%d", &noq);
	printf("\n Enter the queue \n");
	for (i = 0; i < noq; i++)
	{
		scanf_s("%d", &qu[i]);
		visit[i] = 0;
	}
	printf("\n Enter the disk head starting posision : \n");
	scanf_s("%d", &st);
	while (1)
	{
		printf("\n\n\t\t MENU \n");
		printf("\n\n\t\t 1. FCFS \n");
		printf("\n\n\t\t 2. SSTF \n");
		printf("\n\n\t\t 3. SCAN \n");
		printf("\n\n\t\t 4. EXIT \n");
		printf("\nEnter your choice : \n");
		scanf_s("%d", &ch);
		if (ch > 2)
		{
			for (i = 0; i < noq; i++)
				for (j = i + 1; j < noq; j++)
					if (qu[i] > qu[j])
					{
						t = qu[i];
						qu[i] = qu[j];
						qu[j] = t;
					}
		}
		switch (ch)
		{
		case 1: printf("\n FCFS \n");
			printf("\n*****\n");
			fcfs(noq, qu, st);
			printf("\n\n*****\n");
			break;

		case 2: printf("\n SSTF \n");
			printf("\n*****\n");
			sstf(noq, qu, st, visit);
			printf("\n\n*****\n");
			break;
		case 3: printf("\n SCAN \n");
			printf("\n*****\n");
			scan(noq, qu, st, ch);
			printf("\n\n*****\n");
			break;
		case 4: exit(0);
		}
	}
}
