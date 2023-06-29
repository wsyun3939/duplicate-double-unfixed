#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "IntDequeue.h"
#include "lower_bound.h"
#include "branch_and_bound.h"
#include "data_info.h"

#define BUFFER 256
/*変数の定義
Stack:スタックの最大個数
Tier:積み上げられるブロックの最大値
Nblocking_lower:priorityの下側に位置するブロック数
Nblocking_upper:priorityの上側に位置するブロック数
stack[i]:i番目のスタック
Nblocking_upper_again:上側から移動させた後もブロッキングブロックとなる個数
Nblocking_lower_again:下側から移動させた後もブロッキングブロックとなる個数

*/

int main(void)
{
	clock_t start = clock();
	clock_t max = 0;
	clock_t UB_lapse = 0;
	clock_t sol_lapse = 0;

	IntDequeue *stack = malloc(STACK * (sizeof *stack));
	Array_initialize(stack);
	int nblock = NBLOCK;
	int i, j, x, l;
	int k = 0;
	int sum = 0;

	int gap = 0;
	int max_gap = 0;
	int UB_gap = 0;

	int timeup = 0;
	int infeasible = 0;

	int missmatch = 0;
	char filename[BUFFER];
	char str[BUFFER];
	// FILE *fp_write = NULL;

	for (int a = NUMBER; a < NUMBER + 100 * TIER; a++)
	{
		FILE *fp = NULL;
		sprintf(filename, "../alpha=%.1f/%d-%d-%d/%05d.txt", ALPHA, TIER, STACK, nblock, a);
		printf("%s\n", filename);

		//	読み込みモードでファイルを開く
		fp = fopen(filename, "r");

		fgets(str, BUFFER, fp);
		fgets(str, BUFFER, fp);
		fgets(str, BUFFER, fp);

		// スタック数と高さを読み込む　//
		for (i = 0; i < STACK; i++)
		{
			fscanf(fp, "%d ", &l);
			for (j = 0; j < l; j++)
			{
				fscanf(fp, "%d ", &x);
				EnqueRear(&stack[i], x);
			}
		}

		//*---LB1---*//
		int LB1 = lower_bound1(stack);
		printf("LB1:%d\n", LB1);

		qsort(stack, STACK, sizeof(IntDequeue), (int (*)(const void *, const void *))pricmp);

		printf("sort:\n");
		Array_print(stack);
		int UB_cur = LB1;
		int priority = 1;

		clock_t time_start = clock();
		int UB = UpperBound(stack);
		UB_lapse += clock() - time_start;

		time_start = clock();

		Array_print(stack);
		int min_relocation = branch_and_bound(stack, UB, UB_cur, LB1, priority, both, clock());
		sol_lapse += clock() - time_start;
		if (max < clock() - time_start)
		{
			max = clock() - time_start;
		}

		printf("min_relocation:%d,difference%d\n", min_relocation, min_relocation - LB1);
		if (min_relocation == -1)
		{
			timeup++;
		}
		else
		{
			sum += min_relocation;
			gap += min_relocation - LB1;
			if (max_gap < min_relocation - LB1)
			{
				max_gap = min_relocation - LB1;
			}
			if (UB != -1)
			{
				UB_gap += UB - min_relocation;
			}
			else
				infeasible++;
		}
		if (min_relocation == LB1)
		{
			k++;
		}
		fclose(fp);
		// if (a % 100 == 1)
		// {
		// 	sprintf(filename, "../alpha=%.1f/%d-%d-%d_unfixed.csv", ALPHA, TIER, STACK, nblock);
		// 	fp_write = fopen(filename, "w");
		// }
		// if (fp_write != NULL)
		// 	fprintf(fp_write, "%d\n", min_relocation);
		Array_clear(stack);
		if (a % 100 == 0)
		{
			nblock++;
			// fclose(fp_write);
		}
	}
	clock_t end = clock();
	Array_terminate(stack);

	putchar('\n');
	printf("optimal value:%f,ave_gap:%f,max_gap:%d,ave_time:%f,max_time:%f\n", (double)sum / (100 * TIER), (double)gap / (100 * TIER), max_gap, (double)sol_lapse / (100 * TIER * CLOCKS_PER_SEC), (double)max / CLOCKS_PER_SEC);

	// printf("UB_lapse:%f,sol_lapse:%f\n", (double)UB_lapse / CLOCKS_PER_SEC, (double)sol_lapse / CLOCKS_PER_SEC);

	return 0;
}
