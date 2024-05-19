//
// Created by Darel on 17/05/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef EVALUATION_STRUCTURE_DE_DONNEES_DAREL_DOUPAMBY_TRIE_H
#define EVALUATION_STRUCTURE_DE_DONNEES_DAREL_DOUPAMBY_TRIE_H

//Pour utiliser le code ascii et avoir les majuscules
#define NUM_CHARS 256

typedef struct contact {
    char phone[20];
    char email[50];
} contact;

typedef struct trienode {
    struct trienode *children[NUM_CHARS];
    bool terminal;
    contact *info;
} trienode;


trienode *createnode();

bool trieinsert(trienode **root, char *name, char *phone, char *email);

void printtrie_rec(trienode *node, unsigned char *prefix, int length);

void printtrie(trienode *root);

bool searchtrie(trienode *root, char *signedtext);

bool node_has_children(trienode *node);

trienode *deletestr_rec(trienode *node, unsigned char *text, bool *deleted);

bool deletestr(trienode** root, char *signedtext);

void load_from_csv(trienode **root, const char *filename);

void interactive_console(trienode **root);

void freetrie(trienode *root);
#endif //EVALUATION_STRUCTURE_DE_DONNEES_DAREL_DOUPAMBY_TRIE_H