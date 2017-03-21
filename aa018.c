#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define min(a, b) ((a > b) ? b : a)
#define max(a, b) ((a < b) ? b : a)

void algoSelector(char* selector, char* target, char* text);
void naiveAproach(char* target, char* text);
void KMP(char* target, char* text);
void KMP_Matcher(char* target, char* text, int size_text_n, int size_target_m);
int *KMP_Prefix(char* target, int size_target_m);
void BoyerMoore(char* target, char* text);
int BoyerMoore_badChar(char* target, int size_target_m, char alphabet, int position);
int *BoyerMoore_goodSuffix(char* target, int size_target_m);
void BoyerMoore_Matcher(char* target, char* text, int size_text_n, int size_target_m);

int main(int argc, char *argv[]) {

	char* target = NULL;
	char* text = NULL;
	char* auxiliar = NULL;
	FILE *stream;
	char line[10240];

	stream = stdin;
	if (stream == NULL) {
		printf("ERRO!!\n");
	    exit(EXIT_FAILURE);
	}
	while ((fgets(line, sizeof line, stream)) != NULL) {

	    int i;

	    for (i = 0; i < strlen(line); i++) {

	    	int size = 2;
	    	char* comutador_text = "T ";
	    	char* comutador_naive = "N ";
	    	char* comutador_KMP = "K ";
	    	char* comutador_BM = "B ";
	    	char* comutador_exit = "X";
			
	    	if (0 == strncmp(&(line[i]),comutador_text,size)) {

	    		auxiliar = memmove(&line[0], &line[size],(1+ strlen(line)-size));
	    		auxiliar[strlen(auxiliar)-1] = 0;
	    		text = (char*)calloc(strlen(auxiliar), sizeof(char));
	    		strcpy(text, auxiliar);

	    	} else if (0 == strncmp(&(line[i]),comutador_naive,size)) {

	    		auxiliar = memmove(&line[0], &line[size],(1+ strlen(line)-size));
	    		auxiliar[strlen(auxiliar)-1] = 0;
	    		target = (char*)calloc(strlen(auxiliar), sizeof(char));
	    		strcpy(target, auxiliar);
	    		algoSelector(comutador_naive, target, text);

	    	} else if (0 == strncmp(&(line[i]),comutador_KMP,size)) {

	    		auxiliar = memmove(&line[0], &line[size],(1+ strlen(line)-size));
	    		auxiliar[strlen(auxiliar)-1] = 0;
	    		target = (char*)calloc(strlen(auxiliar), sizeof(char));
	    		strcpy(target, auxiliar);
	    		algoSelector(comutador_KMP, target, text);

	    	} else if (0 == strncmp(&(line[i]),comutador_BM,size)) {

	    		auxiliar = memmove(&line[0], &line[size],(1+ strlen(line)-size));
	    		auxiliar[strlen(auxiliar)-1] = 0;
	    		target = (char*)calloc(strlen(auxiliar), sizeof(char));
	    		strcpy(target, auxiliar);
	    		algoSelector(comutador_BM, target, text);

	    	} else {
	    		algoSelector(comutador_exit, target, text);
	    	}
		}
	}

	free(text);
	free(target);
	fclose(stream);
	exit(EXIT_SUCCESS);
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

	int *prefix_pi = malloc(sizeof(int)*size_target_m);
	int k = 0, q;
	prefix_pi[0] = k;

	for (q = 2; q <= size_target_m; q++) {
		while(k > 0 && target[k] != target[q-1])
			k = prefix_pi[k];
		if (target[k] == target[q-1])
			k++;
		prefix_pi[q-1] = k;
	}
	return prefix_pi;
}

void KMP_Matcher(char* target, char* text, int size_text_n, int size_target_m){

	int q = 0, i = 0, cout = 0;;
	int *prefix_pi = KMP_Prefix(target, size_target_m);

	while (i < size_text_n)	{
		if (target[q] == text[i]){
			if (q == size_target_m - 1){
				printf("%d ", i - q);
				i = i - q;
				q = 0;
			}
			else
				q++;
			i++;
			cout++;
		}else{
			i += prefix_pi[q] + 1 - q;
			q = 0;
		}
	}
	printf("\n%d \n", cout);
	free(prefix_pi);
}

void BoyerMoore(char* target, char* text){

	int size_target_m = strlen(target);
	int size_text_n = strlen(text);

	BoyerMoore_Matcher(target, text, size_text_n, size_target_m);
}

int BoyerMoore_badChar(char* target, int size_target_m, char alphabet, int position){
	
	int i;
	for (i = position; i > -1; i--) {
		if (0 == strncmp(&(target[i]),&alphabet,1))
			return  size_target_m - i - 1;
	}
	return size_target_m;
}

int *BoyerMoore_goodSuffix(char* target, int size_target_m){
	int j=0, i=0, k=0;
	int *goddSuffix = malloc(sizeof(int)*(size_target_m));

	for (i = size_target_m - 1; i > -1; i--) {
		k++;
		goddSuffix[i] = size_target_m;
		for(j = i -1; j > -1; j--){
			if (0 == strncmp(&(target[j]),&(target[i]),k)){
				if (target[j-1] != target[i-1] && j != 0) {
					goddSuffix[i] -= j;
					break;
				}				
			}
		}
	}

	return goddSuffix;
}

void BoyerMoore_Matcher(char* target, char* text, int size_text_n, int size_target_m){

	int cout = 0;
	int i = size_target_m - 1;
	int j = i;
	int *move_goodSuffix = BoyerMoore_goodSuffix(target, size_target_m);

	while (i < size_text_n){
		++cout;
		if (target[j] == text[i]){
			if (j == 0)	{
				printf("%d ", i);
				i += size_target_m;
				j = size_target_m - 1;
			} else{
				i--;
				j--;
			}
		}else{
			i += min(BoyerMoore_badChar(target,size_target_m,text[i],j),move_goodSuffix[j]);
			j = size_target_m - 1;
		}
	}
	printf("\n%d \n",cout);
	free(move_goodSuffix);
}
