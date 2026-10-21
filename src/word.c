#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "word.h"
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>

static node *root = NULL;


//Binary Search Tree Implementation
//---------------------------------
//So the goal here is to use strcmp() to add words to the tree.

void tree_add(node **nd, const char *word){
	if (*nd == NULL){			//This is the base case, if the node is NULL, we allocate a tempory node memory with malloc
		node *n = malloc(sizeof(*n));    //Then duplicate the passed word from our word reader using strdup() into temp nodes word var
		if (n == NULL) {
			perror("malloc");
			exit(EXIT_FAILURE);
    	}
		
		n-> word = malloc(strlen(word)+1);

		if(n-> word == NULL){
			perror("malloc");
			free(n);
			exit(EXIT_FAILURE);
		}

		strcpy(n-> word, word);
		//n->word = strdup(word);	//Increment count

		if (n->word == NULL) {
			perror("strcpy");
			free(n);
			exit(EXIT_FAILURE);
    	}

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

void tree_to_array(node *nd, node **arr, size_t *i){		//it takes a node, an arr, and a counter (*i)
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

//Word Reading
//-----------
//

// This should initialize our struct for a clean slate
void wr_init(word_reader *w){
//TO DO
// initialize a small buffer
// set cap to that buffer size
// set len to 0
// set pending_hyphen to false
	w -> cap = 8;
	w -> len = 0;
	w -> pending_hyphen = 0;
	w -> buf = malloc(w -> cap);
	
	if(w->buf == NULL){
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	*(w -> buf) = '\0';
}

static int isletter(char c){
	return ((c >= 'A' && c <= 'Z')||(c >= 'a' && c <= 'z'));
}

static void addchara(word_reader *w, char c)
{
	//update capacity (buffer size) if len + 2 > capacity
	
	if(w->cap < (w->len + 2)){
		size_t new_size = 8;
		while(new_size< (w->len + 2)){
			new_size *=2;
		}
		char * newbuf= realloc(w-> buf, new_size);

		if (newbuf== NULL){
			perror("realloc");
			exit(EXIT_FAILURE);
		}

		w->buf = newbuf;
		w->cap = new_size;
	}

    w->buf[w->len] = c;
	w->len++;
    w->buf[w->len] = '\0';
}

static void wr_addWord(word_reader *w){
	if(w->len == 0) return;

	w-> buf[w->len] = '\0';
	tree_add(&root, w-> buf);

	*(w-> buf) = '\0';
	w-> len = 0;
}

void wr_read(word_reader *w, const char *data, size_t n){
//TO DO
//so lost for this, will look into tonight
	for(size_t i = 0; i < n; i++){
		char c = data[i];

		//if there is an pending character '-';
		if(w->pending_hyphen){
			w-> pending_hyphen=0;

			if(isletter(c)){
				addchara(w, '-');
				addchara(w, c);
				continue;
			}

			wr_addWord(w);
		}

		if(isletter(c)|| c=='\'') {addchara(w, c);} // if c is alphabet or \', no matter where it is, we append it

		else if(c == '-'){
			if (w->len > 0 && isletter(w->buf[w->len - 1])){ w->pending_hyphen = 1; }
			else {wr_addWord(w);}//len==0(addword will do nothing) or the last character is '\''
		}

		else{
			//c is num, whitespace, and punctuation other than ' and -
			wr_addWord(w);
		}

	}
}


void wr_ender(word_reader *w){
//TO DO
//So if a file is on the last word and the file doesnt end in a word serperator, this just still somehow make that work count
//Example:
//"blah blah blah.......... .... .. . . .. word" so word wouldnt count since the file doesnt end with a whitespace
	w -> pending_hyphen = 0;// if the last chara is '-', it does not matter since we would not add it to the word until we read the next letter
	wr_addWord(w);

}

void wr_free(word_reader *w){
//TO DO
//This should free the buffer and word reader memory where applicable	
	free(w -> buf);
	w-> buf = NULL;
	w-> len = 0;
	w-> cap = 0;
	w-> pending_hyphen = 0;
}


//Path Traversing
//---------------
//



void process_path(const char *path, const char *suffix){

	
}

void process_file(const char *path){
	int fd = open(path, O_RDONLY);
	
	if(fd < 0){
		perror(path);
		return;
	}

	word_reader reader;
	wr_init(&reader);
	
	char buffer[4096];
	ssize_t bytesread = read(fd, buffer, sizeof(buffer));

	while(bytesread > 0){
		wr_read(&reader, buffer, (size_t)bytesread);
		bytesread = read(fd, buffer, sizeof(buffer));
	}
	
	if(bytesread < 0){
		perror("read");
		return;
	}

	wr_ender(&reader);
	wr_free(&reader);
	close(fd);
}

void traverse_dir(const char *path, const char *suffix){
	

}

//Output Formatting
//-----------------
//
//This should use the tree to into an array based on count
void print_sorted(node *root){
//TO DO
	size_t Numbernode = tree_size(root);
	if(Numbernode == 0) return;

	node **arr = malloc( Numbernode * sizeof(*arr));

    if (arr == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

	size_t i = 0;
	tree_to_array(root, arr, &i);
	//sort by selection
	for(size_t i = 0; i < Numbernode; i++){
		size_t currentposition = i;
		for(size_t j = i+1; j < Numbernode; j++){
			if (arr[j]->count > arr[currentposition]->count || (arr[j]->count == arr[currentposition]->count && strcmp(arr[j]->word, arr[currentposition]->word) < 0)) {
				currentposition = j;
			}
		}
		node *temp = arr[i];
		arr[i] = arr[currentposition];
		arr[currentposition] = temp;
	}
	for (size_t j = 0; j < Numbernode; j++) {
        printf("%s %d\n", arr[j]->word, arr[j]->count);
    }

    free(arr);
}


//Main
//----
//

int main(/* Arguments with path, suffix, and something else I think*/int argc, char *argv[]){
//TO DO
	 if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    process_file(argv[1]);
    print_sorted(root);
    tree_free(root);
    root = NULL;

    return EXIT_SUCCESS;
}


