%{
#include <stdio.h>
int vowels = 0, consonants = 0;
%}
%%
[aAeEiIoOuU] { vowels++; }
[a-zA-Z] { consonants++; }
%%
int main() {
    printf("Enter a string: ");
    yylex();
    printf("Vowels: %d, Consonants: %d\n", vowels, consonants);
    return 0;
}
int yywrap() { 
    return 1; 
    }