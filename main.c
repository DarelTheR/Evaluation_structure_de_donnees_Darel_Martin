#include "trie.h"

int main(void) {
    //initialisation de la racine de ma trie
    trienode *root = NULL;

    load_from_csv(&root, "contacts.csv");

    //interaction avec l annuaire dans la console
    interactive_console(&root);

    //liberation de la mémoire
    freetrie(root);
    return 0;
}
