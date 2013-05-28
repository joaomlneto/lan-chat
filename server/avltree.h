// #############################################################################
// # FileName:  AvlTree.h
// # Author:    Zhang Li, CS0901, HUST
// #############################################################################
/*
 * AvlTree.h: An implementation of the avl tree data structure.
 *
 * Copyright (C) 2010 by Zhang Li.
 *
 * This file is part of free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 */
#ifndef _AVLTREE_H_
#define _AVLTREE_H_

#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

struct AvlTreeNode
{
    void* m_key;
    void* m_data;
    int m_height;
    struct AvlTreeNode* m_child1;
    struct AvlTreeNode* m_child2;
    struct AvlTreeNode* m_parent;
};

typedef struct AvlTree
{
    int (*m_dataComparator)(const void* data1, const void* data2);
    int m_keySize;
    int m_dataSize;
    int m_avlTreeSize;
    struct AvlTreeNode* m_rootNode;
    struct AvlTreeNode* m_nullNode;
    pthread_mutex_t mutex;
} avltree_t;

struct AvlTree* avl_create(int (*dataComparator)(const void* data1, const void* data2));
void avl_destroy(avltree_t* tree);

int AvlTreeGetSize(struct AvlTree* tree);
void* AvlTreeNodeData(struct AvlTreeNode* node);

struct AvlTreeNode* avl_insert(
        struct AvlTree* tree,
        void* key,
        void* data);
void AvlTreeDeleteNode(struct AvlTree* tree, struct AvlTreeNode* node);
struct AvlTreeNode* avl_search(struct AvlTree* tree, void* key);

struct AvlTreeNode* AvlTreeIterateHead(struct AvlTree* tree);
struct AvlTreeNode* AvlTreeIterateTail(struct AvlTree* tree);
struct AvlTreeNode* AvlTreeIterateNext(
        struct AvlTree* tree,
        struct AvlTreeNode* node);
struct AvlTreeNode* AvlTreeIteratePrev(
        struct AvlTree* tree,
        struct AvlTreeNode* node);
void avl_remove(avltree_t* tree, void* key);

void* avl_lookup(avltree_t* tree, void* key);
void avl_print(avltree_t* tree);
void avl_print_aux(avltree_t* tree, struct AvlTreeNode* node);

#endif
// #############################################################################
// # EOF
