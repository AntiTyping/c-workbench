#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <libkern/OSAtomic.h>

typedef struct _node_t {
  long node;
  struct _node_t *next;
} node_t;

node_t **graph(long side)
{
  node_t **graph1 = malloc(side*side*sizeof(node_t*));

  graph1[0] = malloc(side*sizeof(node_t));
  graph1[1] = malloc(side*sizeof(node_t));
  graph1[2] = malloc(side*sizeof(node_t));
  graph1[3] = malloc(side*sizeof(node_t));

  node_t *node = graph1[0];
  node->node = 1;
  node->next = &node[1];
  node->next->node = 2;

  node = graph1[1];
  node->node = 0;
  node->next = &node[1];
  node->next->node = 3;

  node = graph1[2];
  node->node = 0;
  node->next = &node[1];
  node->next->node = 3;

  node = graph1[3];
  node->node = 1;
  node->next = &node[1];
  node->next->node = 2;

  return graph1;
}

void test_graph_with_4_nodes()
{
  node_t **graph1 = graph(2);

  long expected[][2] = {
    {1, 2},
    {0, 3},
    {0, 3},
    {1, 2}
  };

  int row, i;
  for (row = 0; row < 4; row++)
  {
    node_t *p = graph1[row];
    for (i = 0; i < 2; i++)
    {
      assert(p->node == expected[row][i]);
      p = p->next;
    }
  };
}

int main(int argc, const char * argv[])
{
  clock_t begin, end;
  double time_spent;

  test_graph_with_4_nodes();

  begin = clock();

  end = clock();
  time_spent = 1000*(double)(end - begin) / CLOCKS_PER_SEC;
  printf("Time: %f", time_spent);

  return 0;
}
