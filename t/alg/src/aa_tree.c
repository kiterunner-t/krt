/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <stdio.h>
#include <stdlib.h>
#include "kmisc.h"
#include "kitem.h"
#include "aa_tree.h"


typedef struct aa_node_s aa_node_t;


struct aa_node_s {
  aa_node_t *left;
  aa_node_t *right;
  kitem_t    item;
  int        level;
};


struct aa_tree_s {
  aa_node_t  *root;
  aa_node_t   dummy;
  size_t      size;
  kitem_op_t *op;
};


static inline aa_node_t *_aa_tree_skew(aa_node_t *root);
static inline aa_node_t *_aa_tree_split(aa_node_t *root);
static aa_node_t *_aa_tree_new_node(kitem_t item, aa_node_t *l, aa_node_t *r);
static void _aa_tree_destroy(aa_tree_t *aa, aa_node_t *root);
static void _r_aa_tree_print(const aa_tree_t *aa, const aa_node_t *root);
static kerrno_t _r_aa_tree_insert(aa_tree_t *aa, aa_node_t **root, kitem_t item);
static kerrno_t _r_aa_tree_delete(aa_tree_t *aa, aa_node_t **root, kitem_t item);


aa_tree_t *
aa_tree_init(kitem_op_t *op)
{
  aa_tree_t *aa;
  aa_node_t *dummy;

  aa = (aa_tree_t *) malloc(sizeof(*aa));
  if (aa == NULL)
    return NULL;

  dummy = &aa->dummy;
  dummy->left = dummy;
  dummy->right = dummy;
  dummy->level = 0;
  aa->root = dummy;
  aa->size = 0;
  aa->op = op;
  return aa;
}


void
aa_tree_destroy(aa_tree_t *aa)
{
  if (aa == NULL)
    return ;

  _aa_tree_destroy(aa, aa->root);
  free(aa);
}


void
aa_tree_print(const aa_tree_t *aa)
{
  if (aa == NULL)
    return ;

  if (aa->size == 0) {
    printf("<empty aa tree>\n");
    return ;
  }
    
  _r_aa_tree_print(aa, aa->root);
}


kerrno_t
aa_tree_insert(aa_tree_t *aa, kitem_t item)
{
  kerrno_t res;

  if (aa == NULL)
    return KEINVALID_PARAM;

  res = _r_aa_tree_insert(aa, &aa->root, item);
  if (res != KSUCCESS)
    return res;

  aa->size++;
  return KSUCCESS;
}


kerrno_t
aa_tree_delete(aa_tree_t *aa, kitem_t item)
{
  kerrno_t res;

  if (aa == NULL)
    return KEINVALID_PARAM;

  res = _r_aa_tree_delete(aa, &aa->root, item);
  if (res != KSUCCESS)
    return res;

  aa->size--;
  return KSUCCESS;
}


size_t
aa_tree_size(const aa_tree_t *aa)
{
  if (aa == NULL)
    return 0;

  return aa->size;
}


static kerrno_t
_r_aa_tree_delete(aa_tree_t *aa, aa_node_t **root, kitem_t item)
{
  aa_node_t        *r = *root;
  aa_node_t        *dummy = &aa->dummy;
  kitem_cmp_pt      cmp = aa->op->cmp;
  long              n;
  kerrno_t           res;
  static aa_node_t *delete_node = NULL;
  static aa_node_t *last_node;

  if (r == dummy)
    return KSUCCESS;

  last_node = r;
  n = cmp(item, r->item);
  if (n == 0)
    delete_node = r;
  else if (n < 0)
    res = _r_aa_tree_delete(aa, &r->left, item);
  else
    res = _r_aa_tree_delete(aa, &r->right, item);

  if (res != KSUCCESS)
    return res;
  if (delete_node == NULL)
    return KENOTFOUND;

  if (r == last_node) {
    delete_node->item = last_node->item;
    delete_node = dummy;
    r = r->right;

    if (aa->op->free != NULL)
      aa->op->free(last_node->item);
    free(last_node);

    *root = r;
    return KSUCCESS;
  }

  if (r->left->level<r->level-1 || r->right->level<r->level-1) {
    if (r->right->level > --r->level)
      r->right->level = r->level;

    r = _aa_tree_skew(r);
    r->right = _aa_tree_skew(r->right);
    r->right->right = _aa_tree_skew(r->right->right);
    r = _aa_tree_split(r);
    r->right = _aa_tree_split(r->right);
    *root = r;
  }
  return KSUCCESS;
}


static void
_aa_tree_print_node(const aa_tree_t *aa, const aa_node_t *n)
{
  const aa_node_t *dummy = &aa->dummy;
  kitem_print_pt   print = aa->op->print;

  if (n == dummy)
    printf(" (dummy) ");
  else {
    print(n->item);
    printf("[%d] ", n->level);
  }
}


static void
_r_aa_tree_print(const aa_tree_t *aa, const aa_node_t *root)
{
  if (root == &aa->dummy)
    return ;

  _r_aa_tree_print(aa, root->left);

  _aa_tree_print_node(aa, root);
  _aa_tree_print_node(aa, root->left);
  _aa_tree_print_node(aa, root->right);
  printf("\n");

  _r_aa_tree_print(aa, root->right);
}


static aa_node_t *
_aa_tree_new_node(kitem_t item, aa_node_t *l, aa_node_t *r)
{
  aa_node_t *n;

  n = (aa_node_t *) malloc(sizeof(*n));
  if (n == NULL)
    return NULL;

  n->left = l;
  n->right = r;
  n->level = 1;
  n->item = item;
  return n;
}


static kerrno_t
_r_aa_tree_insert(aa_tree_t *aa, aa_node_t **root, kitem_t item)
{
  long          n;
  kerrno_t      res;
  kitem_cmp_pt  cmp = aa->op->cmp;
  aa_node_t    *dummy = &aa->dummy;
  aa_node_t    *r = *root;

  if (r == dummy) {
    r = _aa_tree_new_node(item, dummy, dummy);
    if (r == NULL)
      return KEMEM;
    
    *root = r;
    return KSUCCESS;
  }

  n = cmp(item, r->item);
  if (n < 0)
    res = _r_aa_tree_insert(aa, &r->left, item);
  else if (n > 0)
    res = _r_aa_tree_insert(aa, &r->right, item);
  else
    return KEEXIST;

  if (res != KSUCCESS)
    return res;

  r = *root;
  r = _aa_tree_skew(r);
  r = _aa_tree_split(r);
  *root = r;
  return KSUCCESS;
}


static void
_aa_tree_destroy(aa_tree_t *aa, aa_node_t *root)
{
  kitem_free_pt ffree = aa->op->free;

  if (root == &aa->dummy)
    return ;

  _aa_tree_destroy(aa, root->left);
  _aa_tree_destroy(aa, root->right);

  if (ffree != NULL)
    ffree(root->item);

  free(root);
}


static inline aa_node_t *
_aa_tree_skew(aa_node_t *root)
{
  aa_node_t *rl;

  if (root->left->level == root->level) {
    rl = root->left;
    root->left = rl->right;
    rl->right = root;
    return rl;
  }

  return root;
}


static inline aa_node_t *
_aa_tree_split(aa_node_t *root)
{
  aa_node_t *rr = root->right;

  if (root->level==rr->level && root->level==rr->right->level) {
    rr->level++;
    root->right = rr->left;
    rr->left = root;
    return rr;
  }

  return root;
}

