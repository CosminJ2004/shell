// gcc myshell.c -o myshell -lreadline
// ./myshell
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <sys/types.h>


void clear() 
{
    printf("\033[H\033[J");
}

char temp_input[200][200];

void get_input(char *c){
    char *aux;
    aux = readline("$ ");
    strcpy(c, aux);
    free(aux);
}

void comanda_ls(){
    pid_t pid=fork();
    if(pid == 0){
        char *argv[]={"ls", NULL};
        execve("/bin/ls", argv, NULL);
        perror(NULL);
    }
    else
    wait();
}

void comanda_echo(int cnt){
    for(int i = 1; i < cnt; i++){
        for(int c = 0; c < strlen(temp_input[i]); c++)
            if (temp_input[i][c] != '"')
                printf("%c", temp_input[i][c]);
        printf(" ");
    }
        

    printf("\n");
}

void comanda_necunoscuta(char *input){
    printf("%s: eroare, comanda invalida\n", input);
}

void split_input(char *c){
    char *token = strtok(c, " ");
    int cnt = 0;
    while (token != NULL)
    {
        strcpy(temp_input[cnt], token);
        cnt++;
        token = strtok(NULL, " ");
    }

    // for (int i=0; i<cnt; i++)
    //     printf("%s\n", temp_input[i]);
    
    if(strcmp(temp_input[0], "ls") == 0)
        comanda_ls();
    else if(strcmp(temp_input[0], "echo") == 0)
         comanda_echo(cnt);
    else{
        comanda_necunoscuta(c);
    }    

}

void show_path(){
    char cwd[1000], aux[1000] = "AguSiCosminShell:";
    if(getcwd(cwd, sizeof(cwd)) == NULL)
        perror("Error getcwd().\n");
    else
    {
        strcat(aux, cwd);
        strcat(aux, "\n");
        printf("%s", aux);
    }
}

void intro()
{
    clear(); 
    printf("Welcome to our Shell!\n    Agusoaei Alexandru, Jalba Cosmin @2024\n");
    sleep(3); 
    clear();
}

//De implementat: echo, pwd, cd, conditionale(&&, ||), pipe(|), redirect(>, <)
int main()
{
    char input[100];
    intro();
    while(1){
        show_path();
        get_input(input);
        split_input(input);
    }
    printf("Done.\n");
    return 0;
}   