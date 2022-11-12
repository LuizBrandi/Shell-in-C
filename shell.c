#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define DIR_TAM 300
#define TAM_BUFFER 64
#define TOKEN_DELIM " \n"

char * leLinha();
char ** separaTextoLinha(char * linha, int * posicao);
void removePuloLinha(char * str);
void lsh_cd(char **args);

int main(void){
    char * linha;
    char ** args;
    int contador;
    int argc;

    char diretorioAtual[DIR_TAM];
    char * path = "/bin/";
    char programPath[20];

    while(1){
        printf("terminal@luiz:~");
        getcwd(diretorioAtual, sizeof(diretorioAtual));
        printf("%s$ ", diretorioAtual);

        linha = leLinha();
        removePuloLinha(linha);

        if(strcmp(linha, "exit") == 0){
            break;
        } 

        args = separaTextoLinha(linha, &contador);
        argc = contador;

        for(int i = 0; i < argc; i++){
            printf("%s\n", args[i]);      
        }

        strcpy(programPath, path);
        strcat(programPath, args[0]);

        pid_t pid = fork();
        
        if(pid == 0){
            if(strcmp(args[0], "cd") == 0){
                lsh_cd(args);
            }else{
                execvp(programPath, args);
                perror("COMANDO INVALIDO!\n");
                _exit(0);
            }
        } else{
            wait(NULL);
        }
    }  
}

char * leLinha(){
    //criamos as variaveis que armazenarao a linha de texto escaneada e o tamanho do buffer
    char * linha = NULL;
    size_t tamanhoBuffer = 0;

    if(getline(&linha, &tamanhoBuffer, stdin) == -1){
        if(feof(stdin)){
            exit(EXIT_SUCCESS); //lemos um EOF
        }else{
            perror("ERRO");
            exit(EXIT_FAILURE);
        }
    }

    return linha;
}

char ** separaTextoLinha(char * linha, int * posicao){
    int tamanhoBuffer = TAM_BUFFER;
    *posicao = 0;

    char * token;
    char ** tokens = (char **) malloc(sizeof(char *) * tamanhoBuffer);

    //divide a primeira parte do texto da linha
    token = strtok(linha, TOKEN_DELIM);

    //divide o resto do texto da linha
    while(token){
        tokens[*posicao] = token;
        *posicao += 1;

        //aumenta o buffer se houver um estouro
        if(*posicao >= tamanhoBuffer){
            tamanhoBuffer += TAM_BUFFER;
            tokens = realloc(tokens, sizeof(char *) * tamanhoBuffer);
            if(!token){
                perror("erro de alocacao");
                exit(EXIT_FAILURE);
            }
        }
    
        token = strtok(NULL, TOKEN_DELIM);
    }

    tokens[*posicao] = NULL;

    return tokens;
}

void removePuloLinha(char * str){
    int tam = strlen(str);
    if(str[tam - 1] == '\n'){
        str[tam - 1] = '\0';
    }
    
}
void adiciona_cd(char **args){
  if (args[1] == NULL) {
    fprintf(stderr, "lsh: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("lsh");
    }
  }
}
