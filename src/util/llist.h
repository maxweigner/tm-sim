#ifndef LINKEDLIST

struct node {
  struct node* next;
  char value;
};

void list_append(struct node* start, char value);

struct node* list_prepend(struct node* start, char value);

int list_insert(struct node* start, char value, int index);

int list_length(struct node* start);

char* list_to_string(struct node* start);

#endif // !LINKEDLIST
