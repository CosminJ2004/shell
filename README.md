
# 🐚 MiniShell in C

**MiniShell** is a lightweight, custom shell implementation written in C. The project demonstrates fundamental concepts of operating systems such as process management, I/O redirection, pipes, logical operators, and basic command execution. It provides a command-line interface similar to traditional Unix shells.

---

## 🚀 Key Features

### ✅ Execution of Basic Commands
- Supports standard commands such as: `ls`, `cd`, `mkdir`, `touch`, `rm`, `nano`, and more.
- Handles both internal and external commands, including system utilities.

### 🔄 I/O Redirection
- Implements output redirection (`>`).
- Input redirection (`<`) can be easily extended for future use.

### 🔗 Pipes (`|`)
- Processes commands connected through pipes, enabling the output of one command to serve as the input for another.

### 🔀 Logical Operators
- Supports basic logical operators such as `&&` (AND) and `||` (OR) to execute commands conditionally.

### ⚙️ Process Management
- Uses `fork()`, `execve()`, and `wait()` to create and manage child processes.
- Process management for background tasks can be handled by `&`.

### 📝 Built-in Command Support
- Includes built-in support for commands like `cd`, `exit`, `echo`, `pwd`.

### 🖥️ File Operations
- Supports basic file operations like creating files (`touch`), removing files (`rm`), viewing files (`cat`, `nano`), etc.

---

## 🛠️ Getting Started

### 📋 Prerequisites
- GCC compiler
- Basic knowledge of C and Unix commands

### 📥 Installation
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
   gcc -o minishell shell.c
   ```

---

## ▶️ Running the Shell
Start the shell by running:
```bash
./minishell
```
The shell will enter interactive mode, allowing you to input commands and receive outputs.

---

## 💡 Usage Examples

### 📂 Basic Commands
```bash
shell>$ ls -l
shell>$ mkdir test_folder
shell>$ touch test_file.txt
```

### 📝 Redirection
Redirect output to a file:
```bash
shell>$ ls > output.txt
```

### 🔗 Piping
Connect commands using pipes:
```bash
shell>$ ls | grep ".c"
```

### ⚡ Logical Operators
Use logical operators to chain commands conditionally:
```bash
shell>$ mkdir new_folder && cd new_folder
shell>$ mkdir another_folder || echo "Folder already exists"
```

### 🖥️ File Operations
- View the contents of a file using `cat`:
```bash
shell>$ cat file.txt
```
- Open a file using `nano`:
```bash
shell>$ nano file.txt
```

### 🧹 Remove Files
Remove files with `rm`:
```bash
shell>$ rm file.txt
```

---

## 🤝 Contributing

Pull requests are welcome! For significant changes, please open an issue first to discuss what you would like to change.

---

**Enjoy exploring the world of system programming with MiniShell!** 🐧✨
