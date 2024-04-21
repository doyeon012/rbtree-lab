#ifndef _RBTREE_H_
#define _RBTREE_H_

#include <stddef.h>

typedef enum { RBTREE_RED, RBTREE_BLACK } color_t; // 열거형

typedef int key_t; // 각 노드에 저장되는 키 값을 나타내는 데이터 형식


// tree의 각 노드를 표현하는 구조체
typedef struct node_t {
  color_t color;
  key_t key;
  struct node_t *parent, *left, *right;
} node_t;

// tree 자체를 나타내는 구조
typedef struct {
  node_t *root; // 루트노드
  node_t *nil;  // nil
} rbtree;

rbtree *new_rbtree(void); // 새로운 RB트리 생성 + 반환
void delete_rbtree(rbtree *); //RB 트리 삭제

node_t *rbtree_insert(rbtree *, const key_t);//노드 삽입

node_t *rbtree_find(const rbtree *, const key_t);// 주어진 키 값을 가진 노드 반환
node_t *rbtree_min(const rbtree *); // 가장 작은 키 값을 가진 노드 반환
node_t *rbtree_max(const rbtree *);// 가장 큰 키 값을 가진 노드를 반환.

int rbtree_erase(rbtree *, node_t *); // 노드 삭제

int rbtree_to_array(const rbtree *, key_t *, const size_t); // RB 트리에 모든 키 값을 배열로 변환

#endif  // _RBTREE_H_
