#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

void accentRemover(char *str);
void textFormater(char *str);

int main(void) {  
	SetConsoleOutputCP(1252);        
	unsigned char c;

	const char *title[] = {
		"_______  _____   ______ _______ _______ _______ _______ ______   _____   ______ ",
		"|______ |     | |_____/ |  |  | |_____|    |    |_____| |     \\ |     | |_____/",
		"|       |_____| |    \\_ |  |  | |     |    |    |     | |_____/ |_____| |    \\_ "
	}; 

	printf("\n\n");

    int n = sizeof(title)/sizeof(title[0]);
    for (int i = 0; i < n; i++) {
        printf("%s\n", title[i]);
    }
	
	printf("\n\n");
 
	while (1) {
		printf("\tNecessario que o texto a ser filtrado esteja usando encoding ANSI ou CP1252!!!\n\n");
		printf("\tlista:\n\t\t(a)-Filtrar acentos\n\t\t(f)-Fast Format\n\t\t(i)-Info\n\t\t(s)-Sair\n\n");
		printf("\tAS ACOES IRAO SOBREESCREVER O ULTIMO DADO COPIADO!!!\n\n		");

		char line[32];  
		if (fgets(line, sizeof(line), stdin) != NULL) {
			c = line[0];  // primeiro caractere
		}
		
		if (toupper(c) == 'S') {
            break;
        } else if (toupper(c) == 'I') {
			printf("\n\tINFORMACOES\n\t\tFiltrar acentos: torna caracteres acentuados em suas versoes sem acentos;\n\t\tFast Format: deleta[\'], deleta[\"], deleta[;], deleta[,], deleta[NULL], [|]subistituido por[,] \n\n\n");
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
						printf("Erro de memďż˝ria\n");
						GlobalUnlock(hData);
						CloseClipboard();
						return 1;
					}

					strcpy(copy, text);         
					GlobalUnlock(hData);         
					
					if (toupper(c) == 'A') {
						accentRemover(copy);  
					} else if (toupper(c) == 'F') {
						textFormater(copy);
					}

					HANDLE hMem = GlobalAlloc(GMEM_MOVEABLE, len);
					memcpy(GlobalLock(hMem), copy, len);
					GlobalUnlock(hMem);

					EmptyClipboard();
					SetClipboardData(CF_TEXT, hMem);

					free(copy); 

					printf("\t\t\t\tSuccess! \n\n");	
					printf("======================================================================== \n\n");	
				}
			}
			CloseClipboard();
		}	
    }

    return 0;
}

void textFormater(char *str) {
    int i = 0, j = 0;  // i = read index, j = write index
    int len = strlen(str);

    while (str[i] != '\0') {
        char c = toupper(str[i]);

        // Skip unwanted characters
        if (c == '\'' || c == '\"' || c == ',' || c == ';') {
            i++;
            continue;
        }

        // Replace | with ,
        if (c == '|') {
            str[j++] = ',';
            i++;
            continue;
        }

        // Skip "NULL"
        if (c == 'N' && strncmp(&str[i], "NULL", 4) == 0) {
            i += 4;
            continue;
        }

        // Copy valid character
        str[j++] = c;
        i++;
    }

    str[j] = '\0';
}

void accentRemover(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        unsigned char c = str[i];

        // Lowercase
        if (c >= 0xE0 && c <= 0xE5) {      // à á â ã ä å
            str[i] = 'a';
        } else if (c >= 0xE8 && c <= 0xEB) { // è é ê ë
            str[i] = 'e';
        } else if (c >= 0xEC && c <= 0xEF) { // ì í î ï
            str[i] = 'i';
        } else if (c >= 0xF2 && c <= 0xF6) { // ò ó ô õ ö
            str[i] = 'o';
        } else if (c >= 0xF9 && c <= 0xFC) { // ù ú û ü
            str[i] = 'u';
        } 
        // Uppercase
        else if (c >= 0xC0 && c <= 0xC5) { // À Á Â Ã Ä Å
            str[i] = 'A';
        } else if (c >= 0xC8 && c <= 0xCB) { // È É Ê Ë
            str[i] = 'E';
        } else if (c >= 0xCC && c <= 0xCF) { // Ì Í Î Ï
            str[i] = 'I';
        } else if (c >= 0xD2 && c <= 0xD6) { // Ò Ó Ô Õ Ö
            str[i] = 'O';
        } else if (c >= 0xD9 && c <= 0xDC) { // Ù Ú Û Ü
            str[i] = 'U';
        }
        // Special characters
        else if (c == 0xC7) { // Ç
            str[i] = 'C';
        } else if (c == 0xE7) { // ç
            str[i] = 'c';
        } else if (c == 0xD1) { // ñ
            str[i] = 'N';
        } else if (c == 0xF1) { // ñ
            str[i] = 'n';
        }
    }
}

//gcc remover.c -o r.exe -luser32 -lkernel32
//x86_64-w64-mingw32-gcc remover.c -o r.exe -luser32 -lkernel32
//chcp 28592