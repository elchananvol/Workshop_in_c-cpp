
#include <stdlib.h>
#include <stdbool.h>
#include "RBTree.h"
#include "Structs.h"
#include <string.h>


Node *createNewNode(void *data);

Node *findTheUncle(Node *node);

Node *oneChild(Node *node);

int insertNode(RBTree *tree, Node *node);

void rotateRight(RBTree *tree, Node *n);

void insertCase4(Node *node, RBTree *tree);

void fixInsert(RBTree *tree, Node *node);

Color getColor(Node *n);

void deleteStep2(RBTree *tree, Node *child);

Node *getBro(Node *n);

void deleteCase1(RBTree *tree, Node *node);

void deleteCase2(RBTree *tree, Node *n);

void deleteCase3(RBTree *tree, Node *n);

void deleteCase4(RBTree *tree, Node *n);

void deleteCase5(RBTree *tree, Node *n);

void deleteCase6(RBTree *tree, Node *n);

void replaceNodesData(Node *node1, Node *node2);

Node *findNode(const RBTree *tree, Node *node, const void *data);

Node *findSuccessor(Node *node);


int forAllNodes(Node *node, forEachFunc func, void *args);

void rotateLeft(RBTree *tree, Node *n);
//void fff(void *a)
//{
////	free(a);
//
//}
//
//
//
//int cmpp(void *a, void *b)
//{
//	int *aa = (int *) a;
//	int *bb = (int *) b;
//	return *aa - *bb;
//}
//
//int main()
//{
//	CompareFunc c = (CompareFunc) cmpp;
//	RBTree *tree = (RBTree *) newRBTree(c, fff);
//	int arr[] = {20,2,3,17,5,6,8,0,20,4,9};
//	for (int i = 0; i < 9; i++)
//	{
//		insertToRBTree(tree, (void *) &arr[i]);
//	}
//	for (int i = 0; i < 15; i++)
//	{
//		deleteFromRBTree(tree, (void *) &arr[i]);
//	}
////	Vector* v = (Vector*)malloc(sizeof(Vector));
////	v->len=3;
////	v->vector=(double *)malloc(3*sizeof(double));
////	v->vector[1]=3.3;
////	v->vector[2]=2.3;
////	v->vector[0]=4.3;
////	Vector* c=(Vector*)malloc(sizeof(Vector));
////	c->len=3;
////	c->vector=(double *)malloc(3*sizeof(double));
////	c->vector[1]=1.3;
////	c->vector[2]=7.3;
////	c->vector[0]=5.3;
////	Vector* new = (Vector*)malloc(sizeof(Vector));
////	copyIfNormIsLarger(v,new);
////
////
////
////
//}
/**
 * constructs a new RBTree with the given CompareFunc.
 * comp: a function two compare two variables.
 * freefunc; func to free 1 node node/
 */
RBTree *newRBTree(CompareFunc compFunc, FreeFunc freeFunc)
{
	RBTree *tree = (RBTree *) malloc(sizeof(RBTree));
	if (tree == NULL)
	{
		return NULL;
	}
	tree->compFunc = compFunc;
	tree->freeFunc = freeFunc;
	tree->root = NULL;
	tree->size = 0;
	return tree;
}

/**
 * add an item to the tree
 * @param tree: the tree to add an item to.
 * @param data: item to add to the tree.
 * @return: 0 on failure, other on success. (if the item is already in the tree - failure).
 */
int insertToRBTree(RBTree *tree, void *data)
{
	Node *n;
	n = createNewNode(data);
	if (n != NULL && tree != NULL)
	{
		if (insertNode(tree, n))
		{
			fixInsert(tree, n);
			tree->size++;
		}
		else
		{
			free(n);
			return false;
		}
	}
	else
	{
		return false;
	}
	return true;
}


/**
 * create new node with data and red color
 * @param data, the data to put on node
 * @return null if fail, else node
 */
Node *createNewNode(void *data)
{
	Node *node = (Node *) malloc(sizeof(Node));
	if (node == NULL)
	{
		return NULL;  //todo
	}
	node->data = data;
	node->color = RED;
	node->parent = NULL;
	node->right = NULL;
	node->left = NULL;
	return node;
}

/**
 * hang some node on tree
 * @param tree THE TREE
 * @param node THE NODE
 * @return false if data on tree, true if node inserted in
 */
int insertNode(RBTree *tree, Node *node)
{
	if (tree->root == NULL)
	{
		tree->root = node;
		return 1;
	}
	struct Node *x = tree->root;
	while (1)
	{
		if (tree->compFunc(x->data, node->data) < 0)
		{
			if (x->right == NULL)
			{
				x->right = node;
				node->parent = x;
				break;
			}
			else
			{
				x = x->right;
			}

		}
		else if (tree->compFunc(x->data, node->data) > 0)
		{
			if (x->left == NULL)
			{
				x->left = node;
				node->parent = x;
				break;
			}
			else
			{
				x = x->left;
			}
		}
		else if (tree->compFunc(x->data, node->data) == 0)
		{
			return false;
		}
	}
	return true;
}


Node *findTheUncle(Node *node)
/**
 * @param node the node we need
 * @return the uncle or null if the uncle doesnt exist
 */
{
	if (node->parent == NULL || node->parent->parent == NULL)
	{ return NULL; }
	if (node->parent->parent->right == node->parent)
	{
		return node->parent->parent->left;
	}
	else
	{
		return node->parent->parent->right;
	}
}


/**
 *do the rotate right according algorithm.
 * @param tree THE TREE
 * @param n: the node to rotate
 */
void rotateRight(RBTree *tree, Node *n)
{
	Node *new = n->left;
	Node *parent = n->parent;
	n->left = new->right;
	new->right = n;
	if (n->left != NULL)
	{
		n->left->parent = n;
	}
	n->parent = new;
	new->parent = parent;
	if (parent != NULL)
	{
		if (n == parent->left)
		{
			parent->left = new;
		}
		else if (n == parent->right)
		{
			parent->right = new;
		}
	}
	else
	{
		tree->root = new;
	}
}


/**
 *do the rotate left according algorithm.
 * @param tree THE TREE
 * @param n: the node to rotate
 */
void rotateLeft(RBTree *tree, Node *n)
{
	Node *new = n->right;
	Node *parent = n->parent;
	n->right = new->left;
	new->left = n;
	n->parent = new;
	new->parent = parent;
	if (n->right != NULL)
	{
		n->right->parent = n;
	}
	if (parent != NULL)
	{
		if (n == parent->left)
		{
			parent->left = new;
		}
		else if (n == parent->right)
		{
			parent->right = new;
		}
	}
	else
	{
		tree->root = new;
	}


}

/**
 * fix the node that inserted according the algo.
 * @param tree the tree.
 * @param node the new node that maybe break the rules.
 */
void fixInsert(RBTree *tree, Node *node)
{
	Node *uncle = findTheUncle(node);
	if (node->parent == NULL)
	{
		node->color = BLACK;
		return;
	}
	else if (node->parent->color == BLACK)
	{
		return;
	}
	else if (node->parent->color == RED && getColor(uncle) == BLACK)
	{
		insertCase4(node, tree);
	}
	else if (node->parent->color == RED && uncle->color == RED)
	{
		node->parent->color = BLACK;
		uncle->color = BLACK;
		node->parent->parent->color = RED;
		fixInsert(tree, node->parent->parent);
	}
}

/**
 * fix the tree in case that parent red and uncle black
 * @param node the node
 * @param tree the tree
 */
void insertCase4(Node *node, RBTree *tree)
{
	if (node == node->parent->right && node->parent == node->parent->parent->left)
	{
		rotateLeft(tree, node->parent);
		node = node->left;
	}
	else if (node == node->parent->left && node->parent == node->parent->parent->right)
	{
		rotateRight(tree, node->parent);
		node = node->right;
	}

	Node *parent = node->parent;
	Node *grandpa = parent->parent;
	if (node == parent->left)
	{
		rotateRight(tree, grandpa);
	}
	else
	{
		rotateLeft(tree, grandpa);
	}
	parent->color = BLACK;
	grandpa->color = RED;

}

/**
 * @param n the node
 * @return the node color (black if node is null.)
 */
Color getColor(Node *n)
{
	if (n == NULL)
	{
		return BLACK;
	}
	else
	{
		return n->color;
	}
}
/**
 * replace data between nodes
 * @param node1
 * @param node2
 */

void replaceNodesData(Node *node1, Node *node2)
{
	void *new = node1->data;
	node1->data = node2->data;
	node2->data = new;
}

void deleteCase1(RBTree *tree, Node *node)
{
	if (node->parent != NULL)
	{
		deleteCase2(tree, node);
	}
}


void deleteCase4(RBTree *tree, Node *n)
{
	Node *bro = getBro(n);

	if ((n->parent->color == RED) && (bro->color == BLACK) &&
		(getColor(bro->left) == BLACK) && (getColor(bro->right) == BLACK))
	{
		bro->color = RED;
		n->parent->color = BLACK;
	}
	else
	{
		deleteCase5(tree, n);
	}
}

/**
 * find the bro of node
 * @param n node
 * @return bro' or null if doesn't exist.
 */
Node *getBro(Node *n)
{
	Node *p = n->parent;

	// No parent means no sibling.
	if (p == NULL)
	{
		return NULL;
	}

	if (n == p->left)
	{
		return p->right;
	}
	else
	{
		return p->left;
	}
}

/**
 * remove an item from the tree
 * @param tree: the tree to remove an item from.
 * @param data: item to remove from the tree.
 * @return: 0 on failure, other on success. (if data is not in the tree - failure).
 */
int deleteFromRBTree(RBTree *tree, void *data)
{
	Node *n = findNode(tree, tree->root, data);
	if (n == NULL)
	{
		return false;
	}
	if (n->right != NULL && n->left != NULL)
	{
		Node *child = findSuccessor(n);
		replaceNodesData(n, child);
		n = child;
	}

	Node *child = findSuccessor(n);
	if (n->color == BLACK)
	{
		if (child->color == RED)
		{
			child->color = BLACK;
		}
		else
		{
			deleteCase1(tree, n);
		}
	}
	replaceNodesData(n, child);
	deleteStep2(tree, child);
	return true;
}
/**
 *for case 2.
 **/
void deleteStep2(RBTree *tree, Node *child)
{
	tree->freeFunc(child->data);
	Node *childOfChild = oneChild(child);
	if (child->parent != NULL)
	{
		if (child == child->parent->right)
		{
			child->parent->right = childOfChild;
			if (childOfChild != NULL)
			{
				childOfChild->parent = child->parent;
			}
		}
		else
		{
			child->parent->left = childOfChild;
			if (childOfChild != NULL)
			{
				childOfChild->parent = child->parent;
			}
		}
	}
	else
	{
		tree->root = childOfChild;
		if (childOfChild != NULL)
		{
			childOfChild->parent = NULL;
		}
	}

	free(child);
	tree->size--;
}

/**
 * bring the child of the node/ for nodes with one child!.
 * @param node node
 * @return the sun.
 */
Node *oneChild(Node *node)
{


	if (node->left != NULL)
	{
		return node->left;
	}
	else
	{
		return node->right;
	}

}
/**
 * for case 6
 * @param tree tree
 * @param n node
 */
void deleteCase6(RBTree *tree, Node *n)
{
	Node *bro = getBro(n);

	bro->color = n->parent->color;
	n->parent->color = BLACK;

	if (n == n->parent->left)
	{
		bro->right->color = BLACK;
		rotateLeft(tree, n->parent);
	}
	else
	{
		bro->left->color = BLACK;
		rotateRight(tree, n->parent);
	}
}

/**
 * 	This if statement is trivial, due to case 2 (even though case 2 changed
	 the sibling to a sibling's child, the sibling's child can't be red, since
	 no red parent can have a red child).
 * @param tree tree
 * @param n node
 */
void deleteCase5(RBTree *tree, Node *n)
{

	Node *bro = getBro(n);
	if (getColor(bro) == BLACK)
	{
		if ((n == n->parent->left) && (getColor(bro->right) == BLACK) &&
			(bro->left->color == RED))
		{
			bro->color = RED;
			bro->left->color = BLACK;
			rotateRight(tree, bro);
		}
		else if ((n == n->parent->right) && (getColor(bro->left) == BLACK) &&
				 (getColor(bro->right) == RED))
		{
			bro->color = RED;
			bro->right->color = BLACK;
			rotateLeft(tree, bro);
		}
	}
	deleteCase6(tree, n);
}

void deleteCase3(RBTree *tree, Node *n)
{
	Node *bro = getBro(n);

	if ((n->parent->color == BLACK) && (bro->color == BLACK) &&
		((getColor(bro->left)) == BLACK) && (getColor(bro->right) == BLACK))
	{
		bro->color = RED;
		deleteCase1(tree, n->parent);
	}
	else
	{
		deleteCase4(tree, n);
	}
}

void deleteCase2(RBTree *tree, Node *n)
{
	Node *bro = getBro(n);

	if (bro->color == RED)
	{
		n->parent->color = RED;
		bro->color = BLACK;
		if (n == n->parent->left)
		{
			rotateLeft(tree, n->parent);
		}
		else
		{
			rotateRight(tree, n->parent);
		}
	}
	deleteCase3(tree, n);
}


Node *findNode(const RBTree *tree, Node *node, const void *data)
{
	if (node == NULL)
	{
		return NULL;
	}
	int gap = tree->compFunc(node->data, data);
	if (gap == 0)
	{
		return node;
	}
	else if (gap < 0)
	{
		return findNode(tree, node->right, data);
	}
	else
	{
		return findNode(tree, node->left, data);
	}
}

/**
 * check whether the tree RBTreeContains this item.
 * @param tree: the tree to add an item to.
 * @param data: item to check.
 * @return: 0 if the item is not in the tree, other if it is.
 */
int RBTreeContains(const RBTree *tree, const void *data)
{
	if (tree == NULL)
	{
		return 0;
	}
	Node *node = findNode(tree, tree->root, data);
	if (node != NULL)
	{
		return true;
	}
	return false;
}

/**
 * Activate a function on each item of the tree. the order is an ascending order. if one of the activations of the
 * function returns 0, the process stops.
 * @param tree: the tree with all the items.
 * @param func: the function to activate on all items.
 * @param args: more optional arguments to the function (may be null if the given function support it).
 * @return: 0 on failure, other on success.
 */
int forEachRBTree(const RBTree *tree, forEachFunc func, void *args)
{
	if (tree == NULL)
	{
		return 0;
	}
	return forAllNodes(tree->root, func, args);
}

/**
 * Activate a function on each item of the tree. the order is an ascending order. if one of the activations of the
 * function returns 0, the process stops.
 * @param node: the node we will search
 * @param func: the function to activate on all items.
 * @param args: more optional arguments to the function (may be null if the given function support it).
 * @return: 0 on failure, other on success.
 */
int forAllNodes(Node *node, forEachFunc func, void *args)
{
	if (node == NULL)
	{
		return true;
	}
	if (!(forAllNodes(node->left, func, args)))
	{
		return false;
	}
	if (func(node->data, args) == 0)
	{
		return false;
	}
	if (!(forAllNodes(node->right, func, args)))
	{
		return false;
	}
	return true;
}

/**
 * free node and all its suns recursively
 * @param node a pointer to the node to delete
 * @param freeFunc: the tree free func
 */
void free1Node(Node *node, FreeFunc freeFunc)
{
	if (node->right != NULL)
	{
		free1Node(node->right, freeFunc);
	}
	if (node->left != NULL)
	{
		free1Node(node->left, freeFunc);
	}


	freeFunc(node->data);
	free(node);
}

/**
 * free all memory of the data structure.
 * @param tree: pointer to pointer to the tree to free.
 */
void freeRBTree(RBTree **tree)
{
	if (tree != NULL)
	{
		if ((*tree)->root != NULL)
		{
			free1Node((*tree)->root, (*tree)->freeFunc);
		}
		free((*tree));
	}


}

Node *findSuccessor(Node *node)
{
	if (node->right == NULL)
	{
		return node;
	}
	Node *successor = node->right;

	while (successor->left != NULL)
	{
		successor = successor->left;
	}
	return successor;
}

