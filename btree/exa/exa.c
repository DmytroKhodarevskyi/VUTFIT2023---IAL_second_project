/*
 * Použití binárních vyhledávacích stromů.
 *
 * S využitím Vámi implementovaného binárního vyhledávacího stromu (soubory ../iter/btree.c a ../rec/btree.c)
 * implementujte funkci letter_count. Výstupní strom může být značně degradovaný (až na úroveň lineárního seznamu) 
 * a tedy implementujte i druhou funkci (bst_balance), která strom, na požadavek uživatele, vybalancuje.
 * Funkce jsou na sobě nezávislé a tedy automaticky NEVOLEJTE bst_balance v letter_count.
 * 
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>


/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 * 
 * Funkce inicilializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku 
 * mezery ' ', a ostatních znaků (ve stromu reprezentováno znakem podtržítka '_'). Výstup je v 
 * uložen ve stromu.
 * 
 * Například pro vstupní řetězec: "abBccc_ 123 *" bude strom po běhu funkce obsahovat:
 * 
 * key | value
 * 'a'     1
 * 'b'     2
 * 'c'     3
 * ' '     2
 * '_'     5
 * 
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce.
*/

//structure for the letters, and its frequency
typedef struct letters {
  int number; //frequency
  char value; //letter
} letters_t;

int sstrlen (char *string) {
  //initialize the length
  int length = 0;

  //count the length of the string
  while (string[length] != '\0'){
    length++;
  }

  //return the length
  return length;
}

void count_frequencies(char *input, int freq[28]) {

  //count frequencies for all string
  for (int i = 0; input[i] != '\0'; i++) {

    //get the character
    char ch = input[i];

    //if the character is a letter, 
    //increment the frequency of that specific letter
    if ('a' <= ch && ch <= 'z') {
      freq[ch - 'a']++;
    
    //if the character is a space, increment space counter
    } else if (ch == ' ') {
      //Index 26 for space
      freq[26]++;

    //if the character is not a letter or a space, increment other characters counter  
    } else {
      //Index 27 for other characters
      freq[27]++;
    }
  }
}

void insert_frequencies_into_bst(bst_node_t **tree, int freq[28]) {

  //fill all the letters into the tree
  for (int i = 0; i < 26; i++) {
    if (freq[i] > 0) {
      letters_t data;
      data.number = freq[i];
      data.value = 'a' + i;
      bst_insert(tree, data.value, data.number);
    }
  }

  //fill the space
  if (freq[26] > 0) {
    letters_t data;
    data.number = freq[26];
    data.value = ' ';
    bst_insert(tree, data.value, data.number);
  }

  //fill the other characters
  if (freq[27] > 0) {
    letters_t data;
    data.number = freq[27];
    data.value = '_';
    bst_insert(tree, data.value, data.number);
  }
}

char* to_lowercase(char *str) {
    //if string is empty, return
    if (str == NULL) 
      return NULL;

    //Allocate memory for the new string (+1 for the null terminator)
    char *lowercase_str = malloc(sstrlen(str) + 1);

    if (lowercase_str == NULL) 
      return NULL;

    //Convert to lowercase
    for (int i = 0; str[i] != '\0'; ++i) {
        lowercase_str[i] = (str[i] >= 'A' && str[i] <= 'Z') ? str[i] + 32 : str[i];
    }

    //Null-terminate the new string
    lowercase_str[sstrlen(str)] = '\0';

    return lowercase_str;
}

void letter_count(bst_node_t **tree, char *input) {

  //initialize the tree
  bst_init(tree);

  if (tree == NULL) {
    return;
  }

  //convert the string to lowercase
  char *lowercase_str = to_lowercase(input);

  //count the frequencies of the letters
  int freq[28] = {0};
  count_frequencies(lowercase_str, freq);

  //insert the frequencies into the tree
  insert_frequencies_into_bst(tree, freq);

  //free the lowercase string
  free(lowercase_str);

}

int number_of_nodes(bst_node_t **tree) {
  //initialize the counter
  int cnt = 0;

  //if the tree is empty, return 0
  if (*tree == NULL) {
    return 0;
  }

  //if the tree is not empty, count the nodes
  //and recursively call the function for the left and right subtree
  cnt += number_of_nodes(&(*tree)->left);
  cnt += number_of_nodes(&(*tree)->right);

  //at the end, return the number of nodes
  return cnt + 1;
}

bst_node_t* construct_balanced_tree(bst_node_t **array, int start, int end) {

  //if the start is greater than the end, return NULL
  if (start > end)
    return NULL;

  //find the middle element of the array
  //to make it the root of the tree
  int mid = (start + end) / 2;
  bst_node_t *root = array[mid];

  //recursively construct the left and right subtree
  root->left = construct_balanced_tree(array, start, mid - 1);
  root->right = construct_balanced_tree(array, mid + 1, end);
  return root;
}


/**
 * Vyvážení stromu.
 * 
 * Vyvážený binární vyhledávací strom je takový binární strom, kde hloubka podstromů libovolného uzlu se od sebe liší maximálně o jedna.
 * 
 * Předpokládejte, že strom je alespoň inicializován. K získání uzlů stromu využijte vhodnou verzi vámi naimplmentovaného průchodu stromem.
 * Následně můžete například vytvořit nový strom, kde pořadím vkládaných prvků zajistíte vyváženost.
 *  
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce. Není nutné, aby funkce fungovala *in situ* (in-place).
*/
void bst_balance(bst_node_t **tree) {

  //count the number of nodes in the tree
  int num_of_nodes = number_of_nodes(tree);

  //initialize items
  bst_items_t *items = malloc(sizeof(bst_items_t));
  if (items == NULL) {
    return;
  }

  items->size = 0;
  items->capacity = 8;
  items->nodes = malloc(items->capacity * sizeof(bst_node_t*));
  if (items->nodes == NULL) {
    free(items);
    return;
  }

  //sort the nodes, and store them into the items
  bst_inorder(*tree, items);

  //construct the balanced tree, with the sorted nodes
  *tree = construct_balanced_tree(items->nodes, 0, num_of_nodes-1);

  //free the items
  free(items->nodes);
  free(items);

}