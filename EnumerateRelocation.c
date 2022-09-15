#include <stdio.h>
#include <stdlib.h>
#include "IntDequeue.h"
#include "lower_bound.h"
#include "branch_and_bound.h"
#include "data_info.h"

int enumerate_relocation(IntDequeue *q,int depth, int priority, direction Dir) {
	static int num_ret;
	static int p_before = 1;
	static int NumBlocking = 0;
	static int MinRelocation = 100;
	int i = 0;
	int j = 0;
	int k = 0;
	int BlockSpace = 0;
	int PriorityEdge = 0;
	direction dir = Dir;
	direction DirNext = Dir;
	IntDequeue *q_temp = NULL;
	switch (dir)
	{
	case both:
		qsort(q + 1, STACK - 1, sizeof(IntDequeue), (int(*)(const void *, const void *))pricmp);
		break;
	default:
		depth++;


#if TEST==0
		printf("Block relocation(depth=%d)\n", depth);
#endif

		if (dir == lower) {
			PriorityEdge = q[0].que[q[0].front];
		}
		else if (dir == upper) {
			PriorityEdge = q[0].que[(q[0].front + q[0].num - 1) % q[0].max];
		}
		NumBlocking = nblocking(q, dir);
		if (NumBlocking == 1) {
			DirNext = both;
			q_temp = malloc(STACK*(sizeof *q_temp));
			Array_initialize(q_temp);
			Array_copy(q_temp, q);
			if (PriorityEdge == priority + 1 && q[1].que[q[1].min_idx[0]] != priority && q[0].num_min == 1) {
				Deque(&q_temp[0], &num_ret, dir);

#if TEST==0
				printf("Number Retrieval:%d\n", num_ret);
#endif

				Deque(&q_temp[0], &num_ret, dir);

#if TEST==0
				printf("Priority Retrieval:%d\n", num_ret);
#endif

				insert_sort(q_temp);
				p_before = 0;
				priority = q_temp[0].que[q_temp[0].min_idx[0]];
				enumerate_relocation(q_temp, depth, priority, DirNext);
					Array_terminate(q_temp);
					free(q_temp);
					return MinRelocation;
			}
			Deque(&q_temp[0], &num_ret, dir);
		}
		Deque(&q[0], &num_ret,dir);
		for (i = 1; i < STACK;i++) {
			if (DirNext == both) {
				if (IsFull(&q_temp[i])) continue;
				Enque(&q_temp[i], PriorityEdge, dir);

#if TEST==0
				Array_print(q_temp);
#endif

				enumerate_relocation(q_temp, depth, p_before = priority, DirNext);
				Array_copy(q_temp, q);

#if TEST==0
				Array_print(q_temp);
#endif

				if(IsEmpty(&q_temp[i])) break;
			}
			else {
				if (IsFull(&q[i])) continue;
				Enque(&q[i], PriorityEdge, dir);


#if TEST==0
				Array_print(q);
#endif

				enumerate_relocation(q, depth, priority, DirNext);
				Deque(&q[i], &num_ret, dir);


#if TEST==0
				Array_print(q);
#endif

				if(IsEmpty(&q[i])) break;
			}
		}
		if (DirNext == both) {
			Array_terminate(q_temp);
			free(q_temp);
		}
		Enque(&q[0], PriorityEdge, dir);
		return MinRelocation;
	}
	while (q[k].que[q[k].min_idx[0]] == priority && !(IsEmpty(&q[k]))) {
		if (k != 0) {
			Swap_IntDequeue(&q[0], &q[k]);

#if TEST==0
			Array_print(q);
#endif

		}
		while ((q[0].que[q[0].front] == priority) || (q[0].que[(q[0].front + q[0].num - 1) % q[0].max] == priority)) {
			if (q[0].que[q[0].front] == priority) {
				DequeFront(&q[0], &num_ret);


#if TEST==0
				printf("Number Retrieval:%d\n", num_ret);
				Array_print(q);
#endif


			}
			else  if (q[0].que[(q[0].front + q[0].num - 1) % q[0].max] == priority) {
				DequeRear(&q[0], &num_ret);


#if TEST==0
				printf("Number Retrieval:%d\n", num_ret);
				Array_print(q);
#endif


			}
			if (ArrayIsEmpty(q)) {
				MinRelocation = depth < MinRelocation ? depth : MinRelocation;
				if (depth == 0) {
					j = MinRelocation;
					MinRelocation = 100;
					return j;
				}
				return MinRelocation;
			}
			if (IsEmpty(&q[0]))	break;
		}
		if (!(insert_sort(q))) {
			if (priority != q[0].que[q[0].min_idx[0]]) {
				k = 0;
				priority = q[0].que[q[0].min_idx[0]];
			}
			else {
				if (depth != 0 && priority == p_before) {
					k = 1;
					while (q[k].que[q[k].min_idx[0]] == priority) {
						k++;
						if (k == STACK) break;
					}
					break;
				}
				else k++;
			}
		}
		else {
			if (priority != q[0].que[q[0].min_idx[0]]) {
				k = 0;
				priority = q[0].que[q[0].min_idx[0]];
			}
			else {
				if (depth != 0 && priority == p_before) {
					k = 1;
					while (q[k].que[q[k].min_idx[0]] == priority) {
						k++;
						if (k == STACK) break;
					}


#if TEST==0
					Array_print(q);
#endif


					break;
				}
				else;
			}
		}


#if TEST==0
		Array_print(q);
#endif


		if (k == STACK) {
			break;
		}
	}
	j = 0;
	for (i = 0; i < STACK; i++) {
		j += q[i].num;
	}
	if (j <= 2 * STACK) {
		MinRelocation = (depth + lower_bound1(q)) < MinRelocation ? (depth + lower_bound1(q)) : MinRelocation;
		if (depth == 0) {
			j = MinRelocation;
			MinRelocation = 100;
			return j;
		}
		return MinRelocation;
	}
	for (i = 0; i < STACK; i++)	BlockSpace += TIER - q[i].num;
	for (i= 0; i < k; i++) {
		if (i != 0) {
			Swap_IntDequeue(&q[0], &q[i]);

#if TEST==0
			printf("swap(%d,%d)\n", 0, i);
			Array_print(q);
#endif

		}
		if (nblocking(&q[0], lower) > BlockSpace - (TIER - q[0].num));
		else {
			enumerate_relocation(q, depth, priority, lower);
		}
		if (nblocking(&q[0], upper) > BlockSpace - (TIER - q[0].num));
		else {
			enumerate_relocation(q, depth, priority, upper);
		}
			Swap_IntDequeue(&q[0], &q[i]);


#if TEST==0
			Array_print(q);
#endif

	}
	if (depth == 0) {
		j = MinRelocation;
		MinRelocation = 100;
		return j;
	}
	return MinRelocation;
}

