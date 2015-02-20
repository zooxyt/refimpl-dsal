/* Red-Black Tree */

/* The implementation of Red-Black Tree in
 * <<Introduction of Algorithms Second Edition>> by CLRS */

#ifndef _RBT_H_
#define _RBT_H_


/* Node */

typedef enum 
{
    RBT_NODE_COLOR_RED,
    RBT_NODE_COLOR_BLACK,
} rbt_node_color_t;

typedef int (*rbt_cmp_t)(void *key1, void *key2);

struct rbt_node
{
    void *key;
    void *value;
    struct rbt_node *left, *right, *parent;
    rbt_node_color_t color;
};
typedef struct rbt_node rbt_node_t;

rbt_node_t *rbt_node_new(void *key, void *value);
void rbt_node_destroy(rbt_node_t *node);


/* Tree */

struct rbt
{
    rbt_node_t *root;
    rbt_node_t *nil;
    rbt_cmp_t cmp;
};
typedef struct rbt rbt_t;

rbt_t *rbt_new(void);
void rbt_destroy(rbt_t *rbt);

void rbt_attach_cmp(rbt_t *rbt, rbt_cmp_t cmp);
int rbt_insert(rbt_t *rbt, void *key, void *value);
int rbt_search(rbt_t *rbt, void **value_out, void *key);
rbt_node_t *rbt_minimum(rbt_t *rbt, rbt_node_t *x);
rbt_node_t *rbt_maximum(rbt_t *rbt, rbt_node_t *x);
rbt_node_t *rbt_successor(rbt_t *rbt, rbt_node_t *x);
rbt_node_t *rbt_predecessor(rbt_t *rbt, rbt_node_t *x);
rbt_node_t *rbt_remove(rbt_t *rbt, void *key);


/* Iterator */

typedef struct rbt_iterator
{
    rbt_node_t *node_cur;
    rbt_t *rbt;
} rbt_iterator_t;

int rbt_iterator_init(rbt_t *rbt, rbt_iterator_t *iterator);
void rbt_iterator_next(rbt_iterator_t *iterator);
rbt_node_t *rbt_iterator_deref(rbt_iterator_t *iterator);
void *rbt_iterator_deref_key(rbt_iterator_t *iterator);
void *rbt_iterator_deref_value(rbt_iterator_t *iterator);
int rbt_iterator_isend(rbt_iterator_t *iterator);

typedef int (*rbt_iterator_callback_t)(void *key, void *value, void *data);
int rbt_iterate(rbt_t *rbt, \
        rbt_iterator_callback_t callback, \
        void *data);


#endif

