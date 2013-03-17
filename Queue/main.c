#include <stdio.h>
#include <assert.h>
#include <libkern/OSAtomic.h>

typedef struct {
    long start;
    long end;
    long queue[10000];
} queue_t;

bool empty(queue_t *queue)
{
    return queue->start == queue->end;
}

long dequeue(queue_t *queue)
{
    return queue->queue[queue->start++];
}

void enqueue(queue_t *queue, long value)
{
    queue->queue[queue->end++] = value;
}

int main(int argc, const char * argv[])
{
    queue_t queue = {0, 0, {0}};
    queue_t queue1 = {0, 0, {0}};

    assert(true == empty(&queue));
    assert(true == empty(&queue1));
    enqueue(&queue, 13);
    enqueue(&queue1, 3);
    assert(false == empty(&queue1));
    assert(false == empty(&queue));
    enqueue(&queue, 23);
    enqueue(&queue, 33);
    enqueue(&queue1, 4);
    assert(13 == dequeue(&queue));
    assert(3 == dequeue(&queue1));
    assert(23 == dequeue(&queue));
    assert(33 == dequeue(&queue));
    assert(true == empty(&queue));
    assert(false == empty(&queue1));

    queue_t queue3 = {0, 0, {0}};
    long i;
    for (i = 0; i < 10000; i++) {
        enqueue(&queue3, i);
    };
    for (i = 0; i < 10000; i++) {
        assert(i == dequeue(&queue3));
    };

    return 0;
}
