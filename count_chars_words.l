%{
#include <stdio.h>
int char_count = 0, word_count = 0, space_count = 0, line_count = 0;
%}

%%
[^\n\t ]+   { word_count++; char_count += yyleng; }  // Count words and characters
[ ]         { space_count++; char_count++; }        // Count spaces
\n          { line_count++; char_count++; }         // Count newlines
.           { char_count++; }                       // Count any other characters
%%

int main() {
    printf("Reading input file...\n");
    yylex();
    printf("Number of characters: %d\n", char_count);
    printf("Number of words: %d\n", word_count);
    printf("Number of spaces: %d\n", space_count);
    printf("Number of lines: %d\n", line_count);
    return 0;
}

int yywrap() {
    return 1;
}
