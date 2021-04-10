#define MIN_PRIORITY 1
#define MAX_PRIORITY 10

// add a task to the list 
void add(char *name, int priority, int burst, struct node **head);

// invoke the scheduler
void schedule(struct node *head);
void findavgTime( struct node *L, int n);
