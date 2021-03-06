/**
 * Copyleft (C) KRT, 2012 2013 by kiterunner_t
 */

#include <stdlib.h>
#include <assert.h>
#include "kmisc.h"
#include "kitem.h"
#include "st.h"
#include "st_bst.h"

#ifndef RECURSION
#include "stack.h"
#endif


static st_bst_node_t *g_head;
static st_bst_node_t *g_dummy;
static kitem_op_t    *g_item_op;

// for delete
static st_bst_node_t *g_delete_node;
static st_bst_node_t *g_last_node;


static void _r_st_bst_destroy(st_bst_node_t *root);
static void _st_bst_destroy(st_bst_node_t *root);
static st_bst_node_t *_st_bst_insert(st_bst_node_t *root, kitem_t item);
static void _st_bst_free_node(st_bst_node_t *node);
static st_bst_node_t *_r_st_bst_delete(st_bst_node_t *root, kitem_t item);
static kitem_t _r_st_bst_search(st_bst_node_t *root, kkey_t key);
static kitem_t _st_bst_search(st_bst_node_t *root, kitem_t key);
static void _r_st_bst_sort(st_bst_node_t *root, st_visit_pt visit);
static st_bst_node_t *_st_bst_new_node(kitem_t item, st_bst_node_t *l, st_bst_node_t *r, size_t n);
static st_bst_node_t *_r_st_bst_insert(st_bst_node_t *root, kitem_t item);


kerrno_t
st_init(kitem_op_t *op)
{
  if (op == NULL)
    return KEINVALID_PARAM;

  g_head = _st_bst_new_node(op->null, NULL, NULL, 0);
  if (g_head == NULL)
    return KEMEM;

  g_dummy = g_head;
  g_dummy->left = g_dummy;
  g_dummy->right = g_dummy;
  g_item_op = op;
  return KSUCCESS;
}


void
st_destroy(void)
{
  if (g_head == g_dummy) {
    free(g_dummy);
    return ;
  }

#ifdef RECURSION
  _r_st_bst_destroy(g_head);
#else
  _st_bst_destroy(g_head);
#endif

  free(g_dummy);
}


static void
_r_st_bst_destroy(st_bst_node_t *root)
{
  if (root == g_dummy)
    return ;

  _r_st_bst_destroy(root->left);
  _r_st_bst_destroy(root->right);
  if (g_item_op->free != NULL)
    g_item_op->free(root->item);
  free(root);
}


static void
_st_bst_destroy(st_bst_node_t *root)
{
  st_bst_node_t **pcur;
  stack_t        *stack;

  // hypothesis: all calls for the stack would not be failed
  stack = stack_init(128, g_item_op->null);
  (void) stack_push(stack, &g_dummy);
  for (pcur = &root; *pcur != g_dummy; ) {
    if ((*pcur)->left==g_dummy && (*pcur)->right==g_dummy) {
      _st_bst_free_node(*pcur);
      *pcur = g_dummy;

      // "g_dummy->right == dummy" gives the terminate condition
      pcur = stack_peek(stack);
      if ((*pcur)->right == g_dummy) {
        pcur = stack_pop(stack);
      } else {
        pcur = &(*pcur)->right;
      }

    } else {
      stack_push(stack, pcur);
      if ((*pcur)->left != g_dummy) {
        pcur = &(*pcur)->left;
      } else {
        pcur = &(*pcur)->right;
      }
    }
  }

  stack_destroy(stack);
}


size_t
st_size(void)
{
  return g_head ? g_head->size : 0;
}


kerrno_t
st_insert(kitem_t item)
{
  st_bst_node_t *n;

#ifdef RECURSION
  n = _r_st_bst_insert(g_head, item);
#else
  n = _st_bst_insert(g_head, item);
#endif

  if (n == NULL)
    return KERROR;
  g_head = n;
  return KSUCCESS;
}


static st_bst_node_t *
_st_bst_insert(st_bst_node_t *root, kitem_t item)
{
  st_bst_node_t  *cur;
  st_bst_node_t **p = &root;
  long            n;

  for (cur = root; cur != g_dummy; ) {
    n = g_item_op->cmp(cur->item, item);
    if (n == 0)
      return NULL;

    p = (n < 0) ? &cur->right : &cur->left;
    if (*p != g_dummy)
      cur = *p;
    else
      break;
  }

  *p = _st_bst_new_node(item, g_dummy, g_dummy, 1);
  if (*p == NULL)
     return NULL;
  return root;
}


void
st_delete(kitem_t item)
{
  if (g_head == g_dummy)
    return ;

  g_head = _r_st_bst_delete(g_head, item);
  g_last_node = NULL;
  g_delete_node = NULL;
}


static void
_st_bst_free_node(st_bst_node_t *node)
{
  if (g_item_op->free != NULL)
    g_item_op->free(node->item);
  free(node);
}


static st_bst_node_t *
_r_st_bst_delete(st_bst_node_t *root, kitem_t item)
{
  st_bst_node_t *node = g_dummy;
  kitem_t        t;
  long           n;

  if (root == g_dummy)
    return root;

  g_last_node = root;
  n = g_item_op->cmp(item, root->item);
  if (n < 0) {
    root->left = _r_st_bst_delete(root->left, item);
  } else if (n > 0) {
    root->right = _r_st_bst_delete(root->right, item);
  } else {
    g_delete_node = root;
    if (root->left == g_dummy) {
      node = root->right;
      _st_bst_free_node(root);
      return node;

    } else {
      root->left = _r_st_bst_delete(root->left, item);
    }
  }

  if (g_delete_node == NULL)
    return root;

  if (root == g_last_node) {
    t = g_delete_node->item;
    g_delete_node->item = root->item;
    root->item = t;

    node = g_dummy;
    if (root->left != g_dummy) 
      node = root->left;

    _st_bst_free_node(root);
    return node;
  }

  root->size--;
  return root;
}


kitem_t
st_search(kkey_t key)
{
  if (g_head == g_dummy)
    return g_item_op->null;

#ifdef RECURSION
  return _r_st_bst_search(g_head, key);
#else
  return _st_bst_search(g_head, key);
#endif
}


static kitem_t
_r_st_bst_search(st_bst_node_t *root, kkey_t key)
{
  long n;
  
  if (root == g_dummy)
    return g_item_op->null;
  
  n = g_item_op->cmp_key1(root->item, key);
  if (n == 0)
    return root->item;
  else if (n < 0)
    return _r_st_bst_search(root->right, key);
  else
    return _r_st_bst_search(root->left, key);
}


static kitem_t
_st_bst_search(st_bst_node_t *root, kitem_t key)
{
  st_bst_node_t *cur;
  long           n;

  for (cur = root; cur != g_dummy; ) {
    n = g_item_op->cmp_key1(root->item, key);
    if (n == 0)
      return cur->item;
    else if (n < 0)
      cur = cur->right;
    else
      cur = cur->left;
  }

  return g_item_op->null;
}


void
st_sort(st_visit_pt visit)
{
  if (g_head == g_dummy)
    return ;

  _r_st_bst_sort(g_head, visit);
}


static void
_r_st_bst_sort(st_bst_node_t *root, st_visit_pt visit)
{
  if (root == g_dummy)
    return ;

  _r_st_bst_sort(root->left, visit);
  visit(root->item);
  printf("\n");
  _r_st_bst_sort(root->right, visit);
}


static st_bst_node_t *
_st_bst_new_node(kitem_t item, st_bst_node_t *l, st_bst_node_t *r, size_t n)
{
  st_bst_node_t *x;

  x = (st_bst_node_t *) malloc(sizeof(*x));
  if (x == NULL)
    return NULL;
  x->item = item;
  x->left = l;
  x->right = r;
  x->size = n;
  return x;
}


static st_bst_node_t *
_r_st_bst_insert(st_bst_node_t *root, kitem_t item)
{
  long           n;
  st_bst_node_t *node;
  
  if (root == g_dummy) 
    return _st_bst_new_node(item, g_dummy, g_dummy, 1);

  n = g_item_op->cmp(item, root->item);
  if (n == 0) {
    return root;

  } else if (n < 0) {
    node = _r_st_bst_insert(root->left, item);
    if (node == NULL)
      return NULL;
    root->left = node;

  } else {
    node = _r_st_bst_insert(root->right, item);
    if (node == NULL)
      return NULL;
    root->right = node;
  }
  
  root->size++;
  return root;
}

