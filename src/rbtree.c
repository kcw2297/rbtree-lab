#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
    rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
    // TODO: initialize struct if needed
    node_t* nilNode = (node_t*)malloc(sizeof(node_t));

    nilNode->color = RBTREE_BLACK;

    p->nil = nilNode;
    p->root = nilNode;

  return p;
}

void delete_rbtree(rbtree *t) {
    // TODO: reclaim the tree nodes's memory
    free(t);
}

void right_rotation(rbtree*tree, node_t* x){
    node_t* y;

    y = x->left;
    x->left = y->right;

    if (y->right != tree->nil)
        y->right->parent = x;

    y->parent = x->parent;

    if(x->parent == tree->nil)
        tree->root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->right = x;
    x->parent = y;
}

void left_rotation(rbtree * tree, node_t * x){
    // TODO!
    node_t *y;

    y = x->right;
    x->right = y->left;
    
    if (y->left != tree->nil) 
        y->left->parent = x;
    
    y->parent = x->parent;
    
    if (x->parent == tree->nil) 
        tree->root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else 
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}

void rbtree_insert_fixup(rbtree*t, node_t* z){
    // 새로운 추가된 노드는 2 consecutive rule만 확인해주면된다
    // 2 consecutive rule이 지속될때까지 돌린다
    while (z->parent == RBTREE_RED){
        // Case 1: right tree
        if (z->parent == z->parent->parent->right){
            node_t* A = z->parent->parent->left;

            // A가 빨강인 경우 - color flip(GP-> red, P&A -> black)
            if(A->color == RBTREE_RED){
                A->color = RBTREE_BLACK;
                z->parent->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                z = z->parent->parent; // 이후 GP를 기준으로 다시 검사실시
            }
            else{
                // A가 블랙인 경우 - rotate - color flip(P-> black, GP->red)
                if (z->parent->left == z){
                    z = z->parent;
                    right_rotation(t, z);
                }
                z->parent->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                left_rotation(t, z->parent->parent);
            }
        }
        // Case 2: left tree
        else{
            node_t* A = z->parent->parent->right;

            // A가 빨강인 경우
            if (A-> color == RBTREE_RED){
                A->color = RBTREE_BLACK;
                z->parent->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                z = z->parent->parent;
            }
            else{
                // A가 블랙인 경우 - rotate - color flip(P-> black, GP->red)
                if (z->parent->right == z){
                    z = z->parent;
                    left_rotation(t,z);
                }
                z->parent->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                right_rotation(t, z->parent->parent);
            }

        }
    }
    t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
    // TODO: implement insert
    node_t* p = t->root;
    node_t* parent = t->nil;

    node_t* newNode = (node_t*)malloc(sizeof(node_t));
    newNode->left = t->nil;
    newNode->right = t->nil;
    newNode->key = key;
    newNode->color = RBTREE_RED;

    while (p != t->nil){
        parent = p;
        if (parent->key < newNode->key)
            p = p->right;
        else
            p = p->left;
    }

    if (parent == t->nil)
        t->root = newNode;
    else{
        if (parent->key > newNode->key)
            parent->left = newNode;
        else
            parent->right = newNode;
    }

    newNode->parent = parent;
    
    rbtree_insert_fixup(t,newNode);

    return newNode;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
    // TODO: implement find

    node_t* p = t->root;
    
    while (p != t->nil){
        if (p->key == key)
            break;
        else if (p->key > key)
            p = p->left;
        else
            p = p->right;    
    }
    if (p == t->nil)
        return NULL;
        
    return p;
}

node_t *rbtree_min(const rbtree *t) {
    // TODO: implement find
    return t->root;
}

node_t *rbtree_max(const rbtree *t) {
    // TODO: implement find
    return t->root;
}


void rbtree_transplant(rbtree* t, node_t* deleteNode, node_t* deleteNodeChild){
    if (deleteNode->parent == t->nil)
        t->root = deleteNodeChild;
    else if (deleteNode == deleteNode->parent->left)
        deleteNode->parent->left = deleteNodeChild;
    else
        deleteNode->parent->right = deleteNodeChild;

    deleteNodeChild->parent = deleteNode->parent;
}


void rbtree_delete_fixup(rbtree *t, node_t *x) {
  while (x != t->root && x->color == RBTREE_BLACK) {
    // CASE 1 ~ 4 : LEFT CASE
    if (x == x->parent->left){
      node_t *w = x->parent->right;
      
      // CASE 1 : x의 형제 w가 적색인 경우
      if (w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotation(t, x->parent);
        w = x->parent->right;
      }

      // CASE 2 : x의 형제 w는 흑색이고 w의 두 지식이 모두 흑색인 경우
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      }

      // CASE 3 : x의 형제 w는 흑색, w의 왼쪽 자식은 적색, w의 오른쪽 자신은 흑색인 경우
      else{ 
        if (w->right->color == RBTREE_BLACK) {
            w->left->color = RBTREE_BLACK;
            w->color = RBTREE_RED;
            right_rotation(t, w);
            w = x->parent->right;
        }

        // CASE 4 : x의 형제 w는 흑색이고 w의 오른쪽 자식은 적색인 경우
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotation(t, x->parent);
        x = t->root;
      }
    }
    // CASE 5 ~ 8 : RIGHT CASE
    else {
      node_t *w = x->parent->left;

      // CASE 5 : x의 형제 w가 적색인 경우
      if (w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotation(t, x->parent);
        w = x->parent->left;
      }

      // CASE 6 : x의 형제 w는 흑색이고 w의 두 지식이 모두 흑색인 경우
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      }

      // CASE 7 : x의 형제 w는 흑색, w의 왼쪽 자식은 적색, w의 오른쪽 자신은 흑색인 경우
      else 
      {
          if (w->left->color == RBTREE_BLACK) {
            w->right->color = RBTREE_BLACK;
            w->color = RBTREE_RED;
            left_rotation(t, w);
            w = x->parent->left;
            }

        // CASE 8 : x의 형제 w는 흑색이고 w의 오른쪽 자식은 적색인 경우
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotation(t, x->parent);
        x = t->root;
      }
    }
  }

  x->color = RBTREE_BLACK;
}


int rbtree_erase(rbtree *t, node_t *p) {
    // TODO: implement erase
    node_t* y;
    node_t* x;
    color_t yOriginalColor;

    y = p;
    yOriginalColor = y->color;

    // 삭제하려는 노드의 왼쪽 or 오른쪽 자녀가 없는 경우
    // 삭제하려는 노드가 삭제되는 색
    if (p->left == t->nil){
        x = p->right;
        rbtree_transplant(t,p,p->right);
    }
    else if (p->right == t->nil){
        x = p->left;
        rbtree_transplant(t,p,p->left);
    }
    else{
        y = p->right;
        while(y->left != t->nil){
            y = y->left;
        }
        yOriginalColor = y->color;
        x = y->right;

        if (y->parent == p){
            x->parent = y;
        }
        else {
            rbtree_transplant(t,y,y->right);
            y->right = p->right;
            y->right->parent = y;
        }
        
        rbtree_transplant(t,p,y);
        y->left = p->left;
        y->left->parent = y;
        y->color = p->color;
    }

    if (yOriginalColor == RBTREE_BLACK)
        rbtree_delete_fixup(t,x);

    free(p);
    return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
    // TODO: implement to_array
    return 0;
}
