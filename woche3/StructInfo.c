#include <stdio.h>
#include <stdlib.h>

//{ STRUCTS
typedef struct sentence{
    int wordCount;
    char **words;
} Sentence;

typedef struct text{
    int sentenceCount;
    Sentence **sentences;
} Text;
//}

//{ WORD FUNCTIONS
char *createWord(){
    char *word = malloc(sizeof(char));
    int lenght = 0;

    while(1){
        char x;

        scanf("%c", &x);
        if(x == 32 || x == 10){ break; }

        word = realloc(word, ++lenght*sizeof(char));
        if(word == NULL){ return NULL; }
        word[lenght-1] = x;
    }

    return word;
}

int getWordLen(char *str){
    int i=0;
    while(str[i] != '\0'){
        i++;
    }

    return i;
}
//}

//{ SENTENCE FUNTIONS
Sentence *createSentence(){

    Sentence *s = malloc(sizeof(Sentence));
    // printf("%d\n", sizeof(s));

    s->words = malloc(1);
    s->wordCount = 0;


    printf("Bitte geben sie ein Satz ein der mit '.' oder '?' aufhört: \n");

    while(1){
        char *str = createWord();
        s->words = realloc(s->words, ++(s->wordCount) * sizeof(char*));

        // printf("%d\n", sizeof(*s));
        if(s->words != NULL) {

            // warum stimmt das? warum nicht &str ??
            s->words[(s->wordCount)-1] = str;

            int lastChar = getWordLen(str);
            if(str[lastChar-1] == '.' || str[lastChar-1] == '?') { break; }
        }
    }

    printf("\n");
    return s;
}

void delSentence(Sentence *s){

    for(int i=0; i< s->wordCount; i++){
        free(s->words[i]); s->words[i] = NULL;
    }

    free(s->words); s->words = NULL;
    // wie kann ich die Struktur selber löschen? Passiert das automatisch wenn es nicht mehr referenziert wird?
    free(s); s = NULL;
}


void printSentence(Sentence *s, int printAdress){

    if(printAdress){
        for(int i=0; i< s->wordCount; i++){
            printf("%s\n%p\n", s->words[i], s->words[i]);
        }
    }

    else{
        for(int i=0; i< s->wordCount; i++){
            printf("%s ", s->words[i]);
        }
    }

    printf("\n");
}
//}

//{ TEXT FUNCTIONS
Text *createText(int sentCount){

    Text *t = malloc(sizeof(Text));
    t->sentenceCount = sentCount;

    t->sentences = malloc(t->sentenceCount * 8);

    for(int i=0; i< t->sentenceCount; i++){
        // Das Problem ist das die Sentence Objekte zerstört werden nach dem Schleifendurchlauf oder?
        // Es gibt dann Pointer auf nichts

        Sentence *newS = createSentence();
        // t->sentences + i = &newSentance;
        t->sentences[i] = newS;
    }

    return t;
}

void printText(Text *t){
    for(int i=0; i< t->sentenceCount ; i++){
        printSentence(t->sentences[i], 0);
    }
}


void delText(Text *t){
    for(int i=0; i< t->sentenceCount ; i++){
        delSentence(t->sentences[i]);
    }

    free(t); t = NULL;
}
//}

void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}

int main()
{
//    Sentence *s2 = createSentence();
//    printSentence(s2, 0);
//    delSentence(s2);

    Text *t1 = createText(1);
    printText(t1);
    delText(t1);

    // printBits(sizeof(*t1), t1);

    return 0;
}


