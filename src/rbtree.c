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
    while (z->parent->color == RBTREE_RED){
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
            return p;
        else if (p->key > key)
            p = p->left;
        else
            p = p->right;    
    }
    return NULL;
}

node_t *rbtree_min(const rbtree *t) {
    // TODO: implement find
    if (t->root == t->nil)
        return NULL;

    node_t* cur = t->root;
    while(cur->left != t->nil){
        cur = cur->left;
    }

    return cur;
}

node_t *rbtree_max(const rbtree *t) {
    // TODO: implement find
    if (t->root == t->nil)
        return NULL;

    node_t* cur = t->root;
    while(cur->right != t->nil){
        cur = cur->right;
    }

    return cur;
}

void rbtree_delete_fixup(rbtree* t, node_t * successor) {
    while (successor != t->root && successor->color == RBTREE_BLACK){
        if (successor == successor->parent->right){
            node_t* brother = successor->parent->left;
            if(brother->color == RBTREE_RED){
                brother->color = RBTREE_BLACK;
                successor->parent->color =RBTREE_RED;
                right_rotation(t,successor->parent);
                brother = successor->parent->left;
            }
            if (brother->right->color == RBTREE_BLACK && brother->left->color == RBTREE_BLACK){
                brother->color = RBTREE_RED;
                successor = successor->parent;
            }
            else{
                if(brother->left->color == RBTREE_BLACK){
                    brother->right->color = RBTREE_BLACK;
                    brother->color = RBTREE_RED;
                    left_rotation(t,brother);
                    brother = successor->parent->left;
                }
                brother->color = successor->parent->color;
                successor->parent->color = RBTREE_BLACK;
                brother->left->color = RBTREE_BLACK;
                right_rotation(t,successor->parent);
                successor = t->root;
            }
        }
        else{
            node_t* brother = successor->parent->right;
            if (brother->color == RBTREE_RED){
                brother->color = RBTREE_BLACK;
                successor->parent->color = RBTREE_RED;
                left_rotation(t,successor->parent);
                brother = successor->parent->right;
            }
            if (brother->left->color == RBTREE_BLACK && brother->right->color == RBTREE_BLACK){
                brother->color = RBTREE_RED;
                successor = successor->parent;
            }
            else{
                if (brother->right->color == RBTREE_BLACK){
                    brother->left->color = RBTREE_BLACK;
                    brother->color = RBTREE_RED;
                    right_rotation(t,brother);
                    brother = successor->parent->right;
                }

                brother->color = successor->parent->color;
                successor->parent->color = RBTREE_BLACK;
                brother->right->color = RBTREE_BLACK;
                left_rotation(t,successor->parent);
                successor = t->root;
            }
        }
    }
    successor->color = RBTREE_BLACK;
}

void rbtree_transplant(rbtree *t, node_t * u, node_t * v) {
  if (u->parent == t->nil) {
    t->root = v;
  }
  else if (u == u->parent->left) {
    u->parent->left = v;
  }
  else {
    u->parent->right = v;
  }
  
  v->parent = u->parent;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  node_t *y;
  node_t *x;
  color_t yOriginalColor;

  y = p;
  yOriginalColor = y->color;

  if (p->left == t->nil) {
    x = p->right;
    rbtree_transplant(t, p, p->right);
  }
  else if (p->right == t->nil) {
    x = p->left;
    rbtree_transplant(t, p, p->left);
  }
  else {
    y = p->right;
    while(y->left != t->nil){
      y = y->left;
    }
    yOriginalColor = y->color;
    x = y->right;

    if (y->parent == p) {
      x->parent = y;
    } 
    else {
      rbtree_transplant(t, y, y->right);
      y->right = p->right;
      y->right->parent = y;
    }

    rbtree_transplant(t, p, y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }

  if (yOriginalColor == RBTREE_BLACK) {
    rbtree_delete_fixup(t, x);
  }

  free(p);

  return 0;
}

void dfs_preorder(const rbtree *t, node_t* cur, key_t* arr, size_t* count, const size_t n){
    if (cur == t->nil)
        return;

    dfs_preorder(t,cur->left,arr,count,n);
    
    if (*count < n)
        arr[(*count)++] = cur->key;
    else
        return;
    dfs_preorder(t,cur->right,arr,count,n);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
    // TODO: implement to_array
    // dfs방식으로 preorder를 사용하여 넣으면 될듯하다
    
    if (t->root == t->nil)
        return 0;

    size_t count = 0;

    dfs_preorder(t,t->root,arr, &count,n);

    return 0;
}
