# Red-black Tree

The implementation of Red-Black Tree in
**Introduction of Algorithms Second Edition** by 
Thomas H. Cormen, Charles E.Lesiserson, Ronald L. Rivest, Clifford Stein


## Types
**rbt_t** 
Red-black Tree

**rbt_node_t**
Node of Red-black Tree

**rbt_cmp_t**
Callback function for comparing.
`int (*)(void *key1, void *key2);`

**rbt_iterator_callback_t**
Callback function for iterating.
`int (*)(void *key, void *value, void *data);`

## API

### Creating and Destroying the Tree
```
rbt_t *rbt_new(void);
void rbt_destroy(rbt_t *rbt);
```

### Operating
```
void rbt_attach_cmp(rbt_t *rbt, rbt_cmp_t cmp);
int rbt_insert(rbt_t *rbt, void *key, void *value);
int rbt_search(rbt_t *rbt, void **value_out, void *key);
rbt_node_t *rbt_minimum(rbt_t *rbt, rbt_node_t *x);
rbt_node_t *rbt_maximum(rbt_t *rbt, rbt_node_t *x);
rbt_node_t *rbt_successor(rbt_t *rbt, rbt_node_t *x);
rbt_node_t *rbt_predecessor(rbt_t *rbt, rbt_node_t *x);
rbt_node_t *rbt_remove(rbt_t *rbt, void *key);
```

### Iterator
```
int rbt_iterator_init(rbt_t *rbt, rbt_iterator_t *iterator);
void rbt_iterator_next(rbt_iterator_t *iterator);
rbt_node_t *rbt_iterator_deref(rbt_iterator_t *iterator);
void *rbt_iterator_deref_key(rbt_iterator_t *iterator);
void *rbt_iterator_deref_value(rbt_iterator_t *iterator);
int rbt_iterator_isend(rbt_iterator_t *iterator);

int rbt_iterate(rbt_t *rbt, \
        rbt_iterator_callback_t callback, \
        void *data);
```

