/* Red-Black Tree */

#include <stdio.h>
#include <stdlib.h>

#include "rbt.h"


/* Node */

rbt_node_t *rbt_node_new(void *key, void *value)
{
    rbt_node_t *new_node = (rbt_node_t *)malloc(sizeof(rbt_node_t));
    if (new_node == NULL) return NULL;
    new_node->key = key;
    new_node->value = value;
    new_node->left = new_node->right = new_node->parent = NULL;
    new_node->color = RBT_NODE_COLOR_BLACK;
    return new_node;
}

void rbt_node_destroy(rbt_node_t *node)
{
    free(node);
}


static void rbt_left_rotate(rbt_t *rbt, rbt_node_t *x)
{
    rbt_node_t *y = x->right;
    x->right = y->left;
    if (y->left != rbt->nil) y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == rbt->nil) { rbt->root = y; }
    else
    {
        if (x == x->parent->left) { x->parent->left = y; }
        else { x->parent->right = y; }
    }
    y->left = x;
    x->parent = y;
}

static void rbt_right_rotate(rbt_t *rbt, rbt_node_t *x)
{
    rbt_node_t *y = x->left;
    x->left = y->right;
    if (y->right != rbt->nil) y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == rbt->nil) { rbt->root = y; }
    else
    {
        if (x == x->parent->right) { x->parent->right = y; }
        else { x->parent->left = y; }
    }
    y->right = x;
    x->parent = y;
}


static void rbt_insert_fixup(rbt_t *rbt, rbt_node_t *z)
{
    rbt_node_t *y = NULL;
    while (z->parent->color == RBT_NODE_COLOR_RED)
    {
        if (z->parent == z->parent->parent->left)
        {
            y = z->parent->parent->right;
            if (y->color == RBT_NODE_COLOR_RED)
            {
                z->parent->color = RBT_NODE_COLOR_BLACK;
                y->color = RBT_NODE_COLOR_BLACK;
                z->parent->parent->color = RBT_NODE_COLOR_RED;
                z = z->parent->parent;
            }
            else 
            {
                if (z == z->parent->right)
                {
                    z = z->parent;
                    rbt_left_rotate(rbt, z);
                }
                z->parent->color = RBT_NODE_COLOR_BLACK;
                z->parent->parent->color = RBT_NODE_COLOR_RED;
                rbt_right_rotate(rbt, z->parent->parent);
            }
        }
        else
        {
            y = z->parent->parent->left;
            if (y->color == RBT_NODE_COLOR_RED)
            {
                z->parent->color = RBT_NODE_COLOR_BLACK;
                y->color = RBT_NODE_COLOR_BLACK;
                z->parent->parent->color = RBT_NODE_COLOR_RED;
                z = z->parent->parent;
            }
            else 
            {
                if (z == z->parent->left)
                {
                    z = z->parent;
                    rbt_right_rotate(rbt, z);
                }
                z->parent->color = RBT_NODE_COLOR_BLACK;
                z->parent->parent->color = RBT_NODE_COLOR_RED;
                rbt_left_rotate(rbt, z->parent->parent);
            }
        }
    }
    rbt->root->color = RBT_NODE_COLOR_BLACK;
}


int rbt_cmp_default(void *key1, void *key2)
{
    if (((size_t)key1)==((size_t)key2)) return 0;
    return (((size_t)key1)<((size_t)key2)) ? -1 : 1;
}


/* Tree */

rbt_t *rbt_new(void)
{
    rbt_t *new_rbt = (rbt_t *)malloc(sizeof(rbt_t));
    if (new_rbt == NULL) return NULL;
    new_rbt->cmp = &rbt_cmp_default;
    new_rbt->nil = rbt_node_new(NULL, NULL);
    if (new_rbt->nil == NULL) { free(new_rbt); return NULL; }
    new_rbt->nil->left = new_rbt->nil->right = new_rbt->nil->parent = new_rbt->nil;
    new_rbt->root = new_rbt->nil;
    return new_rbt;
}

static void rbt_destroy_in(rbt_t *rbt, rbt_node_t *node)
{
    if (node->left != rbt->nil)
    { rbt_destroy_in(rbt, node->left); }
    if (node->right != rbt->nil)
    { rbt_destroy_in(rbt, node->right); }
    rbt_node_destroy(node);
}

void rbt_destroy(rbt_t *rbt)
{
    if (rbt->root != rbt->nil) 
    { rbt_destroy_in(rbt, rbt->root); }
    rbt_node_destroy(rbt->nil);
    free(rbt);
}

void rbt_attach_cmp(rbt_t *rbt, rbt_cmp_t cmp)
{
    rbt->cmp = cmp;
}

int rbt_insert(rbt_t *rbt, void *key, void *value)
{
    rbt_node_t *x = rbt->root, *y = rbt->nil;
    rbt_node_t *z = NULL;
    int cmp_result;
    
    if ((z = rbt_node_new(key, value)) == NULL) return -1;
    z->parent = z->left = z->right = rbt->nil;
    z->color = RBT_NODE_COLOR_RED;
    while (x != rbt->nil)
    {
        y = x;
        cmp_result = rbt->cmp(z->key, x->key);
        if (cmp_result == 0) return 0;
        x = (cmp_result < 0) ? x->left : x->right;
    }
    z->parent = y;
    if (y == rbt->nil) { rbt->root = z; }
    else
    {
        if (rbt->cmp(z->key, y->key) < 0) y->left = z;
        else y->right = z;
    }
    rbt_insert_fixup(rbt, z);
    return 0;
}

static int rbt_search_node(rbt_t *rbt, rbt_node_t **node_out, void *key)
{
    rbt_node_t *node_cur = rbt->root;
    int result;

    while (node_cur != rbt->nil)
    {
        result = rbt->cmp(key, node_cur->key);
        switch (result)
        {
            case 0:
                *node_out = node_cur;
                return 0;
            case -1:
                node_cur = node_cur->left;
                break;
            case 1:
                node_cur = node_cur->right;
                break;
        }
    }

    return -1;
}

int rbt_search(rbt_t *rbt, void **value_out, void *key)
{
    int ret = 0;
    rbt_node_t *node_target;
    ret = rbt_search_node(rbt, &node_target, key);
    if (node_target != NULL) *value_out = node_target->value;
    return ret;
}

rbt_node_t *rbt_minimum(rbt_t *rbt, rbt_node_t *x)
{
    while (x->left != rbt->nil) { x = x->left; }
    return x;
}

rbt_node_t *rbt_maximum(rbt_t *rbt, rbt_node_t *x)
{
    while (x->right != rbt->nil) { x = x->right; }
    return x;
}

rbt_node_t *rbt_successor(rbt_t *rbt, rbt_node_t *x)
{
    rbt_node_t *y;
    if (x->right != rbt->nil) return rbt_minimum(rbt, x->right);
    y = x->parent;
    while ((y != rbt->nil) && (x == y->right))
    {
        x = y;
        y = y->parent;
    }
    return y;
}

rbt_node_t *rbt_predecessor(rbt_t *rbt, rbt_node_t *x)
{
    rbt_node_t *y;
    if (x->left != rbt->nil) return rbt_maximum(rbt, x->left);
    y = x->parent;
    while ((y != rbt->nil) && (x == y->left))
    {
        x = y;
        y = y->parent;
    }
    return y;
}

static void rbt_remove_node_fixup(rbt_t *rbt, rbt_node_t *x)
{
    rbt_node_t *w;
    while ((x != rbt->root) && (x->color == RBT_NODE_COLOR_BLACK))
    {
        if (x == x->parent->left)
        {
            w = x->parent->right;
            if (w->color == RBT_NODE_COLOR_RED)
            {
                w->color = RBT_NODE_COLOR_BLACK;
                x->parent->color = RBT_NODE_COLOR_RED;
                rbt_left_rotate(rbt, x->parent);
                w = x->parent->right;
            }
            if ((w->left->color == RBT_NODE_COLOR_BLACK) && \
                    (w->right->color == RBT_NODE_COLOR_BLACK))
            {
                w->color = RBT_NODE_COLOR_RED;
                x = x->parent;
            }
            else
            {
                if (w->right->color == RBT_NODE_COLOR_BLACK)
                {
                    w->left->color = RBT_NODE_COLOR_BLACK;
                    w->color = RBT_NODE_COLOR_RED;
                    rbt_right_rotate(rbt, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = RBT_NODE_COLOR_BLACK;
                w->right->color = RBT_NODE_COLOR_BLACK;
                rbt_left_rotate(rbt, x->parent);
                x = rbt->root;
            }
        }
        else
        {
            w = x->parent->left;
            if (w->color == RBT_NODE_COLOR_RED)
            {
                w->color = RBT_NODE_COLOR_BLACK;
                x->parent->color = RBT_NODE_COLOR_RED;
                rbt_right_rotate(rbt, x->parent);
                w = x->parent->left;
            }
            if ((w->right->color == RBT_NODE_COLOR_BLACK) && \
                    (w->left->color == RBT_NODE_COLOR_BLACK))
            {
                w->color = RBT_NODE_COLOR_RED;
                x = x->parent;
            }
            else
            {
                if (w->left->color == RBT_NODE_COLOR_BLACK)
                {
                    w->right->color = RBT_NODE_COLOR_BLACK;
                    w->color = RBT_NODE_COLOR_RED;
                    rbt_left_rotate(rbt, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = RBT_NODE_COLOR_BLACK;
                w->left->color = RBT_NODE_COLOR_BLACK;
                rbt_right_rotate(rbt, x->parent);
                x = rbt->root;
            }
        }
    }
    x->color = RBT_NODE_COLOR_BLACK;
}

static rbt_node_t *rbt_remove_node(rbt_t *rbt, rbt_node_t *z)
{
    rbt_node_t *x, *y;
    y = ((z->left == rbt->nil) || \
            (z->right == rbt->nil)) ? \
        z : \
        rbt_successor(rbt, z);
    x = (z->left == rbt->nil) ? y->left : y->right;
    x->parent = y->parent;
    if (y->parent == rbt->nil) { rbt->root = x; }
    else
    {
        if (y == y->parent->left) y->parent->left = x;
        else y->parent->right = x;
    }

    if (y != z)
    {
        z->key = y->key;
        z->value = y->value;
    }

    if (y->color == RBT_NODE_COLOR_BLACK)
    {
        rbt_remove_node_fixup(rbt, x);
    }

    return y;
}

rbt_node_t *rbt_remove(rbt_t *rbt, void *key)
{
    rbt_node_t *node_target = NULL;

    /* Search the node */
    rbt_search_node(rbt, &node_target, key);
    if (node_target == NULL) return NULL;

    /* Remove the node */
    return rbt_remove_node(rbt, node_target);
}


/* Iterator */

int rbt_iterator_init(rbt_t *rbt, rbt_iterator_t *iterator)
{
    iterator->node_cur = rbt_minimum(rbt, rbt->root);
    iterator->rbt = rbt;

    return 0;
}

void rbt_iterator_next(rbt_iterator_t *iterator)
{
    iterator->node_cur = rbt_successor(iterator->rbt, \
            iterator->node_cur);
}

rbt_node_t *rbt_iterator_deref(rbt_iterator_t *iterator)
{
    return iterator->node_cur;
}

void *rbt_iterator_deref_key(rbt_iterator_t *iterator)
{
    return iterator->node_cur->key;
}

void *rbt_iterator_deref_value(rbt_iterator_t *iterator)
{
    return iterator->node_cur->value;
}

int rbt_iterator_isend(rbt_iterator_t *iterator)
{
    return iterator->node_cur == iterator->rbt->nil ? 1 : 0;
}

int rbt_iterate(rbt_t *rbt, \
        rbt_iterator_callback_t callback, \
        void *data)
{
    int ret = 0;
    rbt_iterator_t iter;

    for (rbt_iterator_init(rbt, &iter); \
            !rbt_iterator_isend(&iter); \
            rbt_iterator_next(&iter))
    {
        if ((ret = callback(iter.node_cur->key, \
                        iter.node_cur->value, \
                        data)) != 0)
        { return ret; }
    }

    return 0;
}

