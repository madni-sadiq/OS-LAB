
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
void execute(void (*somefunction)(void *p), void *p);
int pool_submit(void (*somefunction)(void *p), void *p);
void *worker(void *param);
void pool_init(void);
void pool_shutdown(void);
void q_init(void);
int enqueue(task t);
task dequeue();
int IsEmpty(void);
