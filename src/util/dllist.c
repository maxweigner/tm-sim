#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dllist.h"


struct dnode* dlist_from_string(char input[]) {
  struct dnode* new_list = malloc(sizeof(struct dnode));
  
  for (int i = 0; i < strlen(input); ++i) {
    dlist_append(new_list, input[i]);
  }
  
  return new_list;
}


void dlist_append(struct dnode* start, char value) {
  if (start == NULL) {
    start = malloc(sizeof(struct dnode));
  }
  if (start->value == '\0') {
    start->value = value;
    return;
  }

  struct dnode* current = start;
  while (current->next != NULL) {
    current = current->next;
  }

  struct dnode* new = malloc(sizeof(struct dnode));

  new->value = value;
  new->prev = current;
  current->next = new;
}


struct dnode* dlist_prepend(struct dnode* start, char value) {
  struct dnode* new_start = malloc(sizeof(struct dnode));
  
  new_start->next = start;
  start->prev = new_start;

  new_start->value = value;
  return new_start;
}


int dlist_insert(struct dnode* start, char value, int index) {
  if (start == NULL && index != 0) {
    printf("Insert on given index invalid\n");
    return -1;
  }
  
  if (index == 0) {
    dlist_prepend(start, value);
    return 0;
  }

  struct dnode* current = start;
  for (int i = 0; i != index-1; ++i) {
    current = current->next;
  }

  if (current == NULL) {
    printf("Insert on given index invalid\n");
    return -2;
  }

  struct dnode* to_insert = malloc(sizeof(struct dnode));

  to_insert->value = value;

  to_insert->next = current->next;
  to_insert->prev = current;

  current->next = to_insert;
  
  return 0;
}


int dlist_length(struct dnode* start) {
  if (start->value == '\0') {
    return 0;
  }

  int ret = 1;
  struct dnode* current = start;
  
  while (current->next != NULL) {
    ret++;
    current = current->next;
  }

  return ret;
}


char* dlist_to_string(struct dnode* start) {
  char* ret = malloc(dlist_length(start));
  struct dnode* current = start;

  for (int i = 0; current->next != NULL; ++i) {
    ret[i] = current->value;
    current = current->next;
  }

  if (current->value != '\0') {
    strcat(ret, &current->value);
  }

  return ret;
}

