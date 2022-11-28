#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree)); // malloc과 달리 배열 내 값들을 모두 0으로 초기화
  // TODO: initialize struct if needed
  p->nil = (node_t *)calloc(1, sizeof(node_t));
  
  p->nil->color = RBTREE_BLACK;
  p->root = p->nil;

  return p;
}

void delete_node(node_t* t, rbtree *tree)
{
    if (t == tree->nil)
    {
      return;
    }
    delete_node(t->left, tree);
    delete_node(t->right, tree);
    free(t);
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  delete_node(t->root,t);
  free(t->nil);
  free(t); // 동적할당 메모리를 반납. 꼬박꼬박 버릇처럼 쓰기.
}

void left_rotate(rbtree *t, node_t *x){
  node_t *y = x->right;
  x->right = y->left;
  
  if (y->left != t->nil){
    y->left->parent = x;
  }
  
  y->parent = x->parent;

  if (x->parent == t->nil){
    t->root = y;
  }
  else if (x == x->parent->left){
    x->parent->left = y;
  }
  else{
    x->parent->right = y;
  }

  y->left = x;
  x->parent = y;
}


void right_rotate(rbtree *t, node_t *y){
  node_t *x = y->left;
  y->left = x->right;
  
  if (x->right != t->nil){
    x->right->parent = y;
  }
  
  x->parent = y->parent;

  if (y->parent == t->nil){
    t->root = x;
  }
  else if (y == y->parent->left){
    y->parent->left = x;
  }
  else{
    y->parent->right = x;
  }

  x->right = y;
  y->parent = x;
}

void rb_insert_fixup(rbtree *t, node_t *z){
  while(z->parent->color == RBTREE_RED)
  {
    if(z->parent == z->parent->parent->left)
    {

      node_t *y = z->parent->parent->right;

      if (y->color == RBTREE_RED)
      {

        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;

      }
      else
      {
        if(z == z->parent->right){

        z = z->parent;
        /////// left_rotate 
        left_rotate(t, z);

        }

        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t, z->parent->parent);
      }
    }
    else
    {
      node_t *y = z->parent->parent->left;

      if (y->color == RBTREE_RED){

        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;

      }
      else
      {
        if(z == z->parent->left)
        {
          z = z->parent;
          /////// right_rotate 
          right_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *return_node = (node_t *)calloc(1, sizeof(node_t)); //집어넣는 노드
  
  node_t *y = t->nil;
  node_t *x = t->root;

  while(x != t->nil)
  {
      y = x;
      if (key < x->key)
      {
        x = x->left;
      }
      else{
        x = x->right;
      }
  }

  return_node->parent = y;

  if (y == t->nil)
  {
    t->root = return_node;
  }
  else if (key < y->key)
  {
    y->left = return_node;
  }
  else
  {
    y->right = return_node;
  }

  return_node->key = key;
  return_node->left = t->nil;
  return_node->right = t->nil;
  return_node->color = RBTREE_RED;

  rb_insert_fixup(t, return_node);
  ///rb_insert_fixup 구현하기

  return return_node;
}

// 없으면 NULL 반환
node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find

  node_t *x = t->root;
  
  while (x != t->nil){
    if(key < x->key){

      x = x->left;
    }
    else if (key > x->key)
    {
      x = x->right;
    }
    else
    {
      return x;
    }
  }
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find

  node_t *x = t->root;

  while (x->left != t->nil)
  {
    x = x->left;
  }
  return x;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find

  node_t *x = t->root;

  while (x->right != t->nil)
  {
    x = x->right;
  }
  return x;
}

void rb_transplant(rbtree *t, node_t *u, node_t *v) {
  //execuse transplant from u to v
  if (u->parent == t->nil)
  {
    t->root = v;
  }
  else if (u == u->parent->left)
  {
    u->parent->left = v;
  }
  else
  {
    u->parent->right = v;
  }

  v->parent = u->parent;
}

void rb_delete_fixup(rbtree *t, node_t *x){

  while ( x != t->root && x->color == RBTREE_BLACK ) {
    /* code */
    if ( x == x->parent->left ) 
    {
      node_t *w = x->parent->right;

      if ( w->color == RBTREE_RED ){

        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;

      }

      if ( w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        
        w->color = RBTREE_RED;
        x = x->parent;

      }
      else{
        if ( w->right->color == RBTREE_BLACK )
        {
        w->left->color = RBTREE_BLACK;
        w->color = RBTREE_RED;
        right_rotate(t,w);
        w = x->parent->right;
        }

        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root;
      }
    }
    
    else
    {
      node_t *w = x->parent->left;
      if ( w->color == RBTREE_RED )
      {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }

      if ( w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK)
      {
        w->color = RBTREE_RED;
        x = x->parent;
      }

      else{
        if ( w->left->color == RBTREE_BLACK )
        {

        w->right->color = RBTREE_BLACK;
        w->color = RBTREE_RED;
        left_rotate(t,w);
        w = x->parent->left;
        
        }

        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }

  x->color = RBTREE_BLACK;
}


int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  node_t *y = p;
  color_t y_org_color = y->color;
  node_t *x;

  if ( p->left == t->nil) 
  {

    x = p->right;
    rb_transplant(t, p, p->right);
  
  }
  else if ( p->right == t->nil ) 
  {

    x = p->left;
    rb_transplant(t, p, p->left);

  }
  else {

    y = p->right;
    while (y->left != t->nil)
    {
      y = y->left;
    }
    
    y_org_color = y->color;
    x = y->right;

    if ( y->parent == p ) {

      x->parent = y;

    }
    else {

      rb_transplant(t, y, y->right);
      y->right = p->right;
      y->right->parent = y;

    }
    rb_transplant(t, p, y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;    
  }

  if ( y_org_color == RBTREE_BLACK) {

    rb_delete_fixup(t, x);

  }
  free(p);
  return 0;
}

void inorder_traversing(const rbtree *t, node_t *curr, key_t *arr, int *count, const size_t n)
{
    if (curr == t->nil)
    {
        return;
    }
    
    if (*count < n)
    {
  
    inorder_traversing(t, curr->left, arr, count, n);

    arr[(*count)++] = curr->key;
    
    inorder_traversing(t, curr->right, arr, count, n);
    
    }
    else
    {
        return;
    }
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array

  if (t->root == t->nil)
  {
    return 0;
  }
  else
  {
    int a = 0;
    int *count = &a;
    inorder_traversing(t, t->root, arr, count, n);
  }

  return 0;
}
