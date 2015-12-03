#ifndef LINKEDHASH_H
#define LINKEDHASH_H

/*
  Construido por Junqueira, Fausto
  LinkedHashMap sem Tratamento de Colisão
*/

typedef char* lkh_data_type;
typedef char* lkh_key_type;

typedef struct _lkh_value_linked_hash
{
  lkh_data_type value;
  lkh_key_type key;
  struct _lkh_value_linked_hash *next_synonym, *prev_synonym;
  struct _lkh_value_linked_hash *next, *prev;
}lkh_value_linked_hash;

typedef struct
{
  unsigned int capacity, size;
  lkh_value_linked_hash *first, *last; 
  lkh_value_linked_hash **vector;
}lkh_linked_hash;

lkh_linked_hash *lkh_create(unsigned int capacity, float factor);
void lkh_add(lkh_linked_hash *table, lkh_key_type key, lkh_data_type value);
lkh_data_type* lkh_get(lkh_linked_hash *table, lkh_key_type key);
void lkh_remove_lru(lkh_linked_hash *table);
void lkh_remove(lkh_linked_hash *table, lkh_key_type key);

#endif