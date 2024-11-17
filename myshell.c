#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <sys/types.h>

char temp_input[200][200];

void get_input(char *c){
    char *aux;
    aux = readline("$ ");
    strcpy(c, aux);
    free(aux);
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
    
    if(strcmp(temp_input[0], "ls") == 0){
        pid_t pid=fork();
        if(pid == 0){
            char *argv[]={"ls", NULL};
            execve("/bin/ls", argv, NULL);
            perror(NULL);
        }
        
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

int main()
{
    char input[100];

    while(strcmp(input, "exit")){
        show_path();
        get_input(input);
        split_input(input);
    }
    printf("Done.\n");
    return 0;
}