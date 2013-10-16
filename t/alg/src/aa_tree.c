/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <stdio.h>
#include <stdlib.h>
#include "kmisc.h"
#include "item.h"
#include "aa_tree.h"


typedef struct aa_node_s aa_node_t;


struct aa_node_s {
  aa_node_t *left;
  aa_node_t *right;
  int        level;
  void      *item;
};


struct aa_tree_s {
  aa_node_t *root;
  aa_node_t  dummy;
  size_t     size;
  item_op_t *op;
};


static inline aa_node_t *_aa_tree_skew(aa_node_t *root);
static inline aa_node_t *_aa_tree_split(aa_node_t *root);
static aa_node_t *_aa_tree_new_node(void *item, aa_node_t *l, aa_node_t *r);
static void _aa_tree_destroy(aa_tree_t *aa, aa_node_t *root);
static void _r_aa_tree_print(const aa_tree_t *aa, const aa_node_t *root);
static errno_t _r_aa_tree_insert(aa_tree_t *aa, aa_node_t **root, void *item);


aa_tree_t *
aa_tree_init(item_op_t *op)
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


errno_t
aa_tree_insert(aa_tree_t *aa, void *item)
{
  errno_t res;

  if (aa == NULL)
    return KEINVALID_PARAM;

  res = _r_aa_tree_insert(aa, &aa->root, item);
  if (res != KSUCCESS)
    return res;

  aa->size++;
  return KSUCCESS;
}


errno_t
aa_tree_delete(aa_tree_t *aa, void *item)
{
  return KSUCCESS;
}


size_t
aa_tree_size(const aa_tree_t *aa)
{
  if (aa == NULL)
    return 0;

  return aa->size;
}


static void
_aa_tree_print_node(const aa_tree_t *aa, const aa_node_t *n)
{
  const aa_node_t *dummy = &aa->dummy;
  item_print_pt    print = aa->op->print;

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
_aa_tree_new_node(void *item, aa_node_t *l, aa_node_t *r)
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


static errno_t
_r_aa_tree_insert(aa_tree_t *aa, aa_node_t **root, void *item)
{
  long         n;
  errno_t      res;
  item_cmp_pt  cmp = aa->op->cmp;
  aa_node_t   *dummy = &aa->dummy;
  aa_node_t   *r = *root;

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
  item_free_pt ffree = aa->op->free;

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

