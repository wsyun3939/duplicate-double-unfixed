#include <stdio.h>
#include <stdlib.h>
#include "IntDequeue.h"
#include "lower_bound.h"
#include "branch_and_bound.h"
#include "data_info.h"


int insert_sort(IntDequeue *q) {
	IntDequeue q_temp = q[0];
	int i;
	if (IsEmpty(&q[0])) {
		for (i = 1; i < STACK; i++) {
			if (IsEmpty(&q[i])) {
				break;
			}
			q[i - 1] = q[i];
		}
		q[i - 1] = q_temp;
		return 1;
	}
	else {
		for (i = 1; i < STACK; i++) {
			if (IsEmpty(&q[i]) || q_temp.que[q_temp.min_idx[0]] <= q[i].que[q[i].min_idx[0]]) {
				break;
			}
			q[i - 1] = q[i];
		}
		q[i - 1] = q_temp;
		if (i == 1) return 0;
		else return 1;
	}
}

void insert_media(IntDequeue *q, int stack_idx) {
	IntDequeue q_temp = q[stack_idx];
	int i;
	if (stack_idx != STACK-1) {
		for (i = stack_idx + 1; i < STACK; i++) {
			if (q_temp.que[q_temp.min_idx[0]] <= q[i].que[q[i].min_idx[0]] || IsEmpty(&q[i])) {
				break;
			}
			q[i - 1] = q[i];
		}
		q[i - 1] = q_temp;
	}
	if (stack_idx != 0) {
		for (i = stack_idx - 1; i >= 0; i--) {
			if (IsEmpty(&q[i]));
			else if (q_temp.que[q_temp.min_idx[0]] >= q[i].que[q[i].min_idx[0]]) {
				break;
			}
			q[i + 1] = q[i];
		}
		q[i + 1] = q_temp;
	}
}

void insert_front(IntDequeue *q, int stack_idx) {
	if (stack_idx == 0);
	else {
		IntDequeue q_temp = q[stack_idx];
		int i;
		for (i = stack_idx - 1; i >= 0; i--) {
			if (IsEmpty(&q[i]));
			else if (q_temp.que[q_temp.min_idx[0]] >= q[i].que[q[i].min_idx[0]]) {
				break;
			}
			q[i + 1] = q[i];
		}
		q[i + 1] = q_temp;
	}
}

//*---ブロックを取り出す方向を決める---*//
direction retrieval_direction(IntDequeue *q, int *LB) {
	int Nblocking_upper = nblocking_upper(q);
	int Nblocking_lower = nblocking_lower(q);
	direction dir;
	if (q->num < 6) {
		dir = Nblocking_upper < Nblocking_lower ? upper :
			Nblocking_upper > Nblocking_lower ? lower : both;
		if (dir == both) *LB=min_relocation(q, upper);
		else *LB = min_relocation(q, dir);
	}
	else {
		IntDequeue* q_temp = NULL;
		if (Nblocking_lower < Nblocking_upper) {
			if ((Nblocking_lower + 1 + num_expo(Nblocking_upper)) < Nblocking_upper) {
				dir = lower;
				*LB = min_relocation(q, dir);
			}
			else {
				q_temp = malloc(sizeof(*q_temp));
				Initialize(q_temp);
				Copy(q_temp, q);
				//*---上側から取り出した場合の積み替え最小回数---*//
				Nblocking_upper = min_relocation(q, upper);

				//*---下側から取り出した場合の積み替え最小回数---*//
				Nblocking_lower = min_relocation(q_temp, lower);

				Terminate(q_temp);
				free(q_temp);
				dir = Nblocking_upper < Nblocking_lower ? upper :
					Nblocking_upper > Nblocking_lower ? lower : both;
				if (dir == upper) *LB = Nblocking_upper;
				else *LB = Nblocking_lower;
			}
		}
		else {
			if ((Nblocking_upper + 1 + num_expo(Nblocking_lower)) < Nblocking_lower) {
				dir = upper;
				*LB = min_relocation(q, dir);
			}
			else {
				q_temp = malloc(sizeof(*q_temp));
				Initialize(q_temp);
				Copy(q_temp, q);
				//*---上側から取り出した場合の積み替え最小回数---*//
				Nblocking_upper = min_relocation(q, upper);

				//*---下側から取り出した場合の積み替え最小回数---*//
				Nblocking_lower = min_relocation(q_temp, lower);

				Terminate(q_temp);
				free(q_temp);
				dir = Nblocking_upper < Nblocking_lower ? upper :
					Nblocking_upper > Nblocking_lower ? lower : both;
				if (dir == upper) *LB = Nblocking_upper;
				else *LB = Nblocking_lower;
			}
		}
	}
	return dir;
}

//*---下界値優先探索のための比較関数---*//
int LBcmp(const LB_idx *n1, const LB_idx *n2) {
	return n1->blocking<n2->blocking ? -1 :
		n1->blocking>n2->blocking ? 1 : 0;
}

//*---ブロッキングブロック数でターゲットブロックを持つスタックのソートをかける---*//
int BlockingCmp(const IntDequeue *n1, const IntDequeue *n2) {
	int n1_blocking = nblocking(n1, n1->dir);
	int n2_blocking = nblocking(n2, n2->dir);
		return n1_blocking<n2_blocking ? -1 :
			n1_blocking>n2_blocking ? 1 : 
			n1->num<n2->num?-1:
			n1->num>n2->num ? 1 :
			0;
}

int UpperBound(const IntDequeue *q,int priority,direction dir) {
	IntDequeue *q_temp = malloc(STACK*(sizeof *q_temp));
	Array_initialize(q_temp);
	Array_copy(q_temp, q);
	int num_ret;
	int i = 0;
	int j = 0;
	int k = 0;
	int p_before = priority;
	int UB = 0;
	int PriorityEdge = 0;
	int NumBlocking = 0;
	direction DirNext = dir;
	LB_idx *BlockingTable = NULL;
	while (!(IsEmpty(&q_temp[0])))
	{
		j = 0;
		k = 0;
		switch (DirNext)
		{
		case both:
			while (q_temp[k].que[q_temp[k].min_idx[0]] == priority && !(IsEmpty(&q_temp[k]))) {
				if (k != 0) {
					Swap_IntDequeue(&q_temp[0], &q_temp[k]);
				}
				while ((q_temp[0].que[q_temp[0].front] == priority) || (q_temp[0].que[(q_temp[0].front + q_temp[0].num - 1) % q_temp[0].max] == priority)) {
					if (q_temp[0].que[q_temp[0].front] == priority) {
						DequeFront(&q_temp[0], &num_ret);
					}
					else  if (q_temp[0].que[(q_temp[0].front + q_temp[0].num - 1) % q_temp[0].max] == priority) {
						DequeRear(&q_temp[0], &num_ret);
					}
					if (ArrayIsEmpty(q_temp)) {
						Array_terminate(q_temp);
						free(q_temp);
						return UB;
					}
					if (IsEmpty(&q_temp[0]))	break;
				}
				if (!(insert_sort(q_temp))) {
					if (priority != q_temp[0].que[q_temp[0].min_idx[0]]) {
						k = 0;
						priority = q_temp[0].que[q_temp[0].min_idx[0]];
					}
					else {
						if (UB != 0 && priority == p_before) {
							k = 1;
							while (q_temp[k].que[q_temp[k].min_idx[0]] == priority) {
								k++;
								if (k == STACK) break;
							}
							break;
						}
						else k++;
					}
				}
				else {
					if (priority != q_temp[0].que[q_temp[0].min_idx[0]]) {
						k = 0;
						priority = q_temp[0].que[q_temp[0].min_idx[0]];
					}
					else {
						if (UB != 0 && priority == p_before) {
							k = 1;
							while (q_temp[k].que[q_temp[k].min_idx[0]] == priority) {
								k++;
								if (k == STACK) break;
							}
							break;
						}
						else;
					}
				}
				if (k == STACK) {
					break;
				}
			}
			p_before = priority;
			qsort(q_temp, k, sizeof(IntDequeue), (int(*)(const void *, const void *))BlockingCmp);
			int BlockSpace = 0;
			for (i = 0; i < STACK; i++)	BlockSpace += TIER - q_temp[i].num;
			for (i = 0; i < k; i++) {
				if (i != 0) {
					Swap_IntDequeue(&q_temp[0], &q_temp[i]);
				}
				dir = DirNext = q_temp[0].dir;
				if (dir == both) dir = DirNext = q_temp[0].que[q_temp[0].front] < q_temp[0].que[(q_temp[0].front + q_temp[0].num - 1) % q_temp[0].max] ?
					lower : upper;
				switch (dir) {
				case lower:
					if (nblocking_lower(&q_temp[0]) > BlockSpace - (TIER - q_temp[0].num)) {
						Swap_IntDequeue(&q_temp[0], &q_temp[i]);
						continue;
					}
					break;
				case upper:
					if (nblocking_upper(&q_temp[0]) > BlockSpace - (TIER - q_temp[0].num)) {
						Swap_IntDequeue(&q_temp[0], &q_temp[i]);
						continue;
					}
					break;
				}
				break;
			}
			//おいしくない方向から取り出す
			if (i == k) {
				for (i = 0; i < k; i++) {
					if (i != 0) {
						Swap_IntDequeue(&q_temp[0], &q_temp[i]);
					}
					dir = DirNext = q_temp[0].dir;
					if (dir == both) dir = DirNext = q_temp[0].que[q_temp[0].front] < q_temp[0].que[(q_temp[0].front + q_temp[0].num - 1) % q_temp[0].max] ?
						lower : upper;
					switch (dir) {
					case lower:
						if (nblocking_upper(&q_temp[0]) > BlockSpace - (TIER - q_temp[0].num)) {
							Swap_IntDequeue(&q_temp[0], &q_temp[i]);
							continue;
						}
						else {
							dir = DirNext = upper;
						}
						break;
					case upper:
						if (nblocking_lower(&q_temp[0]) > BlockSpace - (TIER - q_temp[0].num)) {
							Swap_IntDequeue(&q_temp[0], &q_temp[i]);
							continue;
						}
						else {
							dir = DirNext = lower;
						}
						break;
					}
					break;
				}
				if (i == k) return 100;
			}
			break;
		default:
			UB++;
			if (dir == lower) {
				PriorityEdge = q_temp[0].que[q_temp[0].front];
			}
			else if (dir == upper) {
				PriorityEdge = q_temp[0].que[(q_temp[0].front + q_temp[0].num - 1) % q_temp[0].max];
			}
			NumBlocking = nblocking(q_temp, dir);
			if (NumBlocking == 1) {
				DirNext = both;
				if (PriorityEdge == priority + 1 && q_temp[1].que[q_temp[1].min_idx[0]] != priority && q_temp[0].num_min == 1) {
					Deque(&q_temp[0], &num_ret, dir);
					Deque(&q_temp[0], &num_ret, dir);
					insert_sort(q_temp);
					p_before = 0;
					priority = q_temp[0].que[q_temp[0].min_idx[0]];
					break;
				}
			}
			if (IsEmpty(&q_temp[STACK - 1])) {
				Deque(&q_temp[0], &num_ret, dir);
				Enque(&q_temp[STACK - 1], num_ret, dir);
				insert_media(q_temp, STACK - 1);
				break;
			}
			BlockingTable = CreateBlockingTable(q_temp, dir, &j);
			for (i = j - 1; i >= 0; i--) {
				if (BlockingTable[i].blocking != 0)	continue;
				Deque(&q_temp[0], &num_ret,dir);
				Enque(&q_temp[BlockingTable[i].idx], num_ret, dir);
				insert_media(q_temp, BlockingTable[i].idx);
				break;
			}
			if (i >= 0);
			else {
				for (i = j - 1; i >= 0; i--) {
					if (BlockingTable[i].blocking != 1)	continue;
					Deque(&q_temp[0], &num_ret,dir);
					Enque(&q_temp[BlockingTable[i].idx], num_ret,dir);
					insert_media(q_temp, BlockingTable[i].idx);
					break;
				}
			}
			free(BlockingTable);
			break;
		}
	}
	Array_terminate(q_temp);
	free(q_temp);
	return UB;
}

LB_idx* CreateBlockingTable(IntDequeue *q, direction dir,int *Size) {
	int *stack_idx = malloc(STACK * sizeof(int));
	int i = 0;
	int j = 0;
	int PriorityEdge = 0;
	for (i = 1; i < STACK; i++) {
		if (!(IsFull(&q[i]))) {
			stack_idx[j] = i;
			j++;
		}
	}
	*Size = j;
	LB_idx *BlockingTable = malloc(j*(sizeof *BlockingTable));
	switch (dir) {
	case lower:
		PriorityEdge = q[0].que[q[0].front];
		for (i = 0; i < j; i++) {
			//*最下部のブロックを動かした際の下界値*//
			BlockingTable[i].blocking = EnqueFront(&q[stack_idx[i]], PriorityEdge);
			BlockingTable[i].idx = stack_idx[i];
			DequeFront(&q[stack_idx[i]], &PriorityEdge);
		}
		break;
	case upper:
		PriorityEdge = q[0].que[(q[0].front + q[0].num - 1) % q[0].max];
		for (i = 0; i < j; i++) {
			//*最上部のブロックを動かした際の下界値*//
			BlockingTable[i].blocking = EnqueRear(&q[stack_idx[i]], PriorityEdge);;
			BlockingTable[i].idx = stack_idx[i];
			DequeRear(&q[stack_idx[i]], &PriorityEdge);
		}
		break;
	}
	free(stack_idx);
	return BlockingTable;
}

