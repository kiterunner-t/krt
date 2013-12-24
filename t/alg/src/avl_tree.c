/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "kmisc.h"
#include "kitem.h"
#include "avl_tree.h"


typedef struct avl_node_s avl_node_t;


struct avl_tree_s {
  avl_node_t *root;
  kitem_op_t *op;
  size_t      size;

  // for delete
  avl_node_t *last_node;
  avl_node_t *delete_node;
};


struct avl_node_s {
  avl_node_t *left;
  avl_node_t *right;
  kitem_t     item;
  int         height;
};


static void _r_avl_tree_destroy(avl_tree_t *avl, avl_node_t *root);
static kerrno_t _r_avl_tree_insert(avl_tree_t *avl, kitem_t item, avl_node_t **r);
static avl_node_t *_avl_tree_new_node(kitem_t item, avl_node_t *l, avl_node_t *r, int height);
static kerrno_t _r_avl_tree_delete(avl_tree_t *avl, kitem_t item, avl_node_t **r);
static inline void _avl_tree_free_node(avl_tree_t *avl, avl_node_t *n);
static kitem_t _r_avl_tree_search(avl_tree_t *avl, kkey_t key, avl_node_t *root);
static void _r_avl_tree_print(avl_tree_t *avl, avl_node_t *root);
static inline int _avl_tree_node_height(avl_node_t *n);
static inline avl_node_t *_avl_tree_rotate_with_leftchild(avl_node_t *root);
static inline avl_node_t *_avl_tree_rotate_with_rightchild(avl_node_t *root);
static inline avl_node_t *_avl_tree_double_rotate_with_leftchild(avl_node_t *root);
static inline avl_node_t *_avl_tree_double_rotate_with_rightchild(avl_node_t *root);


avl_tree_t *
avl_tree_init(kitem_op_t *op)
{
  avl_tree_t *avl;

  avl = (avl_tree_t *) malloc(sizeof(*avl));
  if (avl == NULL)
    return NULL;

  avl->op = op;
  avl->root = NULL;
  avl->size = 0;
  avl->delete_node = NULL;
  avl->last_node = NULL;
  return avl;
}


void
avl_tree_destroy(avl_tree_t *avl)
{
  if (avl == NULL)
    return ;

  _r_avl_tree_destroy(avl, avl->root);
  free(avl);
}


static void
_r_avl_tree_destroy(avl_tree_t *avl, avl_node_t *root)
{
  if (root == NULL)
    return ;

  _r_avl_tree_destroy(avl, root->left);
  _r_avl_tree_destroy(avl, root->right);
  if (avl->op->free != NULL)
    avl->op->free(root->item);
  free(root);
}


int
avl_tree_height(const avl_tree_t *avl)
{
  if (avl == NULL)
    return -1;

  return (avl->root==NULL) ? -1 : avl->root->height;
}


kerrno_t
avl_tree_insert(avl_tree_t *avl, kitem_t item)
{
  kerrno_t ret;

  if (avl == NULL)
    return KEINVALID_PARAM;

  ret = _r_avl_tree_insert(avl, item, &avl->root);
  if (ret != KSUCCESS)
    return ret;

  avl->size++;
  return KSUCCESS;
}


static kerrno_t
_r_avl_tree_insert(avl_tree_t *avl, kitem_t item, avl_node_t **r)
{
  avl_node_t   *root = *r;
  long          n;
  kitem_cmp_pt  cmp = avl->op->cmp;
  kerrno_t      ret;

  if (root == NULL) {
    root = _avl_tree_new_node(item, NULL, NULL, 0);
    if (root == NULL)
      return KEMEM;

    *r = root;
    return KSUCCESS;
  }

  n = cmp(item, root->item);
  if (n == 0) {
    return KEEXIST;
    
  } else if (n < 0) {
    ret = _r_avl_tree_insert(avl, item, &root->left);
    if (ret != KSUCCESS)    
      return ret;

    if (_avl_tree_node_height(root->left) - 
        _avl_tree_node_height(root->right) == 2) {

      if (cmp(item, root->left->item) < 0)
        root = _avl_tree_rotate_with_leftchild(root);
      else
        root = _avl_tree_double_rotate_with_leftchild(root);
    }
  
  } else {
    ret = _r_avl_tree_insert(avl, item, &root->right);
    if (ret != KSUCCESS)
      return ret;

    if (_avl_tree_node_height(root->right) - 
        _avl_tree_node_height(root->left) == 2) {

      if (cmp(item, root->right->item) > 0)
        root = _avl_tree_rotate_with_rightchild(root);
      else
        root = _avl_tree_double_rotate_with_rightchild(root);
    }
  }
  
  root->height = KMAX(_avl_tree_node_height(root->left), 
                      _avl_tree_node_height(root->right)) + 1;
  *r = root;
  return KSUCCESS;
}


static avl_node_t *
_avl_tree_new_node(kitem_t item, avl_node_t *l, avl_node_t *r, int height)
{
  avl_node_t *n;

  n = (avl_node_t *) malloc(sizeof(*n));
  if (n == NULL)
    return NULL;

  n->height = height;
  n->left = l;
  n->right = r;
  n->item = item;
  return n;
}


kerrno_t
avl_tree_delete(avl_tree_t *avl, kitem_t item)
{
  kerrno_t    ret;

  if (avl == NULL)
    return KEINVALID_PARAM;
  if (avl->size==0 || item==avl->op->null)
    return KENOTFOUND;

  ret = _r_avl_tree_delete(avl, item, &avl->root);
  if (ret != KSUCCESS)
    return ret;

  avl->size--;
  avl->last_node = NULL;
  avl->delete_node = NULL;
  return KSUCCESS;
}


static kerrno_t
_r_avl_tree_delete(avl_tree_t *avl, kitem_t item, avl_node_t **r)
{
  avl_node_t *root = *r;
  long        n;
  kerrno_t    ret = KSUCCESS;
  int         lheight;
  int         rheight;
  kitem_t     t;

  if (root == NULL)
    return KSUCCESS;

  avl->last_node = root;
  n = avl->op->cmp(item, root->item);
  if (n == 0) {
    avl->delete_node = root;
    if (root->left==NULL && root->right==NULL)
      *r = NULL;
    else if (root->left==NULL && root->right!=NULL)
      *r = root->right;
    else
      ret = _r_avl_tree_delete(avl, item, &root->left);
  
  } else if (n < 0) {
    ret = _r_avl_tree_delete(avl, item, &root->left);

  } else {
    ret = _r_avl_tree_delete(avl, item, &root->right);
  }

  if (ret != KSUCCESS)
    return ret;
  if (avl->delete_node == NULL)
    return KENOTFOUND;

  if (root == avl->last_node) {
    t = avl->delete_node->item;
    avl->delete_node->item = avl->last_node->item;
    avl->last_node->item = t;

    if (avl->last_node->left != NULL) {
      t = avl->last_node->item;
      avl->last_node->item = avl->last_node->left->item;
      avl->last_node->left->item = t;
      avl->last_node = avl->last_node->left;
      root->left = NULL;
      root->height = _avl_tree_node_height(root->right) + 1;
    } else if (*r != root->right) {
      *r = NULL;
    } else {
      root->height = _avl_tree_node_height(root->right) + 1;
    }

    _avl_tree_free_node(avl, avl->last_node);
    return KSUCCESS;
  }

  lheight = _avl_tree_node_height(root->left);
  rheight = _avl_tree_node_height(root->right);
  if (lheight - rheight > 1) {
    lheight = _avl_tree_node_height(root->left->left);
    rheight = _avl_tree_node_height(root->left->right);
    if (lheight > rheight) {
      *r = _avl_tree_rotate_with_leftchild(root);
    } else {
      *r = _avl_tree_double_rotate_with_leftchild(root);
    }

  } else if (rheight - lheight > 1) {
    lheight = _avl_tree_node_height(root->right->left);
    rheight = _avl_tree_node_height(root->right->right);
    if (rheight > lheight) {
      *r = _avl_tree_rotate_with_rightchild(root);
    } else {
      *r = _avl_tree_double_rotate_with_rightchild(root);
    }
  }

  (*r)->height = KMAX(_avl_tree_node_height((*r)->left), 
                    _avl_tree_node_height((*r)->right)) + 1;
  return KSUCCESS;
}


static inline void
_avl_tree_free_node(avl_tree_t *avl, avl_node_t *n)
{
  if (avl->op->free != NULL)
    avl->op->free(n->item);
  free(n);
}


kitem_t
avl_tree_search(avl_tree_t *avl, kkey_t key)
{
  if (avl==NULL || avl->size==0)
    return avl->op->null;

  return _r_avl_tree_search(avl, key, avl->root);
}


static kitem_t
_r_avl_tree_search(avl_tree_t *avl, kkey_t key, avl_node_t *root)
{
  long n;

  if (root == NULL)
    return avl->op->null;

  n = avl->op->cmp_key1(root->item, key);
  if (n == 0)
    return root->item;
  else if (n < 0)
    return _r_avl_tree_search(avl, key, root->left);
  else
    return _r_avl_tree_search(avl, key, root->right);
}


void
avl_tree_print(avl_tree_t *avl)
{
  if (avl == NULL)
    return ;
  if (avl->size == 0) {
    printf("<empty avl tree>\n");
    return ;
  }

  _r_avl_tree_print(avl, avl->root);
}


static void
_r_avl_tree_print(avl_tree_t *avl, avl_node_t *root)
{
  if (root == NULL)
    return ;

  _r_avl_tree_print(avl, root->left);
  avl->op->print(root->item);
  printf("\n");
  _r_avl_tree_print(avl, root->right);
}


static inline int
_avl_tree_node_height(avl_node_t *n)
{
  return (n==NULL) ? -1 : n->height;
}


static inline avl_node_t *
_avl_tree_rotate_with_leftchild(avl_node_t *root)
{
  avl_node_t *rl = root->left;
  root->left = rl->right;
  rl->right = root;
  root->height = KMAX(_avl_tree_node_height(root->left), 
                      _avl_tree_node_height(root->right)) + 1;
  rl->height = KMAX(_avl_tree_node_height(rl->left), root->height) + 1;
  return rl;
}


static inline avl_node_t *
_avl_tree_rotate_with_rightchild(avl_node_t *root)
{
  avl_node_t *rr = root->right;
  root->right = rr->left;
  rr->left = root;
  root->height = KMAX(_avl_tree_node_height(root->left), 
                      _avl_tree_node_height(root->right)) + 1;
  rr->height = KMAX(_avl_tree_node_height(rr->right), root->height) + 1;
  return rr;
}


static inline avl_node_t *
_avl_tree_double_rotate_with_leftchild(avl_node_t *root)
{
  root->left = _avl_tree_rotate_with_rightchild(root->left);
  return _avl_tree_rotate_with_leftchild(root);
}


static inline avl_node_t *
_avl_tree_double_rotate_with_rightchild(avl_node_t *root)
{
  root->right = _avl_tree_rotate_with_leftchild(root->right);
  return _avl_tree_rotate_with_rightchild(root);
}

