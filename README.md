# Mini Shell (Linux) – Custom Command Line Interpreter in C

## 📌 Project Overview

Mini Shell is a custom-built command line interpreter developed in C that replicates the core functionalities of a Linux terminal. The goal of this project is to understand and implement fundamental operating system concepts such as process creation, execution, inter-process communication, and signal handling.

This shell provides a user-friendly interface where users can execute both built-in and external commands, similar to a standard Unix/Linux shell.

---

## 🎯 Objectives

* To design a simplified version of a Linux shell
* To understand process management using system calls
* To implement command parsing and execution
* To explore inter-process communication using pipes
* To handle signals like Ctrl+C and Ctrl+Z
* To gain hands-on experience with Linux system programming

---

## ⚙️ Features

### 🔹 Built-in Commands

The shell supports several built-in commands:

* `cd` – Change directory
* `pwd` – Print current working directory
* `echo` – Display messages and environment variables
* `exit` – Exit the shell
* `jobs` – Display background jobs
* `fg` – Bring background process to foreground

---

### 🔹 External Command Execution

* Executes system commands like `ls`, `cat`, `wc`, `ps`, etc.
* Uses `fork()` to create child processes
* Uses `execvp()` to replace child process image
* Parent process waits for child using `waitpid()`

---

### 🔹 Pipe Implementation

* Supports single pipe (`|`)
* Supports multiple pipes (N-pipe execution)
* Enables command chaining:

  ```
  ls -l | grep .c | wc
  ```
* Uses `pipe()` and `dup2()` for inter-process communication

---

### 🔹 Signal Handling

* Handles:

  * `Ctrl + C` (SIGINT) → Interrupt running process
  * `Ctrl + Z` (SIGTSTP) → Stop process and move to background
* Ensures shell does not terminate unexpectedly
* Custom signal handlers implemented

---

### 🔹 Process Management

* Tracks background processes using linked list
* Implements:

  * `jobs` → View suspended/background processes
  * `fg` → Resume process in foreground
* Uses process IDs (PID) for management

---

### 🔹 Environment Variables Support

* `echo $$` → Displays shell PID
* `echo $?` → Displays last command exit status
* `echo $SHELL` → Displays shell path

---

### 🔹 Custom Prompt

* User can change shell prompt dynamically:

  ```
  PS1=myshell
  ```

---

## 🛠️ Technologies Used

* C Programming Language
* Linux Operating System
* System Calls:

  * `fork()`, `execvp()`, `waitpid()`
  * `pipe()`, `dup2()`
  * `signal()`
* File Handling APIs

---

## 🧠 Key Concepts Learned

* Process creation and execution
* Parent-child process relationship
* Inter-process communication (IPC)
* Signal handling mechanisms
* Command parsing and tokenization
* Memory management in C
* Working of Unix/Linux shell internally

---

## 📂 Project Structure

```
.
├── main.c
├── scan_input.c
├── command.c
├── signal.c
├── mini.h
├── external.txt
└── README.md
```

---

## ▶️ How to Run

### Step 1: Compile

```
gcc *.c
```

### Step 2: Execute

```
./a.out
```

---

## 🧪 Sample Execution

```
minishell$: ls -l | wc
     11      92     734

minishell$: echo $$
12345

minishell$: sleep 10
^C
minishell$:
```

---

## 🚧 Limitations

* No support for input/output redirection (`>`, `<`)
* Limited error handling for invalid syntax
* Basic job control implementation

---

## 🔮 Future Enhancements

* Add input/output redirection
* Support background execution (`&`)
* Improve job control (bg command)
* Add command history feature
* Implement auto-completion
* Improve parsing for complex commands

---

## 🙌 Conclusion

This project provides a strong foundation in Linux system programming and demonstrates how a shell works internally. It significantly improved my understanding of operating system concepts and low-level programming in C.

---

## 🔗 Connect With Me

If you liked this project or have suggestions, feel free to connect with me!
