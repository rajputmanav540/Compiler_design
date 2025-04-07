#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 20

char grammar[MAX][MAX];
char symbols[MAX]; // combined array for terminals & non-terminals
int n, symCount = 0;

int isTerminal(char ch) {
    return !isupper(ch);
}

int findIndex(char ch) {
    for (int i = 0; i < symCount; i++) {
        if (symbols[i] == ch)
            return i;
    }
    return -1;
}

void addSymbol(char ch) {
    if (findIndex(ch) == -1) {
        symbols[symCount++] = ch;
    }
}

void generatePrecedenceTable(char expr[]) {
    char table[MAX][MAX];

    // Initialize table
    for (int i = 0; i < symCount; i++) {
        for (int j = 0; j < symCount; j++) {
            table[i][j] = '.';
        }
    }

    // Fill precedence based on expression (simplified logic)
    for (int i = 0; expr[i+1] != '\0'; i++) {
        if (isTerminal(expr[i]) && isTerminal(expr[i+1])) {
            int row = findIndex(expr[i]);
            int col = findIndex(expr[i+1]);
            if (row != -1 && col != -1) {
                if (expr[i] == expr[i+1]) table[row][col] = '.';
                else if (expr[i] > expr[i+1]) table[row][col] = '>';
                else table[row][col] = '<';
            }
        }
    }

    // Print table header
    printf("\nOperator Precedence Table:\n\t");
    for (int i = 0; i < symCount; i++) {
        if (isTerminal(symbols[i]))
            printf("%c\t", symbols[i]);
    }

    printf("\n");
    for (int i = 0; i < symCount; i++) {
        if (!isTerminal(symbols[i])) continue; // Only terminals
        printf("%c\t", symbols[i]);
        for (int j = 0; j < symCount; j++) {
            if (isTerminal(symbols[j]))
                printf("%c\t", table[i][j]);
        }
        printf("\n");
    }
}

int main() {
    char expr[MAX];

    printf("Enter number of grammar rules: ");
    scanf("%d", &n);

    printf("Enter the grammar rules (like S->aAb):\n");
    for (int i = 0; i < n; i++) {
        scanf("%s", grammar[i]);
        for (int j = 3; grammar[i][j] != '\0'; j++) {
            addSymbol(grammar[i][j]);
        }
    }

    printf("Enter the expression (like a+b-(c*d)): ");
    scanf("%s", expr);

    // Add terminals from expression
    for (int i = 0; expr[i] != '\0'; i++) {
        if (isTerminal(expr[i]) && expr[i] != '(' && expr[i] != ')') {
            addSymbol(expr[i]);
        }
    }

    generatePrecedenceTable(expr);

    return 0;
}
