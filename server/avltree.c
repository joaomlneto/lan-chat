// #############################################################################
// # FileName:  AvlTree.c
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

#include <stdio.h>
#include <pthread.h>
#include "avltree.h"

static void DoDestroyNode(struct AvlTree* tree, struct AvlTreeNode* node);
static void DoInsertNode(
        struct AvlTree* tree,
        struct AvlTreeNode* root,
        struct AvlTreeNode* node);
static void DoRebalance(struct AvlTree* tree, struct AvlTreeNode* node);
static void DoUpdateHeight(struct AvlTree* tree, struct AvlTreeNode* node);
static void DoRotateRR(struct AvlTree* tree, struct AvlTreeNode* node);
static void DoRotateLL(struct AvlTree* tree, struct AvlTreeNode* node);
static void DoRotateLR(struct AvlTree* tree, struct AvlTreeNode* node);
static void DoRotateRL(struct AvlTree* tree, struct AvlTreeNode* node);
static struct AvlTreeNode* DoFindDeletionScapegoat(
        struct AvlTree* tree,
        struct AvlTreeNode* node);

struct AvlTree* avl_create(int (*dataComparator)(const void* data1, const void* data2))
{
    struct AvlTree* newTree = malloc(sizeof(struct AvlTree));

    // initialize a new avl tree structure
    newTree->m_keySize = sizeof(void*);
    newTree->m_dataSize = sizeof(void*);
    newTree->m_dataComparator = dataComparator;
    newTree->m_avlTreeSize = 0;
    pthread_mutex_init(&newTree->mutex, NULL);

    // initialize the null node
    newTree->m_nullNode = malloc(sizeof(struct AvlTreeNode));
    newTree->m_nullNode->m_child1 = newTree->m_nullNode;
    newTree->m_nullNode->m_child2 = newTree->m_nullNode;
    newTree->m_nullNode->m_parent = newTree->m_nullNode;
    newTree->m_nullNode->m_height = -1;
    newTree->m_rootNode = newTree->m_nullNode;

    return newTree;
}

void avl_destroy(avltree_t* tree)
{
    // destroy the tree and free the avl tree structure
    DoDestroyNode(tree, tree->m_rootNode);
    free(tree->m_nullNode);
    pthread_mutex_destroy(&tree->mutex);
    free(tree);
    return;
}

int AvlTreeGetSize(struct AvlTree* tree)
{
    return tree->m_avlTreeSize;
}

void* AvlTreeNodeData(struct AvlTreeNode* node)
{
    return node->m_data;
}

struct AvlTreeNode* avl_insert(
        struct AvlTree* tree,
        void* key,
        void* data)
{
    struct AvlTreeNode* newNode = malloc(sizeof(struct AvlTreeNode));

    // initialize the new node
    newNode->m_key = key; //memcpy(malloc(tree->m_keySize), key, tree->m_keySize);
    newNode->m_data = data; //memcpy(malloc(tree->m_dataSize), data, tree->m_dataSize);
    newNode->m_child1 = tree->m_nullNode;
    newNode->m_child2 = tree->m_nullNode;
    newNode->m_parent = tree->m_nullNode;
    newNode->m_height = 0;

	pthread_mutex_lock(&tree->mutex);

    if(tree->m_rootNode == tree->m_nullNode)
    {
        // regard the new node as the root if it's an empty tree
        tree->m_rootNode = newNode;
    }
    else
    {
        // insert the new node into the avl tree
        DoInsertNode(tree, tree->m_rootNode, newNode);
        DoRebalance(tree, newNode);
    }

    tree->m_avlTreeSize += 1;
    //printf("Info to insert: key = '%d'. data = '%p'.\n", *(int*)key, data);
    //printf("Inserted. key = '%d'. data = '%p'.\n", *(int*)newNode->m_key, newNode->m_data);
    //printf("NODE PTR = %p\n", newNode);
	pthread_mutex_unlock(&tree->mutex);
    return newNode;
}

void AvlTreeDeleteNode(struct AvlTree* tree, struct AvlTreeNode* node)
{
    struct AvlTreeNode* scapegoatNode;
    struct AvlTreeNode* checkNode;

    // copy the scapegoat's key and data into the deleting node
    // the scapegoat is the real node to be deleted
    scapegoatNode = DoFindDeletionScapegoat(tree, node);
    if(node != scapegoatNode)
    {
        memcpy(node->m_key, scapegoatNode->m_key, tree->m_keySize);
        memcpy(node->m_data, scapegoatNode->m_data, tree->m_dataSize);
        AvlTreeDeleteNode(tree, scapegoatNode);
        return;
    }

    // delete scapegoat
    checkNode = scapegoatNode->m_parent;
    free(scapegoatNode->m_key);
    free(scapegoatNode->m_data);
    free(scapegoatNode);

    if(checkNode != tree->m_nullNode)
    {
        if(checkNode->m_child1 == scapegoatNode)
            checkNode->m_child1 = tree->m_nullNode;
        if(checkNode->m_child2 == scapegoatNode)
            checkNode->m_child2 = tree->m_nullNode;
        DoUpdateHeight(tree, checkNode);
    }
    else tree->m_rootNode = tree->m_nullNode;

    // rebalance
    DoRebalance(tree, checkNode);

    tree->m_avlTreeSize -= 1;
    return;
}

struct AvlTreeNode* avl_search(struct AvlTree* tree, void* key)
{
    struct AvlTreeNode* currentNode = tree->m_rootNode;

    pthread_mutex_lock(&tree->mutex);

	//printf("[avl_search] searching for key '%d'\n", *(int*)key);

    // search the given key in the tree
    while(currentNode != tree->m_nullNode)
    {
        if(tree->m_dataComparator(key, currentNode->m_key) == 0) {
				//printf("Found it!\n");
				//printf("Key = '%d', Data = '%p'.\n", *(int*)currentNode->m_key, currentNode->m_data);
				pthread_mutex_unlock(&tree->mutex);
            return currentNode->m_data;
        }
        else if(tree->m_dataComparator(key, currentNode->m_key) < 0)
            currentNode = currentNode->m_child1;
        else if(tree->m_dataComparator(key, currentNode->m_key) > 0)
            currentNode = currentNode->m_child2;
    }

    pthread_mutex_unlock(&tree->mutex);
	//printf("Not found.\n");
    return NULL;
}

struct AvlTreeNode* AvlTreeIterateHead(struct AvlTree* tree)
{
    struct AvlTreeNode* currentNode = tree->m_rootNode;

    if(currentNode != tree->m_nullNode)
    {
        // find the smallest node
        while(currentNode->m_child1 != tree->m_nullNode)
            currentNode = currentNode->m_child1;
        return currentNode;
    }
    return NULL;
}

struct AvlTreeNode* AvlTreeIterateTail(struct AvlTree* tree)
{
    struct AvlTreeNode* currentNode = tree->m_rootNode;

    if(currentNode != tree->m_nullNode)
    {
        // find the greatest node
        while(currentNode->m_child2 != tree->m_nullNode)
            currentNode = currentNode->m_child2;
        return currentNode;
    }
    return NULL;
}

struct AvlTreeNode* AvlTreeIterateNext(
        struct AvlTree* tree,
        struct AvlTreeNode* node)
{
    struct AvlTreeNode* currentNode;

    if(node->m_child2 != tree->m_nullNode)
    {
        // look for right child's smallest node
        currentNode = node->m_child2;
        while(currentNode->m_child1 != tree->m_nullNode)
            currentNode = currentNode->m_child1;
        return currentNode;
    }
    else if(node->m_parent != tree->m_nullNode)
    {
        currentNode = node;

        // the node has no right child, look back
        while(currentNode != tree->m_rootNode)
            if(currentNode->m_parent->m_child2 != currentNode)
                return currentNode->m_parent;
            else currentNode = currentNode->m_parent;
    }
    return NULL;
}

struct AvlTreeNode* AvlTreeIteratePrev(
        struct AvlTree* tree,
        struct AvlTreeNode* node)
{
    struct AvlTreeNode* currentNode;

    if(node->m_child1 != tree->m_nullNode)
    {
        currentNode = node->m_child1;

        // look for left child's greatest node
        while(currentNode->m_child2 != tree->m_nullNode)
            currentNode = currentNode->m_child2;
        return currentNode;
    }
    else if(node->m_parent != tree->m_nullNode)
    {
        currentNode = node;

        // the node has no left child, look back
        while(currentNode != tree->m_rootNode)
            if(currentNode->m_parent->m_child1 != currentNode)
                return currentNode->m_parent;
            else currentNode = currentNode->m_parent;
    }
    return NULL;
}

static void DoDestroyNode(struct AvlTree* tree, struct AvlTreeNode* node)
{
    if(node != tree->m_nullNode)
    {
        // recursively destroy all children
        DoDestroyNode(tree, node->m_child1);
        DoDestroyNode(tree, node->m_child2);

        // destroy current node
        free(node->m_key);
        free(node->m_data);
    }
}

static void DoInsertNode(
        struct AvlTree* tree,
        struct AvlTreeNode* root,
        struct AvlTreeNode* node)
{
    // normal binary search tree's insertion method
    if(tree->m_dataComparator(node->m_key, root->m_key) < 0)
    {
        if(root->m_child1 != tree->m_nullNode)
            DoInsertNode(tree, root->m_child1, node);
        else
        {
            root->m_child1 = node;
            root->m_child1->m_parent = root;
        }
        DoUpdateHeight(tree, root);
    }
    else
    {
        if(root->m_child2 != tree->m_nullNode)
            DoInsertNode(tree, root->m_child2, node);
        else
        {
            root->m_child2 = node;
            root->m_child2->m_parent = root;
        }
        DoUpdateHeight(tree, root);
    }
    return;
}

static void DoRebalance(struct AvlTree* tree, struct AvlTreeNode* node)
{
    struct AvlTreeNode* nextNode;

    while(node != tree->m_nullNode)
    {
        nextNode = node->m_parent;

        // check balance
        if(node->m_child1->m_height - node->m_child2->m_height > 1)
        {
            if(node->m_child1->m_child1->m_height
                    > node->m_child1->m_child2->m_height)
                DoRotateLL(tree, node);
            else
                DoRotateLR(tree, node);
        }
        else if(node->m_child2->m_height - node->m_child1->m_height > 1)
        {
            if(node->m_child2->m_child2->m_height
                    > node->m_child2->m_child1->m_height)
                DoRotateRR(tree, node);
            else
                DoRotateRL(tree, node);
        }

        // update height
        if(nextNode != tree->m_nullNode)
            DoUpdateHeight(tree, nextNode);
        node = nextNode;
    }
    return;
}

static void DoUpdateHeight(struct AvlTree* tree, struct AvlTreeNode* node)
{
    if(node != tree->m_nullNode)
    {
        if(node->m_child1->m_height > node->m_child2->m_height)
            node->m_height = 1 + node->m_child1->m_height;
        else
            node->m_height = 1 + node->m_child2->m_height;
    }
    return;
}

static void DoRotateLL(struct AvlTree* tree, struct AvlTreeNode* node)
{
    struct AvlTreeNode* node0 = node->m_parent;
    struct AvlTreeNode* node1 = node;
    struct AvlTreeNode* node2 = node->m_child1;

    // rotate left child to right
    node1->m_child1 = node2->m_child2;
    node1->m_child1->m_parent = node1;
    node2->m_child2 = node1;
    node2->m_child2->m_parent = node2;

    // update heights
    DoUpdateHeight(tree, node1);
    DoUpdateHeight(tree, node2);

    if(node0->m_child1 == node1)
    {
        node0->m_child1 = node2;
        node0->m_child1->m_parent = node0;
        return;
    }
    if(node0->m_child2 == node1)
    {
        node0->m_child2 = node2;
        node0->m_child2->m_parent = node0;
        return;
    }
    tree->m_rootNode = node2;
    tree->m_rootNode->m_parent = tree->m_nullNode;
    return;
}

static void DoRotateRR(struct AvlTree* tree, struct AvlTreeNode* node)
{
    struct AvlTreeNode* node0 = node->m_parent;
    struct AvlTreeNode* node1 = node;
    struct AvlTreeNode* node2 = node->m_child2;

    // rotate right child to left
    node1->m_child2 = node2->m_child1;
    node1->m_child2->m_parent = node1;
    node2->m_child1 = node1;
    node2->m_child1->m_parent = node2;

    // update heights
    DoUpdateHeight(tree, node1);
    DoUpdateHeight(tree, node2);

    if(node0->m_child1 == node1)
    {
        node0->m_child1 = node2;
        node0->m_child1->m_parent = node0;
        return;
    }
    if(node0->m_child2 == node1)
    {
        node0->m_child2 = node2;
        node0->m_child2->m_parent = node0;
        return;
    }
    tree->m_rootNode = node2;
    tree->m_rootNode->m_parent = tree->m_nullNode;
    return;
}

static void DoRotateLR(struct AvlTree* tree, struct AvlTreeNode* node)
{
    struct AvlTreeNode* node1 = node;
    struct AvlTreeNode* node2 = node->m_child1;

    // LR-rotate method
    DoRotateRR(tree, node2);
    DoRotateLL(tree, node1);
    return;
}

static void DoRotateRL(struct AvlTree* tree, struct AvlTreeNode* node)
{
    struct AvlTreeNode* node1 = node;
    struct AvlTreeNode* node2 = node->m_child2;

    // RL-rotate method
    DoRotateLL(tree, node2);
    DoRotateRR(tree, node1);
    return;
}

static struct AvlTreeNode* DoFindDeletionScapegoat(
        struct AvlTree* tree,
        struct AvlTreeNode* node)
{
    struct AvlTreeNode* scapegoatNode;

    // find scapegoat
    if(node->m_child1 != tree->m_nullNode)
    {
        scapegoatNode = node->m_child1;
        while(scapegoatNode->m_child2 != tree->m_nullNode)
            scapegoatNode = scapegoatNode->m_child2;
        if(scapegoatNode != node)
        {
            return scapegoatNode;
        }
    }
    else if(node->m_child2 != tree->m_nullNode)
    {
        scapegoatNode = node->m_child2;
        while(scapegoatNode->m_child1 != tree->m_nullNode)
            scapegoatNode = scapegoatNode->m_child1;
        if(scapegoatNode != node)
        {
            return scapegoatNode;
        }
    }
    return node;
}

void avl_remove(avltree_t* tree, void* key) {
    struct AvlTreeNode* node = NULL;
    struct AvlTreeNode* currentNode = tree->m_rootNode;

    pthread_mutex_lock(&tree->mutex);

    // search the given key in the tree
    while(currentNode != tree->m_nullNode)
    {
        if(tree->m_dataComparator(key, currentNode->m_key) == 0) {
            pthread_mutex_unlock(&tree->mutex);
            node = currentNode;
            break;
        }
        else if(tree->m_dataComparator(key, currentNode->m_key) < 0)
            currentNode = currentNode->m_child1;
        else if(tree->m_dataComparator(key, currentNode->m_key) > 0)
            currentNode = currentNode->m_child2;
    }

    AvlTreeDeleteNode(tree, node);

    pthread_mutex_unlock(&tree->mutex);

}

void* avl_lookup(avltree_t* tree, void* key) {
	struct AvlTreeNode* node;

	node = avl_search(tree, key);
	//printf("NODE = %p\n", node);

	//printf("[avl_lookup] locked.\n");
	//printf("[avl_lookup] lookup for key '%d'.\n", *(int*)key);

	/*if (node == NULL) {
		printf("[avl_lookup] node not found.\n");
		return NULL;
	}*/

	//printf("[avl_lookup] node found.\n");
	//printf("[avl_lookup] key = '%d'\n", (int)node->m_key);
	//printf("[avl_lookup] info = '%p'\n", node->m_data);

	//printf("[avl_lookup] returning '%p'\n", node->m_data);
	return node/*->m_data*/;
}

void avl_print(avltree_t* tree) {
	printf("\n#### AVL PRINT ####\n");
	avl_print_aux(tree, AvlTreeIterateHead(tree));
}

void avl_print_aux(avltree_t* tree, struct AvlTreeNode* node) {
	if (node == NULL) {
		printf("#### END PRINT ####\n");
		return;
	}
	printf("KEY = %d\n", *(int*)node->m_key);
	avl_print_aux(tree, AvlTreeIterateNext(tree, node));
}

// #############################################################################
// # EOF
