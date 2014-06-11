/**
 * Copyleft (C) KRT, 2013 by kiterunner_t
 */

#ifndef PIPELINE_H_
#define PIPELINE_H_

#include "kmisc.h"

typedef struct pipeline_s pipeline_t;


extern pipeline_t *pipeline_create(int stages);
extern void        pipeline_destroy(pipeline_t *pl);

extern kerrno_t    pipeline_start(pipeline_t *pl, void *data);
extern kerrno_t    pipeline_wait(pipeline_t *pl);


#endif

