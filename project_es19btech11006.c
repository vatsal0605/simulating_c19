



//*****************I have made a certain number of people infected from the beginning. And also I have assumed that the one who recovered from ***//
//******************the disease isolated himself and no longer can be ever susceptible*****************//



#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define MAX_EDGES 500
#define MAX_VERTICES 3000
#define DAYS 300

struct node {       //if event == 1 it means it can be infected(transmitted)
    int id;         //if event == 2, it means it can recover
    int time;       //if status == 1, it is susceptible
    int event;
    int status;     //if status == 2, it is infected
    int length;
    struct node *parent;
    struct node *right;
    struct node *left;
};

struct node *Infected = NULL;
struct node *Susceptible = NULL;
struct node *Recovered = NULL;

struct heap {
    int id;
    int time;
    int event;
    int status;
};
void sink(struct heap *heap, int postion, int size) {
    while(2*postion <= size) {
        int j = 2*postion;
        if(j < size && heap[j].time > heap[j+1].time) j++;
        if(!(heap[postion].time > heap[j].time)) break;
        struct heap temp = heap[postion];
        heap[postion] = heap[j];
        heap[j] = temp;
        postion = j;
    }
}
void swim(struct heap *heap, int postion) {
    while(postion > 1 && heap[postion/2].time > heap[postion].time) {
        //swap index at position and position/2
        struct heap temp = heap[postion/2];
        heap[postion/2] = heap[postion];
        heap[postion] = temp;
        postion = postion/2;
    }

}
void insert(struct heap *heap, struct heap newMember, int size) {
    heap[++size] = newMember;
    swim(heap, size);
}
void removeMin(struct heap *heap, int size) {
    heap[1] = heap[size--];
    sink(heap, 1, size);
}
void print(struct node *node) {
    if(node != NULL) {
        print(node->left);
        printf("%d ",node->id);
        print(node->right);
    }
}
//creating a new node
struct node* newNode(int id, int time, int event, int status) {
    struct node *temp;
    temp = (struct node *)malloc(sizeof(struct node));
    temp->id = id;
    temp->time = time;
    temp->event = event;
    temp->status = status;
    temp->length = 1;
    temp->right = NULL;
    temp->left = NULL;
    temp->parent = NULL;
    return temp;
}

//function prototype to insert a new node
struct node* insertTree(struct node *node, int id, int time, int event, int status) {
    if(node == NULL) {
        return newNode(id, time, event, status);
    }
    if(node->id > id) {
        struct node *temp;
        temp = insertTree(node->left, id, time, event, status);
        node->left = temp;
        node->length++;
        temp->parent = node;
    } else if(node->id < id) {
        struct node *temp;
        temp = insertTree(node->right, id, time, event, status);
        node->right = temp;
        node->length++;
        temp->parent = node;
    }
    return node;
}
//function prototype to find the inorder predecessor which is needed in deletion of a node with two children
struct node* inorderPredecessor(struct node* root) {
    struct node *temp;
    temp = root;
    while(temp->right != NULL) {
        temp = temp->right;
    }
    return temp;
}
struct node* delete(struct node *root, int id) {
    if(root == NULL) {
        return root;
    }

    if(id < root->id) {
        root->left = delete(root->left, id);
    } else if(id > root->id) {
        root->right = delete(root->right, id);
    } else {
        if(root->left == NULL) {
            struct node *temp;
            temp = root->right;
            //temp->lenth--;
            free(root);
            return temp;
        } else if(root->right == NULL) {
            struct node *temp;
            temp = root->left;
            //temp->length--;
            free(root);
            return temp;
        } else {
            struct node *temp = inorderPredecessor(root->left);
            root->id = temp->id;
            root->left = delete(root->left, temp->id);
        }
    }
    return root;
}
//function prototype to find the location of a particular node
struct node* findNode(struct node *node, int id) {
    //printf("c1\n");
    if(node == NULL) {
        return node;
    }
    if(id == node->id) {
        return node;
    }
    if(id < node->id) {
        return findNode(node->left,id);
    } else if(id > node->id) {
        return findNode(node->right, id);
    }
}
int length(struct node *node) {

    if(node == NULL) return 0;
    return node->length;
}
int main() {
    srand(time(0));
    int vertices = rand() % MAX_VERTICES;


    int maxEdges = rand() % MAX_EDGES;
    printf("vertices: %d maximum number of edges: %d\n",vertices, maxEdges);
    printf("I will print the list of Susceptible, Infected and Recovered people for every Day.\n");
    printf("Please, be patient.For a data of such length, it may take a couple of minutes for the program to complete.\n");
    struct node *a[vertices];

    //initialising the head of each liked list to NULL
    for(int i = 0; i < vertices; i++) a[i] = NULL;
    //forming the undirected graph
    for(int i = 0; i < vertices; i++) {

        int flag = 0;
        for(int j = 1; j <= maxEdges; j++) {

            if(rand()%2 == 1) {
                //generate a unique random number number everytime

                int linkedVertex = rand() % vertices;
                while(findNode(a[i], linkedVertex) != NULL) {
                    //regenerate a random number

                    linkedVertex = rand() % vertices;
                }

                if(length(a[i]) < maxEdges && length(a[linkedVertex]) < maxEdges && linkedVertex != i) {

                    a[i] = insertTree(a[i],linkedVertex, 0, 0, 0);
                    a[linkedVertex] = insertTree(a[linkedVertex],i, 0, 0, 0);
                } else flag = 1;
            }
            if(flag == 1) break;
        }
    }
    for(int i = 0; i < vertices; i++) {
        //print each linked list  in the array
        printf("%d : ",i);
        print(a[i]);
        puts("");
    }

    struct heap heap[vertices];
    int size = -1; //the variable to store the size of the heap i.e the number of elements in the heap

    struct heap newNode = {-1, -1, -1, -1};
    insert(heap, newNode, size++);

    //lets infect random number of people
    int infected = rand() % vertices;       //people with id from 0 to infected-1 are infected from the beginning
    printf("No of people infected from the beginning: %d\n",infected);
    for(int i = 0; i < infected; i++) {
        //printHeap(heap,size);
        struct heap newNode2 = {i, 1, 2, 2};
        while(rand() % 10 > 1) newNode2.time++;     //to get the days after which it will recover
        insert(heap, newNode2, size++);
        Infected = insertTree(Infected, newNode2.id, newNode2.time, 2, 2);
    }
    auto void infect(struct node *node, int time) {
        if(node != NULL) {
            infect(node->left, time);
            if(findNode(Recovered, node->id) == NULL) {
                if(findNode(Infected, node->id) == NULL)
                Susceptible = insertTree(Susceptible, node->id, time, 1, 1);
            }
            infect(node->right, time);
        }
    }
    auto void daysPass(struct node *node, int days) {
        if(node  != NULL) {
            daysPass(node->left, days);
            if(node->time < days) {
                Susceptible = delete(Susceptible, node->id);
            } else {
                if(rand() % 10 > 4) {
                    int toRemoveId = node->id;
                    Susceptible = delete(Susceptible, toRemoveId);
                    Infected = insertTree(Infected, toRemoveId, days, 1, 1);
                    struct heap newMember  = {toRemoveId, days+1, 2, 2};
                    while(rand()%10 > 1) {
                        newMember.time++;
                    }
                    insert(heap, newMember, size++);
                    infect(a[toRemoveId], newMember.time);
                }
            }
            daysPass(node->right, days);
        }
    }
    for(int i = 1; i <= size; i++) {
        infect(a[heap[i].id], heap[i].time);
    }


    printf("DAY 0:\n");
    printf("Susceptible :");print(Susceptible);
    puts("");
    //Infected = newnode(Infected, heap[1].id, heap[1].time);
    printf("Infected: ");print(Infected);
    puts("");

    printf("Recovered: ");print(Recovered);
    puts("");
    //tossing a coin for each neighbor to get infected
    int days = 1;
    while(days <= DAYS) {
        //checking the priority queue if today is the day for the patient to be recovered

        if(size >= 1) {
            while(heap[1].time == days) {

                int toRemoveId = heap[1].id;
                removeMin(heap, size--);


                Infected = delete(Infected, toRemoveId);

                Recovered = insertTree(Recovered, toRemoveId, days, -1, 3);

                if(size == 0) break;
            }
        }

        //to see if the susceptible get infected today

        daysPass(Susceptible, days);

        printf("\n");
        printf("DAY :%d\n\n",days);
        printf("Susceptible: ");
        print(Susceptible);
        puts("");

        printf("Infected: ");
        print(Infected);
        puts("");

        printf("Recovered: ");
        print(Recovered);
        puts("");

        days++;
    }
}

