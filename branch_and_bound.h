#ifndef ___branch_and_bound
#define ___branch_and_bound

#include "IntDequeue.h"
#include "data_info.h"

#ifdef EITHER
typedef struct{
	int idx;		//*スタックの添え字*//
	int blocking;		//*ブロッキングとなるかどうか*//
}	LB_idx;
#endif
#ifdef BOTH
typedef struct {
	int idx;		//*スタックの添え字*//
	int blocking;		//*ブロッキングとなるかどうか*//
	direction dir;
}	LB_idx;
#endif

typedef struct {
	int num_blocking; //ブロッキングブロック数
	int num_block;	//スタックのブロック数
	int stack_idx;		//取り出すスタックの添え字
	direction dir;		//取り出す方向
} Blocking;

//*---仮上界値を用いた分枝限定法---*//
int branch_and_bound(IntDequeue *q, int UB,int UB_cur, int LB, int priority,direction dir);

//*---挿入ソート---*//
int insert_sort(IntDequeue *q);

//*---指定されたスタックをソートする---*//
void insert_media(IntDequeue *q, int stack_idx);

void insert_front(IntDequeue *q, int stack_idx);

direction retrieval_direction(IntDequeue *q, int *LB);

//*---下界値優先探索のための比較関数---*//
int LBcmp(const LB_idx *n1, const LB_idx *n2);

//*---ブロッキングブロック数でターゲットブロックを持つスタックのソートをかける---*//
int BlockingCmp(const IntDequeue *n1, const IntDequeue *n2);

//*---近似解法により上界値を求める---*//
int UpperBound(const IntDequeue *q,int priority, direction dir);

//*---ブロックを積み替えた際、移動先でブロッキングブロックになるか---*//
LB_idx* CreateBlockingTable(IntDequeue *q, direction dir,int *Size);

//*---通常の分枝限定法---*//
int enumerate_relocation(IntDequeue *q, int depth, int priority, direction Dir);

#endif
