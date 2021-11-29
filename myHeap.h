#pragma once
#define ERROR -1

typedef struct HeapNode {
	bool valid;
	int id;
	double data;
	struct HeapNode* left;
	struct HeapNode* right;
	struct HeapNode* parent;
}HeapNode;

typedef struct Heap {
	struct HeapNode* head;
	int size;
}Heap;

//void BuildHeap(Heap* heap, HeapNode* base, int size);
void InitHeap(Heap* heap);
void InitHeapNode(HeapNode* node);
void UpdateHeapHead(Heap* heap);
HeapNode* PopHeap(Heap* heap);
HeapNode* PushHeap(Heap* heap, double data, int id);