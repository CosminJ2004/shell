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
// -de scris in readme exemple despre comenzi logice false true, pipe, redirect
// -de facut mai frumos totusi redirect
//

char *sir_comenzi = "ls cd echo clear exit mkdir rmdir cp mv pwd grep cat touch nano ./";
const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
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
void last_comand(char *c)
{

}
int find_base64_char_index(char c) {
    for (int i = 0; i < 64; i++) {
        if (base64_chars[i] == c) {
            return i;
        }
    }
    return -1;
}
void decode_base64(char *input) {
    int i = 0, j = 0;
    unsigned char input_buffer[4], output_buffer[3];
        char* token;
        char output[100];
token=strtok(input," ");
        token=strtok(NULL," ");
        token=strtok(NULL," ");
        strcpy(input,token);

    while (input[i] != '\0') {
        for (int k = 0; k < 4; k++) {
            if (input[i] != '=') {
                input_buffer[k] = find_base64_char_index(input[i++]);
            } else {
                input_buffer[k] = 0;
                i++;
            }
        }

        output_buffer[0] = (input_buffer[0] << 2) + ((input_buffer[1] & 0x30) >> 4);
        output_buffer[1] = ((input_buffer[1] & 0x0F) << 4) + ((input_buffer[2] & 0x3C) >> 2);
        output_buffer[2] = ((input_buffer[2] & 0x03) << 6) + input_buffer[3];

        for (int k = 0; k < 3; k++) {
            if (input[i - 1] != '=') {
                output[j++] = output_buffer[k];
            } else {
                break;
            }
        }
    }

    output[j] = '\0';
        printf("%s",output);
        printf("\n");
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
void comanda_mv(int cnt) {
    if (cnt != 3) {
        printf("Eroare: Trebuie ss specifici fisierul sursa si destinatia.\n");
        return;
    }

    // Verifică dacă destinația este un director
    struct stat dest_stat;
    
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
    char linie_comanda_2[100]; // Alocare buffer pe stivă
    strcpy(linie_comanda_2, linie_comanda);
    char *token;
    char *sep = " \t\n"; // Adăugăm spațiul ca separator
    char *aux[10];
    char ofile[10];
    char comanda[100];
    int i = 0, k=0;
    
    
    int pozitie = -1,pozitie2=-1; // Inițializăm cu -1, pentru cazul în care caracterul nu este găsit
    for (int i = strlen(linie_comanda_2) - 1; i >= 0; i--) { // Parcurgem șirul de la dreapta la stânga
        if (linie_comanda_2[i] == '>') {
            pozitie = i; // Salvăm poziția
            break;       // Ne oprim, deoarece e prima apariție din dreapta
        }
        if(linie_comanda_2[k]=='<') {pozitie2=k; break;}
        k++;
    }
        if(pozitie!=-1){
   for (int i = 0; i <= pozitie - 1; i++) {
        comanda[i] = linie_comanda_2[i];
    }
    comanda[pozitie] = '\0'; // Adăugăm terminatorul de șir

    // Copierea numelui fișierului de ieșire
    int j = 0;
    for (int i = pozitie + 1; i <= strlen(linie_comanda_2) - 1; i++) {
        ofile[j++] = linie_comanda_2[i];
    }
    ofile[j] = '\0'; // Adăugăm terminatorul de șir
    strcpy(ofile,trim_spaces(ofile));
    strcpy(comanda,trim_spaces(comanda));
    printf("Comanda: %s\n", comanda);
    printf("Output file: %s\n", ofile);
   
    
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

    // Execută comanda
    if (execvp(args[0], args) == -1) {
        perror("Eroare execvp");
        exit(1);
    }}   
        else if (pid > 0){
            wait(NULL);
        }}
if (pozitie2 != -1) {
        // Redirecționare intrare (<)
        char *token_;
        char *sep_ = " \t\n"; // Adăugăm spațiul ca separator
        char *aux_[10];
        char ifile_[10];
        char comanda_[100];
        int i_ = 0;

        for (int i = 0; i <= pozitie2 - 1; i++) {
            comanda_[i] = linie_comanda_2[i];
        }
        comanda_[pozitie2] = '\0'; // Adăugăm terminatorul de șir

        // Copierea numelui fișierului de intrare
        int j = 0;
        for (int i = pozitie2 + 1; i <= strlen(linie_comanda_2) - 1; i++) {
            ifile_[j++] = linie_comanda_2[i];
        }
        ifile_[j] = '\0'; // Adăugăm terminatorul de șir
        strcpy(ifile_, trim_spaces(ifile_));
        strcpy(comanda_, trim_spaces(comanda_));
        printf("Comanda: %s\n", comanda_);
        printf("Input file: %s\n", ifile_);

        pid_t pid = fork();
        if (pid == 0) {
            // Deschide fisierul pentru redirect
            int fd = open(ifile_, O_RDONLY);
            if (fd < 0) {
                perror("Eroare la deschiderea fisierului");
                exit(1);
            }

            // Redirectioneaza stdin in fisier
            if (dup2(fd, STDIN_FILENO) == -1) {
                perror("Eroare dup2");
                close(fd);
                exit(1);
            }
            close(fd);

            // Execută comanda
            char *args[10] = {0};
            int i = 0;
            char *token = strtok(comanda_, " ");
            while (token != NULL) {
                args[i++] = token;
                token = strtok(NULL, " ");
            }
            args[i] = NULL;

            if (execvp(args[0], args) == -1) {
                perror("Eroare execvp");
                exit(1);
            }
        } else if (pid > 0) {
            wait(NULL);
        }
    }}

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
void print_permissions(mode_t mode) {
    char permissions[10];

    // Tipul fișierului
    if (S_ISREG(mode)) permissions[0] = '-';
    else if (S_ISDIR(mode)) permissions[0] = 'd';
    else if (S_ISLNK(mode)) permissions[0] = 'l';
    else if (S_ISCHR(mode)) permissions[0] = 'c';
    else if (S_ISBLK(mode)) permissions[0] = 'b';
    else if (S_ISFIFO(mode)) permissions[0] = 'p';
    else if (S_ISSOCK(mode)) permissions[0] = 's';
    else permissions[0] = '?';

    // Permisiuni pentru proprietar
    permissions[1] = (mode & S_IRUSR) ? 'r' : '-';
    permissions[2] = (mode & S_IWUSR) ? 'w' : '-';
    permissions[3] = (mode & S_IXUSR) ? 'x' : '-';

    // Permisiuni pentru grup
    permissions[4] = (mode & S_IRGRP) ? 'r' : '-';
    permissions[5] = (mode & S_IWGRP) ? 'w' : '-';
    permissions[6] = (mode & S_IXGRP) ? 'x' : '-';

    // Permisiuni pentru ceilalți
    permissions[7] = (mode & S_IROTH) ? 'r' : '-';
    permissions[8] = (mode & S_IWOTH) ? 'w' : '-';
    permissions[9] = (mode & S_IXOTH) ? 'x' : '-';

    permissions[10] = '\0'; // Terminator de șir

    printf("Permisiuni fișier: %s\n", permissions);
}

void comanda_file(char *input)
{

    char *file;
    file=strtok(input, " ");
    file=strtok(NULL," ");

    pid_t pid=fork();
    if(pid==0)
    {
     struct stat sb;
        if(stat(file, &sb)) //stat intoarce informatii despre un obiect (aici, fisier)
        {
            perror("Eroare Fisier dat catre file.\n");
            
        }else{
        if (S_ISREG(sb.st_mode)) {
    printf("Tipul fișierului: Fișier obișnuit\n");
} else if (S_ISDIR(sb.st_mode)) {
    printf("Tipul fișierului: Director\n");
} else if (S_ISLNK(sb.st_mode)) {
    printf("Tipul fișierului: Link simbolic\n");
}

printf("ID-ul utilizatorului proprietar: %u\n", sb.st_uid);
printf("ID-ul grupului proprietar: %u\n", sb.st_gid);
printf("Numărul de linkuri: %lu\n", (unsigned long)sb.st_nlink);

      printf("Timpul ultimei accesări: %s", ctime(&sb.st_atime));
        printf("Dimensiunea fișierului (în octeți): %lld\n", (long long)sb.st_size);
        printf("Drepturi de acces (octal): %o\n", sb.st_mode & 0777);
        printf("ID-ul inode-ului: %lu\n", (unsigned long)sb.st_ino);
        print_permissions(sb.st_mode);       
 }
    
    }
        else if(pid>0) wait(NULL);


    
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
        else if(strstr(linie_comanda,"base64 -d"))
        decode_base64(linie_comanda);
        else if(strstr(temp_input[0],"file"))
         comanda_file(linie_comanda);
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