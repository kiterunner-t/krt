/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "kmisc.h"
#include "item.h"
#include "redblack_tree.h"


typedef enum redblack_color_s redblack_color_e;
typedef struct redblack_node_s redblack_node_t;


enum redblack_color_s {
  BLACK = 0,
  RED
};


struct redblack_node_s {
  redblack_node_t  *left;
  redblack_node_t  *right;
  redblack_color_e  color;
  void             *item;
};


struct redblack_tree_s {
  redblack_node_t *root;
  redblack_node_t  dummy;
  size_t           size;
  size_t           rotate_count;
  item_op_t       *op;
};


static void _redblack_node_free(redblack_tree_t *rb, redblack_node_t *h);
static redblack_node_t *_redblack_node_new(void *item, redblack_node_t *l, 
        redblack_node_t *r, redblack_color_e c);
static redblack_node_t *_redblack_rotate_with_right(redblack_node_t *h);
static redblack_node_t *_redblack_rotate_with_left(redblack_node_t *h);
static redblack_node_t *_redblack_insert(redblack_tree_t *rb, 
        redblack_node_t *h, void *item);
static void _redblack_reblance(redblack_tree_t *rb, void *item, 
        redblack_node_t *current, redblack_node_t *parent,
        redblack_node_t *grand, redblack_node_t *great);
static redblack_node_t *_r_redblack_insert(redblack_tree_t *rb, 
        redblack_node_t *h, void *item, int sw);
static void _redblack_print(redblack_tree_t *rb, redblack_node_t *r);
static redblack_node_t *_redblack_successor(redblack_tree_t *rb, 
        redblack_node_t **del, redblack_node_t *current, void *item);
static void _redblack_delete_reblance(redblack_tree_t *rb, 
        redblack_node_t *grand, redblack_node_t *parent, 
        redblack_node_t *current, redblack_node_t *sibling);
static void _redblack_free_node(redblack_tree_t *rb, redblack_node_t *node);


redblack_tree_t *
redblack_tree_init(item_op_t *op, void *max)
{
  redblack_tree_t *rb;
  redblack_node_t *dummy;
  redblack_node_t *root;

  rb = (redblack_tree_t *) malloc(sizeof(*rb));
  if (rb == NULL)
    return NULL;

  root = (redblack_node_t *) malloc(sizeof(*root));
  if (root == NULL) {
    free(rb);
    return NULL;
  }

  dummy = &rb->dummy;
  dummy->left = dummy;
  dummy->right = dummy;
  dummy->color = BLACK;
  dummy->item = 0;

  root->left = dummy;
  root->right = dummy;
  root->color = BLACK;
  root->item = max;

  rb->root = root;
  rb->op = op;
  rb->rotate_count = 0;
  return rb;
}


void
redblack_tree_destroy(redblack_tree_t *rb)
{
  if (rb == NULL)
    return ;

  _redblack_node_free(rb, rb->root);
}


size_t
redblack_tree_size(redblack_tree_t *rb)
{
  return rb->size;
}


size_t
redblack_tree_rotate_count(redblack_tree_t *rb)
{
  return rb->rotate_count;
}


static void
_redblack_node_free(redblack_tree_t *rb, redblack_node_t *h)
{
  if (h == &rb->dummy)
    return ;

  _redblack_node_free(rb, h->left);
  _redblack_node_free(rb, h->right);

  if (rb->op->free != NULL)
    (rb->op->free)(h->item);
  free(h);
}


static redblack_node_t *
_redblack_node_new(void *item, redblack_node_t *l, redblack_node_t *r, 
                   redblack_color_e c)
{
  redblack_node_t *node;

  node = (redblack_node_t *) malloc(sizeof(*node));
  if (node == NULL)
    return NULL;

  node->item = item;
  node->left = l;
  node->right = r;
  node->color = c;
  return node;
}


int
redblack_tree_insert(redblack_tree_t *rb, void *item)
{
  redblack_node_t *r;

#ifdef RECURSIVE
  r = _r_redblack_insert(rb, rb->root, item, 0);
#else
  r = _redblack_insert(rb, rb->root, item);
#endif
  if (r == NULL)
    return -1;

  rb->size++;
  rb->root = r;
  return 0;
}


static void
_redblack_delete_reblance(redblack_tree_t *rb, redblack_node_t *grand, 
                          redblack_node_t *parent, redblack_node_t *current, 
                          redblack_node_t *sibling)
{
  redblack_node_t *gg;
  redblack_node_t *dummy = &rb->dummy;

  if (sibling->left->color==BLACK && sibling->right->color==BLACK) {
    parent->color = BLACK;
    current->color = RED;
    if (sibling != dummy)
      sibling->color = RED;

    return ;
  }

  if (parent->right == sibling) {
    if (sibling->left->color == RED) {
      parent->color = BLACK;
      current->color = RED;
      parent->right = _redblack_rotate_with_right(sibling);
      gg = _redblack_rotate_with_left(parent);

    } else {
      parent->color = BLACK;
      current->color = RED;
      sibling->color = RED;
      sibling->right->color = BLACK;
      gg = _redblack_rotate_with_left(parent);
    }

  } else {
    if (sibling->right->color == RED) {
      parent->color = BLACK;
      current->color = RED;
      parent->left = _redblack_rotate_with_left(sibling);
      gg = _redblack_rotate_with_right(parent);

    } else {
      parent->color = BLACK;
      current->color = RED;
      sibling->color = RED;
      sibling->left->color = BLACK;
      gg = _redblack_rotate_with_right(parent);
    }
  }

  if (grand->left == parent)
    grand->left = gg;
  else
    grand->right = gg;
}


void
redblack_tree_delete(redblack_tree_t *rb, void *item)
{
  redblack_node_t *delnode = NULL;
  redblack_node_t *grand;
  redblack_node_t *parent;
  redblack_node_t *current;
  redblack_node_t *sibling;
  redblack_node_t *gg;
  redblack_node_t *dummy = &rb->dummy;
  void            *t;

  grand = rb->root;
  parent = grand;
  parent->color = RED;
  sibling = parent->right;
  current = parent->left;
  while (current != dummy) {
    assert(parent->color == RED);

    if (current->left->color==BLACK && current->right->color==BLACK) 
      _redblack_delete_reblance(rb, grand, parent, current, sibling);

    grand = parent;
    parent = current;
    current = _redblack_successor(rb, &delnode, current, item);
    sibling = (parent->left == current) ? parent->right : parent->left;

    if (current->color == RED) {
      grand = parent;
      parent = current;
      current = _redblack_successor(rb, &delnode, current, item);
      sibling = (parent->left == current) ? parent->right : parent->left;

    } else if (current->left->color==RED || current->right->color==RED) {
      parent->color = RED;
      sibling->color = BLACK;

      if (parent->left == current) {
        gg = _redblack_rotate_with_left(parent);
        sibling = parent->left;
      } else {
        gg = _redblack_rotate_with_right(parent);
        sibling = parent->right;
      }

      if (grand->left == parent) 
        grand->left = gg;
      else
        grand->right = gg;

      grand = gg;
    }
  }

  if (delnode == NULL)
    return ;

  assert(parent->left==dummy || parent->right==dummy);

  if (parent->left==dummy && parent->right==dummy) 
    gg = dummy;
  else
    gg = (parent->left != dummy) ? parent->left : parent->right;

  if (grand->left == parent)
    grand->left = gg;
  else
    grand->right = gg;

  t = delnode->item;
  delnode->item = parent->item;
  parent->item = t;

  _redblack_free_node(rb, parent);
  rb->root->color = BLACK;
  rb->root->left->color = BLACK;
  rb->dummy.color = BLACK;
  rb->size--;
}


static redblack_node_t *
_redblack_successor(redblack_tree_t *rb, redblack_node_t **del, 
                    redblack_node_t *current, void *item)
{
  long n;

  n = (rb->op->cmp)(item, current->item);
  if (n == 0) 
    *del = current;

  current = (n < 0) ? current->left : current->right;
  return current;
}


static void
_redblack_free_node(redblack_tree_t *rb, redblack_node_t *node)
{
  item_free_pt item_free = rb->op->free;

  if (item_free)
    item_free(node->item);
  free(node);
}


static redblack_node_t *
_redblack_insert(redblack_tree_t *rb, redblack_node_t *h, void *item)
{
  redblack_node_t *current;
  redblack_node_t *parent;
  redblack_node_t *grand;
  redblack_node_t *great;
  redblack_node_t *dummy = &rb->dummy;
  redblack_node_t *node;
  item_cmp_pt      cmp = rb->op->cmp;

  node = _redblack_node_new(item, dummy, dummy, RED);
  if (node == NULL)
    return NULL;

  current = rb->root;
  parent = dummy;
  grand = dummy;
  great = dummy;

  while (current != dummy) {
    great = grand;
    grand = parent;
    parent = current;
    current = (cmp(item, current->item) < 0) ? current->left : current->right;
    if (current == dummy)
      break;

    if (current->left->color==RED && current->right->color==RED)
      _redblack_reblance(rb, item, current, parent, grand, great);
  }

  if (cmp(item, parent->item) < 0)
    parent->left = node;
  else
    parent->right = node;

  _redblack_reblance(rb, item, node, parent, grand, great);
  rb->root->left->color = BLACK;
  return rb->root;
}


static void
_redblack_reblance(redblack_tree_t *rb, void *item,
                   redblack_node_t *current, redblack_node_t *parent, 
                   redblack_node_t *grand, redblack_node_t *great)
{
  int              gx;
  int              px;
  item_cmp_pt      cmp = rb->op->cmp;
  redblack_node_t *gg;

  current->color = RED;
  current->left->color = BLACK;
  current->right->color = BLACK;

  if (parent->color == BLACK) 
    return ;

  gx = cmp(item, grand->item);
  px = cmp(item, parent->item);
  if (gx<0 && px<0) {
    grand->color = RED;
    parent->color = BLACK;
    gg = _redblack_rotate_with_right(grand);
    rb->rotate_count++;

  } else if (gx>0 && px>0) {
    grand->color = RED;
    parent->color = BLACK;
    gg = _redblack_rotate_with_left(grand);
    rb->rotate_count++;

  } else if (gx<0 && px>0) {
    grand->color = RED;
    current->color = BLACK;
    grand->left = _redblack_rotate_with_left(parent);
    gg = _redblack_rotate_with_right(grand);
    rb->rotate_count++;

  } else if (gx>0 && px<0) {
    grand->color = RED;
    current->color = BLACK;
    grand->right = _redblack_rotate_with_right(parent);
    gg = _redblack_rotate_with_left(grand);
    rb->rotate_count++;
  }

  if (cmp(item, great->item) < 0)
    great->left = gg;
  else
    great->right = gg;
}


static redblack_node_t *
_redblack_rotate_with_right(redblack_node_t *h)
{
  redblack_node_t *hl;

  hl = h->left;
  h->left = hl->right;
  hl->right = h;
  return hl;
}


static redblack_node_t *
_redblack_rotate_with_left(redblack_node_t *h)
{
  redblack_node_t *hr;

  hr = h->right;
  h->right = hr->left;
  hr->left = h;
  return hr;
}


static redblack_node_t *
_r_redblack_insert(redblack_tree_t *rb, redblack_node_t *h, void *item, int sw)
{
  printf("not support\n");
  return NULL;
}


void
redblack_tree_print(redblack_tree_t *rb)
{
  if (rb->size == 0) {
    printf("empty red black tree\n");
    return ;
  }

  _redblack_print(rb, rb->root->left);
}


static void
_redblack_print(redblack_tree_t *rb, redblack_node_t *r)
{
  item_print_pt print = rb->op->print;

  if (r == &rb->dummy)
    return ;

  _redblack_print(rb, r->left);
  print(r->item);
  printf("\n");
  _redblack_print(rb, r->right);
}

