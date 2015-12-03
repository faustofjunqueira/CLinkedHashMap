#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedhash.h"


unsigned int lkh_hash_function(lkh_linked_hash *table, lkh_key_type key) {
  unsigned int length = 0, first, second;

  while(key[length++]); // tamanho da key
  
  first = length > 0 ? (unsigned int) key[0] : 0;
  second = length > 1 ? (unsigned int) key[1] : 0;

  return ((length * first) + second) % table->capacity;
};

lkh_linked_hash *lkh_create(unsigned int capacity){
  lkh_linked_hash *hash = (lkh_linked_hash*) malloc (sizeof(lkh_linked_hash));
  hash->capacity = capacity;
  hash->size = 0;
  hash->first = hash->last = NULL;  
  hash->vector = (lkh_value_linked_hash*) malloc (sizeof(lkh_value_linked_hash) * capacity);
  memset(hash->vector,0, sizeof(lkh_value_linked_hash) * capacity);
  return hash;
}

lkh_value_linked_hash *lkh_get_policy(lkh_linked_hash *table, lkh_key_type key, char is_get) {
  lkh_value_linked_hash *data = &table->vector[lkh_hash_function(table,key)];

  if(!table->first && !table->last){ // Existe somente uma situação que ambos estão NULL que eh quando nao tem ninguem na lista
    data->next = NULL;
    data->prev = NULL;
    table->first = table->last = data;
  } else if (!data->next && !data->prev ) { // Esse caso acontece quando estou inserindo esse cara na lista
    if(!is_get) {
      data->next = NULL;
      data->prev = table->last;
      data->prev->next = table->last = data;
    } else {
      return NULL;
    }
  } else { // esse eh o caso que o cara já esta na lista e está sendo requisitado
    data->prev->next = data->next;
    data->next->prev = data->prev;
    data->next = NULL;
    data->prev = table->last;
    data->prev->next = table->last = data;
  }
  
  return data;
}

void lkh_add(lkh_linked_hash *table, lkh_key_type key, lkh_data_type value){
  if(table->capacity > table->size) {
    lkh_value_linked_hash *data = lkh_get_policy(table,key,0);
  } else {
    lkh_value_linked_hash *data = lkh_remove_lru(table);
  }
  table->size++;
  data->value = value;
}

lkh_data_type* lkh_get(lkh_linked_hash *table, lkh_key_type key) {
  lkh_value_linked_hash *data = lkh_get_policy(table,key,1);
  return data ? &(data->value) : NULL;
}

lkh_value_linked_hash lkh_remove_lru(lkh_linked_hash *table){
  lkh_value_linked_hash *data = table->first;
  table->first = data->next;
  table->first->prev = NULL;
  memset(data, 0, sizeof(lkh_value_linked_hash)); // jogando todos os valores para null
  table-size--;
  return data;
}