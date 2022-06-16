#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define NUM_CHARS	256

typedef struct _trienode {
	struct _trienode* children[NUM_CHARS];
	bool terminal;
} trienode;

trienode* createnode()
{
	trienode* newnode = malloc(sizeof *newnode);

	for (int i = 0; i < NUM_CHARS; i++) {
		newnode->children[i] = NULL;
	}
	newnode->terminal = false;
	return newnode;
}

bool trieinsert(trienode **root, char *signedtext)
{
	if (*root == NULL) {
		*root = createnode();
	}

	unsigned char* text = (unsigned char*)signedtext;
	trienode* curr = *root;
	int length = (int)strlen(signedtext);

	for (int i = 0; i < length; i++) {
		if (curr->children[text[i]] == NULL) {
			curr->children[text[i]] = createnode();
		}
		curr = curr->children[text[i]];
	}
	if (curr->terminal) {
		return false;
	}
	else {
		curr->terminal = true;
		return true;
	}
}

bool searchtrie(trienode* root, char* signedtext)
{
	unsigned char* text = (unsigned char*)signedtext;
	trienode* curr = root;
	int length = (int)strlen(signedtext);

	for (int i = 0; i < length; i++) {
		if (curr->children[text[i]] == NULL) {
			return false;
		}
		curr = curr->children[text[i]];
	}
	return curr->terminal;
}

bool node_has_children(trienode* node)
{
	if (node == NULL) return false;

	for (int i = 0; i < NUM_CHARS; i++) {
		if (node->children[i] != NULL)
			return true;
	}
	return false;
}

trienode* deletestr_rec(trienode* node, unsigned char* text, bool* deleted)
{
	if (node == NULL) return node;
	printf("text: %s\n", text);
	if (*text == '\0') {
		if (node->terminal) {
			node->terminal = false;
			*deleted = true;

			if (node_has_children(node) == false) {
				free(node);
				node = NULL;
			}
		}
		return node;
	}

	node->children[text[0]] = deletestr_rec(node->children[text[0]], text + 1, deleted);
	if (*deleted &&
			node_has_children(node) == false &&
			node->terminal == false) {
		free(node);
		node = NULL;
	}
	return node;
}

bool deletestr(trienode** root, char* signedtext)
{
	unsigned char* text = (unsigned char*)signedtext;
	bool result = false;

	if (*root == NULL) return false;

	*root = deletestr_rec(*root, text, &result);
	return result;
}

void printtrie_rec(trienode* node, unsigned char *prefix, int length)
{
	unsigned char *newprefix = malloc((length + 2) * sizeof(unsigned char));
	memcpy(newprefix, prefix, length);
	newprefix[length + 1] = 0;

	//printf("prefix: %s, length: %d\n", prefix, length);
	if (node->terminal) {
		printf("WORD: %s\n", prefix);
	}
	for (int i = 0; i < NUM_CHARS; i++) {
		if (node->children[i] != NULL) {
			newprefix[length] = i;
			printtrie_rec(node->children[i], newprefix, length + 1);
		}
	}
	free(newprefix);
}

void printtrie(trienode* root)
{
	trienode* curr = root;
	if (curr == NULL) {
		printf("TRIE EMPTY!\n");
		return;
	}

	printtrie_rec(root, NULL, 0);
}

int main()
{
	trienode* root = NULL;

	trieinsert(&root, "KIT");
	trieinsert(&root, "CATTLE");
	trieinsert(&root, "KIN");
	trieinsert(&root, "CAT");
	trieinsert(&root, "HAPPY");
	printtrie(root);
	printf("search for CATTLE: %d\n", searchtrie(root, "CATTLE"));
	printf("search for KIN: %d\n", searchtrie(root, "KIN"));
	printf("search for KITTEN: %d\n", searchtrie(root, "KITTEN"));
	deletestr(&root, "CATTLE");
	deletestr(&root, "KIT");
	printtrie(root);
}
