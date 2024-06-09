#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h> 

#define E "E"
#define T "T"
#define F "F"

typedef struct TreeNode {
    char value[20];
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

TreeNode* createNode(const char* value);
void freeParseTree(TreeNode* root);
int parse_string(char* input, int len, TreeNode** tree, char** variables, int numVariables);
int parse_addsub(char* str, int len, TreeNode** tree, char** variables, int numVariables);
int parse_muldiv(char* str, int len, TreeNode** tree, char** variables, int numVariables);
int parse_var(char* str, int len, TreeNode** tree, char** variables, int numVariables);

TreeNode* createNode(const char* value) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    strcpy_s(node->value, 20, value);
    node->left = NULL;
    node->right = NULL;
    return node;
}

void freeParseTree(TreeNode* root) {
    if (root == NULL) {
        return;
    }
    freeParseTree(root->left);
    freeParseTree(root->right);
    free(root);
}

int parse_string(char* input, int len, TreeNode** tree, char** variables, int numVariables) {
    int result = parse_addsub(input, len, tree, variables, numVariables);
    return result;
}

int parse_addsub(char* str, int len, TreeNode** tree, char** variables, int numVariables) {
    TreeNode* node = createNode("E");

    if (len > 0 && (str[0] == '(' || isalpha(str[0]))) {
        int j = parse_muldiv(str, len, &(node->left), variables, numVariables);

        if (j > 0) {
            int i = j;

            if (i < len && (str[i] == '+' || str[i] == '-')) {
                char op[2] = { str[i], '\0' };
                strcpy_s(node->value, 20, op);

                j = parse_addsub(str + i + 1, len - i - 1, &(node->right), variables, numVariables);

                if (j > 0) {
                    i += j + 1;
                }
                else {
                    free(node);
                    *tree = NULL;
                    return 0;
                }
            }
            *tree = node;
            return i;
        }
        else {
            free(node);
            *tree = NULL;
            return 0;
        }
    }
    else {
        free(node);
        *tree = NULL;
        return 0;
    }
}

int parse_muldiv(char* str, int len, TreeNode** tree, char** variables, int numVariables) {
    TreeNode* node = createNode("T");

    if (len > 0 && (str[0] == '(' || isalpha(str[0]))) {
        int j = parse_var(str, len, &(node->left), variables, numVariables);

        if (j > 0) {
            int i = j;

            if (i < len && (str[i] == '*' || str[i] == '/')) {
                char op[2] = { str[i], '\0' };
                strcpy_s(node->value, 20, op);

                j = parse_muldiv(str + i + 1, len - i - 1, &(node->right), variables, numVariables);

                if (j > 0) {
                    i += j + 1;
                }
                else {
                    free(node);
                    *tree = NULL;
                    return 0;
                }
            }
            *tree = node;
            return i;
        }
        else {
            free(node);
            *tree = NULL;
            return 0;
        }
    }
    else {
        free(node);
        *tree = NULL;
        return 0;
    }
}

int parse_var(char* str, int len, TreeNode** tree, char** variables, int numVariables) {
    TreeNode* node = createNode("F");

    if (len > 0) {
        if (str[0] == '(') {
            int i = 1;

            int j = parse_addsub(str + i, len - i, &(node->left), variables, numVariables);

            if (j > 0) {
                i += j;

                if (i < len && str[i] == ')') {
                    i++;
                }
                else {
                    free(node);
                    *tree = NULL;
                    return 0;
                }
            }
            else {
                free(node);
                *tree = NULL;
                return 0;
            }
            *tree = node;
            return i;
        }
        else if (isalpha(str[0])) {
            for (int k = 0; k < numVariables; k++) {
                if (str[0] == variables[k][0]) {
                    strcpy_s(node->value, 20, variables[k]);
                    *tree = node;
                    return 1;
                }
            }
            free(node);
            *tree = NULL;
            return 0;
        }
    }

    free(node);
    *tree = NULL;
    return 0;
}

int main() {
    char input[100];
    int result;
    TreeNode* parseTree = NULL;
    int numVariables;

    printf("\t\t\t\t\t**************************************************************\n");
    printf("\t\t\t\t\t*                                                            *\n");
    printf("\t\t\t\t\t*                  Welcome to Arithmetic Expression          *\n");
    printf("\t\t\t\t\t*                          Parser                            *\n");
    printf("\t\t\t\t\t*                                                            *\n");
    printf("\t\t\t\t\t**************************************************************\n\n");

    printf("Please enter the number of variables: ");
    scanf_s("%d", &numVariables);
    getchar();

    char** variables = (char**)malloc(numVariables * sizeof(char*));
    for (int i = 0; i < numVariables; i++) {
        variables[i] = (char*)malloc(2 * sizeof(char));
    }

    printf("\nEnter the variable names (one character each):\n");
    for (int i = 0; i < numVariables; i++) {
        printf("Variable %d: ", i + 1);
        scanf_s(" %c", &variables[i][0]);
        getchar();
        variables[i][1] = '\0';
    }

    printf("\nEnter an arithmetic expression ('exit' to end): \n");

    while (1) {
        fgets(input, 100, stdin);
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0) {
            break;
        }

        char* temp = input;
        char* dest = input;
        while (*temp != '\0') {
            if (!isspace(*temp)) {
                *dest++ = *temp;
            }
            temp++;
        }
        *dest = '\0';

        result = parse_string(input, strlen(input), &parseTree, variables, numVariables);

        if (result > 0 && result == strlen(input)) {
            printf("Status: Accepted!\n");
        }
        else {
            printf("Status: Rejected!\n");
        }

        if (parseTree != NULL) {
            freeParseTree(parseTree);
            parseTree = NULL;
        }
    }

    for (int i = 0; i < numVariables; i++) {
        free(variables[i]);
    }
    free(variables);

    return 0;
}
