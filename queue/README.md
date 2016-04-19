# Queue

The implementation of Queue.

## Types

**queue_t**

Queue

**queue_node_t**

Node of Queue

## API

### Create and destroy queue

```
queue_t *queue_new(void);
void queue_destroy(queue_t *queue);
```

### Operatings

```
int queue_push(queue_t *queue, void *new_element);
void queue_pop(queue_t *queue);
void *queue_front(queue_t *queue);
void *queue_back(queue_t *queue);
size_t queue_pop(queue_t *queue);
```
