/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <stdio.h>
#include <stdlib.h>
#include "kmisc.h"
#include "kitem.h"
#include "krand.h"
#include "treap_tree.h"


typedef struct treap_node_s treap_node_t;


struct treap_node_s {
  treap_node_t *left;
  treap_node_t *right;
  kitem_t       item;
  long          priority;
};


struct treap_tree_s {
  treap_node_t *root;
  treap_node_t  dummy;
  size_t        size;
  kitem_op_t   *op;
};


static kerrno_t _r_treap_tree_delete(treap_tree_t *treap, 
                                     treap_node_t **r, kitem_t item);
static treap_node_t *_treap_tree_new_node(kitem_t item, treap_node_t *l, 
                                          treap_node_t *r);
static kitem_t _r_treap_tree_search(treap_tree_t *treap, treap_node_t *root, 
                                    kkey_t key);
static kerrno_t _r_treap_tree_insert(treap_tree_t *treap, treap_node_t **r, 
                                     kitem_t item);
static void _r_treap_tree_destroy(treap_tree_t *treap, treap_node_t *node);
static treap_node_t *_treap_tree_rotate_with_left(treap_node_t *root);
static treap_node_t *_treap_tree_rotate_with_right(treap_node_t *root);
static void _r_treap_tree_print(treap_tree_t *treap, treap_node_t *root);


treap_tree_t *
treap_tree_init(kitem_op_t *op)
{
  treap_tree_t *treap;
  treap_node_t *dummy;

  treap = (treap_tree_t *) malloc(sizeof(*treap));
  if (treap == NULL)
    return NULL;

  dummy = &treap->dummy;
  dummy->left = dummy;
  dummy->right = dummy;
  dummy->item = KITEM_NULL;
  dummy->priority = KRAND_MAX;
  treap->root = dummy;
  treap->size = 0;
  treap->op = op;

  krand_init();
  return treap;
}


void
treap_tree_destroy(treap_tree_t *treap)
{
  if (treap == NULL)
    return ;

  _r_treap_tree_destroy(treap, treap->root);
}


static void
_r_treap_tree_destroy(treap_tree_t *treap, treap_node_t *node)
{
  if (node == &treap->dummy)
    return ;

  _r_treap_tree_destroy(treap, node->left);
  _r_treap_tree_destroy(treap, node->right);

  if (treap->op->free != NULL)
    treap->op->free(node->item);
  free(node);
}


kerrno_t
treap_tree_insert(treap_tree_t *treap, kitem_t item)
{
  kerrno_t ret;

  if (treap == NULL)
    return KEINVALID_PARAM;

  ret = _r_treap_tree_insert(treap, &treap->root, item);
  if (ret != KSUCCESS)
    return ret;

  treap->size++;
  return KSUCCESS;
}


static kerrno_t
_r_treap_tree_insert(treap_tree_t *treap, treap_node_t **r, kitem_t item)
{
  treap_node_t *dummy = &treap->dummy;
  treap_node_t *root = *r;
  long          n;
  kerrno_t      ret;

  if (root == dummy) {
    root = _treap_tree_new_node(item, dummy, dummy);
    if (root == NULL)
      return KEMEM;
    *r = root;
    return KSUCCESS;
  }

  n = treap->op->cmp(item, root->item);
  if (n == 0) {
    return KEEXIST; // duplicate key is not permitted

  } else if (n > 0) {
    ret = _r_treap_tree_insert(treap, &root->right, item);
    if (ret != KSUCCESS)
      return ret;
    if (root->right->priority < root->priority)
      root = _treap_tree_rotate_with_left(root);

  } else {
    ret = _r_treap_tree_insert(treap, &root->left, item);
    if (ret != KSUCCESS)
      return ret;
    if (root->left->priority < root->priority)
      root = _treap_tree_rotate_with_right(root);
  }

  *r = root;
  return KSUCCESS;
}


static treap_node_t *
_treap_tree_new_node(kitem_t item, treap_node_t *l, treap_node_t *r)
{
  treap_node_t *n;

  n = (treap_node_t *) malloc(sizeof(*n));
  if (n == NULL)
    return NULL;

  n->item = item;
  n->left = l;
  n->right = r;
  n->priority = krand_get();
  return n;
}


kerrno_t
treap_tree_delete(treap_tree_t *treap, kitem_t item)
{
  kerrno_t ret;

  if (treap == NULL)
    return KEINVALID_PARAM;

  ret = _r_treap_tree_delete(treap, &treap->root, item);
  if (ret != KSUCCESS)
    return ret;

  treap->size--;
  return KSUCCESS;
}


static kerrno_t
_r_treap_tree_delete(treap_tree_t *treap, treap_node_t **r, kitem_t item)
{
  treap_node_t   *dummy = &treap->dummy;
  treap_node_t   *root = *r;
  long            n;

  if (root == dummy)
    return KENOTFOUND;

  n = treap->op->cmp(item, root->item);
  if (n == 0) {
    if (root->left->priority < root->right->priority)
      *r = _treap_tree_rotate_with_right(root);
    else
      *r = _treap_tree_rotate_with_left(root);

    if (*r == dummy) {
      if (treap->op->free != NULL)
        treap->op->free(root->item);
      free(root);
      return KSUCCESS;

    } else {
      return _r_treap_tree_delete(treap, r, item);
    }

  } else if (n > 0) {
    return _r_treap_tree_delete(treap, &root->right, item);

  } else {
    return _r_treap_tree_delete(treap, &root->left, item);
  }
}


kitem_t
treap_tree_search(treap_tree_t *treap, kkey_t key)
{
  if (treap == NULL)
    return KITEM_NULL;

  return _r_treap_tree_search(treap, treap->root, key);
}


static kitem_t
_r_treap_tree_search(treap_tree_t *treap, treap_node_t *root, kkey_t key)
{
  long n;

  if (root == &treap->dummy)
    return KITEM_NULL;

  n = treap->op->cmp_key1(root->item, key);
  if (n == 0)
    return root;
  else if (n < 0)
    return _r_treap_tree_search(treap, root->right, key);
  else
    return _r_treap_tree_search(treap, root->left, key);
}


void
treap_tree_print(treap_tree_t *treap)
{
  if (treap == NULL)
    return ;
  if (treap->size == 0) {
    printf("empty treap tree>\n");
    return ;
  }

  _r_treap_tree_print(treap, treap->root);
}


static void
_r_treap_tree_print(treap_tree_t *treap, treap_node_t *root)
{
  if (root == &treap->dummy)
    return ;

  _r_treap_tree_print(treap, root->left);
  treap->op->print(root->item);
  printf(" (%ld)\n", root->priority);
  _r_treap_tree_print(treap, root->right);
}


static treap_node_t *
_treap_tree_rotate_with_left(treap_node_t *root)
{
  treap_node_t *rr = root->right;
  root->right = rr->left;
  rr->left = root;
  return rr;
}


static treap_node_t *
_treap_tree_rotate_with_right(treap_node_t *root)
{
  treap_node_t *rl = root->left;
  root->left = rl->right;
  rl->right = root;
  return rl;
}

