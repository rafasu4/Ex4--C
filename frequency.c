#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


#define SIZE 1*sizeof(char)
#define NUM_LETTERS ((int)26)

enum boolean {
    FALSE = 0, TRUE = 1
};


struct Node {
    int frequency;
    int counter;
    enum boolean isLeaf;
    char letter;
    struct Node *children[NUM_LETTERS];
} typedef node;

//free all the nodes
void clean(node *current){
    for (int i = 0; i < NUM_LETTERS; ++i) {
        if (current->children[i] != NULL){
            clean(current->children[i]);
        }
    }
    free(current);
}

void upperToLower(char *str, int len) {
    int i = 0;
    int length = len;
    while (i < length) {
        str[i] = tolower(
                str[i]); //The tolower() function takes an uppercase alphabet and convert it to a lowercase character.
        i++;
    }
}

char* getWord(char *w, int len, char firstChar) {
    char c = firstChar;
    int i = 0;
    while (c != EOF && c != '\n' && c != ' ' && c != '\t') {
        if (i == len) {
            len += 1;
            w = (char*)realloc(w, len*SIZE);
            if (w == NULL){
                exit(1);
            }
        }
        w[i] = c;
        c = getchar();
        i++;
    }
    //check if there's a room for \0
    if (i == len) {
        len += 1;
        w = (char *)realloc(w, len*SIZE);
        if (w == NULL){
            exit(1);
        }
    }
    w[i] = '\0';
    upperToLower(w, len);
    return w;
}

//constructor
struct Node *Initializing() {
    struct Node *node = (struct Node *) malloc(sizeof(struct Node));
    if (node == NULL) {
        printf("error!");
        exit(1);
    }
    int i = 0;
    while (i < NUM_LETTERS) {
        node->children[i] = NULL;
        i++;
    }
    node->isLeaf = FALSE;
    node->frequency = 0;
    node->counter = 1;
    return node;
}


void add(node *current, char *word) {
    int length = strlen(word);
    for (int i = 0; i < length; i++) {
        char letter = word[i];
        if (letter >= 'a' && letter <= 'z') { //As long as our letter is in the lower case
            int index = letter - 'a'; //Put in the appropriate place in the array (normalize)
            //if no such node exist - creat a new node with current letter
            if (current->children[index] == NULL) {
                current->children[index] = Initializing();
                current->children[index]->letter = letter;
                current = current->children[index];//pointing on the new node
            }
            //if a node already exist
            else {
                current = current->children[index];// /Now the "child" becomes the current node
                current->counter++;
            }
        }
    }
    current->frequency++;
    current->isLeaf = TRUE;
}

int printWordLecUp(struct Node* root){
    int ans = 0; //how much we'll need to subtract from the father node
    if(root->isLeaf == TRUE){
        printf("%c %d\n", root->letter, root->frequency);
        root->isLeaf = FALSE;
        ans = root->frequency;
        root->counter -= root->frequency;
        root->frequency = 0;
        return ans;//going back to the father of current
    }
    for (int i = 0; i < NUM_LETTERS; i++){
        if (root->children[i] != NULL && root->children[i]->counter > 0){
            printf("%c", root->letter);
            ans = printWordLecUp(root->children[i]);
            root->counter -= ans;//subtracting the frequency
            return ans;//going back to the father of current
        }
    }
    return ans;
}

int printWordLecDown(struct Node* root){
    int ans = 0; //how much we'll need to subtract from the father node
    //if the node has no children
    if(root->isLeaf == TRUE && root->counter == 1){
        printf("%c %d\n", root->letter, root->frequency);
        root->isLeaf = FALSE;
        ans = root->frequency;
        root->counter -= root->frequency;
        root->frequency = 0;
        return ans;//going back to the father of current
    }
    for (int i = NUM_LETTERS - 1 ; i >= 0; i--){
        if (root->children[i] != NULL && root->children[i]->counter > 0){
            printf("%c", root->letter);
            ans = printWordLecDown(root->children[i]);
            root->counter -= ans;//subtracting the frequency
            return ans;//going back to the father of current
        }
    }
    //if the node has children - fo deeper
    if(root->isLeaf == TRUE){
        printf("%c %d\n", root->letter, root->frequency);
        root->isLeaf = FALSE;
        ans = root->frequency;
        root->counter -= root->frequency;
        root->frequency = 0;
        return ans;
    }
    return ans;
}


int main(int argc , char* argv[]) {
    char *word, chr;
    node *ptr = Initializing();//pointer to root
    while (fscanf(stdin, " %c", &chr) == 1) {
        word = (char *) malloc(SIZE);
        //checking if malloc succed
        if(word == NULL){
            exit(1);
        }
        word = getWord(word, SIZE, chr);//getting a word each iteration
        add(ptr, word);
        free(word);
    }
    if (argc == 1) {
        for (int j = 0; j < NUM_LETTERS; j++) {
            if (ptr->children[j] != NULL) {
                while (ptr->children[j]->counter > 0) {
                    printWordLecUp(ptr->children[j]);
                }
            }
        }
    }
    if (argc == 2) {
        if (**(argv + 1) == 'r') {
            for (int j = NUM_LETTERS -1 ; j >= 0; j--) {
                if (ptr->children[j] != NULL) {
                    while (ptr->children[j]->counter > 0) {
                        printWordLecDown(ptr->children[j]);
                    }
                }
            }
        }
        else{
            printf("error , try again");
            clean(ptr);
            exit(1);
        }
    }
    clean(ptr);
    return 0;
}
