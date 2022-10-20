#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "IntDequeue.h"
#include "lower_bound.h"
#include "branch_and_bound.h"
#include "data_info.h"

#ifdef EITHER
int branch_and_bound(IntDequeue *q, int UB,int UB_cur, int LB,int priority, direction Dir,clock_t start) {
	static int num_ret;
	static int depth = 0;
	static int SecondPosition = 0;
	static int MinRelocation = 0;
	static int NumBlocking = 0;
	static int p_before = 1;
	if (UB == UB_cur) {
		depth = 0;
		return MinRelocation = UB;
	}
	int i = 0;
	int j = 0;
	int k = 0;
	int d=0;
	int LB_temp = 0;
	int PriorityEdge = 0;
	int BG_index[STACK]={0};
	direction dir = Dir;
	direction DirNext = Dir;
	IntDequeue *q_temp = NULL;
	static int ans=0;
	clock_t end;

	end=clock();
	if(((double)(end-start)/CLOCKS_PER_SEC)>7200){
		depth=0;
		return -1;
	}
	switch (dir)
	{
	case both:
		qsort(q + 1, STACK - 1, sizeof(IntDequeue), (int(*)(const void *, const void *))pricmp);

		while (q[k].que[q[k].min_idx[0]] == priority && !(IsEmpty(&q[k]))) {
			if(Array_check(q)==1){
				MinRelocation=LB+depth;
				depth=0;
				return MinRelocation;
			}
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
					depth = 0;
					return MinRelocation = UB_cur;
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
			if (k == STACK) {
				break;
			}
		}

		break;
	}


		if(Array_check(q)==1){
			MinRelocation=LB+depth;
			depth=0;
			return MinRelocation;
		}



		qsort(q, k, sizeof(IntDequeue), (int(*)(const void *, const void *))BlockingCmp);

#if TEST==0
		Array_print(q);
#endif

		depth++;
		if(Dir==both){
		for(d=0;d<=1;d++){	//下界値を与える方向から順に積み替えを吟味する
		for (i = 0; i< k; i++) {
			if (i != 0) {
				Swap_IntDequeue(&q[0], &q[i]);
			}

#if TEST==0
				printf("swap(%d,%d)\n", 0, i);
				Array_print(q);
#endif

			dir = q[0].dir;
			if (dir == both) dir = q[0].que[q[0].front] < q[0].que[(q[0].front + q[0].num - 1) % q[0].max] ?
				lower : upper;
			if(d==1){
				if(dir==lower) dir=upper;
				else dir=lower;
			}
			LB_temp=LB;
			if (dir == lower) {
			PriorityEdge = q[0].que[q[0].front];
			SecondPosition = q[0].que[(q[0].front + 1) % q[0].max];
		}
		else if (dir == upper) {
			PriorityEdge = q[0].que[(q[0].front + q[0].num - 1) % q[0].max];
			SecondPosition = q[0].que[(q[0].front + q[0].num - 2) % q[0].max];
		}
		LB_temp-=q[0].LB;
		Deque(q,&num_ret,dir);
		LB_temp +=q[0].LB;
		if (LB_temp + depth > UB_cur);
		else{
		NumBlocking = nblocking(q, dir);
		if (NumBlocking == 0) { //積み替え後，取り出しが可能な場合
			DirNext = both;
			q_temp = malloc(STACK*(sizeof *q_temp));
				Array_initialize(q_temp);
				Array_copy(q_temp, q);
			if (PriorityEdge == priority + 1 && q[1].que[q[1].min_idx[0]] != priority && q[0].num_min == 1) { //ドミナンス

				Deque(&q_temp[0], &num_ret, dir);

#if TEST==0
				printf("dominance:%d,%d\n", num_ret,num_ret+1);
				Array_print(q_temp);
#endif

				insert_media(q_temp, 0);
				p_before = 0;

#if TEST==0
				Array_print(q_temp);
#endif
		
				
				ans=branch_and_bound(q_temp, UB, UB_cur, LB_temp,q_temp[0].que[q_temp[0].min_idx[0]], DirNext,start);
				if (ans!=0 && ans!=-1) {
					Array_terminate(q_temp);
					free(q_temp);
					return MinRelocation;
				}
				else if(ans==-1){
					Array_terminate(q_temp);
					free(q_temp);
					return -1;
				}

#if TEST==0
				printf("Lower termination.\n");
				Array_print(q);
#endif

				}
				else{
					for(j=STACK-1;j>0;j--){
						BG_index[j]=Enque(&q_temp[j],PriorityEdge,dir);
						if(BG_index[j]!=-1) Deque(&q_temp[j],&num_ret,dir);
					}
					for(j=STACK-1;j>0;j--){
					if(BG_index[j]==0){ //BG積み替えが可能
						Enque(&q_temp[j],PriorityEdge,dir);

#if TEST==0
				printf("relocation(0,%d)\n", j);
				Array_print(q_temp);
#endif

					ans=branch_and_bound(q_temp, UB, UB_cur, LB_temp,priority, DirNext,start);
					if(ans!=0 && ans!=-1){
						Array_terminate(q_temp);
						free(q_temp);
						return MinRelocation;
					}
					else if(ans==-1){
						Array_terminate(q_temp);
						free(q_temp);
						return -1;
					}
					Array_copy(q_temp, q);

#if TEST==0
					Array_print(q_temp);
#endif

					}
					}
					if(LB_temp+depth+1>UB_cur);
					else{
					for(j=STACK-1;j>0;j--){
					if(BG_index[j]==1){ //BG積み替え不可能
						Enque(&q_temp[j],PriorityEdge,dir);

#if TEST==0
				printf("relocation(0,%d)\n", j);
				Array_print(q_temp);
#endif

					ans=branch_and_bound(q_temp, UB, UB_cur, LB_temp+1,priority, DirNext,start);
					if(ans!=0 && ans!=-1){
						Array_terminate(q_temp);
						free(q_temp);
						return MinRelocation;
					}
					else if(ans==-1){
						Array_terminate(q_temp);
						free(q_temp);
						return -1;
					}
					Array_copy(q_temp, q);

#if TEST==0
					Array_print(q_temp);
#endif

					}
					}
					}
					}
					Array_terminate(q_temp);
					free(q_temp);
		}
		else{
			for(j=STACK-1;j>0;j--){
						BG_index[j]=Enque(&q[j],PriorityEdge,dir);
						if(BG_index[j]!=-1) Deque(&q[j],&num_ret,dir);
					}
					for(j=STACK-1;j>0;j--){
					if(BG_index[j]==0){ //BG積み替え可能
						Enque(&q[j],PriorityEdge,dir);

#if TEST==0
				printf("relocation(0,%d)\n", j);
				Array_print(q);
#endif

						ans=branch_and_bound(q, UB, UB_cur, LB_temp,priority, dir,start);
						if(ans!=0 && ans!=-1){
							return MinRelocation;
						}
						else if(ans==-1){
							return -1;
						}
						Deque(&q[j],&num_ret,dir);

#if TEST==0
				Array_print(q);
#endif

						
					}
					}
					if(LB_temp+depth+1>UB_cur);
					else{
					for(j=STACK-1;j>0;j--){
					if(BG_index[j]==1){ //BG積み替え不可能
						Enque(&q[j],PriorityEdge,dir);

#if TEST==0
				printf("relocation(0,%d)\n", j);
				Array_print(q);
#endif

					ans=branch_and_bound(q, UB, UB_cur, LB_temp+1,priority, dir,start);
					if(ans!=0 && ans!=-1){
						return MinRelocation;
					}
					else if(ans==-1){
						return -1;
					}
					Deque(&q[j],&num_ret,dir);

#if TEST==0
					Array_print(q);
#endif

						
					}
					}
					}
		}	
		}
		Enque(q,PriorityEdge,dir);

#if TEST==0
				Array_print(q);
#endif

		LB_temp=LB;
		Swap_IntDequeue(&q[0], &q[i]);
		}
		}
		}
		else{
			dir = q[0].dir;
			if (dir == both) dir = q[0].que[q[0].front] < q[0].que[(q[0].front + q[0].num - 1) % q[0].max] ?
				lower : upper;
			for(d=0;d<=1;d++){	//下界値を与える方向から順に積み替えを吟味する
			LB_temp=LB;
			if (dir == lower) {
			PriorityEdge = q[0].que[q[0].front];
			SecondPosition = q[0].que[(q[0].front + 1) % q[0].max];
		}
		else if (dir == upper) {
			PriorityEdge = q[0].que[(q[0].front + q[0].num - 1) % q[0].max];
			SecondPosition = q[0].que[(q[0].front + q[0].num - 2) % q[0].max];
		}
		LB_temp-=q[0].LB;
		Deque(q,&num_ret,dir);
		LB_temp +=q[0].LB;
		if (LB_temp + depth > UB_cur);
		else{
		NumBlocking = nblocking(q, dir);
		if (NumBlocking == 0) { //積み替え後，取り出しが可能な場合
			DirNext = both;
			q_temp = malloc(STACK*(sizeof *q_temp));
				Array_initialize(q_temp);
				Array_copy(q_temp, q);
			if (PriorityEdge == priority + 1 && q[1].que[q[1].min_idx[0]] != priority && q[0].num_min == 1) { //ドミナンス

				Deque(&q_temp[0], &num_ret, dir);

#if TEST==0
				printf("dominance:%d,%d\n", num_ret,num_ret+1);
#endif

#if TEST==0
				Array_print(q_temp);
#endif

				insert_media(q_temp, 0);
				p_before = 0;

				if(Array_check(q_temp)==1){
		
					printf("ブロック数が３個以下\n");

					MinRelocation=LB_temp+depth;
					depth=0;
					Array_terminate(q_temp);
					free(q_temp);
					return MinRelocation;
				}			
				ans=branch_and_bound(q_temp, UB, UB_cur, LB_temp,q_temp[0].que[q_temp[0].min_idx[0]], DirNext,start);
				if (ans!=0 && ans!=-1) {
					Array_terminate(q_temp);
					free(q_temp);
					return MinRelocation;
				}
				else if(ans==-1){
					Array_terminate(q_temp);
					free(q_temp);
					return -1;
				}

#if TEST==0
				printf("Lower termination.\n");
				Array_print(q);
#endif

				}
				else{
					for(j=STACK-1;j>0;j--){
						BG_index[j]=Enque(&q_temp[j],PriorityEdge,dir);
						if(BG_index[j]!=-1) Deque(&q_temp[j],&num_ret,dir);
					}
					for(j=STACK-1;j>0;j--){
					if(BG_index[j]==0){ //BG積み替えが可能
						Enque(&q_temp[j],PriorityEdge,dir);

#if TEST==0
				printf("relocation(0,%d)\n", j);
				Array_print(q_temp);
#endif

					ans=branch_and_bound(q_temp, UB, UB_cur, LB_temp,priority, DirNext,start);
					if(ans!=0 && ans!=-1){
						Array_terminate(q_temp);
						free(q_temp);
						return MinRelocation;
					}
					else if(ans==-1){
						Array_terminate(q_temp);
						free(q_temp);
						return -1;
					}
					Array_copy(q_temp, q);

#if TEST==0
					Array_print(q_temp);
#endif

					}
					}
					if(LB_temp+depth+1>UB_cur);
					else{
					for(j=STACK-1;j>0;j--){
					if(BG_index[j]==1){ //BG積み替え不可能
						Enque(&q_temp[j],PriorityEdge,dir);

#if TEST==0
				printf("relocation(0,%d)\n", j);
				Array_print(q_temp);
#endif

					ans=branch_and_bound(q_temp, UB, UB_cur, LB_temp+1,priority, DirNext,start);
					if(ans!=0 && ans!=-1){
						Array_terminate(q_temp);
						free(q_temp);
						return MinRelocation;
					}
					else if(ans==-1){
						Array_terminate(q_temp);
						free(q_temp);
						return -1;
					}
					Array_copy(q_temp, q);

#if TEST==0
					Array_print(q_temp);
#endif

					}
					}
					}
					}
			Array_terminate(q_temp);
			free(q_temp);
		}
		else{
			for(j=STACK-1;j>0;j--){
						BG_index[j]=Enque(&q[j],PriorityEdge,dir);
						if(BG_index[j]!=-1) Deque(&q[j],&num_ret,dir);
					}
					for(j=STACK-1;j>0;j--){
					if(BG_index[j]==0){ //BG積み替え可能
						Enque(&q[j],PriorityEdge,dir);

#if TEST==0
				printf("relocation(0,%d)\n", j);
				Array_print(q);
#endif

				ans=branch_and_bound(q, UB, UB_cur, LB_temp,priority, dir,start);
				if(ans!=0 && ans!=-1){
					return MinRelocation;
				}
				else if(ans==-1){
					return -1;
				}
				Deque(&q[j],&num_ret,dir);

#if TEST==0
				Array_print(q);
#endif

						
					}
					}
					if(LB_temp+depth+1>UB_cur);
					else{
					for(j=STACK-1;j>0;j--){
					if(BG_index[j]==1){ //BG積み替え不可能
						Enque(&q[j],PriorityEdge,dir);

#if TEST==0
				printf("relocation(0,%d)\n", j);
				Array_print(q);
#endif

				ans=branch_and_bound(q, UB, UB_cur, LB_temp+1,priority, dir,start);
				if(ans!=0 && ans!=-1){
					return MinRelocation;
				}
				else if(ans==-1){
					return -1;
				}
				Deque(&q[j],&num_ret,dir);

#if TEST==0
				Array_print(q);
#endif

						
					}
					}
					}
		}	
		}
		Enque(q,PriorityEdge,dir);

#if TEST==0
				Array_print(q);
#endif

		LB_temp=LB;
		if(dir==lower) dir=upper;
		else dir=lower;
		}
		}
		depth--;
		if (depth == 0) {
			UB_cur++;

#if TEST==0
			printf("UB_cur++\n");
#endif

			ans=branch_and_bound(q, UB, UB_cur, LB, priority, both,start);
			if (ans!=0 && ans!=-1) {
				return MinRelocation;
			}
			else if(ans==-1) return -1;
			return -1;
		}
		else {
			return 0;
		}
}
#endif
