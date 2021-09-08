#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "jrb.h"
#include "jval.h"
#include "dllist.h"

#define INFINITIVE_VALUE 9999.0

typedef struct
{
    JRB edges;
    JRB vertices;
} Graph;

Graph createGraph();
void dropGraph(Graph graph);
void addVertex(Graph graph, int id, char *name);
void addEdge(Graph graph, int v1, int v2, double weight);
double getEdgeValue(Graph graph, int v1, int v2);
char *getVertex(Graph graph, int id);
int hasEdge(Graph graph, int v1, int v2);
int indegree(Graph graph, int v, int *output);
int outdegree(Graph graph, int v, int *output);
double shortestPath(Graph graph, int s, int t, int *path, int *length);
int getNumOfV(Graph graph);
int getNumOfE(Graph graph);

int main()
{
    FILE *f;
    JRB node;
    Graph graph = createGraph();
    int numOfV, numOfE, choice, path[100], output[100], length;
    int i, j, k, id, total, output1[100], output2[100], output3[100], max;
    int id1, id2;
    int total1, total2, count = 0;
    double w, maxW = -1;
    char str[50];

    do
    {
        printf("\n===C - Advance, HK20192===\n");
        printf("1. In ma tran ke\n");
        printf("2. In danh sach ke cua moi dinh\n");
        printf("3. Danh sach thanh tri chi di bo\n");
        printf("4. Danh sach thanh tri ban bac\n");
        printf("5. Duong di ngan nhat\n");
        printf("6. Thoat\n");
        printf("Lua chon cua ban: ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            f = fopen("k62.txt", "r");
            fscanf(f, "%d", &numOfV);
            fscanf(f, "%d", &numOfE);
            for (i = 0; i < numOfV; i++)
            {
                addVertex(graph, i + 1, " ");
                //printf("%d %s\n",id,str);
            }

            for (i = 0; i < numOfE; i++)
            {
                fscanf(f, "%d %d %lf", &id1, &id2, &w);
                addEdge(graph, id1, id2, w);
            }
            fclose(f);

            for (int i = 0; i < numOfV; i++)
            {
                for (int j = 0; j < numOfV; j++)
                {
                    if (hasEdge(graph, i + 1, j + 1))
                        printf("%d ", (int)getEdgeValue(graph, i + 1, j + 1));
                    else
                        printf("0 ");
                }
                printf("\n");
            }
            break;

        case 2:
            printf("Danh sach ke cua moi thanh tri se la: ");
            for (int i = 0; i < numOfV; i++)
            {
                printf("\n- Castle %d:", i + 1);
                total = outdegree(graph, i + 1, output);
                for (int j = 0; j < total; j++)
                {
                    printf("%d ", output[j]);
                }
            }
            break;

        case 3:
            printf("Danh sach co the di bo se la: ");
            for (int i = 1; i <= numOfV; i++)
            {
                total = outdegree(graph, i, output);
                for (j = 0; j < total; j++)
                {
                    if (50 > getEdgeValue(graph, i, output[j]))
                    {
                        break;
                    }
                }
                if (j == total)
                    printf("%d ", i);
            }
            break;

        case 4:
            printf("Danh sach ban bac lon nhat se la: ");
            max = 0;
            for (int i = 0; i < numOfV; i++)
            {
                total = outdegree(graph, i + 1, output);
                if (total >= max)
                {
                    max = total;
                }
            }
            for (int i = 0; i < numOfV; i++)
            {
                if (outdegree(graph, i + 1, output) == max)
                {
                    printf("%d ", i + 1);
                }
            }
            break;
        case 5:
            printf("Nhap s: ");
            scanf("%d", &id1);
            printf("Nhap t: ");
            scanf("%d", &id2);
            w = shortestPath(graph, id1, id2, path, &length);
            if (w != INFINITIVE_VALUE)
            {
                printf("%lf\n ", w);
                printf("- Thanh tri phai di qua :");
                for (int i = 0; i < length; i++)
                {
                    printf("%d ", path[i]);
                }
            }
            else
            {
                printf("ROUTE NOT FOUND\n");
            }

            break;

        case 6:
            printf("Cam on ban da su dung\n");
            break;
        default:
            printf("Ban da nhap sai. Moi ban nhap lai!\n");
            break;
        }
    } while (choice != 5);

    return 0;
}

Graph createGraph()
{
    Graph graph;

    graph.edges = make_jrb();
    graph.vertices = make_jrb();

    return graph;
}

void dropGraph(Graph graph)
{
    JRB node, tree;

    jrb_traverse(node, graph.edges)
    {
        tree = (JRB)jval_v(node->val);
        jrb_free_tree(tree);
    }
    jrb_free_tree(graph.edges);
    jrb_free_tree(graph.vertices);
}

void addVertex(Graph graph, int id, char *name)
{
    JRB node = jrb_find_int(graph.vertices, id);

    if (node == NULL)
        jrb_insert_int(graph.vertices, id, new_jval_s(strdup(name)));
}

void addEdge(Graph graph, int v1, int v2, double weight)
{
    JRB node, tree;
    if (!hasEdge(graph, v1, v2))
    {
        //add v1->v2
        node = jrb_find_int(graph.edges, v1);
        if (node == NULL)
        {
            tree = make_jrb();
            jrb_insert_int(graph.edges, v1, new_jval_v(tree));
        }
        else
        {
            tree = (JRB)jval_v(node->val);
        }
        jrb_insert_int(tree, v2, new_jval_d(weight));
        //add v2->v1
        node = jrb_find_int(graph.edges, v2);
        if (node == NULL)
        {
            tree = make_jrb();
            jrb_insert_int(graph.edges, v2, new_jval_v(tree));
        }
        else
        {
            tree = (JRB)jval_v(node->val);
        }
        jrb_insert_int(tree, v1, new_jval_d(weight));
    }
}

// in ma tran ke
double getEdgeValue(Graph graph, int v1, int v2)
{
    JRB node, tree, node1;

    node = jrb_find_int(graph.edges, v1);
    if (node == NULL)
        return INFINITIVE_VALUE;
    tree = (JRB)jval_v(node->val);
    node1 = jrb_find_int(tree, v2);

    if (node1 == NULL)
        return INFINITIVE_VALUE;

    return jval_d(node1->val);
}

char *getVertex(Graph graph, int id)
{
    JRB node = jrb_find_int(graph.vertices, id);

    if (node == NULL)
        return NULL;
    else
        return jval_s(node->val);
}

int hasEdge(Graph graph, int v1, int v2)
{
    JRB node, tree;

    node = jrb_find_int(graph.edges, v1);
    if (node == NULL)
        return 0;
    tree = (JRB)jval_v(node->val);
    if (jrb_find_int(tree, v2) == NULL)
        return 0;
    else
        return 1;
}

int indegree(Graph graph, int v, int *output)
{
    JRB tree, node;
    int total = 0;
    jrb_traverse(node, graph.edges)
    {
        tree = (JRB)jval_v(node->val);
        if (jrb_find_int(tree, v))
        {
            output[total] = jval_i(node->key);
            total++;
        }
    }
    return total;
}

// in canh ke cua moi dinh
int outdegree(Graph graph, int v, int *output)
{
    JRB tree, node;
    int total = 0;

    node = jrb_find_int(graph.edges, v);

    if (node == NULL)
        return 0;

    tree = (JRB)jval_v(node->val);

    jrb_traverse(node, tree)
    {
        output[total] = jval_i(node->key);
        total++;
    }
    return total;
}

double shortestPath(Graph graph, int s, int t, int *path, int *length)
{
    double distance[1000], min, w, total;
    int previous[1000], tmp[1000], visited[1000];
    int n, output[100], i, v, u;
    Dllist queue, ptr, node;

    for (i = 0; i < 1000; i++)
    {
        distance[i] = INFINITIVE_VALUE;
        visited[i] = 0;
        previous[i] = 0;
    }
    distance[s] = 0;
    previous[s] = s;
    visited[s] = 1;

    queue = new_dllist();
    dll_append(queue, new_jval_i(s));

    while (!dll_empty(queue))
    {
        min = INFINITIVE_VALUE;
        dll_traverse(ptr, queue)
        {
            u = jval_i(ptr->val);
            if (min > distance[u])
            {
                min = distance[u];
                node = ptr;
            }
        }
        u = jval_i(node->val);
        visited[u] = 1;
        dll_delete_node(node);

        if (u == t)
            break;

        n = outdegree(graph, u, output);

        for (i = 0; i < n; i++)
        {
            v = output[i];
            w = getEdgeValue(graph, u, v);
            if (distance[v] > distance[u] + w)
            {
                distance[v] = distance[u] + w;
                previous[v] = u;
            }
            if (visited[v] == 0)
                dll_append(queue, new_jval_i(v));
        }
    }

    total = distance[t];
    if (total != INFINITIVE_VALUE)
    {
        tmp[0] = t;
        n = 1;
        while (t != s)
        {
            t = previous[t];
            tmp[n++] = t;
        }

        for (i = n - 1; i >= 0; i--)
        {
            path[n - i - 1] = tmp[i];
        }
        *length = n;
    }

    return total;
}

int getNumOfV(Graph graph)
{
    JRB node;
    int total = 0;

    jrb_traverse(node, graph.vertices)
        total++;

    return total;
}

int getNumOfE(Graph graph)
{
    JRB node1, node2;
    int total = 0;

    jrb_traverse(node1, graph.vertices)
        jrb_traverse(node2, graph.vertices) if (hasEdge(graph, jval_i(node1->key), jval_i(node2->key)))
            total++;

    return total;
}