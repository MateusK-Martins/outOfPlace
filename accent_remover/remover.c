#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

#define LENGTH 84

void accentRemover(const char *table, char *str);

int main(void) {
	if (!SetConsoleCP(28592)) {
        printf("Erro ao mudar input code page.\n");
    }
    if (!SetConsoleOutputCP(28592)) {
        printf("Erro ao mudar output code page.\n");
    }
	
	const char *key = "N4X";              
	
	const char accent[] = "������������������������������������������������ ";
	const char clean[]  = "aeiouaeiouaeiouaoaeioucnAEIOUAEIOUAEIOUAOAEIOUCN ";

    char table[256] = {0};

    for (int i = 0; i < LENGTH; i++) {
        table[(unsigned char)accent[i]] = clean[i];
    }
	
	unsigned char c;
	
	const char *title[] = {
		"____ ____ ____ ____ _  _ ___       ____ ____ _  _ ____ _  _ ____ ____",
		"|__| |    |    |___ |\\ |  |  __ __ |__/ |___ |\\/| |  | |  | |___ |__/", 
		"|  | |___ |___ |___ | \\|  |        |  \\ |___ |  | |__|  \\/  |___ |  \\"
	};

	printf("\n\n");

    int n = sizeof(title)/sizeof(title[0]);
    for (int i = 0; i < n; i++) {
        printf("%s\n", title[i]);
    }
	
	printf("\n\n");
 
	while (1) {
		printf("Escreva \"%s\" e \"E\" para sair. \n\n", key);
		printf("ESTA A�?O IR� DELETAR O �LTIMO DADO COPIADO!!!\n\n		");

		char line[32];  
		if (fgets(line, sizeof(line), stdin) != NULL) {
			c = line[0];  // primeiro caractere
		}
		
		line[strcspn(line, "\n")] = '\0';
		
		if (strcasecmp(line, "E") == 0) {
            break;
        } else if (strcasecmp(line, key) == 0) {
            printf("\n Senha correta, continuando...\n");
        } else {
            printf("\n Senha incorreta!\n");
			continue;
        }
		
		printf("\n");
		
		if (OpenClipboard(NULL)) {
			HANDLE hData = GetClipboardData(CF_TEXT);
			if (hData != NULL) {
				char *text = (char*)GlobalLock(hData);
				if (text != NULL) {
					size_t len = strlen(text) + 1;
					char *copy = (char*)malloc(len);
					if (!copy) {
						printf("Erro de mem�ria\n");
						GlobalUnlock(hData);
						CloseClipboard();
						return 1;
					}

					strcpy(copy, text);         
					GlobalUnlock(hData);         

					accentRemover(table, copy);  

					HANDLE hMem = GlobalAlloc(GMEM_MOVEABLE, len);
					memcpy(GlobalLock(hMem), copy, len);
					GlobalUnlock(hMem);

					EmptyClipboard();
					SetClipboardData(CF_TEXT, hMem);

					free(copy); 

					printf(" Success! \n\n");	
					printf(" <============================ ============================> \n\n");	
				}
			}
			CloseClipboard();
		}	
    }

    return 0;
}

void accentRemover(const char *table, char *str){
	for (int i = 0; str[i] != '\0'; i++) {
		unsigned char c = str[i];
        if (table[c] != 0) {
            str[i] = table[c];
        }
	}
}

//gcc remover.c -o r.exe -luser32 -lkernel32
//x86_64-w64-mingw32-gcc remover.c -o r.exe -luser32 -lkernel32
//chcp 28592