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
	
	const char accent[] = "áàâãäåÁÀÂÃÄÅéèêëÉÈÊËíìîïÍÌÎÏóòôõöÓÒÔÕÖúùûüÚÙÛÜçÇñÑ";
	const char clean[]  = "aaaaaaAAAAAAeeeeEEEEiiiiIIIIoooooOOOOOuuuuUUUUcCnN";

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
        printf("\033[32m %s\n \033[0m", title[i]);
    }
	
	printf("\n\n");
 
	while (1) {
		printf("\033[36m Escreva \"%s\" e \"E\" para sair. \033[0m\n\n", key);
		printf("\033[31m ESTA AÇAO IRÁ DELETAR O ÚLTIMO DADO COPIADO!!!\033[0m\n\n		");

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
						printf("Erro de memória\n");
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
				}
			}
			CloseClipboard();
		}
		printf("\033[32m Success! \033[0m \n\n");	
		printf("\033[34m <============================ ============================> \033[0m \n\n");		
    }

    return 0;
}

void accentRemover(const char *table, char *str){
	for (int i = 0; str[i] != '\0'; i++) {
		printf(" %d->", c);
        if (table[c] != 0) {
            str[i] = table[c];
        }
	}
}

//gcc remover.c -o r.exe -luser32 -lkernel32
//chcp 28592