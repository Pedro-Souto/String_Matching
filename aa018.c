#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define max(x,y) ((x) >= (y)) ? (x) : (y)

void algoSelector(char* selector, char* target, char* text);
void naiveAproach(char* target, char* text);
void KMP(char* target, char* text);
void KMP_Matcher(char* target, char* text, int size_text_n, int size_target_m);
int *KMP_Prefix(char* target, int size_target_m);
void BoyerMoore(char* target, char* text);
int *BoyerMoore_PreProcess_badChar(char* target, int size_target_m);
int *BoyerMoore_goodSuffix(char* target, int size_target_m);
void BoyerMoore_Matcher(char* target, char* text, int size_text_n, int size_target_m);
int checkerBM(char letra);

int main(int argc, char *argv[]) {

    size_t newSize = 2250;
    char* text = (char*)calloc(newSize, sizeof(char));
    char* line = (char*)calloc(newSize, sizeof(char));
    FILE* stream;

    size_t size = 2;
    char* comutador_text = "T ";
    char* comutador_naive = "N ";
    char* comutador_KMP = "K ";
    char* comutador_BM = "B ";

    stream = fopen(argv[1], "r");
    if (stream == NULL) {
        printf("ERRO!!\n");
        exit(EXIT_FAILURE);
    }

    while (fscanf(stream, " %[^\n]",line) == 1){

        size_t size_line = strlen(line);

        if (size_line > newSize) {
            newSize = 2*newSize;
            line = (char*)malloc(newSize * sizeof(char));
            fseek(stream, -size_line, SEEK_CUR);
        } else {
            if (0 == strncmp(&(line[0]),comutador_text,size)) {

                char auxiliar[newSize];
                memcpy(auxiliar, &line[size],(1+ strlen(line)-size));
                text = (char*)calloc(strlen(auxiliar), sizeof(char));
                strcpy(text,auxiliar);

            } else if (0 == strncmp(&(line[0]),comutador_naive,size)) {

                char target_NV[newSize];
                memcpy(target_NV, &line[size],(1+ strlen(line)-size));
                algoSelector(comutador_naive, target_NV, text);

            } else if (0 == strncmp(&(line[0]),comutador_KMP,size)) {

                char target_KMP[newSize];
                memcpy(target_KMP, &line[size],(1+ strlen(line)-size));
                algoSelector(comutador_KMP, target_KMP, text);

            } else if (0 == strncmp(&(line[0]),comutador_BM,size)) {

                char target_BM[newSize];
                memcpy(target_BM, &line[size],(1+ strlen(line)-size));
                algoSelector(comutador_BM, target_BM, text);
            }
        }
    }

    free(line);
    free(text);
    fclose(stream);
    return 0;
}


void algoSelector(char* selector, char* target, char* text){

    if (strcmp(selector,"N ") == 0) {
        naiveAproach(target, text);
    } else if (strcmp(selector,"K ") == 0) {
        KMP(target,text);
    } else if (strcmp(selector,"B ") == 0) {
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
    for (q = 1; q < size_target_m; q++)	{
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
    int *goodSuffix_auxiliar = malloc(sizeof(int)*(size_target_m));
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

    free(goodSuffix_auxiliar);
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
