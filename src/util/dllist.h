#ifndef DOUBLELINKEDLIST

struct dnode {
  struct dnode* prev;
  struct dnode* next;
  char value;
};

struct dnode* dlist_from_string(char input[]);

void dlist_append(struct dnode* start, char value);

struct dnode* dlist_prepend(struct dnode* start, char value);

int dlist_insert(struct dnode* start, char value, int index);

int dlist_length(struct dnode* start);

char* dlist_to_string(struct dnode* start);

void dlist_free(struct dnode* start);

#endif // !DOUBLELINKEDLIST
