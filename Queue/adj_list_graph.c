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

  int row, col;
  for (row = 0; row < side; row++)
  {
    for (col = 0; col < side; col++)
    {
      int node = row * side + col;
      if (row == 0)
      {
        if (col == 0)
        {
          graph1[node] = malloc(2 * sizeof(node_t));
          graph1[node]->node = node + 1;
          graph1[node]->next = &graph1[node][1];
          graph1[node][1].node = node + side;
        }
        else if (col == (side - 1))
        {
          graph1[node] = malloc(2 * sizeof(node_t));
          graph1[node]->node = node - 1;
          graph1[node]->next = &graph1[node][1];
          graph1[node][1].node = node + side;
        }
        else
        {
          graph1[node] = malloc(3 * sizeof(node_t));
          graph1[node]->node = 0;
          graph1[node]->next = &graph1[node][1];
          graph1[node][1].node = 2;
          graph1[node][1].next = &graph1[node][2];
          graph1[node][2].node = 4;
        }
      }
      else if (row == (side - 1))
      {
        if (col == 0)
        {
          graph1[node] = malloc(2 * sizeof(node_t));
          graph1[node]->node = node - side;
          graph1[node]->next = &graph1[node][1];
          graph1[node][1].node = node + 1;
        }
        else if (col == (side - 1))
        {
          graph1[node] = malloc(2 * sizeof(node_t));
          graph1[node]->node = node - side;
          graph1[node]->next = &graph1[node][1];
          graph1[node][1].node = node - 1;
        }
        else
        {
          graph1[node] = malloc(3 * sizeof(node_t));
          graph1[node]->node = 4;
          graph1[node]->next = &graph1[node][1];
          graph1[node][1].node = 6;
          graph1[node][1].next = &graph1[node][2];
          graph1[node][2].node = 8;
        }
      }
      else
      {
        if (col == 0)
        {
          graph1[node] = malloc(3 * sizeof(node_t));
          graph1[node]->node = node - side;
          graph1[node]->next = &graph1[node][1];
          graph1[node][1].node = node + 1;
          graph1[node][1].next = &graph1[node][2];
          graph1[node][2].node = node + side;
        }
        else if (col == (side -1 ))
        {
          graph1[node] = malloc(3 * sizeof(node_t));
          graph1[node]->node = 2;
          graph1[node]->next = &graph1[node][1];
          graph1[node][1].node = 4;
          graph1[node][1].next = &graph1[node][2];
          graph1[node][2].node = 8;
        }
        else
        {
          graph1[node] = malloc(4 * sizeof(node_t));
          graph1[node]->node = 1;
          graph1[node]->next = &graph1[node][1];
          graph1[node][1].node = 3;
          graph1[node][1].next = &graph1[node][2];
          graph1[node][2].node = 5;
          graph1[node][2].next = &graph1[node][3];
          graph1[node][3].node = 7;
        }
      }
    }
  }

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

void test_graph_with_9_nodes()
{
  node_t **graph1 = graph(3);

  long expected[][4] = {
    {1,3,-1,-1},{0,2,4,-1},{1,5,-1,-1},
    {0,4,6,-1},{1,3,5,7},{2,4,8,-1},
    {3,7,-1,-1},{4,6,8,-1},{5,7,-1,-1}};

  int node, i;
  for (node = 0; node < 9; node++)
  {
    node_t *p = graph1[node];
    for (i = 0; i < 4; i++)
    {
      if (expected[node][i] > -1)
      {
        assert(p->node == expected[node][i]);
        p = p->next;
      }
    }
  };
}

int main(int argc, const char * argv[])
{
  clock_t begin, end;
  double time_spent;

  test_graph_with_4_nodes();
  test_graph_with_9_nodes();

  begin = clock();

  end = clock();
  time_spent = 1000*(double)(end - begin) / CLOCKS_PER_SEC;
  printf("Time: %f", time_spent);

  return 0;
}
