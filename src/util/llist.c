#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "llist.h"


void list_append(struct node* start, char value) {
  if (start == NULL) {
    start = malloc(sizeof(struct node));
  }
  if (start->value == '\0') {
    start->value = value;
    return;
  }

  struct node* current = start;
  while (current->next != NULL) {
    current = current->next;
  }

  struct node* new = malloc(sizeof(struct node));

  new->value = value;
  current->next = new;
}


struct node* list_prepend(struct node* start, char value) {
  struct node* new_start = malloc(sizeof(struct node));
  new_start->next = start;
  new_start->value = value;
  return new_start;
}


int list_insert(struct node* start, char value, int index) {
  if (start == NULL && index != 0) {
    printf("Insert on given index invalid\n");
    return -1;
  }
  
  if (index == 0) {
    list_prepend(start, value);
    return 0;
  }

  struct node* current = start;
  for (int i = 0; i != index-1; ++i) {
    current = current->next;
  }

  if (current == NULL) {
    printf("Insert on given index invalid\n");
    return -2;
  }

  struct node* to_insert = malloc(sizeof(struct node));

  to_insert->value = value;
  to_insert->next = current->next;
  current->next = to_insert;
  
  return 0;
}


int list_length(struct node* start) {
  if (start->value == '\0') {
    return 0;
  }

  int ret = 1;
  struct node* current = start;
  
  while (current->next != NULL) {
    ret++;
    current = current->next;
  }

  return ret;
}


char* list_to_string(struct node* start) {
  char* ret = malloc(list_length(start));
  struct node* current = start;

  for (int i = 0; current->next != NULL; ++i) {
    ret[i] = current->value;
    current = current->next;
  }

  if (current->value != '\0') {
    strcat(ret, &current->value);
  }

  return ret;
}

