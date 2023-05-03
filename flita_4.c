#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int vertices, edges;
    int isConnected;
    char** matrix;
} mtrx;

void read_dim(int* vertices, int* edges) {
    char transit;
    FILE *matrix = fopen("matrix.txt", "r");
    for (*vertices = 0; !feof(matrix); (*vertices)++) {
        for (*edges = 0; !feof(matrix); (*edges)++) {
            fscanf(matrix, "%c", &transit);
            if (transit == ' ') (*edges)--;
            if (transit == '\n') break;
        }
    }
    fclose(matrix);
}

void read_matrix(mtrx *graph){
    char transit[8];
    FILE *matrix = fopen("matrix.txt", "r");
    for (int i = 0; i < (*graph).vertices && !feof(matrix); i++)
        for (int j = 0; j < (*graph).edges && !feof(matrix); j++){
            fscanf(matrix, "%s ", &transit);
            (*graph).matrix[i][j] = (char)atoi(transit);
        }
    fclose(matrix);
}

void write_dot (mtrx *graph){
    FILE *dot_file = fopen("graph.dot", "w");

    fprintf(dot_file, "digraph G {\n");

    for (int i = 0; i < (*graph).edges; i++) {
        for (int j = 0; j < (*graph).vertices; j++) {
            if ((*graph).matrix[j][i] != 0) {
            
                int direction = (*graph).matrix[j][i] < 0 ? -1 : 1;
                int label = (*graph).matrix[j][i] * direction;

                int k = j + 1;
                for (k; k < (*graph).vertices; k++)
                    if((*graph).matrix[k][i] != 0) break;
                
                if (k == (*graph).vertices) k = j;
                
                if (direction > 0)
                    fprintf(dot_file, "\t%c -> %c [label = %d];\n", j + 'a', k + 'a', label);
                else
                    fprintf(dot_file, "\t%c -> %c [label = %d];\n", k + 'a', j + 'a', label);

                break;
            }
        }
    }

    for (int i = 0; i < (*graph).vertices; i++) {
        for (int j = 0; j < (*graph).edges; j++) {
            if ((*graph).matrix[j][i] != 0)
                j = (*graph).edges;
            else
                fprintf(dot_file, "\t%c;\n", i + 'a');
        }
    }
    
    fprintf(dot_file, "}");
    fclose(dot_file);
}

void DFS(int vertex, char** matrix, int* visited, int vertices) {
    visited[vertex] = 1;
    for (int i = 0; i < vertices; ++i) {
        if (matrix[vertex][i] != 0 && !visited[i]) {
            DFS(i, matrix, visited, vertices);
        }
    }
}

int isGraphConnected(mtrx* graph) {
    int* visited = (int*)calloc(graph->vertices, sizeof(int));;
    
    DFS(0, graph->matrix, visited, graph->vertices);
    for (int i = 0; i < graph->vertices; ++i) {
        if (!visited[i]) {
            free(visited);
            return 0;
        }
    }
    free(visited);
    return 1;
}

void separateVertice(int vert_number, mtrx *graph){

    for (int j = 0; j < (*graph).edges; j++) {
        if ((*graph).matrix[vert_number][j] != 0){
            for (int i = 0; i < (*graph).vertices; i++)
                (*graph).matrix[i][j] = 0;
        }
    }

    write_dot (graph);

}

int main() {

    mtrx graph;

    read_dim(&graph.vertices, &graph.edges); 

    graph.matrix = (char**)malloc(sizeof(char*) * graph.edges);
    for(int i = 0; i < graph.edges; i++){
        graph.matrix[i] = (char*)malloc(sizeof(char) * graph.vertices);
    }

    read_matrix(&graph);

    graph.isConnected = isGraphConnected(&graph);
    
    write_dot(&graph);

    system("copy graph.dot C:\\Graphviz\\bin & cd C:\\Graphviz\\bin & dot.exe -Tpng graph.dot -o graph.png & move graph.png C:\\c4");

    int vert_number;
    do {
        puts("Enter vertice number to separate it\n>>");
        scanf("%d", &vert_number);
        separateVertice(vert_number, &graph);
        system("copy graph.dot C:\\Graphviz\\bin & cd C:\\Graphviz\\bin & dot.exe -Tpng graph.dot -o graph.png & move graph.png C:\\c4");

    } while (vert_number != -1);


    return 0;
}