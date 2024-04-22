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


// 노드 삽입하고 불균형을 복구하는 함수
node_t *rbtree_insert(rbtree *t, const key_t key) {

  // 새노드 생성
  node_t *new_node = (node_t *)calloc(1, sizeof(rbtree));
  new_node->color = RBTREE_RED; // 항상 레드로 추가

  new_node->left = t->nil; //추가한 노드의 왼쪽 자식 nil 노드로 설정
  new_node->right = t->nil; // 추가한 노드의 오른쪽 자식 nil 노드로 설정 

  new_node->key = key;
  
  //새 노드를 삽입할 위치 탐색
  node_t *current = t->root;
  while (current != t->nil)
  {

    //키 값이 탐색중인 노드보다 작아? > 왼쪽자식으로 이동
    if(key < current->key)
    {
      // 탐색 중인 노드의 자식 노드가 nil노드야?
      if (current->left = t->nil)
      {
        current->left = new_node; // 새 노드를 해당 자식 노드로 추가
        break;
      }
      //왼쪽으로 이동(탐색)
      current = current->left;
    }

    //키 값이 탐색중인 노드보다 클 때
    else
    {
      // 
      if (current->right == t->nil)
      {
        current->right = new_node;
        break;
      }
      current = current->right;
    }
  }
  
  new_node->parent = current; // 새 노드의 부모지정

  // root 가 nil(트리가 비어있어?) > 새 노드를 트리의 루트로 지정
  if(current == t->nil)
  {
    t->root = new_node;
  }
  rbtree_insert_fixup(t, new_node);
  return new_node;
}

node_t *rbtree_insert_fixup(rbtree *t, node_t *node)
{
  // 새 노드 기준
  node_t *parent = node->parent; //부모
  node_t * grand_parent = parent->parent;//조부모
  node_t * uncle;//삼촌

  int is_left = node = parent->left; // 현재 노드가 왼쪽 자식이야?
  int is_parent_is_left; // 부모가 왼쪽 자식이야?

  // 추가한 노드가 root 노드야? > 그러면 색만 변경
  if(node == t->root)
  {
    node->color - RBTREE_BLACK;
  }

  // 부모가 블랙이야? 그럼 문제 없다.
  if(parent->color == RBTREE_BLACK)
    return;

  //조부모의 왼쪽 자식이 부모야? 그러면 삼촌은 조부모의 오른쪽 자식
  is_parent_is_left = grand_parent->left == parent;
  uncle = (is_parent_is_left) ? grand_parent->right : grand_parent->left;

  if (uncle->color == RBTREE_RED)
  {
    parent->color = RBTREE_BLACK;
    uncle->color = RBTREE_BLACK;
    grand_parent->color = RBTREE_RED;
    rbtree_insert_fixup(t, grand_parent);
    return;
  }

  

  if (is_parent_is_left)
  {
    //여기서 부터는 부모의 헝제가 black
    if(is_left)

    // case 2 : 부모, 현재가 왼쪽
    {
      right_rotate(t, parent);
      exchange_color(parent, parent->right);//부모와 형제 노드의 색상 바꿈
      return;
    }
    // case 3 : 부모 왼쪽, 현재 오른쪽
    left_rotate(t, node); // 부모가 왼쪽이므로 왼쪽 부터
    right_rotate(t, node);
    exchange_color(node, node->right); // 새 노드와 자식 노드의 색상 바꿈
    return;
  }

  if (is_left)
  {
    // case 3 : 부모 오른쪽, 현재 왼쪽
    right_rotate(t, node);// 부모가 오른쪽이므로 오른쪽 부터
    left_rotate(t, node);
    exchange_color(node, node->left); // 새 노드와 
    return;
  }

  // case 2 : 부모 오른쪽, 현재 오른쪽
  left_rotate(t, parent);
  exchange_color(parent, parent->left);

}

//오른쪽 회전하는 함수
void right_rotate(rbtree *t, node_t *node)
{
  node_t *parent = node->parent;
  node_t *grand_parent = parent->parent;
  node_t *node_right = node->right;

  if (parent == t->root)
    t->root = node;
  else{
    // 1.1 노드의 부모를 grand_parent로 변경
    if (grand_parent->left == parent)
      grand_parent->left = node;
    else
      grand_parent->right = node;
  }
  node->parent = grand_parent; // 1.2 노드를 grand_parent 자식으로 변경( 양방향 연결)

  parent->parent = node; // 2.1 parent의 부모를 노드로 변경
  node->right = parent; // 2.2 parent를 노드의 자식으로 변경(양방향 연결)

  node_right->parent = parent; // 3.1 노드의 자식의 부모를 parent로 변경
  parent->left = node->right; // 3.2 노드의 자식을 부모의 자식으로 변경(양방향 연결)
}

void left_rotate(rbtree *t, node_t *node)
{
  node_t *parent = node->parent;
  node_t *grand_parent = parent->parent;
  node_t *node_left = node_left;

  if(parent == t->root)
    t->root = node;
  else{

    //1.1 노드의 부모를 grand_parent로 변경
    if (grand_parent->left == parent)
      grand_parent->left = node;
    else
      grand_parent->right = node;
  }
  node->parent = grand_parent; //1.2 노드를 grand_parent 자식으로 변경(양방향 연결)
  
  parent->parent = node; // 2.1  parent의 부모를 노드로 변경
  node->left = parent; // 2.2 parent를 노드의 자식으로 변경(양방향)

  node_left->parent = parent; // 3.1 노드의 자식의 부모를 parent로 변경
  parent->right = node_left; // 3.2 노드의 자식을 부모의 자식으로 변경(양방향)
  
}

//노드 검색
node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *current = t->root;
  while (current != t->nil)
  {
    if (key == current->key)
      return current;
    else
      current = (key < current->key) ? current->left : current->right;
  }
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  node_t *current = t->root;
  while (current != t->nil)
  {
    current = current->left;
  }
  return current;
}
node_t *rbtree_max(const rbtree *t) {
  node_t *current = t->root;
  while (current!= t->nil)
  {
    current = current->right;
  }
  return current;
}

//노드 삭제
int rbtree_erase(rbtree *t, node_t *p) {
  
  return 0;
}
// 트리를 배열로 변환 > 중위 순회로 구현
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  node_t *current = rbtree_min(t);
  arr[0] = current->key;

  for (int i = 1; i < n; i++)
  {
    if (current == t->nil)
      break; // 노드가 끝까지 탐색되었어? > loop 탈출
    current = get_next_node(t, current); // 다음 노드로 이동

    if (current == t->nil)
      break; // 노드가 끝까지 탐색된 경우 loop 탈출
    arr[i] = current->key; // 현재 노드의 key 값을 배열에 저장
  }

  return 0;
}

//
node_t *get_next_node(const rbtree *t, node_t *p)
{
  node_t *current = p->right;
  if(current == t->nil) // 오른쪽 자식이 없으면 
  {
    current = p; // 본인이 current가 된다.
    while (1)
    {
      if ( current->parent->right == current) // currnet가 오른쪽 자식인 경우
        current = current->parent; // 부모 노드로 이동 후 이어서 탐색
      else
        return current->parent; // current가 왼쪽 자식인 경우 부모 리턴
    }
  }

  while (current->left != t->nil) // 왼쪽 자식이 있어?
    current = current->left; // 왼쪽 끝으로 이동
  return current;
  
}
