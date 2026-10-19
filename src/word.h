#include <stddef.h>

// Word Storage

typedef struct node {
	char *word;
	int count;
	struct node *left, *right;
} node;

void tree_add(node **root, const char *word);
size_t tree_size(node *root);
void tree_to_array(node *root, node **arr, size_t *i);
void tree_free(node *root);

// Word Reader

typedef struct {
	char *buf;
	size_t len, cap;
	int pending_hyphen;
} word_reader;

void wr_init(word_reader *w);
void wr_read(word_reader *w, const char *data, size_t n, node **root);
void wr_ender(word_reader *w, node **root);
void wr_free(word_reader *w);

// Path Traversal

void process_path(const char *path, const char *suffix, node **root);
void process_file(const char *path, node **root);
void traverse_dir(const char *path, const char *suffix, node **root);

//Output

void print_sorted(node *root);
