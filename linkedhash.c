#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedhash.h"

#define lkh_malloc(T,S) (T*) malloc(sizeof(T)*S);

unsigned int lkh_hash_function(lkh_linked_hash *table, lkh_key_type key) {
  unsigned int length = 0, first, second;

  while(key[length++]); // tamanho da key
  
  first = length > 0 ? (unsigned int) key[0] : 0;
  second = length > 1 ? (unsigned int) key[1] : 0;

  return ((length * first) + second) % table->capacity;
};


lkh_linked_hash *lkh_create(unsigned int capacity, float factor){
  lkh_linked_hash *table = lkh_malloc(lkh_linked_hash,1);
  
  capacity = (unsigned int)capacity/factor;

  table->capacity = capacity;
  table->size = 0;
  table->first = NULL;
  table->last = NULL;
  table->vector = lkh_malloc(lkh_value_linked_hash*,capacity);
  memset(table->vector,0,sizeof(lkh_value_linked_hash*)*capacity);
  return table;
}

lkh_value_linked_hash *lkh_create_value(lkh_key_type key,lkh_data_type value){
  lkh_value_linked_hash *data = lkh_malloc(lkh_value_linked_hash,1);
  data->value = value;
  data->key = key;
  data->next_synonym = NULL; 
  data->prev_synonym = NULL; 
  data->next = NULL;
  data->prev = NULL;
  return data;
}

void lkh_append_table(lkh_value_linked_hash **position, lkh_value_linked_hash *value) {
  
  if(*position){
    lkh_value_linked_hash* it = *position;
    while(it->next_synonym != NULL) it = it->next_synonym;
    it->next_synonym = value;
    value->prev_synonym = it;
  }else{
    *position = value;
  }
}

void lkh_lru_handle_add(lkh_linked_hash *table, lkh_value_linked_hash *data){
  if(table->last){
    table->last->next = data;
    data->prev = table->last;
    data->next = NULL;
    table->last = data;
  }else{
    table->last = data;
    table->first = data;
  }
}

void lkh_add(lkh_linked_hash *table, lkh_key_type key, lkh_data_type value){
  lkh_value_linked_hash *data = lkh_create_value(key,value);
  unsigned int index = lkh_hash_function(table,key);
  lkh_append_table(table->vector+index, data);
  lkh_lru_handle_add(table,data);
  table->size++;
}

lkh_value_linked_hash *lkh_get_ptr(lkh_linked_hash *table, lkh_key_type key){
  unsigned int index = lkh_hash_function(table,key);
  if(*(table->vector+index)){
    lkh_value_linked_hash *it = table->vector[index];
    while(it){
      if(strcmp(it->key,key) == 0) return it; //Fazer função de comparação
      it = it->next_synonym;
    }
    return NULL;
  }else{
    return NULL;
  }
}

void lkh_handle_remove_lru_key(lkh_linked_hash *table,lkh_value_linked_hash *data){
  if(data->next){
    data->next->prev = data->prev;
  }
  if(data->prev){
    data->prev->next = data->next;
  }
  if(table->first == data){
    table->first = data->next;
  }
  if(table->last == data){
    table->last = data->prev;
  }
}

lkh_data_type *lkh_get(lkh_linked_hash *table, lkh_key_type key){
  lkh_value_linked_hash *data = lkh_get_ptr(table,key);
  lkh_handle_remove_lru_key(table,data);
  lkh_lru_handle_add(table,data);  
  return data ? &data->value : NULL;
}

void lkh_handle_remove_key(lkh_value_linked_hash *value){
  if(value->prev_synonym){
    lkh_value_linked_hash *p = value->prev_synonym;
    p->next_synonym = value->next_synonym;
  }
  if(value->next_synonym){
    lkh_value_linked_hash *n = value->next_synonym;
    n->prev_synonym = value->prev_synonym;
  }
}

void lkh_remove(lkh_linked_hash *table, lkh_key_type key){
  lkh_value_linked_hash *value = lkh_get_ptr(table,key);
  if(value){
    //Remove hash table
    lkh_handle_remove_key(value);
    lkh_handle_remove_lru_key(table,value);
    if(!value->prev_synonym){
      unsigned int index = lkh_hash_function(table,key);
      table->vector[index] = value->next_synonym;
    }
    free(value);
    table->size--;
  }
}

void lkh_remove_lru(lkh_linked_hash *table){
  lkh_remove(table,table->first->key);
}