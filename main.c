#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

long long regionSizesID[100];

//Directions
int dx[4] = {0, 1, 0, -1};
int dy[4] = {1, 0, -1 , 0};

//Array List 
typedef struct Array{
    int size;
    int cap;
    int * list;
}Array;

// Structure to represent a point in the map
typedef struct {
    int x, y;
} Point;

// Node for the queue that will hold a Point
typedef struct Node {
    struct Node *next;
    Point point;
} Node;

typedef struct Queue {
    Node *front;
    Node *rear;
} Queue;


// Create a new node with the given Point
Node *createNode(Point point) {
    Node *newNode = malloc(sizeof(Node));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    newNode->point = point;
    newNode->next = NULL;
    return newNode;
}

// Create a new empty queue
Queue *createQueue() {
    Queue *queue = malloc(sizeof(Queue));
    if (queue == NULL) {
        fprintf(stderr, "Queue creation failed\n");
        return NULL;
    }
    queue->front = queue->rear = NULL;
    return queue;
}

// Check if the queue is empty
int isEmpty(const Queue *queue) {
    return (queue->front == NULL);
}

// Enqueue a Point into the queue
void enqueue(Queue *queue, Point point) {
    Node *newNode = createNode(point);
    if (newNode == NULL) {
        fprintf(stderr, "Enqueue failed: couldn't create node\n");
        return;
    }
    if (isEmpty(queue)) {
        queue->front = queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}

// Dequeue a Point from the queue
Point dequeue(Queue *queue) {
    if (isEmpty(queue)) {
        fprintf(stderr, "Queue is empty\n");
        return (Point){-1, -1};  // Return an invalid point in case of an empty queue
    }
    Node *temp = queue->front;
    Point point = temp->point;
    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    free(temp);
    return point;
}

// Free the queue
void freeQueue(Queue *queue) {
    while (!isEmpty(queue)) {
        dequeue(queue);
    }
    free(queue);
}



// debug functions
void printMap(char ** map,int  row){
    for (int i = 0; i < row; i++){
        printf("%s\n", map[i]);
    }
}


void printVisited(int ** visited, int col, int row){
    printf("\n========Visited=========\n");
    for (int pointerIndex = 0; pointerIndex < row; pointerIndex++){
        for (int arrayIndx = 0; arrayIndx < col; arrayIndx++){ // selfexplanatory 
            printf("%d ", visited[pointerIndex][arrayIndx]);
        }
        printf("\n");
    }
}


long long int floodSearch(char ** map,int **visited, int startX, int startY,int rows, int cols, int regionId){
    
    Queue * q = createQueue();
    Point p; // point coordinates as a struct
    p.x = startX;
    p.y= startY;
    enqueue(q,p); // add starting point to queue to start looking in both directions

    visited[p.x][p.y] = regionId; // set current starting point as visited
    long long int regionSize = 0;

    //begin BFS search
    while (!isEmpty(q)){
        Point currPt = dequeue(q);
        regionSize++; // while the queue is not empty continue

        for (int i = 0; i < 4; i++){ // look at all 4 directions
            int newX = currPt.x + dx[i];
            int newY = currPt.y + dy[i];

            if (newX >= 0 && newX < rows && newY < cols && newY >= 0 &&
             visited[newX][newY] == 0 && map[newX][newY] == '.'){
                p.x = newX;
                p.y = newY;
                enqueue(q, p);
                visited[newX][newY] = regionId;
                
            }

        }
    
    }
    freeQueue(q);
    return regionSize;
}



//take input
void input(int * row, int * col, char *** map){
    scanf("%d %d", row, col); // take input for row and column
    //printf("row: %d col: %d\n", *row, *col);

    *map = (char **)malloc(sizeof(char *) * (*row)); // init row of pointers
    for (int i = 0; i < *row; i++){
        (*map)[i] = (char *)malloc(sizeof(char) * (*col + 1)); //init the col
        //printf("%dith\n", i);

        scanf("%s", (*map)[i]); // now read each row
    }
}

long long worstCase(char ** map , int row, int col, int ** visited, long long int * regionSizesID, int x, int y, long long totalMagicLeak){ // calculate the worst or max amount of magic leak
    int adjacentRegionsIds[4];
    int numOfRegions = 0;

    // look through all 4 directions
    for ( int i = 0; i < 4; i++){
        int newX = x + dx[i];
        int newY = y + dy[i];

        //check if in bounds
        if (newX < row && newX >= 0 && newY < col && newY >= 0){
            if(map[newX][newY] == '.'){
                int regionId = visited[newX][newY];


                //check if region was not check
                bool isUnique = true;
                for (int j = 0; j < numOfRegions; j++){
                    if (regionId == adjacentRegionsIds[j]){
                    isUnique = false;
                    break;
                    }
                }
                //if unique add to list
                if(isUnique){
                    adjacentRegionsIds[numOfRegions] = regionId;
                    numOfRegions++;
                }
            }
        }
    }

    //if there are no regions just add 1 to the total magic(x,y)
    if(numOfRegions == 0){
        return totalMagicLeak + 1;
    }
    else{//calculate the old magic leak of the region adjacent(s) regions
        long long OldMagicLeak = 0;
        long long totalSize = 0;
        for (int i = 0; i < numOfRegions; i++){
            int regionId = adjacentRegionsIds[i];
            long long size = regionSizesID[regionId];
            totalSize += size;
            OldMagicLeak += size * (size + 1) / 2; 

        }
        // new region size after merging
        long long newSize = totalSize + 1; //  include the X
        long long newMagicalLeak = newSize * (newSize + 1) / 2;

        // Calculate combine magical leak
        long long newTotalMagicalLeak  = totalMagicLeak - OldMagicLeak + newMagicalLeak;

        return newTotalMagicalLeak;


    }
}


int main(){
    int regionCounter = 1;
    int  row, col;
    char **map;
    long long totalMagicLeak = 0;
    


    input(&row, &col, &map);
    printMap(map, row); 

    //visited array
    int **visited;
    visited = (int **)malloc(sizeof(int*) * row); // allocate array of int pointers
    for( int i = 0; i < row; i++){
        visited[i] = (int *)calloc(col, sizeof(int)); //array of ints
    }

    //iterate through each indices in map
    for (int pIndx = 0; pIndx < row; pIndx++){
        for (int arryIndx = 0; arryIndx < col; arryIndx++){
            if(map[pIndx][arryIndx] == '.' && !visited[pIndx][arryIndx]){ //check for a new region
                long long int regionSize = floodSearch(map,visited,pIndx,arryIndx,row,col,regionCounter);
                long long magicLeak = regionSize * (regionSize + 1) / 2;
                regionSizesID[regionCounter] = regionSize;
                totalMagicLeak += magicLeak; 
                regionCounter++;
                printf("size: %lld\n leak amount: %lld\n", regionSize, magicLeak);
            }    
        } 
    }
    long long maxMagic = totalMagicLeak; // so now max magic is the initial  BFS search

    for (int i = 1; i < regionCounter; i++ )
        printf("region size of %d: %lld\n", i, regionSizesID[i]);
        
    // For loop for the failing point cases
    for (int x = 0; x < row; x++ ){
        for (int y = 0; y < col; y++){
            if(map[x][y] == 'X'){
            long long current = worstCase(map, row, col, visited, regionSizesID, x, y,totalMagicLeak);
            if(maxMagic < current)
                maxMagic = current;
            }
        }
    }

    printf("%lld\n",maxMagic);

    // free all of map
    for (int i = 0; i < row; i++)
        free(map[i]);
    free(map);

    //free all of visited
    //printVisited(visited, col, row);
    for (int i = 0; i < row; i++)
        free(visited[i]);
    free(visited);


    return 0;
}