#include <stdio.h>
#include <stdlib.h>
#include "linkedhash.h"

void print_sorted(lkh_linked_hash *table){
  lkh_value_linked_hash *it = (lkh_value_linked_hash*) malloc(sizeof(lkh_value_linked_hash));
  lkh_value_linked_hash *fake = it;
  it->next = table->first;
  while((it = it->next)){
    printf("%s\n", it->value);
  }
  free(fake);
}


void print(lkh_linked_hash *table){
  int i;
  for(i = 0; i < table->capacity; i++){
    printf("%d) ", i);
    lkh_value_linked_hash *it = table->vector[i];
    while(it){
      printf("(%s %s) ", it->key,it->value);
      it = it->next_synonym;
    }
    printf("NULL\n");
  }
}

void print_detail(lkh_linked_hash *table){
  printf("capacity: %d\n",table->capacity);
  printf("size: %d\n",table->size);
  printf("first: %lX\n",(long) table->first);
  printf("last: %lX\n",(long) table->last);
}
int main(){
  lkh_linked_hash *table = lkh_create(5, .5);
  lkh_add(table, "a1","oi 1");
  lkh_add(table, "a2","oi 2");
  lkh_add(table, "a3","oi 3");
  lkh_add(table, "a4","oi 4");
  lkh_add(table, "a5","oi 5");
  lkh_add(table, "a6","oi 6");
  lkh_add(table, "a7","oi 7");
  lkh_add(table, "aa","aa 7");

  print_detail(table);
  print(table);
  char **st = lkh_get(table, "a4");
  printf("------------------------------\n");
  printf("encontrado: %s\n", *st);
  printf("------------------------------\n");
  lkh_remove(table, "a2");
  print(table);
  printf("------------------------------\n");

  lkh_remove_lru(table);
  print_sorted(table);
  printf("------------------------------\n");

  lkh_remove_lru(table);
  print_sorted(table);
  printf("------------------------------\n");

  lkh_remove_lru(table);
  print_sorted(table);
  printf("------------------------------\n");

  lkh_remove_lru(table);
  print_sorted(table);
  printf("------------------------------\n");

  lkh_remove_lru(table);
  print_sorted(table);
  printf("------------------------------\n");

  lkh_remove_lru(table);
  print_sorted(table);
  printf("------------------------------\n");

  lkh_remove_lru(table);
  print_sorted(table);
  printf("------------------------------\n");

  print_detail(table);

  return 0;
}