/*
 * Binární vyhledávací strom — iterativní varianta
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h 
 * a připravených koster funkcí implementujte binární vyhledávací 
 * strom bez použití rekurze.
 */

#include "../btree.h"
#include "stack.h"
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
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {

  //if the tree is empty, return false
  if (tree == NULL) {
    return false;
  }

  //create a temporary node
  bst_node_t *tmp = tree;

  //while the temporary node is not NULL
  while (tmp != NULL){

    //if the key is found, set the value and return true
    if (tmp->key == key) {
      *value = tmp->value;
      return true;

    //if the key is not found
    } else {

      //if the key is smaller than the current node's key
      //go to the left subtree
      if (key <= tmp->key) {
        tmp = tmp->left;

      //if the key is greater than the current node's key
      //go to the right subtree
      } else {
        tmp = tmp->right;
      }

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
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {

  //create a new node
  bst_node_t *new_node = malloc(sizeof(bst_node_t));

  if (new_node == NULL){
    return;
  }

  //set the key and value
  //and set the left and right child to NULL
  new_node->key = key;
  new_node->value = value;
  new_node->left = NULL;
  new_node->right = NULL;

  //if the tree is empty, set the tree to the new node
  if (*tree == NULL) {
    *tree = new_node;
    return;
  }

  //create a temporary node
  bst_node_t *tmp = *tree;

  //while the temporary node is not NULL
  while(tmp != NULL) {

    //if the key is already in the tree
    //replace the value
    //and free the new node
    if (key == tmp->key) {
      tmp->value = value;
      free(new_node);
      return;
    }

    //if the key is smaller than the current node's key
    if (key <= tmp->key) {

      //if we're at the leaf
      if (tmp->left == NULL) {
        //set the left child to the new node
        tmp->left = new_node;
        return;
      //else go to the left subtree
      } else {
        tmp = tmp->left;
      }

    //if the key is greater than the current node's key
    //same process as above, but with right subtree
    } else {

      if (tmp->right == NULL) {
        tmp->right = new_node;
        return;
      } else {
        tmp = tmp->right;
      }

    }
  }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {

  //remember the rightmost node
  //and its parent
  bst_node_t *rightmost = *tree;
  bst_node_t *prev = target;

  //if the rightmost is NULL, return
  //the left subtree is NULL
  if(rightmost == NULL)
    return;

  //find the rightmost node
  //and set its parent
  while(rightmost->right) {
    prev = rightmost;
    rightmost = rightmost->right;
  }

  //replace the target node with the rightmost node
  target->value = rightmost->value;
  target->key = rightmost->key;

  //if the rightmost node has left subtree
  if(rightmost->left == NULL) {

    //if the rightmost node is not the target node
    if(target != prev) {
      //set the parent's right child to NULL
      prev->right = NULL;
    }
    
    //if the rightmost node is the target node
    else if (target == prev) {
      //set the parent's left child to NULL
      prev->left = NULL;
    }

  }

  //if the rightmost node has left subtree
  else {
    //set the parent's right child to the left subtree
    prev->right = rightmost->left;
  }

  //free the rightmost node
  free(rightmost);

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
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {

  //if the tree is empty, return
  if (*tree == NULL) {
    return;
  }

  //create a temporary node
  //set found to false
  //and set the parent to NULL
  bst_node_t *tmp = *tree;
  bool found = false;
  bst_node_t *parent = NULL;

  //find the node with the given key
  while (tmp != NULL && tmp->key != key) {

    //set the parent to the current node
    parent = tmp;

    //if the key is smaller than the current node's key
    //go to the left subtree
    //else go to the right subtree
    if (key < tmp->key) {
      tmp = tmp->left;
    } else if (key > tmp->key) {
      tmp = tmp->right;
    }

    //if the key is found, set found to true
    if (tmp != NULL && tmp->key == key)
      found = true;

  }

  //if the key is not found, return
  if ((*tree)->key == key)
    found = true;

  if (!found) return;


  //if the node is a leaf
  if (tmp->left == NULL && tmp->right == NULL) {
    
    //if the node is the root
    if (parent == NULL) {
      //set the tree to NULL
      *tree = NULL;
    }

    //if node has parent on the right
    else if (parent->left == tmp) {
      //change the parent's left child to NULL
      parent->left = NULL;
    }

    //if node has parent on the left
    else if (parent->right == tmp) {
      //change the parent's right child to NULL
      parent->right = NULL;
    }

    //free the node
    free(tmp);
  }


  //if node has right subtree
  else if (tmp->left == NULL) {

    //if the node is the root
    if (parent == NULL) {
      //set the tree to the right subtree
      *tree = tmp->right;
    }

    //if node has parent on the right
    else if (parent->left == tmp) {
      //change the parent's left child to the right subtree
      parent->left = tmp->right;
    }

    //if node has parent on the left
    else if (parent->right == tmp) {
      //change the parent's right child to the right subtree
      parent->right = tmp->right;
    }

    //free the node
    free(tmp);
  }

  //same process as above, but with left subtree
  else if (tmp->right == NULL) {

    if (parent == NULL) {
      *tree = tmp->left;
    }

    else if (parent->left == tmp) {
      parent->left = tmp->left;
    }

    else if (parent->right == tmp) {
      parent->right = tmp->left;
    }

    free(tmp);
  }

  //if node has both subtrees
  //replace the node with the rightmost node of the left subtree
  else if (tmp->left != NULL && tmp->right != NULL) {
    bst_replace_by_rightmost(tmp, &tmp->left);
  }
}

/*
 * Zrušení celého stromu.
 * 
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po 
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených 
 * uzlů.
 * 
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {

  //if the tree is empty, return
  if (*tree == NULL) {
    return;
  }

  //initialize the stack
  //set the current node to the root
  //and set the last node visited to NULL
  stack_bst_t stack;
  stack_bst_init(&stack);
  bst_node_t *current = *tree;
  bst_node_t *lastNodeVisited = NULL;

  //while the stack is not empty or the current node is not NULL
  while (!stack_bst_empty(&stack) || current != NULL) {

    //if the current node is not NULL
    if (current != NULL) {
      //push the current node to the stack
      stack_bst_push(&stack, current);
      //go to the left subtree
      current = current->left;
    } else {
      //get the top node from the stack
      bst_node_t *peekNode = stack_bst_top(&stack);
      // if right child exists and traversing node from left child, then move right
      if (peekNode->right != NULL && lastNodeVisited != peekNode->right) {
        current = peekNode->right;
      } else {
        //if not, pop the node from the stack
        //and free it
        //set the last node visited to the popped node
        stack_bst_pop(&stack);
        free(peekNode);
        lastNodeVisited = peekNode;
      }
    }
  }

  //reset the tree
  *tree = NULL;

}

/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items) {

  //make a temporary node
  bst_node_t *tmp = tree;

  //while the temporary node is not NULL
  while (tmp != NULL) {
    //push the temporary node to the stack
    stack_bst_push(to_visit, tmp);
    //add the temporary node to the items
    bst_add_node_to_items(tmp, items);
    //go to the left subtree
    tmp = tmp->left;
  }

}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {

  //initialize the stack
  stack_bst_t stack;
  stack_bst_init(&stack);

  //make a temporary node
  bst_node_t *tmp = tree;

  //go to the leftmost node and add the nodes to the stack
  bst_leftmost_preorder(tmp, &stack, items);

  //while the stack is not empty
  while (!stack_bst_empty(&stack)) {
    //pop the top node from the stack
    tmp = stack_bst_top(&stack);
    stack_bst_pop(&stack);
    
    //go to the leftmost node of the right subtree
    bst_leftmost_preorder(tmp->right, &stack, items);
  }

}

/*
 * Pomocná funkce pro iterativní inorder.
 * 
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {

  //make a temporary node
  bst_node_t *tmp = tree;

  //while the temporary node is not NULL
  while (tmp != NULL) {
    //push the temporary node to the stack 
    stack_bst_push(to_visit, tmp);
    //go to the left subtree
    tmp = tmp->left;
  }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {

  //initialize the stack
  stack_bst_t stack;
  stack_bst_init(&stack);

  //make a temporary node
  bst_node_t *tmp = tree;

  //go to the leftmost node and add the nodes to the stack
  bst_leftmost_inorder(tmp, &stack);

  //while the stack is not empty and the temporary node is not NULL
  while(!stack_bst_empty(&stack) && tmp != NULL) {

    //pop the top node from the stack
    tmp = stack_bst_top(&stack);
    stack_bst_pop(&stack);

    //add the node to the items 
    bst_add_node_to_items(tmp, items);

    //go to the leftmost node of the right subtree
    bst_leftmost_inorder(tmp->right, &stack); 
  }
}

/*
 * Pomocná funkce pro iterativní postorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů. Do zásobníku bool hodnot ukládá informaci, že uzel
 * byl navštíven poprvé.
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {

  //make a temporary node
  bst_node_t *tmp = tree;

  //while the temporary node is not NULL
  while (tmp != NULL) {
    //push the temporary node to the stack
    stack_bst_push(to_visit, tmp);
    //push true to the stack of bool values
    stack_bool_push(first_visit, tmp);
    //go to the left subtree
    tmp = tmp->left;
  }
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {

  //initialize the stacks
  stack_bst_t stack_to_visit;
  stack_bool_t stack_visit_first;
  stack_bst_init(&stack_to_visit);
  stack_bool_init(&stack_visit_first);

  //make a temporary node
  bst_node_t *tmp = tree;

  //go to the leftmost node and add the nodes to the stacks
  bst_leftmost_postorder(tmp, &stack_to_visit, &stack_visit_first);

  //while the stack is not empty
  while (!stack_bst_empty(&stack_to_visit)) {

    //pop the top node from the stack
    tmp = stack_bst_top(&stack_to_visit);
    bool First_visit = stack_bool_top(&stack_visit_first);

    //pop the top value from the stacks
    stack_bst_pop(&stack_to_visit);
    stack_bool_pop(&stack_visit_first);

    //if the node was not visited yet
    if (First_visit) {
      //push the node back to the stack
      stack_bst_push(&stack_to_visit, tmp);
      //push false to the stack of bool values
      //since the node was visited
      stack_bool_push(&stack_visit_first, false);
      //go to the leftmost node of the right subtree
      bst_leftmost_postorder(tmp->right, &stack_to_visit, &stack_visit_first);
    }

    //if the node was visited
    else {
      //add the node to the items
      bst_add_node_to_items(tmp, items);
    }
  }
  

}
