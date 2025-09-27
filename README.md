# Custom Linux Shell in C

A POSIX-style Linux shell implemented in C, supporting advanced command execution, process control, and history management. This shell mimics real-world shells like **bash** and **zsh**, while being lightweight and memory-leak free.

---

## 🔹 Features

### Command Execution
- Supports nested execution with `()` and command substitution `$()`
- Handles sequencing and operators: `;`, `|`, `<`, `<<`, `>`, `>>`
- Multiple command pipelines (e.g., `ls -la | grep Makefile; cat Makefile > file.txt`)

### History
- `history` → view command history
- `history 3` → re-executes the 3rd command in history
- Optimized with a **circular buffer** for O(1) access

### Built-in Commands
- `exit` → terminate the shell gracefully
- `Ctrl+C` → interrupt the current foreground process without killing the shell

### System-Level Implementation
- Process creation with `fork()`, `execvp()`, `wait()`
- Inter-process communication using **pipes**
- Recursive parsing for nested commands
- I/O redirection (`<`, `>`, `>>`, `<<`)
- Signal handling for interactive control

### Reliability
- Verified with **Valgrind** to ensure **zero memory leaks**


### Install and Run
- Run `install.sh`
- Arguments -> `default`   to build the shell ,
-              `debug`   to add `-g` flag,
-               if no arguments, default built will be made

- Run the shell use - `SHELL` command for default built
-                     `SHELL_D` command for debug built
