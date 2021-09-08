#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "jrb.h"
#include "jval.h"
#include "dllist.h"

#define INFINITIVE_VALUE 9999

typedef struct
{
    JRB edges;
    JRB vertices;
} Graph;

typedef struct
{
    int id;
    double trongso;
} Link;
Link link[100];
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
void swap(double *a, double *b);
int findByW(double weight, Link *link);

int i, numOfV;

int main()
{

    Graph graph = createGraph();
    JRB node;
    FILE *f, *f1;
    int choice, id, total, output[10], output1[100];
    int minOut = 999, maxOut = -1;
    int minID, maxID;
    int minIn = 999, maxIn = -1;
    double minW = 999, maxW = -1;
    int numOfE;
    int total41 = 0, total42 = 0;
    int id1, id2, path[100], length;
    char str[50], str1[100];
    char nameMaxW[50], nameMinW[50];
    int n, j, k, m;
    double arr[100], prev[100];

    do
    {
        printf("1. Chuc nang 1\n");
        printf("2. Chuc nang 2\n");
        printf("3. Chuc nang 3\n");
        printf("4. Chuc nang 4\n");
        printf("5. Chuc nang 5\n");
        printf("6. Thoat\n");
        printf("Lua chon: ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            printf("a. In ra so luong webpage kem url va id\n");
            f = fopen("webpages.txt", "r");
            fscanf(f, "%d", &numOfV);
            printf("So luong webpage la: %d\n", numOfV);
            for (i = 0; i < numOfV; i++)
            {
                fscanf(f, "%s", str);
                fscanf(f, "%d", &id);
                printf("%s %d\n", str, id);
                addVertex(graph, id, str);
                link[i].id = id;
                link[i].trongso = 1;
            }
            fclose(f);

            printf("b. In ra webpage nao co nhieu lien ket toi nhat va it lien ket toi nhat\n");

            f1 = fopen("pagesConnection.txt", "r");
            if (f1 == NULL)
            {
                printf("Khong the mo file\n");
                return (-1);
            }
            while (fgets(str, 50, f1) != NULL)
            {
                char *token;
                token = strtok(str, " ");
                int num1 = atoi(token);
                printf("%d\n", num1);
                while (token != NULL)
                {
                    token = strtok(NULL, " ");
                    if (token == NULL)
                        break;
                    int num2 = atoi(token);
                    printf("%d\n", num2);
                    addEdge(graph, num1, num2, 1);
                }
            }
            for (int i = 0; i < numOfV; i++)
            {
                int totalOut = indegree(graph, link[i].id, output);
                printf("%d ", link[i].id);
                printf("%d\n", totalOut);
                if (minOut > totalOut)
                {
                    minOut = totalOut;
                    minID = link[i].id;
                }

                if (maxOut < totalOut)
                {
                    maxOut = totalOut;
                    maxID = link[i].id;
                }
            }

            printf("Webpage co nhieu lien ket toi nhat: %s %d\n", getVertex(graph, maxID), maxOut);
            printf("Webpage co it lien ket toi nhat: %s %d\n", getVertex(graph, minID), minOut);

            fclose(f1);

            break;

        case 2:
            jrb_traverse(node, graph.vertices)
            {
                int key = (int)jval_v(node->key);
                double totalW = 0;
                int numOfIn = indegree(graph, key, output); // so luong ban bac vao
                for (i = 0; i < numOfIn; i++)
                {
                    for (j = 0; j < numOfV; j++)
                    {
                        if (output[i] != link[j].id)
                            continue;
                        int numOfOut = outdegree(graph, output[i], output1);
                        if (numOfOut > 0)
                        {
                            totalW += link[j].trongso / numOfOut;
                        }
                    }
                    if (minW > totalW)
                    {
                        minW = totalW;
                        strcpy(nameMinW, getVertex(graph, key));
                    }
                    if (maxW < totalW)
                    {
                        maxW = totalW;
                        strcpy(nameMaxW, getVertex(graph, key));
                    }
                    for (int i = 0; i < numOfV; i++)
                    {
                        if (key == link[i].trongso)
                            link[i].trongso = totalW;
                    }
                }
            }
            printf("Webpage co trong so cao nhat: %s\n", nameMaxW);
            printf("Webpage co trong so thap nhat: %s\n", nameMinW);
            break;

        case 3:

            printf("Nhap so lan lap: ");
            scanf("%d", &n);
            for (m = 0; m < n; m++)
            {
                jrb_traverse(node, graph.vertices)
                {
                    int key = jval_i(node->key);
                    printf("%d key", key);
                    double totalW = 0;
                    int numOfIn = indegree(graph, key, output); // so luong ban bac vao
                    for (i = 0; i < numOfIn; i++)
                    {
                        for (j = 0; j < numOfV; j++)
                        {
                            if (output[i] != link[j].id)
                                continue;
                            int numOfOut = outdegree(graph, output[i], output1);
                            if (numOfOut > 0)
                            {
                                totalW = totalW + link[j].trongso / numOfOut;
                            }
                        }
                        for (k = 0; k < numOfV; k++)
                        {
                            if (link[k].id == key)
                            {
                                link[k].trongso = totalW;
                                break;
                            }
                        }
                    }
                }
            }
            for (i = 0; i < numOfV; i++)
            {
                arr[i] = link[i].trongso;
            }

            for (i = 0; i < numOfV - 1; i++)
            {
                for (j = i + 1; j < numOfV; j++)
                {
                    if (arr[i] < arr[j])
                        swap(&arr[i], &arr[j]);
                }
            }

            printf("Top 3 webpage quan trong nhat\n");

            for (i = 0; i < 3; i++)
            {
                int index = findByW(arr[i], link);
                printf("%s\n", getVertex(graph, link[index].id));
            }

            break;

        case 4:
            printf("So luong webpage chi co lien ket den: ");
            jrb_traverse(node, graph.vertices)
            {
                if (outdegree(graph, (int)jval_v(node->key), output) == 0)
                    total41++;
            }
            printf("%d\n", total41);

            printf("So luong webpage chi co lien ket di: ");
            jrb_traverse(node, graph.vertices)
            {
                if (indegree(graph, (int)jval_v(node->key), output) == 0)
                    total42++;
            }
            printf("%d\n", total42);

            break;

        case 5:

            printf("Nhap id1: ");
            scanf("%d", &id1);
            printf("Nhap id2: ");
            scanf("%d", &id2);
            n = (int)shortestPath(graph, id1, id2, path, &length);
            // printf("%d\n",(int)w);
            if (n == INFINITIVE_VALUE)
                n = -1;
            printf("Khoang cach nho nhat tu %d -> %d : %d\n", id1, id2, n);

            break;

        case 6:
            printf("Cam on ban da su dung\n");
            break;

        default:
            printf("Ban da nhap sai, moi ban nhap lai!\n");
            break;
        }

    } while (choice != 6);
    return 0;
}

void swap(double *a, double *b)
{
    double tmp = *a;
    *a = *b;
    *b = tmp;
}

int findByW(double weight, Link *link)
{
    for (i = 0; i < numOfV; i++)
    {
        if (weight == link[i].trongso)
            return i;
    }
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
    }
}

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

double shortestPath_5(Graph graph, int s, int t, int *path, int *length)
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
            if ((min > distance[u] && distance[u] >= 50) || distance[u] == 0)
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
            if (w < 50)
                continue;
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