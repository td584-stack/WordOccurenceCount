#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "word.h"
#include <sys/stat.h>

static node *root = NULL;


//So the goal here is to use strcmp() to add words to the tree.
void tree_add(node **nd, const char *word){
	if (*nd == NULL){			//This is the base case, if the node is NULL, we allocate a tempory node memory with malloc
		node *n = malloc(sizeof(*n);    //Then duplicate the passed word from our word reader using strdup() into temp nodes word var
		n->word = strdup(*word);	//Increment count
		n->count = 1;			//Set left and right children to null
		n->left = n->right = NULL;	//Set the actual node = temp node and return
		*nd = n;
		return;
	}

	int c = strcmp(word, (*nd)->word);	//we compare the word with the current nodes word
	if(c == 0) (*nd)->count++;	//if they are equal, we increase the nodes count
	else if(c < 0){				//if word is < nodes word then we recursively call tree_add into the left subtree
		tree_add(&(*nd)->left, word);
	} else if(c > 0){
		tree_add(&(*nd)->right, word);	//same thing as previous but for right subtree.
	}
}


size_t tree_size(node *nd){	//this is pretty straight forward; it will recursively traverse the tree and return the num of unique 'words'/'nodes' in the tree.
	if(nd == NULL) return 0;
	return 1 + tree_size(nd->left) + tree_size(nd->right);
}

void tree_to_array(node **nd, node **arr, size_t *i){		//it takes a node, an arr, and a counter (*i)
	if (nd==NULL) return;					//it then recursevly adds the left subtree to the arr, then the current node, then right subtree
	tree_to_array(nd->left, arr, i);			
	arr[(*i)++] = nd;		//arr[(*i)++] just means arr[i++], i is a pointer to a passed counter of *i which is defined to be 0 in the caller function.
	tree_to_array(nd->right, arr, i); // its kinda weird but it makes sure that the same counter is being incremented every time
}

void tree_free(node *nd){	//So for free we first traverse down the left subtree
	if(nd==NULL) return;	// Then the right subtree
	tree_free(nd->left);	// We free the malloc'd word
	tree_free(nd->right);	// and last free the actual node itself.
	free(nd->word);
	free(nd);		// So children are always freed before parent node.
}




void process_path(const char *path, const char *suffix){

	


}
