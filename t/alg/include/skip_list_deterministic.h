/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#ifndef SKIP_LIST_DETERMINISTIC_H_
#define SKIP_LIST_DETERMINISTIC_H_

#include "kitem.h"


typedef struct dskip_list_s dskip_list_t;


dskip_list_t *dskip_list_init(kitem_op_t *op, kitem_t max);
void          dskip_list_destroy(dskip_list_t *dsl);

int           dskip_list_find(dskip_list_t *dsl, kitem_t item, void **result);
int           dskip_list_insert(dskip_list_t *dsl, kitem_t item);
void          dskip_list_delete(dskip_list_t *dsl, kitem_t item);

void          dskip_list_print(dskip_list_t *dsl);


#endif

