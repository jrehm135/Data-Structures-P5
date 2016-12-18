#include "treap_bst.h"

// implement your TreapBST  here
// using BinarySearchTree as a starting point

template <typename KeyType, typename ValueType>
TreapBST<KeyType, ValueType>::TreapBST():root(nullptr)
{
	std::random_device rd;
	rndgen.seed(rd());
}

template <typename KeyType, typename ValueType>
TreapBST<KeyType, ValueType>::~TreapBST()
{
	while (root != nullptr)
	{
		remove(root->key);
	}
}

template <typename KeyType, typename ValueType>
TreapBST<KeyType, ValueType>::TreapBST(const TreapBST<KeyType, ValueType>& x)
{
	clone(this->root, x.root);
}

template <typename KeyType, typename ValueType>
TreapBST<KeyType, ValueType>& TreapBST<KeyType, ValueType>:: operator=(TreapBST x)
{
	swap(*this, x);

	return *this;
}

template <typename KeyType, typename ValueType>
void TreapBST<KeyType, ValueType>::swap(TreapBST<KeyType, ValueType>& x, TreapBST<KeyType, ValueType>& y)
{
	std::swap(x.root, y.root);
}

template <typename KeyType, typename ValueType>
bool TreapBST<KeyType, ValueType>::empty()
{
	if (root == nullptr)
		return true;
	else return false;
}

template <typename KeyType, typename ValueType>
ValueType TreapBST<KeyType, ValueType>::search(const KeyType& key, bool& found)
{
	Node<KeyType, ValueType> *current = root;
	if (current == nullptr)
	{
		found = false;
		throw std::logic_error("Nothing in treap");
	}
	while (current != nullptr)
	{
		if (current->key > key)
			current = current->left;
		else if (current->key < key)
			current = current->right;
		else if (current->key == key)
		{
			found = true;
			return current->data;
		}
	}
	found = false;
	ValueType standard;
	return standard;
}

template <typename KeyType, typename ValueType>
void TreapBST<KeyType, ValueType>::insert(const KeyType& key, const ValueType& value)
{
	Node<KeyType, ValueType> *tempRoot = root;
	if (tempRoot == nullptr)
	{
		tempRoot = new Node<KeyType, ValueType>(key, value, nullptr);
		tempRoot->priority = rndgen();
		root = tempRoot;
		return;
	}
	bool found;
	ValueType val;
	val = search(key, found);
	if (!found)
	{
		while (tempRoot->hasChildren(tempRoot))
		{
			if (key < tempRoot->key && tempRoot->hasLeft(tempRoot))
				tempRoot = tempRoot->left;
			else if (key < tempRoot->key && !tempRoot->hasLeft(tempRoot))
				break;
			else if (key > tempRoot->key && tempRoot->hasRight(tempRoot))
				tempRoot = tempRoot->right;
			else if (key > tempRoot->key && !tempRoot->hasRight(tempRoot))
				break;
		}
		Node<KeyType, ValueType> *child;
		bool leftR;
		if (key < tempRoot->key)
		{
			tempRoot->left = new Node<KeyType, ValueType>(key, value, tempRoot);
			tempRoot->left->priority = rndgen();
			child = tempRoot->left;
			child->parent = tempRoot;
			leftR = false;
		}
		else
		{
			tempRoot->right = new Node<KeyType, ValueType>(key, value, tempRoot);
			tempRoot->right->priority = rndgen();
			child = tempRoot->right;
			child->parent = tempRoot;
			leftR = true;
		}

		Node<KeyType, ValueType> *par = tempRoot->parent;

		while (tempRoot->priority < child->priority)
		{
			if (leftR)
			{
				rotateLeft(tempRoot);
			}
			else if (!leftR)
			{
				rotateRight(tempRoot);
			}

			tempRoot = child->parent;
			if (tempRoot == nullptr)
				break;
			if (tempRoot->key < child->key)
				leftR = true;
			else if (tempRoot->key > child->key)
				leftR = false;
		}
	}
	else
	{
		throw std::logic_error("Key is already in tree");
		return;
	}
}

template <typename KeyType, typename ValueType>
void TreapBST<KeyType, ValueType>::remove(const KeyType& key)
{
	Node<KeyType, ValueType> *foundNode = root;
	bool found;
	bool top = true;
	search(key, found);
	if (found)
	{
		foundNode = foundNode->findNode(foundNode, key);
		while (foundNode->hasBothChildren(foundNode))
		{
			if (foundNode->left->priority < foundNode->right->priority)
			{
				rotateLeft(foundNode);
				top = false;
				if (foundNode->right != nullptr)
					foundNode = foundNode->right;
			}
			else
			{
				rotateRight(foundNode);
				top = false;
				if (foundNode->left != nullptr)
					foundNode = foundNode->left;
			}
		}
		if (foundNode->hasLeft(foundNode))
		{
			Node<KeyType, ValueType> *tempRoot = foundNode->getRootNode(foundNode);
			foundNode->left->parent = foundNode->parent;
			if (foundNode->parent != nullptr)
			{
				if (foundNode->parent->left == foundNode)
					foundNode->parent->left = foundNode->left;
				else
					foundNode->parent->right = foundNode->left;
			}
			if (foundNode != tempRoot)
				root = tempRoot;
			else
				root = foundNode->left;
			delete foundNode;
		}
			
		else if (foundNode->hasRight(foundNode))
		{
			Node<KeyType, ValueType> *tempRoot = foundNode->getRootNode(foundNode);
			foundNode->right->parent = foundNode->parent;
			if (foundNode->parent != nullptr)
			{
				if (foundNode->parent->left == foundNode)
					foundNode->parent->left = foundNode->right;
				else
					foundNode->parent->right = foundNode->right;
			}
			if (foundNode != tempRoot)
				root = tempRoot;
			else
				root = foundNode->right;
				
			delete foundNode;
		}
		else if (!foundNode->hasChildren(foundNode))
		{
			Node<KeyType, ValueType> *tempRoot = foundNode->getRootNode(foundNode);
			if (foundNode->parent != nullptr)
			{
				if (foundNode->parent->left == foundNode)
					foundNode->parent->left = nullptr;
				else
					foundNode->parent->right = nullptr;
			}
			if (foundNode != tempRoot)
				root = tempRoot;
			else
				root = nullptr;
			delete foundNode;
		}
	}
}

template <typename KeyType, typename ValueType>
void TreapBST<KeyType, ValueType>::clone(Node<KeyType, ValueType> *& newRoot, Node<KeyType, ValueType> * oldRoot)
{
	if (oldRoot == nullptr)
		newRoot = nullptr;
	else
	{
		if (oldRoot->parent != nullptr)
		{
			newRoot = new Node<KeyType, ValueType>(oldRoot->key, oldRoot->data, oldRoot->parent);
			newRoot->priority = oldRoot->priority;
		}
		else
		{
			newRoot = new Node<KeyType, ValueType>(oldRoot->key, oldRoot->data, nullptr);
			newRoot->priority = oldRoot->priority;
		}

		if (oldRoot->left != nullptr && newRoot->left == nullptr)
		{
			newRoot->left = new Node<KeyType, ValueType>(oldRoot->left->key, oldRoot->left->data, oldRoot);
			newRoot->left->priority = oldRoot->left->priority;
		}
		if (oldRoot->right != nullptr && newRoot->right == nullptr)
		{
			newRoot->right = new Node<KeyType, ValueType>(oldRoot->right->key, oldRoot->right->data, oldRoot);
			newRoot->right->priority = oldRoot->right->priority;
		}
		if (oldRoot->parent != nullptr && newRoot->parent == nullptr)
		{
			if (oldRoot->parent->parent != nullptr)
			{
				newRoot->parent = new Node<KeyType, ValueType>(oldRoot->parent->key, oldRoot->parent->data, oldRoot->parent->parent);
				newRoot->parent->priority = oldRoot->parent->priority;
			}
			else
			{
				newRoot->parent = new Node<KeyType, ValueType>(oldRoot->parent->key, oldRoot->parent->data, nullptr);
				newRoot->parent->priority = oldRoot->parent->priority;
			}
		}
		clone(newRoot->left, oldRoot->left);
		clone(newRoot->right, oldRoot->right);
	}
}