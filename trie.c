//
// Created by Darel on 17/05/2024.
//

#include <string.h>
#include "trie.h"

trienode *createnode(){
    trienode *newnode = malloc(sizeof *newnode);

    if(newnode == NULL){
        fprintf(stderr,"Erreur allocation memoire. \n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < NUM_CHARS; ++i) {
        newnode->children[i] = NULL;
    }
    newnode->terminal = false;
    //newnode->info = NULL;

    return newnode;
}

/*-----------------------------------------------*/

bool trieinsert(trienode **root, char *name, char *phone, char *email){
    if(*root == NULL){
        *root = createnode();
    }
    unsigned char *text = (unsigned char *)name;
    trienode *tmp = *root;
    int length = strlen(name);

    for (int i = 0; i < length; ++i) {
        if(tmp->children[text[i]] == NULL){
            //create a new node
            tmp->children[text[i]] = createnode();
        }
        tmp = tmp->children[text[i]];
    }
    if(tmp->terminal){
        return false;
    }else{
        tmp->terminal = true;
        tmp->info = malloc(sizeof(contact));
        if (tmp->info == NULL) {
            fprintf(stderr, "Erreur allocation memoire. \n");
            exit(EXIT_FAILURE);
        }
        strcpy(tmp->info->phone, phone);
        strcpy(tmp->info->email, email);

        return true;
    }
}

/*-----------------------------------------------*/

void printtrie_rec(trienode *node, unsigned char *prefix, int length){
    unsigned char newprefix[length+2];
    mempcpy(newprefix, prefix, length);
    newprefix[length+1]=0;

    if (node->terminal) {
        printf("Nom: %s, Telephone: %s, Email: %s\n", prefix, node->info->phone, node->info->email);
    }

    for (int i = 0; i < NUM_CHARS; ++i) {
        if(node->children[i] != NULL){
            newprefix[length] = i;
            printtrie_rec(node->children[i], newprefix, length+1);
        }
    }
}

/*-----------------------------------------------*/

void printtrie(trienode *root){
    if(root == NULL){
        printf("TRIE EMPTY!\n");
        return;
    }
    printtrie_rec(root, NULL, 0);
}

/*-----------------------------------------------*/

bool searchtrie(trienode *root, char *signedtext){
    unsigned char *text = (unsigned char *) signedtext;
    int length = strlen(signedtext);
    trienode *tmp = root;

    for (int i = 0; i < length; ++i) {
        if(tmp->children[text[i]] == NULL){
            return false;
        }
        tmp = tmp->children[text[i]];
    }
    return tmp->terminal;
}


bool node_has_children(trienode *node){
    if(node == NULL) return false;

    for (int i = 0; i < NUM_CHARS; ++i) {
        if(node->children[i] != NULL){
            return true;
        }
    }
    return false;
}

/*-----------------------------------------------*/

trienode *deletestr_rec(trienode *node, unsigned char *text, bool *deleted){
    if(node == NULL) return node;

    if(*text == '\0'){
        if(node->terminal){
            node->terminal = false;
            *deleted = true;

            if(node_has_children(node) == false){
                free(node);
                node = NULL;
            }
        }
        return node;
    }
    node->children[text[0]] = deletestr_rec(node->children[text[0]], text+1, deleted);
    if(*deleted &&
    node_has_children(node) == false &&
    node->terminal == false){
        free(node);
        node = NULL;
    }
    return node;
}

/*-----------------------------------------------*/

bool deletestr(trienode** root, char *signedtext){
    unsigned char *text = (unsigned char *)signedtext;
    bool result = false;

    if(*root == NULL) return false;

    *root = deletestr_rec(*root, text, &result);
    return result;
}

/*-----------------------------------------------*/

void load_from_csv(trienode **root, const char *filename){
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Erreur ouverture fichier %s\n", filename);
        return;
    }
    char line[100];
    while (fgets(line, sizeof(line), file)) {
        char name[50], phone[20], email[50];
        if (sscanf(line, "%[^,],%[^,],%s", name, phone, email) == 3) {
            trieinsert(root, name, phone, email);
        }
    }
    fclose(file);
}

/*-----------------------------------------------*/

void interactive_console(trienode **root){
    char command[50], name[50], phone[20], email[50];
    while (true) {
        printf("Entrez une commande (ajouter, supprimer, rechercher, afficher, quitter) : ");
        scanf("%s", command);

        if (strcmp(command, "ajouter") == 0) {
            printf("Entrez le nom : ");
            scanf("%s", name);
            printf("Entrez le telephone : ");
            scanf("%s", phone);
            printf("Entrez l'email : ");
            scanf("%s", email);
            trieinsert(root, name, phone, email);
            printf("Contact ajoute.\n");
        } else if (strcmp(command, "supprimer") == 0) {
            printf("Entrez le nom : ");
            scanf("%s", name);
            if (deletestr(root, name)) {
                printf("Contact supprime.\n");
            } else {
                printf("Contact non trouve.\n");
            }
        } else if (strcmp(command, "rechercher") == 0) {
            printf("Entrez le nom : ");
            scanf("%s", name);
            if (searchtrie(*root, name)) {
                printf("Contact trouve.\n");
            } else {
                printf("Contact non trouve.\n");
            }
        } else if (strcmp(command, "afficher") == 0) {
            printtrie(*root);
        } else if (strcmp(command, "quitter") == 0) {
            printf("Au revoir");
            break;
        } else {
            printf("Commande non reconnue.\n");
        }
    }
}

void freetrie(trienode *root){
    if (root == NULL) return;

    for (int i = 0; i < NUM_CHARS; ++i) {
        if (root->children[i] != NULL) {
            freetrie(root->children[i]);
        }
    }
    if (root->info) {
        free(root->info);
    }
    free(root);
}