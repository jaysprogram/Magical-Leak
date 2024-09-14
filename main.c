#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Make link list first


typedef struct Node {
    Node * next;
    int val;
}Node;

typedef struct Queue {
    Node * head;
    Node * tail;
}LL;

typedef struct Queue{
    LL * list; 
}Queue; 

//List functions

Node * createNode(int val){
    Node * newNode = malloc(sizeof(Node));
    if( newNode->next != NULL)
        printf("New node failed");

    newNode->val = val;
    newNode->next = NULL;
    return newNode;
} 

void removeHead(LL * list){
    //if no node
    if (list->head == NULL){
        printf("List is empty\n");
        return;
    }// if only one node
    if(list->head->next == NULL){
    free(list->head);
    list -> head = NULL;
    list -> tail = NULL;
    return;
    }
    // if 2 or more node in list
    Node * newHead = list->head->next;
    free(list->head);
    list->head = newHead;
}

void addToTail(LL * list, int val){
    Node * newTail = createNode(val);
    Node * oldTail = list->tail;
    // check if empty
    if(list->head == NULL){
        printf("Add to tail: list is empty.");
        list->head = list->tail = newTail;
        return;
    }//add to tail
    oldTail->next = newTail; // set old tail to point to new tail
    list->tail = newTail; // now new tail is tail in LL
    return;
}

// Queue functions

// remove front of list
void deque(Queue * q){
    removeHead(&(q->list));
}

void enque(Queue * q, int val){
    addToTail(&(q->list), val);
}

// peak at head
int peek(Queue * q){
    return q->list->head->val;
}

int dx[4] = {0, 1, 0, -1};
int dy[4] = {1, 0, -1 , 0};

int main(){



// Initialize
    
// Make a map

// Flood loop

// 3 checks
    // bounds check
    // wall check
    // visited check

// read out the results

// Clean up the memory

}










 

