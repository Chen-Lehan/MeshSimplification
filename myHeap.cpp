//#include <stdio.h>
#include <iostream> 
#include <stdlib.h>
#include "myHeap.h"

void _PrintHeap(HeapNode* node, int layer)
{
	int i;
	if (layer <= 0)
		exit(ERROR);

	for (i = 0; i < layer; i++) {
		std::cout << "-" ;
	}

	if (!node)
		printf(" NULL");
	else {
		std::cout << node->id;
		if (node->left || node->right) {
			std::cout << "\n";
			_PrintHeap(node->left, layer + 1);
			std::cout << "\n";
			_PrintHeap(node->right, layer + 1);
		}
	}
}

void _MoveUp(HeapNode* a) 
{
	if (!a->parent)
		return;

	HeapNode* b = a->parent;
	HeapNode* temp;
	int i;

	if (b->left == a)
	{
		temp = b->right;
		i = 0;
	}
	else
	{
		temp = b->left;
		i = 1;
	}

	if (temp)
		temp->parent = a;
	b->left = a->left;
	b->right = a->right;
	if (!i)
	{
		a->left = b;
		a->right = temp;
	}
	else
	{
		a->right = b;
		a->left = temp;
	}
	
	a->parent = b->parent;

	if (b->parent)
		if (b == b->parent->left)
			b->parent->left = a;
		else
			b->parent->right = a;

	if (b->left)
	{
		b->left->parent = b;
		if (b->right)
			b->right->parent = b;
	}
	
	b->parent = a;
}

HeapNode* _FindLast(Heap* heap)
{
	HeapNode* lastnode = heap->head;
	int size = heap->size;
	int layer = 0;
	while (size > 1)
	{
		size = size / 2;
		layer++;
	}
	size = heap->size;

	for (; 0 < layer;)
	{
		if ((1 << --layer) & size)
			lastnode = lastnode->right;
		else
			lastnode = lastnode->left;
	}

	if (!lastnode)
		_PrintHeap(heap->head, 1);

	return lastnode;
}

void _AddLast(Heap* heap, HeapNode* newnode, int id)
{
	HeapNode* lastnode = heap->head;
	int size = heap->size;
	int layer = 0;
	while (size > 1)
	{
		size = size / 2;
		layer++;
	}
	size = heap->size;

	for (; 0 < layer;)
	{
		if (layer != 1)
		{
			if ((1 << --layer) & size)
				lastnode = lastnode->right;
			else
				lastnode = lastnode->left;
		}
		else
		{
			if ((1 << --layer) & size)
				lastnode->right = newnode;
			else
				lastnode->left = newnode;

			newnode->parent = lastnode;
		}
	}
}

/*---------------------------------------------------------------------*/

void InitHeap(Heap* heap)
{
	heap->size = 0;
	heap->head = NULL;
}

void InitHeapNode(HeapNode* node) 
{
	if (node) {
		if (node->right)
		{
			if (node->left->data < node->right->data)
			{
				if (node->left->data < node->data)
				{
					_MoveUp(node->left);
					InitHeapNode(node);
				}
			}
			else
			{
				if (node->right->data < node->data)
				{
					_MoveUp(node->right);
					InitHeapNode(node);
				}
			}
		}
		else if (node->left) 
		{
			if (node->left->data < node->data) {
				_MoveUp(node->left);
				InitHeapNode(node);
			}
		}

		if (node->parent)
		{
			if (node->data < node->parent->data)
			{
				_MoveUp(node);
				InitHeapNode(node);
			}
		}
	}
}

void UpdateHeapHead(Heap* heap)
{
	HeapNode* head = heap->head;
	while (head->parent)
		head = head->parent;
	heap->head = head;
}

HeapNode* PopHeap(Heap* heap)
{
	HeapNode* lastnode = _FindLast(heap);
	HeapNode* popnode = heap->head;

	if (lastnode->parent)
	{
		if (lastnode->parent->left == lastnode)
			lastnode->parent->left = NULL;
		else
			lastnode->parent->right = NULL;

		lastnode->left = heap->head->left;
		lastnode->right = heap->head->right;
		if (lastnode->left)
			lastnode->left->parent = lastnode;
		if (lastnode->right)
			lastnode->right->parent = lastnode;
		lastnode->parent = NULL;
		heap->head = lastnode;

		InitHeapNode(lastnode);

		UpdateHeapHead(heap);
	}
	else
		heap->head = NULL;
	
	heap->size--;
	return popnode;
}

HeapNode* PushHeap(Heap* heap, double data, int id)
{
	int size = heap->size++;
	HeapNode* newnode = new HeapNode;
	newnode->valid = true;
	newnode->id = id;
	newnode->data = data;
	newnode->parent = NULL;
	newnode->left = NULL;
	newnode->right = NULL;

	if (size == 0)
		heap->head = newnode;
	else
		_AddLast(heap, newnode, id);
	InitHeapNode(newnode);
	UpdateHeapHead(heap);
	return newnode;
}