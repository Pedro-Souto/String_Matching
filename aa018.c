#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAXSIZE 9048
#define max(x,y) ((x) >= (y)) ? (x) : (y)

void algoSelector(char selector, char* target, char* text);
void naiveAproach(char* target, char* text);
void KMP(char* target, char* text);
void KMP_Matcher(char* target, char* text, int size_text_n, int size_target_m);
int *KMP_Prefix(char* target, int size_target_m);
void BoyerMoore(char* target, char* text);
int *BoyerMoore_PreProcess_badChar(char* target, int size_target_m);
int *BoyerMoore_goodSuffix(char* target, int size_target_m);
void BoyerMoore_Matcher(char* target, char* text, int size_text_n, int size_target_m);
int checkerBM(char letra);
void processBlock();

char* doText(char* string, int *size){
    
  int used = 0;

  int freeSpace = (*size);
  char c;
  while( (c=getchar())!='\n'){
      if ( c== ' '){
        continue;
      }
      else if (freeSpace > 0){
          string[used]= c;
          freeSpace--;
          used++;
      }
      else{
          string = (char*)realloc(string,2*(*size));
          string[used] = c;
          used++;
          freeSpace = (*size)-1;
          *size = 2*(*size);
      }
  }
  string[used]='\0';
  freeSpace--;
  used++;
  while(used < ((*size)*0.25)){
    *size = (*size)/2;
    string = (char*) realloc(string, (*size));

  }
  return string;
}

int main(int argc, char *argv[]) {

    getchar();

    while(true){
        processBlock();
    }

    return EXIT_SUCCESS;
}

void processBlock(){

    getchar();

    char text[MAXSIZE];
    scanf("%s", text);
    getchar();

    while(true){

        char rule;
        scanf("%c", &rule);

        if(rule == 'T') break;
        if(rule == 'X') exit(0);

        char target[MAXSIZE];
        scanf("%s", target);

        algoSelector(rule, target, text);
        getchar();
    }
}

void algoSelector(char selector, char* target, char* text){

    if (selector=='N') {
        naiveAproach(target, text);
    } else if (selector=='K') {
        KMP(target,text);
    } else if (selector=='B') {
        BoyerMoore(target,text);
    }
}

void naiveAproach(char* target, char* text){

    int i = 0;
    int size_target = strlen(target);
    int size_text = strlen(text);

    while(i + size_target <= size_text){
        if (0 == strncmp(&(text[i]),target,size_target)){
            printf("%d ",i);
        }
        i++;
    }
    printf("\n");
}

void KMP(char* target, char* text){

    int size_target_m = strlen(target);
    int size_text_n = strlen(text);

    KMP_Matcher(target, text, size_text_n, size_target_m);
}

int *KMP_Prefix(char* target, int size_target_m){

    int q = 0,k = 0;
    int *prefix_pi = malloc(sizeof(int)*size_target_m);

    prefix_pi[q]=0;
    for (q = 1; q < size_target_m; q++) {
        while ( k > 0 && target[k] != target[q]) k = prefix_pi[k-1];
        if (target[k] == target[q]) k++;
        prefix_pi[q] = k;
    }
    return prefix_pi;
}

void KMP_Matcher(char* target, char* text, int size_text_n, int size_target_m){

    int q = 0, i = 0, cout = 0;
    int *prefix_pi = KMP_Prefix(target, size_target_m);

    while (i < size_text_n) {
        cout++;
        while (q > 0 && target[q] != text[i]) {
            q = prefix_pi[q-1];
            cout++;
        }
        if (target[q] == text[i]) {
            q++;
        }
        if (q == size_target_m) {
            printf("%d ", i - size_target_m +1);
            q = prefix_pi[q-1];
            if (i + 1 < size_text_n) cout++;
        }
        i++;
    }

    printf("\n%d \n", cout);
    free(prefix_pi);
}

void BoyerMoore(char* target, char* text){

    int size_target_m = strlen(target);
    int size_text_n = strlen(text);

    BoyerMoore_Matcher(target, text, size_text_n, size_target_m);
}

int *BoyerMoore_PreProcess_badChar(char* target, int size_target_m){
    int i, j;
    int n = 4;
    char alphabet[4] = {'A','C','G','T'};
    int *BadChar = malloc(sizeof(int)*(size_target_m));

    for (i = 0; i < n; i++){
        BadChar[i] = size_target_m;
        for (j = size_target_m-1; j >= 0; j--) {
            if (0 == strncmp(&alphabet[i],&target[j],1)){
                BadChar[i] += - j - 1;
                break;
            }
        }
    }
    return BadChar;
}

int *BoyerMoore_goodSuffix(char* target, int size_target_m){
    int g = 0, i = 0, j = 0, f = 0;
    int goodSuffix_auxiliar[size_target_m];
    int *move_goodSuffix = malloc(sizeof(int)*(size_target_m));

    goodSuffix_auxiliar[size_target_m-1] = size_target_m;
    g = size_target_m - 1;

    for(i = size_target_m - 2; i >= 0; i--){
        if(i > g && goodSuffix_auxiliar[i + size_target_m -1 - f] < i-g)
            goodSuffix_auxiliar[i] = goodSuffix_auxiliar[i + size_target_m - 1 - f];
        else {
            if(i < g)
                g = i;
            f = i;
            while( g >= 0 && target[g] == target[g + size_target_m - 1 -f]) --g;
            goodSuffix_auxiliar[i] = f - g;
        }
    }

    for (i = 0; i <size_target_m; ++i){
        move_goodSuffix[i] = size_target_m;
    }

    for (i =size_target_m - 1; i >= 0; --i){
        if (goodSuffix_auxiliar[i] == i + 1){
            for (j = 0; j <size_target_m - 1 - i; ++j){
                if (move_goodSuffix[j] == size_target_m)
                    move_goodSuffix[j] = size_target_m - 1 - i;
            }
        }
    }
    for (i = 0; i <=size_target_m - 2; ++i){
        move_goodSuffix[size_target_m - 1 - goodSuffix_auxiliar[i]] = size_target_m - 1 - i;
    }

    return move_goodSuffix;
}

void BoyerMoore_Matcher(char* target, char* text, int size_text_n, int size_target_m){
    int j = 0, i = 0, cout = 0;
    int *move_badChar = BoyerMoore_PreProcess_badChar(target, size_target_m);
    int *move_goodSuffix = BoyerMoore_goodSuffix(target, size_target_m);

    while(j <= size_text_n - size_target_m){
        for (i = size_target_m - 1; i >= 0 && target[i] == text[i+j]; --i) cout++;
        if (i < 0) {
            printf("%d ", j);
            j += move_goodSuffix[0];
        } else{
            cout++;
            if(i+j < size_text_n){
                j += max(move_goodSuffix[i], move_badChar[checkerBM(text[i+j])] - size_target_m + 1 + i);
            }
        }
    }

    printf("\n%d \n",cout);
    free(move_goodSuffix);
    free(move_badChar);
}

int checkerBM(char letra){
    int i;
    char alphabet[4] = {'A','C','G','T'};
    for (i = 0; i < 4; i++) {
        if(letra == alphabet[i]) {
            return i;
        }
    }
    return -1;
}
