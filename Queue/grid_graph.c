#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <libkern/OSAtomic.h>

long* zeros(long n)
{
  return (long*)malloc(n*n*n*n*sizeof(long));
}

void grid_graph(long n, long graph[])
{
  long i;
  long size = n * n;
  long total = size * size;

  for (i = 1; i < size; i++)
  {
    long col = i;
    long row = i - 1;
    if ((row + 1) % n != 0) {
      if (row < size && col < size) {
        graph[row * size + col] = 1;
        graph[col * size + row] = 1;
      }
    }
  }
  for (i = n; i < size; i++)
  {
    long col = i;
    long row = i - n;
    long idx = row * size + col;
    if (row < size && col < size) {
        graph[row * size + col] = 1;
        graph[col * size + row] = 1;
    }
  }
}

void test_2()
{
  long *graph = zeros(2);
  grid_graph(2, graph);
  long expected_graph[] = {
    0,1,1,0,
    1,0,0,1,
    1,0,0,1,
    0,1,1,0};

  assert(0 == memcmp(graph, expected_graph, 4*4*sizeof(long)));
}

void test_3()
{
  long *graph = zeros(3);
  grid_graph(3, graph);
  long expected_graph[] = {
    0,1,0,1,0,0,0,0,0,
    1,0,1,0,1,0,0,0,0,
    0,1,0,0,0,1,0,0,0,
    1,0,0,0,1,0,1,0,0,
    0,1,0,1,0,1,0,1,0,
    0,0,1,0,1,0,0,0,1,
    0,0,0,1,0,0,0,1,0,
    0,0,0,0,1,0,1,0,1,
    0,0,0,0,0,1,0,1,0,
  };

  assert(0 == memcmp(graph, expected_graph, 4*4*sizeof(long)));
}
void test_4()
{
  long *graph = zeros(4);
  grid_graph(4, graph);
  long expected_graph[] = {
    0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,
    1,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,
    0,1,0,1,0,0,1,0,0,0,0,0,0,0,0,0,
    0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,
    1,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,
    0,1,0,0,1,0,1,0,0,1,0,0,0,0,0,0,
    0,0,1,0,0,1,0,1,0,0,1,0,0,0,0,0,
    0,0,0,1,0,0,1,0,0,0,0,1,0,0,0,0,
    0,0,0,0,1,0,0,0,0,1,0,0,1,0,0,0,
    0,0,0,0,0,1,0,0,1,0,1,0,0,1,0,0,
    0,0,0,0,0,0,1,0,0,1,0,1,0,0,1,0,
    0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,1,
    0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,
    0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,0,
    0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,
    0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,
  };

  assert(0 == memcmp(graph, expected_graph, 4*4*sizeof(long)));
}

typedef struct {
  long *list;
  long tail;
} list_t;

void list_append(list_t *list, long value)
{
  list->list[++list->tail] = value;
}

typedef struct {
    long start;
    long end;
    long queue[100000];
} queue_t;

int empty(queue_t *queue)
{
    return queue->start - queue->end;
}

int any(queue_t *queue)
{
    return queue->start != queue->end;
}

long dequeue(queue_t *queue)
{
    return queue->queue[queue->start++];
}

void enqueue(queue_t *queue, long value)
{
    queue->queue[queue->end++] = value;
}

void bfs(long n, long *graph, long *path_, long source)
{
  long size = n * n;
  list_t path = {path_, -1};
  queue_t queue = {0, 0, {0}};
  long visited[100000] = {0};

  enqueue(&queue, source);
  visited[source] = 1;
  while(any(&queue))
  {
    long node = dequeue(&queue);
    list_append(&path, node);
    long row_start = node * (size);
    int i;
    for (i = 0; i < size; i++)
    {
      if (graph[row_start + i] == 1)
      {
        if (visited[i] == 0) {
          enqueue(&queue, i);
          visited[i] = 1;
        }
      }
    }
  }
}

void test_bfs_2()
{
  long *graph = zeros(2);
  long path[] = { 0, 0, 0, 0 };

  grid_graph(2, graph);
  bfs(2L, graph, path, 0L);

  long expected_path[] = { 0, 1, 2, 3 };
  int i;
  printf("\n");
  for (i = 0; i < 4; i++) printf("%li, ", path[i]);
  printf("\n");
  for (i = 0; i < 4; i++) printf("%li, ", expected_path[i]);
  printf("\n");
  assert(0 == memcmp(path, expected_path, 4*sizeof(long)));
}

void test_bfs_3()
{
  long *graph = zeros(3);
  long path[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

  grid_graph(3, graph);
  bfs(3L, graph, path, 0L);

  long expected_path[] = { 0, 1, 3, 2, 4, 6, 5, 7, 8 };
  int i;
  printf("\n");
  for (i = 0; i < 9; i++) printf("%li, ", path[i]);
  printf("\n");
  for (i = 0; i < 9; i++) printf("%li, ", expected_path[i]);
  printf("\n");
  assert(0 == memcmp(path, expected_path, 4*sizeof(long)));
}

void test_bfs_large()
{
  clock_t begin, end;
  double time_spent;
  long size = 128;
  long *graph = zeros(size);
  long *path = malloc(size);

  grid_graph(size, graph);

  begin = clock();

  /* long *graph = zeros(200); */
  /* assert(NULL != graph); */
  /* grid_graph(200, graph); */
  bfs(size, graph, path, 0L);

  end = clock();
  time_spent = 1000*(double)(end - begin) / CLOCKS_PER_SEC;
  printf("Time: %f", time_spent);
  /* long expected_path[] = { 0, 1, 3, 2, 4, 6, 5, 7, 8 }; */
  /* int i; */
  /* printf("\n"); */
  /* for (i = 0; i < size*size; i++) printf("%li, ", path[i]); */
  /* printf("\n"); */
  /* for (i = 0; i < size*size; i++) printf("%li, ", expected_path[i]); */
  /* printf("\n"); */
  /* assert(0 == memcmp(path, expected_path, 4*sizeof(long))); */
}

int main(int argc, const char * argv[])
{
  clock_t begin, end;
  double time_spent;


  test_2();
  test_3();
  test_4();
  test_bfs_2();
  test_bfs_3();

  begin = clock();

  /* long *graph = zeros(200); */
  /* assert(NULL != graph); */
  /* grid_graph(200, graph); */
  test_bfs_large();

  end = clock();
  time_spent = 1000*(double)(end - begin) / CLOCKS_PER_SEC;
  printf("Time: %f", time_spent);

  return 0;
}
