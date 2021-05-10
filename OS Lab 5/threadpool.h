
// data structures
typedef struct {
    void (*function)(void *p);
    void *data;
} task;

// a node for linked list
typedef struct node node;
struct node{
    task T;
    node* Next;
};

typedef struct {
    node* Front;
    node* Rear;
} Queue;

// function prototypes
int pool_submit(void (*somefunction)(void *p), void *p);
void pool_init(void);
void pool_shutdown(void);
