// gcc myshell.c -o myshell -lreadline
// ./myshell
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h> //istoric comenzi
#include <stdlib.h>
#include <sys/types.h>
#include <stdbool.h>
#include <unistd.h> // STDOUT_FILENO
#include <fcntl.h>  // O_RDONLY, O_WRONLY, O_CREAT, O_TRUNC
#include <sys/stat.h>
#include <sys/wait.h> // wait() din fork

// -de colorat la ls
// -corectat mv
// -de scris in readme exemple despre comenzi logice false true, pipe

char *sir_comenzi = "ls cd echo clear exit mkdir rmdir cp mv pwd grep cat touch nano ./";

void clear() 
{
    printf("\033[H\033[J");
}

char temp_input[200][200];

void get_input(char *c){
    char *aux;
    aux = readline("$ ");
    strcpy(c, aux);
    
    add_history(aux);
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
        wait(NULL);
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
        wait(NULL);
}

void comanda_cd(int cnt){
    if (cnt != 2) {
        printf("Eroare: Trebuie sa specifici calea directorului.\n");
        return;
    }
    
    int result = chdir(temp_input[1]);
    if (result == 0) {
        printf("Director schimbat la: %s\n", temp_input[1]);
    } else {
        perror("Eroare la schimbarea directorului");
    }
}
void comanda_mkdir(int cnt){
    if (cnt != 2) {
        printf("Eroare: Trebuie sa specifici numele directorului.\n");
        return;
    }
    
    int result = mkdir(temp_input[1], 0755); // 0755 este permisiunea standard pentru directoare
    if (result == 0) {
        printf("Director creat: %s\n", temp_input[1]);
    } else {
        perror("Eroare la crearea directorului");
    }
}
void comanda_rmdir(int cnt){
    if (cnt != 2) {
            printf("Eroare: Trebuie sa specifici directorul pe care vrei sa-l stergi.\n");
            return;
        }

        int result = rmdir(temp_input[1]);
        if (result == 0) {
            printf("Directorul a fost sters cu succes!\n");
        } else {
            perror("Eroare la stergerea directorului");
        }
}
void comanda_cp(int cnt){
    if (cnt != 3) {
        printf("Eroare: Trebuie sa specifici fisierul sursa si destinatia.\n");
        return;
    }

    // Deschide fisierul sursa pentru citire
    int src_fd = open(temp_input[1], O_RDONLY);
    if (src_fd == -1) {
        perror("Eroare la deschiderea fisierului sursa");
        return;
    }

    // Deschide fisierul destinatie pentru scriere
    int dest_fd = open(temp_input[2], O_WRONLY | O_CREAT | O_TRUNC, 200);
    if (dest_fd == -1) {
        perror("Eroare la deschiderea fisierului destinatie");
        close(src_fd);
        return;
    }
    else{
        struct stat sb;
        stat(temp_input[1], &sb);

        int size = sb.st_size;
        char buffer[size];

        read(src_fd, buffer, size);
        write(dest_fd, buffer, size);

        printf("Fisierul a fost copiat cu succes!\n");
    }    

    close(src_fd);
    close(dest_fd);
}
void comanda_mv(int cnt){
    if (cnt != 3) {
        printf("Eroare: Trebuie să specifici fisierul sursa si destinatia.\n");
        return;
    }

    int result = rename(temp_input[1], temp_input[2]);
    if (result == 0) {
        printf("Fisierul a fost mutat cu succes!\n");
    } else {
        perror("Eroare la mutarea fisierului");
    }
}
void comanda_exit(){
    printf("Iesire din shell...\n");
    exit(0);
}

void comanda_rm(int cnt){
    if (cnt < 2) {
        printf("Utilizare incorecta rm\n");
        return;
    }

    pid_t pid = fork();
    if (pid == 0) {
        char *argv[cnt + 1];
        argv[0] = "/bin/rm";
        for (int i = 1; i < cnt; i++) {
            argv[i] = temp_input[i];
        }
        argv[cnt] = NULL;

        execve("/bin/rm", argv, NULL);
        perror("Eroare la executia comenzii rm");
    } else {
        wait(NULL);
    }
}
void comanda_grep(int cnt){
    if (cnt < 3){
        printf("Utilizare incorecta grep\n");
        return;
    }

    pid_t pid = fork();
    if (pid == 0) {
        char *argv[cnt + 1];
        argv[0] = "/bin/grep";
        argv[1] = temp_input[1];
        for (int i = 2; i < cnt; i++) {
            argv[i] = temp_input[i];
        }
        argv[cnt] = NULL;

        execve("/bin/grep", argv, NULL);
        perror("Eroare la executia comenzii grep");
    } else {
        wait(NULL);
    }
}

void comanda_touch(int cnt) {
    if (cnt < 2) {
        printf("Utilizare gresita touch\n");
        return;
    }

    pid_t pid = fork();
    if (pid == 0) {
        char *argv[cnt + 1];
        argv[0] = "/bin/touch";
        for (int i = 1; i < cnt; i++) {
            argv[i] = temp_input[i];
        }
        argv[cnt] = NULL;

        execve("/bin/touch", argv, NULL);
        perror("Eroare la executia comenzii touch");
        exit(1);
    } else if (pid > 0) {
        wait(NULL);
    }
}

void comanda_cat(int cnt) {
    if (cnt < 2) {
        printf("Utilizare gresita cat\n");
        return;
    }

    pid_t pid = fork();
    if (pid == 0) {

        char *argv[cnt + 1];
        argv[0] = "/bin/cat";
        for (int i = 1; i < cnt; i++) {
            argv[i] = temp_input[i];
        }
        argv[cnt] = NULL;

        if (execve("/bin/cat", argv, NULL) == -1) {
            perror("Eroare la executia comenzii cat");
            exit(1);
        }
    } else {
        wait(NULL);
    }
}

void comanda_nano(int cnt){
    if (cnt < 2) {
        printf("Utilizare incorecta nano\n");
        return;
    }

    pid_t pid = fork();
    if (pid == 0) {
        
        char *argv[cnt + 1];
        argv[0] = "/bin/nano";
        for (int i = 1; i < cnt; i++) {
            argv[i] = temp_input[i];
        }
        argv[cnt] = NULL;

        execve("/bin/nano", argv, NULL);
        perror("Eroare nano");
        exit(1);
    } else if (pid > 0){
        wait(NULL);
    } else{
        perror("Eroare la fork");
    }
}

void comanda_exec(int cnt) {

    pid_t pid = fork();
    if (pid == 0){
        char *argv[] = {"/bin/sh", "-c", temp_input[0], NULL};
        
        execve("/bin/sh", argv, NULL);
        perror("Eroare la exec ./");
        exit(1);
    } else if (pid > 0) {
        wait(NULL);
    } else {
        perror("Eroare la fork");
    }
}

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
        comanda_cd(cnt);
    else if(strcmp(temp_input[0], "mkdir") == 0)
        comanda_mkdir(cnt);
    else if(strcmp(temp_input[0], "rmdir") == 0)
        comanda_rmdir(cnt);
    else if(strcmp(temp_input[0], "cp") == 0)
        comanda_cp(cnt);
    else if(strcmp(temp_input[0], "mv") == 0)
        comanda_mv(cnt);
    else if(strcmp(temp_input[0], "rm") == 0)
        comanda_rm(cnt);
    else if(strcmp(temp_input[0], "grep") == 0)
        comanda_grep(cnt);
    else if(strcmp(temp_input[0], "exit") == 0)
        comanda_exit();
    else if(strcmp(temp_input[0], "cat") == 0)
        comanda_cat(cnt);
    else if(strcmp(temp_input[0], "touch") == 0)
        comanda_touch(cnt);  
    else if(strcmp(temp_input[0], "nano") == 0)
        comanda_nano(cnt);
    else 
        comanda_exec(cnt);
    
}

int procesare_comanda(char *c);

char *trim_spaces(char *str) {
    while (*str == ' ') str++;
    
    char *end = str + strlen(str) - 1;
    while (end > str && *end == ' ') {
        *end = '\0';
        end--;
    }
    return str;
}

void comenzi_redirect(char *linie_comanda) {
    char linie_comanda_2[100];
    strcpy(linie_comanda_2, linie_comanda);
    char *token;
    char *sep = " \t\n";
    char *aux[10];
    char ofile[10];
    char comanda[100];
    int i = 0;
    
    // Pozitia primului ">" de la dreapta la stanga
    int pozitie = -1;
    for (int i = strlen(linie_comanda_2) - 1; i >= 0; i--) {
        if (linie_comanda_2[i] == '>') {
            pozitie = i; 
            break;
        } 
    }

    for (int i = 0; i <= pozitie - 1; i++) {
        comanda[i] = linie_comanda_2[i];
    }
    comanda[pozitie] = '\0';

    int j = 0;
    for (int i = pozitie + 1; i <= strlen(linie_comanda_2) - 1; i++) {
        ofile[j++] = linie_comanda_2[i];
    }
    ofile[j] = '\0';

    strcpy(ofile,trim_spaces(ofile));
    strcpy(comanda,trim_spaces(comanda));
   
    int n=i;
    pid_t pid = fork();  
        if (pid == 0){
            // Deschide fisierul pentru redirect
            int fd = open(ofile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                perror("Eroare la deschiderea fisierului");
                exit(1);
            }

            // Redirectioneaza stdout in fisier
            if (dup2(fd, STDOUT_FILENO) == -1) {
                perror("Eroare dup2");
                close(fd);
                exit(1);
            }
            close(fd);

            char *args[10] = {0};
            int i = 0;
            char *token = strtok(comanda, " ");
            while (token != NULL) {
                args[i++] = token;
                token = strtok(NULL, " ");
            }
            args[i] = NULL;

            if (execvp(args[0], args) == -1) {
                perror("Eroare execvp");
                exit(1);
            }
        }   
        else if (pid > 0){
            wait(NULL);
        }
    
}

void comenzi_pipe(char *linie_comanda) {
    char temp[200];
    strcpy(temp, linie_comanda);

    char *cmd1 = strtok(temp, "|");
    char *cmd2 = strtok(NULL, "");

    if (cmd1 == NULL || cmd2 == NULL) {
        printf("Format invalid pentru pipe.\n");
        return;
    }

    cmd1 = trim_spaces(cmd1);
    cmd2 = trim_spaces(cmd2);
    

    int fd[2];
    if (pipe(fd) == -1) {
        perror("Eroare la crearea pipe-ului");
        return;
    }

    pid_t pid1 = fork();
    if (pid1 == 0){
        close(fd[0]);  
        dup2(fd[1], STDOUT_FILENO);  
        close(fd[1]);

        char *argv1[] = {"/bin/sh", "-c", cmd1, NULL};
        execve("/bin/sh", argv1, NULL);  
        perror("Eroare la executia cmd1");
        exit(1);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {  // Al doilea proces copil
        close(fd[1]);  
        dup2(fd[0], STDIN_FILENO);  
        close(fd[0]);

        char *argv2[] = {"/bin/sh", "-c", cmd2, NULL};
        execve("/bin/sh", argv2, NULL);  
        perror("Eroare la executia cmd2");
        exit(1);
    }

    close(fd[0]);
    close(fd[1]);

    wait(NULL);
    wait(NULL);   
}

void comenzi_logice(char *linie_comanda) {
    char temp[200];
    strcpy(temp, linie_comanda);

    // Pointer pentru strtok_r
    char *saveptr;

    if (strstr(temp, "&&") != NULL) {
        char *cmd = strtok_r(temp, "&&", &saveptr);  // strtok_r e thread-safe
        while (cmd != NULL) {
            cmd = trim_spaces(cmd);

            int status = procesare_comanda(cmd);
            if(status != 0){
                break;
            }

            cmd = strtok_r(NULL, "&&", &saveptr);
        }
    }
    else if(strstr(temp, "||") != NULL){
        char *cmd = strtok_r(temp, "||", &saveptr);  // strtok_r e thread-safe
        while (cmd != NULL) {
            cmd = trim_spaces(cmd);
            int status;
            
            if(strcmp(cmd, "false") == 0)
                status = 1;
            else if(strcmp(cmd, "true") == 0)
                status = 0;
            else
                status = procesare_comanda(cmd);

            if(status == 0){
                break;
            }

            cmd = strtok_r(NULL, "||", &saveptr);
        }    
    }
}


int procesare_comanda(char *c){
    char linie_comanda[200]; // Sa avem linia de comanda intreaga
    strcpy(linie_comanda, c);
    char *token = strtok(c, " ");
    int cnt = 0;
    while (token != NULL)
    {
        strcpy(temp_input[cnt], token);            
        cnt++;
        token = strtok(NULL, " ");
    }
    
    if(strstr(linie_comanda, ">") || strstr(linie_comanda, "<"))
        comenzi_redirect(linie_comanda);
    else if(strstr(linie_comanda, "&&") || strstr(linie_comanda, "||"))
        comenzi_logice(linie_comanda);
    else if(strstr(linie_comanda, "|"))
        comenzi_pipe(linie_comanda);
    else if(strstr(sir_comenzi, temp_input[0]) || strstr(linie_comanda, "./"))
        comenzi_simple(cnt);
    else{
        comanda_necunoscuta(linie_comanda);
    }

    return 0;
}

void show_path(){
    char cwd[1000], aux[1000] = "AguSiCosminShell:";
    if(getcwd(cwd, sizeof(cwd)) == NULL)
        perror("Error getcwd().\n");
    else
    {
        strcat(aux, cwd);
        strcat(aux, "\n");
        printf("\033[32m%s\033[0m", aux);
    }
}

void intro()
{
    clear(); 
    printf("\033[32mWelcome to our Shell!\n    Agusoaei Alexandru, Jalba Cosmin @2024\033[0m\n");
    sleep(3); 
    clear();
}

int main()
{
    char input[100];
    intro();
    while(1){
        show_path();
        get_input(input);
        procesare_comanda(input);
    }
    return 0;
}   
