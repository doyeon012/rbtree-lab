#include "rbtree.h"

#include <stdlib.h>

//두 노드의 색상을 교환하는 유틸리티 함수
void exchange_color(node_t *a, node_t *b)
{
  int tmp = a->color;
  a->color = b->color;
  b->color = (tmp == RBTREE_BLACK) ? RBTREE_BLACK : RBTREE_RED;
}

//오른쪽 회전
void right_rotate(rbtree *t, node_t *node)
{
  node_t *parent = node->parent;
  node_t *grand_parent = parent->parent;
  node_t *node_right = node->right;

  if (parent == t->root)
    t->root = node;
  else{
    // 1.1 노드의 부모 > grand_parent
    if (grand_parent->left == parent)
      grand_parent->left = node;
    else
      grand_parent->right = node;
  }
  node->parent = grand_parent; // 1.2 노드의 부모 = 조부모(양방향)

  parent->parent = node; // 2.1  부모의 부모 = 노드
  node->right = parent; // 2.2 노드 자식(왼쪽) = 부모(양방향)

  node_right->parent = parent; // 3.1 노드의 자식의 부모 = 부모
  parent->left = node_right; // 부모의 오른쪽 자식 = 노드 왼쪽 자식(양방향)
}
//왼쪽 회전
void left_rotate(rbtree *t, node_t *node)
{
  node_t *parent = node->parent;
  node_t *grand_parent = parent->parent;
  node_t *node_left = node->left;

  if(parent == t->root)
    t->root = node;
  else{
    //1.1 조부모의 자식 = 노드
    if (grand_parent->left == parent)
      grand_parent->left = node;
    else
      grand_parent->right = node;
  }
  node->parent = grand_parent; //1.2 노드의 부모 = 조부모(양방향)
  
  parent->parent = node; // 2.1  부모의 부모 = 노드
  node->left = parent; // 2.2 노드 자식(왼쪽) = 부모(양방향)

  parent->right = node_left; // 부모의 오른쪽 자식 = 노드 왼쪽 자식(양방향)
  node_left->parent = parent; // 3.1 노드의 자식의 부모 = 부모
}


// 레드-블랙 트리 생성
rbtree *new_rbtree(void)
{
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

//노드 메모리 해제 함수(후위 순회)
void free_node(rbtree *t, node_t *node)
{
  if(node->left != t->nil)
    free_node(t, node->left);

  if(node->right != t->nil)
    free_node(t, node->right);

  //현재 노드의 메모리를 반환
  free(node);
}

// 레드-블랙 트리 해제 함수
void delete_rbtree(rbtree *t) {
  node_t *node = t->root;

  //트리를 순회하면서 각 노드의 메모리를 반환하는 함수
  if (node != t->nil)
    free_node(t, node);
  
  //nil 노드와 rbtree 구조체의 메모리를 반환
  free(t->nil);
  free(t);
}

//삽입 불균형 복구
void rbtree_insert_fixup(rbtree *t, node_t *node)
{
  // 새 노드 기준
  node_t *parent = node->parent; //부모
  node_t *grand_parent = parent->parent;//조부모
  node_t *uncle;//삼촌

  int is_left = node == parent->left; // 현재 노드가 왼쪽 자식이야?
  int is_parent_is_left; // 부모가 왼쪽 자식이야?

  // 추가한 노드가 root인 경우, 색만 변경하고 종료
  if(node == t->root)
  {
    node->color = RBTREE_BLACK;
    return;
  }

  // 부모가 블랙이야?
  if(parent->color == RBTREE_BLACK)
    return;

  //조부모의 왼쪽 자식이 부모야?
  is_parent_is_left = grand_parent->left == parent;
  uncle = (is_parent_is_left) ? grand_parent->right : grand_parent->left; //삼촌은 조부모의 오른쪽 자식

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

    {// case 2 : 부모, 현재(왼)
      right_rotate(t, parent);
      exchange_color(parent, parent->right);//부모와 형제 노드의 색상 바꿈
      return;
    }
    // case 3 : 부모(왼), 현재(오)
    left_rotate(t, node); // 부모가 왼쪽이므로 왼쪽 부터
    right_rotate(t, node);
    exchange_color(node, node->right); // 새 노드와 자식 노드의 색상 바꿈
    return;
  }

  if (is_left)
  {
    // case 3 : 부모(오), 현재(왼) 
    right_rotate(t, node);// 부모가 오른쪽이므로 오른쪽 부터
    left_rotate(t, node);
    exchange_color(node, node->left); // 새 노드와 
    return;
  }
  // case 2 : 부모, 현재(오)
  left_rotate(t, parent);
  exchange_color(parent, parent->left);

}

// 노드 삽입
node_t *rbtree_insert(rbtree *t, const key_t key) {

  // 새노드 생성
  node_t *new_node = (node_t *)calloc(1, sizeof(node_t));
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
      if (current->left == t->nil)
      {
        current->left = new_node; // 새 노드를 해당 자식 노드로 추가
        break;
      }
      current = current->left;
    }

    else
    {
      
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

//노드 검색
node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *current = t->root;

  while (current != t->nil)
  {
    if (key == current->key) //찾았을 때
      return current;
    else
      current = (key < current->key) ? current->left : current->right;
  }

  return NULL;
}

//최솟값 조회
node_t *rbtree_min(const rbtree *t)
{
  node_t *current = t->root;
  while (current->left != t->nil) // 트리에 nil을 만날때까지 왼쪽으로 
    current = current->left;
  return current;
}

// 최댓값 조회
node_t *rbtree_max(const rbtree *t)
{
  node_t *current = t->root;
  while (current->right != t->nil) // 트리에 nil을 만날때까지 오른쪽으로
    current = current->right;
  return current;
}

//다음 노드를 찾는 함수
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

  while (current->left != t->nil) // 왼쪽 자식이 있을 때
    current = current->left; // 왼쪽 끝으로 이동
  return current;
}

//트리를 중위 순회하여 배열에 저장
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  node_t *current = rbtree_min(t);
  arr[0] = current->key;
  for (int i = 1; i < n; i++)
  {
    if (current == t->nil)
      break;                             // 노드가 끝까지 탐색된 경우 loop 탈출
    current = get_next_node(t, current); // 다음 노드로 이동
    if (current == t->nil)
      break;               // 노드가 끝까지 탐색된 경우 loop 탈출
    arr[i] = current->key; // 현재 노드의 key 값을 배열에 저장
  }
  return 0;
}

//삭제 후 불균형을 복구하는 함수
// is_left(extra_black가 왼쪽 자식 확인), parent(extra_black의 부모)
void rbtree_erase_fixup(rbtree *t, node_t *parent, int is_left)
{
  node_t *extra_black = is_left ? parent->left : parent->right;

  //삭제 후 대체한 노드가 red인 경우, black으로 변경
  if (extra_black->color == RBTREE_RED)
  {
    extra_black->color = RBTREE_BLACK;
    return;
  }

  node_t *sibling = is_left ? parent->right : parent->left; // 형제
  node_t *sibling_left = sibling->left; // 형제의 왼쪽 자식
  node_t *sibling_right = sibling->right; // 형제의 오른쪽 자식

  // case1, 형제(red)
  if (sibling->color == RBTREE_RED)
  {
    if (is_left)
      left_rotate(t, sibling); 
    else
      right_rotate(t, sibling);

    exchange_color(sibling, parent);
    rbtree_erase_fixup(t, parent, is_left);
    return;
  }

  node_t *near = is_left ? sibling_left : sibling_right;    // 꺾인 자식
  node_t *distant = is_left ? sibling_right : sibling_left; // 펴진 자식

  //case2, 3, 4는 형제(black)
  //case3 꺾인 자식(red), 펴진 자식(black)
  if (is_left && near->color == RBTREE_RED && distant->color == RBTREE_BLACK)
  { 
    right_rotate(t, near);
    exchange_color(sibling, near);
    rbtree_erase_fixup(t, parent, is_left);
    return;
  }
  else if (near->color == RBTREE_RED && distant->color == RBTREE_BLACK)
  {
    left_rotate(t, near);
    exchange_color(sibling, near);
    rbtree_erase_fixup(t, parent, is_left);
    return;
  }

  // case4 꺾인 자식(black), 펴진 자식(red)
  if (is_left && distant->color == RBTREE_RED)
  { 
    left_rotate(t, sibling);
    exchange_color(sibling, parent);
    distant->color = RBTREE_BLACK;
    return;
  }
  else if (distant->color == RBTREE_RED)
  {
    right_rotate(t, sibling);
    exchange_color(sibling, parent);
    distant->color = RBTREE_BLACK;
    return;
  }

  // case2 형제의 자식들(black)
  sibling->color = RBTREE_RED;
  if (parent != t->root)
    rbtree_erase_fixup(t, parent->parent, parent->parent->left == parent); //위로 위임하고 재귀
}

//노드 삭제
int rbtree_erase(rbtree *t, node_t *delete) {
  node_t *remove; // delete를 대체함. 즉 사라지는 노드.
  node_t *remove_parent, *replace_node; //
  int is_remove_black, is_remove_left;

  //1. delete 삭제 후 대체할 replace_node 찾기
  //1.1 delete 노드의 자식이 둘이야? > delete의 키를 후계자 노드의 키 값으로 대체, 노드의 색은 delete의 색 유지
  if(delete->left != t->nil && delete->right != t->nil)
  {
    remove = get_next_node(t, delete); // 후계자 노드(오른쪽 서브트리에서 가장 작은 노드)
    replace_node = remove->right; 
    delete->key = remove->key; // delete의 키를 후계자 노드의 키값으로 대체(색 변경x)
  } 
  else
  {
  // delete의 노드의 자식이 없거나 or 하나인 경우 > delete 노드를 자식으로 대체, 노드의 색도 대체되는 노드의 색으로 변경
  remove = delete;
  // 대체할 노드 : 자식이 있어? > 자식 노드로, 없어?> nil 노드로 대체
  replace_node = (remove->right != t->nil) ? remove->right : remove->left;
  }
  remove_parent = remove->parent;

  //2. remove 노드 제거하기
  //2.1 remove 노드가 루트인 경우
  if (remove == t->root)
  {
    t->root = replace_node; // 대체할 노드를 트리의 루트로 지정
    t->root->color = RBTREE_BLACK; // 루트 노드느 항상 black 이므로 black로 설정
    free(remove); // remove 반환하고 종료
    return 0; // 불균형 복구 함수 호출 불필요(제거 전 트리에 노드가 하나 or 두개이므로 불균형x)
  }

  // 대체할 노드와 부모 노드 연결 2.1 remove 기준 부모와 자식 이어주기
  is_remove_black = remove->color; //remove 노드 제거 하기 전에 지워진 노드의 색 저장
  is_remove_left = remove_parent->left == remove;

  // 2.1.1 자식 연결
   if (is_remove_left) 
    remove_parent->left = replace_node;
  else // remove가 오른쪽 자식이었을 경우? > remove 부모의 오른쪽에 이어주기
    remove_parent->right = replace_node;

  // 2.1.2 부모 연결
  replace_node->parent = remove_parent;
  free(remove); //remove 지워.

  //case 1~4 remove 노드가 검정 노드인 경우
  //3. 불균형 복구 함수 호출
  if (is_remove_black)
    rbtree_erase_fixup(t, remove_parent, is_remove_left);
  return 0;
}
