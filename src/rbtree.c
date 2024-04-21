#include "rbtree.h"

#include <stdlib.h>

//메모리 : 새트리 생성
rbtree *new_rbtree(void) {

// tree 구조체 동적 할당
rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));

// nil 노드 생성 및 초기화
node_t *nil = (node_t *)calloc(1, sizeof(node_t));
nil->color = RBTREE_BLACK;

// tree의 nil과 root를 nil 노드로 설정(tree가 비었어? > root는 nil 노드여야 한다.)
p->nil = nil;
p->root = nil;

return p;
}

// 트리 삭제
void delete_rbtree(rbtree *t) {
  node_t * node = t->root;

  //트리를 순회하면서 각 노드의 메모리를 반환하는 함수
  if (node != t->nil)
  {
    free_node(t, node);
  }

  //nil 노드와 rbtree 구조체의 메모리를 반환
  free(t->nil);
  free(t);
}

//각 노드와 그 자식 노드들의 메모리를 반환하는 함수, 후위 순회방식
void free_node(rbtree *t, node_t *node)
{
  if(node->left != t->nil)
    free_node(t, node->left);

  if(node->right != t->nil)
    free_node(t, node->right);

  //현재 노드의 메모리를 반환
  free(node);
  node = NULL;
}


// 노드 삽입
node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  return t->root;
}

//노드 검색
node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}
node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}
node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

//노드 삭제
int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}
// 트리를 배열로 변환 > 중위 순회로 구현
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
