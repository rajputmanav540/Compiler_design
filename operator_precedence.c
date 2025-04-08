#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 10

char productions[MAX][MAX];
char non_terminals[MAX], terminals[MAX];
int n, nt_count = 0, t_count = 0;

char leading[MAX][MAX], trailing[MAX][MAX];
int leading_count[MAX] = {0}, trailing_count[MAX] = {0};

char precedence_table[MAX][MAX];

// Check if symbol is terminal
int is_terminal(char symbol) {
    return !isupper(symbol) && symbol != '\0';
}

// Add unique character to an array
void add_unique(char arr[], int *count, char c) {
    for (int i = 0; i < *count; i++)
        if (arr[i] == c) return;
    arr[(*count)++] = c;
}

// Get index of non-terminal
int get_nt_index(char nt) {
    for (int i = 0; i < nt_count; i++)
        if (non_terminals[i] == nt) return i;
    return -1;
}

// Compute LEADING set
void compute_leading() {
    for (int i = 0; i < nt_count; i++) {
        char nt = non_terminals[i];
        for (int j = 0; j < n; j++) {
            if (productions[j][0] == nt) {
                if (is_terminal(productions[j][2])) {
                    add_unique(leading[i], &leading_count[i], productions[j][2]);
                } else if (isupper(productions[j][2]) && is_terminal(productions[j][3])) {
                    int idx = get_nt_index(productions[j][2]);
                    if (idx != -1)
                        add_unique(leading[i], &leading_count[i], productions[j][3]);
                }
            }
        }
    }
}

// Compute TRAILING set
void compute_trailing() {
    for (int i = 0; i < nt_count; i++) {
        char nt = non_terminals[i];
        for (int j = 0; j < n; j++) {
            if (productions[j][0] == nt) {
                int len = strlen(productions[j]);
                if (is_terminal(productions[j][len - 1])) {
                    add_unique(trailing[i], &trailing_count[i], productions[j][len - 1]);
                } else if (isupper(productions[j][len - 1]) && is_terminal(productions[j][len - 2])) {
                    int idx = get_nt_index(productions[j][len - 1]);
                    if (idx != -1)
                        add_unique(trailing[i], &trailing_count[i], productions[j][len - 2]);
                }
            }
        }
    }
}

// Set precedence relation
void set_precedence(char a, char b, char rel) {
    int i = -1, j = -1;
    for (int k = 0; k < t_count; k++) {
        if (terminals[k] == a) i = k;
        if (terminals[k] == b) j = k;
    }
    if (i != -1 && j != -1)
        precedence_table[i][j] = rel;
}

// Construct operator precedence table
void construct_table() {
    for (int i = 0; i < t_count; i++)
        for (int j = 0; j < t_count; j++)
            precedence_table[i][j] = ' ';

    for (int i = 0; i < n; i++) {
        char *prod = productions[i];
        for (int j = 2; j < strlen(prod) - 1; j++) {
            if (is_terminal(prod[j]) && is_terminal(prod[j + 1])) {
                set_precedence(prod[j], prod[j + 1], '=');
            } else if (is_terminal(prod[j]) && isupper(prod[j + 1])) {
                int idx = get_nt_index(prod[j + 1]);
                for (int k = 0; k < leading_count[idx]; k++) {
                    set_precedence(prod[j], leading[idx][k], '<');
                }
            } else if (isupper(prod[j]) && is_terminal(prod[j + 1])) {
                int idx = get_nt_index(prod[j]);
                for (int k = 0; k < trailing_count[idx]; k++) {
                    set_precedence(trailing[idx][k], prod[j + 1], '>');
                }
            }
        }
    }
}

// Print the operator precedence table
void print_table() {
    printf("\nOperator Precedence Table:\n    ");
    for (int i = 0; i < t_count; i++) {
        printf("%c   ", terminals[i]);
    }
    printf("\n");

    for (int i = 0; i < t_count; i++) {
        printf("%c | ", terminals[i]);
        for (int j = 0; j < t_count; j++) {
            char c = precedence_table[i][j];
            printf("%c   ", c == ' ' ? '-' : c);
        }
        printf("\n");
    }
}

// Main
int main() {
    printf("Enter number of productions: ");
    scanf("%d", &n);
    getchar();  // consume newline

    printf("Enter productions (e.g., E=E+T):\n");
    for (int i = 0; i < n; i++) {
        fgets(productions[i], MAX, stdin);
        productions[i][strcspn(productions[i], "\n")] = '\0';  // Remove newline

        // Build non-terminal list
        add_unique(non_terminals, &nt_count, productions[i][0]);

        // Build terminal list
        for (int j = 2; j < strlen(productions[i]); j++) {
            if (is_terminal(productions[i][j]))
                add_unique(terminals, &t_count, productions[i][j]);
        }
    }

    compute_leading();
    compute_trailing();
    construct_table();
    print_table();

    return 0;
}
