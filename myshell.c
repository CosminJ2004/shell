// gcc myshell.c -o myshell -lreadline
// ./myshell
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdbool.h>

char *comenzi_simple = "ls cd echo clear exit mkdir rmdir cp mv";

void clear() 
{
    printf("\033[H\033[J");
}


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

void comanda_pwd(){
    pid_t pid=fork();
    if(pid == 0){
        char *argv[]={"pwd", NULL};
        execve("/bin/pwd", argv, NULL);
        perror(NULL);
    }
    else
        wait();
}

void comanda_cd();
void comanda_mkdir();
void comanda_rmdir();
void comanda_cp();
void comanda_mv();


void comanda_necunoscuta(char *input){
    printf("%s: eroare, comanda invalida\n", input);
}

void comenzi_simple(int cnt){
    if(strcmp(temp_input[0], "ls") == 0)
        comanda_ls();
    else if(strcmp(temp_input[0], "echo") == 0)
        comanda_echo(cnt);
    else if(strcmp(temp_input[0], "pwd") == 0)
        comanda_pwd();
    else if(strcmp(temp_input[0], "clear") == 0)
        clear();
    else if(strcmp(temp_input[0], "cd") == 0)
        comanda_cd();
    else if(strcmp(temp_input[0], "mkdir") == 0)
        comanda_mkdir();
    else if(strcmp(temp_input[0], "rmdir") == 0)
        comanda_rmdir();
    else if(strcmp(temp_input[0], "cp") == 0)
        comanda_cp();
    else if(strcmp(temp_input[0], "mv") == 0)
        comanda_mv();
    else if(strcmp(temp_input[0], "exit") == 0);

}

void comenzi_redirect();

void comenzi_pipe();

void split_input(char *c){
    char *token = strtok(c, " ");
    char *linie_comanda; // Sa avem linia de comanda intreaga
    strcpy(linie_comanda, c);
    int cnt = 0;
    while (token != NULL)
    {
        strcpy(temp_input[cnt], token);            
        cnt++;
        token = strtok(NULL, " ");
    }
    
    if(strstr(comenzi_simple, temp_input[0]))
        comenzi_simple(cnt);
    else if(strstr(linie_comanda, ">") || strstr(linie_comanda, "<"))
        comenzi_redirect();
    else if(strstr(linie_comanda, "|"))
        comenzi_pipe();
    else if(strstr(linie_comanda, "&&") || strstr(linie_comanda, "||"))
        comenzi_logice();
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

//De implementat: cd, conditionale(&&, ||), pipe(|), redirect(>, <)
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
