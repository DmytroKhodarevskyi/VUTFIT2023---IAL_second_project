/*
 * Binární vyhledávací strom — rekurzivní varianta
 *
 * S využitím datových typů ze souboru btree.h a připravených koster funkcí
 * implementujte binární vyhledávací strom pomocí rekurze.
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci. 
 */
void bst_init(bst_node_t **tree) {
  //set the tree to NULL
  *tree = NULL;
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * hodnotu daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 * 
 * Funkci implementujte rekurzivně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {

  //if the tree is empty, return false
  if (tree == NULL) {
    return false;
  }

  //if the key is equal to the key of the current node, return true
  if (tree->key == key) {
    *value = tree->value;
    return true;
  }

  //if the key is not equal to the key of the current node, recursively call the function
  //for the left or right subtree
  else {
    if (tree->key > key) {
      return bst_search(tree->left, key, value);
    }
    else if (tree->key < key) {
      return bst_search(tree->right, key, value);
    }
  }

  //if the key is not found, return false
  return false;
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší. 
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {

  //set the temporary node to the tree
  bst_node_t *tmp = *tree;

  //if we are at the end of the tree, insert the node
  if ((*tree) == NULL) {
      (*tree) = malloc(sizeof(bst_node_t));
      if ((*tree) == NULL)
        return;
      (*tree)->key = key;
      (*tree)->value = value;
      (*tree)->left = NULL;
      (*tree)->right = NULL;
      return;
  }

  //if the key is equal to the key of the current node, change the value
  if (key == tmp->key) {
    tmp->value = value;
    return;
  }

  //if the key is smaller than the key of the current node,
  //recursively call the function for the left subtree
  if (tmp->key > key) {
    bst_insert(&(tmp->left), key, value);
  }

  //if the key is greater than the key of the current node,
  //recursively call the function for the right subtree
  else if (tmp->key < key) {
    bst_insert(&(tmp->right), key, value);
  }

}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazeny klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využitá při implementaci funkce bst_delete.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {

  //if it is not yet the rightmost node, recursively call the function
  if((*tree)->right != NULL) {
    bst_replace_by_rightmost(target, &(*tree)->right);
  }

  //if it is the rightmost node
  else {
    //replace the key and value of the target node
    target->key = (*tree)->key;
    target->value = (*tree)->value;

    //free the rightmost node
    bst_node_t *tmp = *tree;
    //if the rightmost node has a left child, replace it with the left child
    *tree = (*tree)->left;
    free(tmp);
  }

}


/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 * 
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 * 
 * Funkci implementujte rekurzivně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {

  //if the tree is empty, return
  if (*tree == NULL)
    return;

  //if the key is smaller than the key of the current node, 
  //recursively call the function for the left subtree
  if ((*tree)->key > key) {
    bst_delete(&((*tree)->left), key);
  }

  //if the key is greater than the key of the current node, 
  //recursively call the function for the right subtree
  else if ((*tree)->key < key) {
    bst_delete(&((*tree)->right), key);
  }

  //if the key is equal to the key of the current node
  if ((*tree)->key == key) {

    //if the node is a leaf, free it and set it to NULL
    if ((*tree)->left == NULL && (*tree)->right == NULL) {
      free(*tree);
      *tree = NULL;
      return;
    }

    //if the node has only one child on the left, replace it with the child
    if ((*tree)->left == NULL && (*tree)->right != NULL) {
      bst_node_t *tmp = (*tree)->right;
      free(*tree);
      *tree = tmp;
      return;
    }

    //if the node has only one child on the right, replace it with the child
    else if ((*tree)->right == NULL && (*tree)->left != NULL) {
      bst_node_t *tmp = (*tree)->left;
      free(*tree);
      *tree = tmp;
      return;
    }

    //if the node has two children, replace it with the rightmost node of the left subtree
    else if ((*tree)->right != NULL && (*tree)->left != NULL) {
      bst_replace_by_rightmost(*tree, &(*tree)->left);
      return;
    }
  }
  
}

/*
 * Zrušení celého stromu.
 * 
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po 
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených 
 * uzlů.
 * 
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {

  //if the tree is empty, return
  if ((*tree) == NULL) 
    return;

  //if the left tree is not empty, recursively call the function for the left subtree
  if ((*tree)->left != NULL)
    bst_dispose(&((*tree)->left));

  //if the right tree is not empty, recursively call the function for the right subtree
  if ((*tree)->right != NULL)
    bst_dispose(&((*tree)->right));

  //if the node is a leaf, free it
  if ((*tree)->left == NULL && (*tree)->right == NULL)
    free(*tree);

  //in the end, set the tree to NULL
  *tree = NULL;
}


/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
  if (tree == NULL)
    return;

  bst_add_node_to_items(tree, items);

  bst_preorder(tree->left, items);

  bst_preorder(tree->right, items);

}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
  if (tree == NULL)
    return;

  bst_inorder(tree->left, items);

  bst_add_node_to_items(tree, items);

  bst_inorder(tree->right, items);

}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
  if (tree == NULL)
    return;

  bst_postorder(tree->left, items);

  bst_postorder(tree->right, items);

  bst_add_node_to_items(tree, items);
}
