// define your TreapBST here
#ifndef _TREAP_BST_H_
#define _TREAP_BST_H_

#include "abstract_bst.h"
#include <iostream>
#include <utility>
#include <stdexcept>
#include <random>
#include <stack>

template <typename KeyType, typename ValueType>
class TreapBST
{
public:

	// Constructor
	TreapBST();

	// Destructor
	~TreapBST();

	// copy constructor
	TreapBST(const TreapBST & x);

	// copy assignment
	TreapBST& operator=(TreapBST x);

	//for copy-swap idiom
	void swap(TreapBST<KeyType, ValueType>& x, TreapBST<KeyType, ValueType>& y);

	bool empty();

	ValueType search(const KeyType& key, bool& found);

	void insert(const KeyType& key, const ValueType& value);

	void remove(const KeyType& key);

private:
	std::mt19937 rndgen;

	template <typename KeyType, typename ValueType>
	struct Node {
		KeyType key;
		ValueType data;
		std::mt19937::result_type priority;
		Node* parent;
		Node* left;
		Node* right;

		Node(const KeyType& k, const ValueType& d, Node* p = nullptr) : key(k), data(d), parent(p), left(nullptr), right(nullptr)
		{
		}

		Node* operator=(Node* x)
		{
			swap(*this, x);
			return *this;
		}

		Node* getRootNode(Node* tempNode)
		{
			while (tempNode->parent != nullptr)
			{
				tempNode = tempNode->parent;
			}
			return tempNode;
		}
		Node* findNode(Node* tempRoot, KeyType key)
		{
			while (tempRoot != nullptr)
			{
				if (tempRoot->key > key)
					tempRoot = tempRoot->left;
				if (tempRoot->key < key)
					tempRoot = tempRoot->right;
				if (tempRoot->key == key)
					return tempRoot;
			}
			return nullptr;
		}

		Node* newNode(KeyType key)
		{
			Node* temp = new Node();
			temp->key = key;
			temp->priority = rndgen();
			temp->left = temp->right = nullptr;
			return temp;
		}

		bool isLeaf(Node* tempNode)
		{
			return (tempNode->left == nullptr && tempNode->right == nullptr);
		}

		bool hasChildren(Node* tempNode)
		{
			return (tempNode->left != nullptr || tempNode->right != nullptr);
		}

		bool hasBothChildren(Node* tempNode)
		{
			return (tempNode->left != nullptr && tempNode->right != nullptr);
		}
			
		bool hasLeft(Node* tempNode)
		{
			return tempNode->left != nullptr;
		}
		
		bool hasRight(Node* tempNode)
		{
			return tempNode->right != nullptr;
		}
	};

	template <typename K, typename I>
	struct stackvar {
		Node<K, I>* rhs;
		Node<K, I>** lhs;
	};

	typedef Node<KeyType, ValueType> *NodePtr;

	NodePtr root;
	
	void clone(Node<KeyType, ValueType> *& newRoot, Node<KeyType, ValueType> * oldRoot);

	void rotateLeft(Node<KeyType, ValueType> *& u)
	{
		Node<KeyType, ValueType> * w = u->right;
		w->parent = u->parent;
		if (w->parent != nullptr)
		{
			if (w->parent->left == u)
				w->parent->left = w;
			else
				w->parent->right = w;
		}

		u->right = w->left;
		if (u->right != nullptr)
			u->right->parent = u;
		u->parent = w;
		w->left = u;
		if (u == root)
		{
			root = w;
			root->parent = nullptr;
		}
	}
	void rotateRight(Node<KeyType, ValueType> *& u)
	{
		Node<KeyType, ValueType> * w = u->left;
		w->parent = u->parent;
		if (w->parent != nullptr)
		{
			if (w->parent->left == u)
				w->parent->left = w;
			else
				w->parent->right = w;
		}
		u->left = w->right;
		if (u->left != nullptr)
			u->left->parent = u;
		u->parent = w;
		w->right = u;
		if (u == root)
		{
			root = w;
			root->parent = nullptr;
		}
	}
};

#include "treap_bst.txx"
#endif // _TREAP_BST_H_