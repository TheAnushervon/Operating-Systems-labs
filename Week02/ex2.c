#include <stdio.h>
#include<string.h> 
int main() {
    char sentence[256];
    printf("Write a string\n");
    fgets(sentence, sizeof(sentence), stdin);
    size_t length = strlen(sentence);
    if (length > 0 && sentence[length - 1] == '\n') {
        sentence[length - 1] = '\0';
    }
    int dot = -1;

    for (int i = 0; i < length; i++) {
        if (sentence[i] == '.') { dot = i; break; }
    };
    if (dot != -1) {
        char sen[256] = "\"";

        int counter = 1;

        for (int i = dot - 1; i >= 0; i--, counter++) {
            sen[counter] = sentence[i];
        }
        sen[counter] = '"';
        printf("%s", sen);
    }
    else {

        char sen[256] = "\"";
       
        int counter = 1;

        for (int i = length-2; i >= 0; i--, counter++) {
            sen[counter] = sentence[i];
        }
        sen[counter] = '"';
        printf("%s", sen);
    }

}