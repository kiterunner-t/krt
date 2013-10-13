/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <stdlib.h>
#include "misc.h"
#include "item.h"
#include "splay_tree.h"


typedef struct splay_node_s splay_node_t;



struct splay_node_s {
  splay_node_t *left;
  splay_node_t *right;
  void         *item;
};


struct splay_tree_s {
  splay_node_t *root;
  size_t        size;
  splay_node_t  dummy;
  item_op_t    *op;
};


static inline splay_node_t *_splay_tree_new_node(void *item,
    splay_node_t *l, splay_node_t *r);
static inline splay_node_t *_splay_tree_rotate_with_left(splay_node_t *r);
static inline splay_node_t *_splay_tree_rotate_with_right(splay_node_t *r);
static splay_node_t *_splay_tree_splay(splay_tree_t *splay,
                                       splay_node_t *root, void *item);
static void _splay_tree_print(const splay_tree_t *splay, 
                              const splay_node_t *root);

splay_tree_t *
splay_tree_init(item_op_t *op)
{
  splay_tree_t *splay;
  splay_node_t *dummy;

  splay = (splay_tree_t *) malloc(sizeof(*splay));
  if (splay == NULL)
    return NULL;

  dummy = &splay->dummy;
  dummy->left = dummy;
  dummy->right = dummy;
  dummy->item = NULL;
  splay->root = dummy;
  splay->size = 0;
  splay->op = op;
  return splay;
}


void
splay_tree_destroy(splay_tree_t *splay)
{
  if (splay == NULL)
    return ;

  free(splay);
}


int
splay_tree_insert(splay_tree_t *splay, void *item)
{
  splay_node_t *t;
  splay_node_t *dummy;
  splay_node_t *root;
  item_cmp_pt   cmp;
  long          n;

  cmp = splay->op->cmp;
  root = splay->root;
  dummy = &(splay->dummy);
  if (root == dummy) {
    root = _splay_tree_new_node(item, dummy, dummy);
    if (root == NULL)
      return -1;

    splay->size++;
    splay->root = root;
    return 0;
  }

  root = _splay_tree_splay(splay, root, item);
  splay->root = root;
  n = cmp(item, root->item);
  if (n == 0) 
    return 1;

  t = (splay_node_t *) malloc(sizeof(*t));
  if (t == NULL)
    return -1;

  t->item = item;
  if (n > 0) {
    t->left = root;
    t->right = root->right;
    root->right = dummy;
    
  } else {
    t->left = root->left;
    t->right = root;
    root->left = dummy;
  }

  splay->size++;
  splay->root = t;
  return 0;
}


int
splay_tree_delete(splay_tree_t *splay, void *item)
{
  return 0;
}


int
splay_tree_search(splay_tree_t *splay, void *item)
{
  return 0;
}


void
splay_tree_print(const splay_tree_t *splay)
{
  if (splay->root == &splay->dummy) {
    printf("<empty splay tree>\n");
    return ;
  }

  _splay_tree_print(splay, splay->root);
}


static void
_splay_tree_print(const splay_tree_t *splay, const splay_node_t *root)
{
  if (root == &(splay->dummy))
    return ;

  _splay_tree_print(splay, root->left);
  splay->op->print(root->item);
  printf("\n");
  _splay_tree_print(splay, root->right);
}


static inline splay_node_t *
_splay_tree_new_node(void *item, splay_node_t *l, splay_node_t *r)
{
  splay_node_t *n;

  n = (splay_node_t *) malloc(sizeof(*n));
  if (n == NULL)
    return NULL;

  n->left = l;
  n->right = r;
  n->item = item;
  return n;
}


static splay_node_t *
_splay_tree_splay(splay_tree_t *splay, splay_node_t *root, void *item)
{
  splay_node_t *left_max = NULL;
  splay_node_t *right_min = NULL;
  splay_node_t *dummy = &splay->dummy;
  splay_node_t *t;
  splay_node_t  head;
  item_cmp_pt   cmp = splay->op->cmp;
  long          n;

  head.left = dummy;
  head.right = dummy;
  left_max = &head;
  right_min = &head;
  dummy->item = item;
  t = root;

  for ( ; ; ) {
    n = cmp(item, t->item);
    if (n == 0) {
      break;

    } else if (n > 0) {
      if (t->right == dummy)
        break;

      n = cmp(item, t->right->item);
      if (n==0 || (n>0 && t->right->right==dummy) 
          || (n<0 && t->right->left==dummy)) {  // zig
        left_max->right = t;
        left_max = t;
        t = t->right;
        left_max->right = dummy;
        break;

      } else if (n > 0) {                        // zig-zig
        t = _splay_tree_rotate_with_left(t);
        left_max->right = t;
        left_max = t;
        t = t->right;
        left_max->right = dummy;

      } else {                                   // zig-zag
        left_max->right = t;
        left_max = t;
        right_min->left = t->right;
        right_min = t->right;
        t = t->right->left;
        left_max->right = dummy;
        right_min->left = dummy;
      }

    } else {
      if (t->left == dummy)
        break;

      n = cmp(item, t->left->item);
      if (n==0 || (n<0 && t->left->left==dummy) 
          || (n>0 && t->left->right==dummy)) {
        right_min->left = t;
        right_min = t;
        t = t->left;
        right_min->left = dummy;
        break;

      } else if (n < 0) {
        t = _splay_tree_rotate_with_right(t);
        right_min->left = t;
        right_min = t;
        t = t->left;
        right_min->left = dummy;

      } else {
        right_min->left = t;
        right_min = t;
        left_max->right = t->left;
        left_max = t->left;
        t = t->left->right;
        right_min->left = dummy;
        left_max->right = dummy;
      }
    }
  }

  left_max->right = t->left;
  right_min->left = t->right;
  t->left = head.right;
  t->right = head.left;
  return t;
}


static inline splay_node_t *
_splay_tree_rotate_with_left(splay_node_t *r)
{
  splay_node_t *t;

  t = r->right;
  r->right = t->left;
  t->left = r;
  return t;
}


static inline splay_node_t *
_splay_tree_rotate_with_right(splay_node_t *r)
{
  splay_node_t *t;

  t = r->left;
  r->left = t->right;
  t->right = r;
  return t;
}

