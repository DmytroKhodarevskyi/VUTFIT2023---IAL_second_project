/*
 * Tabulka s rozptýlenými položkami
 *
 * S využitím datových typů ze souboru hashtable.h a připravených koster
 * funkcí implementujte tabulku s rozptýlenými položkami s explicitně
 * zretězenými synonymy.
 *
 * Při implementaci uvažujte velikost tabulky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptylovací funkce která přidělí zadanému klíči index z intervalu
 * <0,HT_SIZE-1>. Ideální rozptylovací funkce by měla rozprostírat klíče
 * rovnoměrně po všech indexech. Zamyslete sa nad kvalitou zvolené funkce.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializace tabulky — zavolá sa před prvním použitím tabulky.
 */
void ht_init(ht_table_t *table) {

  //initialize the table
  //set all the values to NULL
  for (int i = 0; i < HT_SIZE; i++) {
    (*table)[i] = NULL;
  }
}

/*
 * Vyhledání prvku v tabulce.
 *
 * V případě úspěchu vrací ukazatel na nalezený prvek; v opačném případě vrací
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {

  //check if the table is initialized
  //if not, return NULL
  if (table == NULL) {
    return NULL;
  }

  //for all the items in the table
  for (int i = 0; i < HT_SIZE; i++) {

    //get the item
    ht_item_t *tmp = (*table)[i];

    //search in the list, while the item is not NULL
    while (tmp != NULL) {
      //if the key is the same, return the item
      if (strcmp(tmp->key, key) == 0) {
        return tmp;
      }

      //else, go to the next item
      tmp = tmp->next;
    }
  }

  //if the item is not found, return NULL
  return NULL;
}

/*
 * Vložení nového prvku do tabulky.
 *
 * Pokud prvek s daným klíčem už v tabulce existuje, nahraďte jeho hodnotu.
 *
 * Při implementaci využijte funkci ht_search. Pri vkládání prvku do seznamu
 * synonym zvolte nejefektivnější možnost a vložte prvek na začátek seznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {

  //search for the item
  ht_item_t *tmp = ht_search(table, key);

  //if the item is found, change its value
  if (tmp != NULL) {
    tmp->value = value;
    return;
  }

  //else, create a new item
  int index = get_hash(key);
  ht_item_t *new_item = malloc(sizeof(ht_item_t));

  if(new_item == NULL) {
    return;
  }

  //set the values
  new_item->key = key;
  new_item->value = value;
  new_item->next = NULL;

  //if the hashtable place is empty, add the new item
  if ((*table)[index] == NULL) {
    (*table)[index] = new_item;
    return;
  }

  //if the hashtable place is occupied, add the new item to the beginning of the list
  new_item->next = (*table)[index];
  (*table)[index] = new_item;

}

/*
 * Získání hodnoty z tabulky.
 *
 * V případě úspěchu vrací funkce ukazatel na hodnotu prvku, v opačném
 * případě hodnotu NULL.
 *
 * Při implementaci využijte funkci ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {

  //search for the item
  ht_item_t *tmp = ht_search(table, key);

  //if the item is found, return its value
  if (tmp != NULL) {
    return &tmp->value;
  }

  //else, return NULL
  return NULL;
}

/*
 * Smazání prvku z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje přiřazené k danému prvku.
 * Pokud prvek neexistuje, funkce nedělá nic.
 *
 * Při implementaci NEPOUŽÍVEJTE funkci ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {

  //for all the items in the table
  for (int i = 0; i < HT_SIZE; i++) {

    //set the item and the previous item
    ht_item_t *tmp = (*table)[i];
    ht_item_t *prev = NULL;

    //search in the list, while the item is not NULL
    while (tmp != NULL) {

      //if the key is the same, delete the item
      //and make the previous item point to the next item
      if (strcmp(tmp->key, key) == 0) {

        if (prev == NULL) {
          (*table)[i] = tmp->next;
        } else {
          prev->next = tmp->next;
        }

        free(tmp);
        return;
      }

      //else, go to the next item
      prev = tmp;
      tmp = tmp->next;

    }
  }
}

/*
 * Smazání všech prvků z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje a uvede tabulku do stavu po 
 * inicializaci.
 */
void ht_delete_all(ht_table_t *table) {

  //for all the items in the table
  for (int i = 0; i < HT_SIZE; i++) {

    //set the temporary item
    ht_item_t *tmp = (*table)[i];

    //for all the items in the list 
    while (tmp != NULL) {

      //delete the item and go to the next item
      ht_item_t *next = tmp->next;
      free(tmp);
      tmp = next;

    }

    //set the item to NULL
    (*table)[i] = NULL;
    
  }
}
