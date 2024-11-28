
# MiniShell in C

**MiniShell** is a lightweight implementation of a Unix-like shell, written in C. This project demonstrates fundamental operating system concepts, such as process management, I/O redirection, pipes, and logical operators. The shell provides a command-line interface similar to traditional Unix shells.

---

## Key Features

### ✅ Execution of Basic Commands
- Supports standard commands such as: `ls`, `cd`, `mkdir`, `touch`, `rm`, and more.
- Handles both internal and external commands.

### ✅ I/O Redirection
- Implements output redirection (`>`).
- Input redirection (`<`) can be easily extended.

### ✅ Pipes (`|`)
- Processes commands connected through pipes, enabling the output of one command to serve as the input for the next.

### ✅ Logical Operators
- Supports basic logical operators such as `&&` (AND) and `||` (OR) to execute commands conditionally.

### ✅ Process Management
- Uses `fork()`, `execve()`, and `wait()` to create and manage child processes.

---

## Getting Started

### Prerequisites
- GCC compiler
- Basic knowledge of C and Unix commands

### Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/shell.git
   ```
2. Navigate to the project directory:
   ```bash
   cd shell
   ```
3. Compile the project:
   ```bash
   gcc -o shell shell.c
   ```

### Running the Shell
Start the shell by running:
```bash
./minishell
```

---

## Usage Examples

### Basic Commands
```bash
shell>$ ls -l
shell>$ mkdir test_folder
```

### Redirection
```bash
shell>$ ls > output.txt
```

### Piping
```bash
shell>$ ls | grep ".c"
```

### Logical Operators
```bash
shell>$ mkdir new_folder && cd new_folder
```

---

## Contributing
Pull requests are welcome! For significant changes, please open an issue first to discuss what you would like to change.

---

**Enjoy exploring the world of system programming with MiniShell!** 🚀
