#include <stdio.h>
#include <string.h>
#include <ctype.h> 
void count(){
char input[256]; 
    char el;
    printf("Enter a string:\n");
    scanf("%s", input);
    char text[256];
    int i;

    for (i = 0; i < strlen(input); i++) {
        if (input[i] == ',') {
            break;
        }
        text[i] = tolower(input[i]);
    }
    text[i] = '\0';

    el = tolower(input[strlen(input) - 1]);

    int element[256] = {0}; 

    for (i = 0; i < strlen(text); i++) {
        element[text[i]]++;
    }
    for (int i = 0 ; i < strlen(text); i++) {
    printf("%c:%d\n", text[i], element[text[i]]);}

}
int main() {
    count(); 
    return 0;
}
