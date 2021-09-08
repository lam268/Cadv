#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "jrb.h"
#include "jval.h"
#include "dllist.h"

#define INFINITIVE_VALUE 9999.0

typedef struct {
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

int main() {
	FILE *f;
	JRB node;
	Graph graph = createGraph();
	int numOfV,numOfE,choice,path[100],output[100],length;
	int i,j,k,id,total,output1[100],output2[100],output3[100];
	int id1,id2;
	int total1,total2,count = 0;
	double w,maxW=-1;
	char str[50];

	do {
		printf("\n===C - Advance, HK20192===\n");
		printf("1. In danh sach dinh\n");
		printf("2. Tim ban than thiet\n");
		printf("3. Kiem tra quan he bac cau\n");
		printf("4. Tim ban chung\n");
		printf("5. Thoat\n");
		printf("Lua chon cua ban: ");
		scanf("%d",&choice);
		switch(choice) {
			case 1:
				f = fopen("data.txt","r");
				fscanf(f,"%d",&numOfV);
				fscanf(f,"%d",&numOfE);
				for(i=0;i<numOfV;i++) {
					fscanf(f,"%d",&id);
					fscanf(f,"%s",str);
					addVertex(graph,id,str);
					//printf("%d %s\n",id,str);
				}

				for(i=0;i<numOfE;i++) {
					fscanf(f,"%d %d %lf",&id1,&id2,&w);
					addEdge(graph,id1,id2,w);
				}
				fclose(f);

				jrb_traverse(node,graph.vertices) {
					printf("%d %s\n",jval_i(node->key),getVertex(graph,jval_i(node->key)));
				}
				break;

			case 2:
				printf("Nhap id: ");
				scanf("%d",&id);
				total = outdegree(graph,id,output);
				if(total == 0) {
					printf("-1\n");
					break;
				}
				for(i=0;i<total;i++) {
					w = getEdgeValue(graph,id,output[i]);
					if(maxW < w)
						maxW = w;
				}
				printf("%f\n",maxW);
				for(i=0;i<total;i++) {
					if (maxW == getEdgeValue(graph,id,output[i]))
						printf("%d %s\n",output[i],getVertex(graph,output[i]));
				}
				break;

			case 3:
				printf("Nhap id1: ");
				scanf("%d",&id1);
				printf("Nhap id2: ");
				scanf("%d",&id2);
				//printf("%d\n", hasEdge(graph,id1,id2));
				w = shortestPath(graph,id1,id2,path,&length);
				if( (!hasEdge(graph,id1,id2)) && (w != INFINITIVE_VALUE)) {
					printf("1\n");
				} else {
					printf("-1\n");
				}
				
				break;

			case 4:
				count = 0;
				printf("Nhap id1: ");
				scanf("%d",&id1);
				printf("Nhap id2: ");
				scanf("%d",&id2);
				total1 = outdegree(graph,id1,output1);
				total2 = outdegree(graph,id2,output2);

				for(i=0;i<total1;i++) {
					for(j=0;j<total2;j++) {
						if(output1[i] == output2[j]) {
							output3[count] = output2[j];
							//printf("%d\n",output3[count]);
							//printf("%s\n", getVertex(graph,output3[count]));
							count++;
						}
					}
				}
				if(count == 0) {
					printf("-1\n");
				} else {
					printf("%d\n",count );
					for(i=0;i<count;i++) {
						printf("%d %s\n",output3[i],getVertex(graph,output3[i]));
					}
				}

				break;


			case 5: 
				printf("Cam on ban da su dung\n");
				break;
			default:
				printf("Ban da nhap sai. Moi ban nhap lai!\n");
				break;
		}
	} while(choice != 5);

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

    while(!dll_empty(queue))
    {
        min = INFINITIVE_VALUE;
        dll_traverse(ptr, queue)
        {
            u = jval_i(ptr->val);
            if(min > distance[u])
            {
                min = distance[u];
                node = ptr;
            }
        }
        u = jval_i(node->val);
        visited[u] = 1;
        dll_delete_node(node);

        if(u == t)
            break;

        n = outdegree(graph, u, output);

        for (i = 0; i < n; i++)
        {
            v = output[i];
            w = getEdgeValue(graph, u, v);
            if( distance[v] > distance[u] + w)
            {
                distance[v] = distance[u] + w;
                previous[v] = u;
            }
            if(visited[v] == 0)
                dll_append(queue, new_jval_i(v));
        }
    }

    total = distance[t];
    if(total != INFINITIVE_VALUE)
    {
        tmp[0] = t;
        n = 1;
        while(t != s)
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