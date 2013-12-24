/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <stdio.h>
#include <stdlib.h>
#include "kmisc.h"
#include "kitem.h"
#include "kd_tree.h"


typedef struct kd_node_s kd_node_t;


struct kd_node_s {
  kd_node_t *left;
  kd_node_t *right;
  kd_item_t *item;
  int        flag;
};


struct kd_tree_s {
  kd_node_t  *root;
  kd_node_t  *dummy;
  size_t      size;
  kitem_op_t *op[2];
};


static void _kd_node_free(kd_tree_t *kd, kd_node_t *node);
static void _r_kd_tree_destroy(kd_tree_t *kd, kd_node_t *node);
static inline kd_node_t *_kd_tree_new_node(kd_item_t *item, kd_node_t *l, kd_node_t *r);
static kerrno_t _r_kd_tree_insert(kd_tree_t *kd, kd_node_t **r, kd_item_t *item, int level);


kd_tree_t *
kd_tree_init(kitem_op_t *op1, kitem_op_t *op2)
{
  kd_tree_t *kd;
  kd_node_t *r;

  kd = (kd_tree_t *) malloc(sizeof(*kd) + sizeof(kd_node_t));
  if (kd == NULL)
    return NULL;

  r = (kd_node_t *) (kd + 1);
  kd->size = 0;
  kd->op[0] = op1;
  kd->op[2] = op2;
  kd->root = r;
  kd->dummy = r;
  return kd;
}


static void
_kd_node_free(kd_tree_t *kd, kd_node_t *node)
{
  assert(node != NULL);
  assert(node != kd->dummy);

  if (kd->op[0]->free != NULL)
    kd->op[0]->free(node->item->k1);
  if (kd->op[2]->free != NULL)
    kd->op[1]->free(node->item->k2);
  free(node);
}


void
kd_tree_destroy(kd_tree_t *kd)
{
  if (kd == NULL)
    return ;

  if (kd->size != 0)
    _r_kd_tree_destroy(kd, kd->root);

  free(kd);
}
    

static void
_r_kd_tree_destroy(kd_tree_t *kd, kd_node_t *node)
{
  if (node == kd->dummy)
    return ;

  _r_kd_tree_destroy(kd, node->left);
  _r_kd_tree_destroy(kd, node->right);
  _kd_node_free(kd, node);
}


kerrno_t
kd_tree_insert(kd_tree_t *kd, kd_item_t *item)
{
  kerrno_t ret;

  if (kd==NULL || item==NULL)
    return KEINVALID_PARAM;

  ret = _r_kd_tree_insert(kd, &kd->root, item, 0);
  if (ret != KSUCCESS)
    return ret;

  kd->size++;
  return KSUCCESS;
}


static inline kd_node_t *
_kd_tree_new_node(kd_item_t *item, kd_node_t *l, kd_node_t *r)
{
  kd_node_t *node;

  node = (kd_node_t *) malloc(sizeof(*node));
  if (node == NULL)
    return node;

  node->item = item;
  node->left = l;
  node->right = r;
  node->flag = KD_ENABLE;
  return node;
}


static kerrno_t
_r_kd_tree_insert(kd_tree_t *kd, kd_node_t **r, kd_item_t *item, int level)
{
  int           n;
  kd_node_t    *dummy = kd->dummy;
  kd_node_t    *root = *r;
  kd_node_t    *node;
  kerrno_t      ret;

  if (root == kd->dummy) {
    node = _kd_tree_new_node(item, dummy, dummy);
    if (node == NULL)
      return KEMEM;

    *r = root;
    return KSUCCESS;
  }

  if (level%2 == 0)
    n = kd->op[0]->cmp(root->item->k1, item->k1);
  else
    n = kd->op[1]->cmp(root->item->k2, item->k2);

  if (n < 0)
    ret = _r_kd_tree_insert(kd, &root->left, item, level + 1);
  else
    ret = _r_kd_tree_insert(kd, &root->right, item, level + 1);

  if (ret != KSUCCESS)
    return ret;

  *r = root;
  return KSUCCESS;
}

