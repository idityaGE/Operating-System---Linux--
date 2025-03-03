# Process Control in C: fork(), exec, wait, and More

Process management is a fundamental concept in operating systems and C programming. Let's explore these concepts from the basics all the way to advanced usage.

## Understanding Processes

A process is an instance of a running program with its own memory space, system resources, and execution context. Each process has a unique identifier called a Process ID (PID).

## Basic Process Creation: fork()

The `fork()` function creates a new process by duplicating the calling process. The new process is called the child process, while the original is the parent process.

```c
#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t pid;
    
    // Create a new process
    pid = fork();
    
    if (pid < 0) {
        // Error occurred
        fprintf(stderr, "Fork failed\n");
        return 1;
    } else if (pid == 0) {
        // Child process
        printf("Child process: My PID is %d, my parent's PID is %d\n", 
               getpid(), getppid());
    } else {
        // Parent process
        printf("Parent process: My PID is %d, my child's PID is %d\n", 
               getpid(), pid);
    }
    
    return 0;
}
```

### Key points about fork():

1. `fork()` returns different values in parent and child:
   - Returns child's PID to parent
   - Returns 0 to child
   - Returns -1 if an error occurs

2. After `fork()`, both processes continue executing from the next line of code.

3. The child process gets a copy of the parent's data space, heap, and stack.

## Running Different Programs: exec Family

The `exec` family of functions replaces the current process image with a new process image. This is how we run different programs from within our process.

There are several variations:
- `execl()`, `execlp()`, `execle()`
- `execv()`, `execvp()`, `execvpe()`

The differences relate to how arguments are passed and how the system searches for the executable.

```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    pid_t pid = fork();
    
    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
        return 1;
    } else if (pid == 0) {
        // Child process runs the ls command
        printf("Child is about to execute ls -l\n");
        
        // Replace the child process with the ls command
        execl("/bin/ls", "ls", "-l", NULL);
        
        // If execl returns, an error occurred
        perror("execl failed");
        exit(1);
    } else {
        // Parent process
        printf("Parent process continues execution\n");
    }
    
    return 0;
}
```

### Key points about exec:

1. If successful, `exec` functions never return, as the calling process is completely replaced.

2. If `exec` returns, an error has occurred.

3. The differences between exec variants:
   - `l` vs `v`: how arguments are passed (list vs array)
   - `p` suffix: searches PATH environment variable
   - `e` suffix: allows specifying environment variables

## Waiting for Child Processes: wait() and waitpid()

Parents can wait for their children to finish using `wait()` or `waitpid()`.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();
    int status;
    
    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
        return 1;
    } else if (pid == 0) {
        // Child process
        printf("Child process (PID: %d) running\n", getpid());
        sleep(2);  // Simulate work
        printf("Child process exiting\n");
        exit(42);  // Exit with a specific value
    } else {
        // Parent process
        printf("Parent waiting for child (PID: %d) to finish\n", pid);
        
        // Wait for the child to finish
        wait(&status);
        
        if (WIFEXITED(status)) {
            printf("Child exited with status: %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Child terminated by signal: %d\n", WTERMSIG(status));
        }
    }
    
    return 0;
}
```

### Key points about wait() and waitpid():

1. `wait(&status)` blocks the parent until any child terminates.

2. `waitpid(pid, &status, options)` allows waiting for a specific child.

3. Use macros to interpret the status value:
   - `WIFEXITED(status)`: true if child terminated normally
   - `WEXITSTATUS(status)`: returns exit status of child (0-255)
   - `WIFSIGNALED(status)`: true if child terminated by signal
   - `WTERMSIG(status)`: returns the signal number that caused termination

## More Advanced Process Control

### Creating Multiple Children

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUM_CHILDREN 5

int main() {
    pid_t pids[NUM_CHILDREN];
    int i, status;
    
    // Create multiple children
    for (i = 0; i < NUM_CHILDREN; i++) {
        pids[i] = fork();
        
        if (pids[i] < 0) {
            // Error handling
            perror("Fork failed");
            exit(1);
        } else if (pids[i] == 0) {
            // Child process
            printf("Child %d (PID: %d) started\n", i, getpid());
            sleep(i + 1);  // Each child sleeps for a different time
            printf("Child %d (PID: %d) exiting\n", i, getpid());
            exit(i + 10);  // Each child exits with a different status
        }
    }
    
    // Parent code - wait for all children
    for (i = 0; i < NUM_CHILDREN; i++) {
        pid_t terminated_pid = waitpid(pids[i], &status, 0);
        
        if (WIFEXITED(status)) {
            printf("Child %d (PID: %d) exited with status %d\n", 
                   i, terminated_pid, WEXITSTATUS(status));
        } else {
            printf("Child %d terminated abnormally\n", i);
        }
    }
    
    return 0;
}
```

### Non-blocking Wait with WNOHANG

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        // Child process
        printf("Child (PID: %d) sleeping for 5 seconds\n", getpid());
        sleep(5);
        printf("Child exiting\n");
        exit(0);
    } else {
        // Parent process
        int status;
        pid_t result;
        
        printf("Parent checking if child is done (non-blocking):\n");
        
        // Non-blocking wait
        for (int i = 0; i < 7; i++) {
            result = waitpid(pid, &status, WNOHANG);
            
            if (result == 0) {
                printf("Child still running, checking again in 1 second...\n");
                sleep(1);
            } else if (result == pid) {
                printf("Child (PID: %d) has finished\n", pid);
                if (WIFEXITED(status)) {
                    printf("Exit status: %d\n", WEXITSTATUS(status));
                }
                break;
            } else {
                perror("waitpid error");
                break;
            }
        }
    }
    
    return 0;
}
```

### Using Pipes for Inter-Process Communication (IPC)

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 100

int main() {
    int pipefd[2];  // pipefd[0] is read end, pipefd[1] is write end
    pid_t pid;
    char buffer[BUFFER_SIZE];
    
    // Create the pipe
    if (pipe(pipefd) == -1) {
        perror("Pipe creation failed");
        exit(1);
    }
    
    pid = fork();
    
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        // Child process - will write to the pipe
        
        // Close unused read end
        close(pipefd[0]);
        
        // Prepare message
        const char *message = "Hello from child process!";
        
        // Write to pipe
        write(pipefd[1], message, strlen(message) + 1);
        printf("Child (PID: %d) wrote message to pipe\n", getpid());
        
        // Close write end
        close(pipefd[1]);
        exit(0);
    } else {
        // Parent process - will read from the pipe
        
        // Close unused write end
        close(pipefd[1]);
        
        // Read from pipe
        int bytes_read = read(pipefd[0], buffer, BUFFER_SIZE);
        printf("Parent (PID: %d) received: %s\n", getpid(), buffer);
        
        // Close read end
        close(pipefd[0]);
        
        // Wait for child to finish
        wait(NULL);
    }
    
    return 0;
}
```

## Zombie and Orphan Processes

### Zombie Process

A zombie process is a process that has completed execution but still has an entry in the process table because its parent hasn't called `wait()` to read its exit status.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        // Child process
        printf("Child (PID: %d) running\n", getpid());
        printf("Child exiting\n");
        exit(0);
    } else {
        // Parent process
        printf("Parent (PID: %d) created child (PID: %d)\n", getpid(), pid);
        printf("Parent sleeping for 10 seconds - child becomes zombie\n");
        printf("Run 'ps -aux | grep Z' in another terminal to see the zombie\n");
        sleep(10);
        
        // Finally collect the zombie
        wait(NULL);
        printf("Parent collected child's exit status, zombie is gone\n");
    }
    
    return 0;
}
```

### Orphan Process

An orphan process is a process whose parent has terminated, while the child is still running. The orphan is adopted by the init process (PID 1).

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        // Child process
        printf("Child (PID: %d) running, parent is (PID: %d)\n", getpid(), getppid());
        sleep(2);  // Give parent time to exit
        
        printf("Child still running, but parent has exited\n");
        printf("Child's new parent is PID: %d (init/systemd)\n", getppid());
        sleep(5);
        printf("Child exiting\n");
    } else {
        // Parent process
        printf("Parent (PID: %d) created child (PID: %d)\n", getpid(), pid);
        printf("Parent exiting, child will become orphan\n");
        exit(0);  // Parent exits immediately
    }
    
    return 0;
}
```

## Advanced Exec Examples

### Using execvp() for Command-Line Arguments

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s command [args...]\n", argv[0]);
        exit(1);
    }
    
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        // Child process
        printf("Child (PID: %d) executing: %s\n", getpid(), argv[1]);
        
        // Execute the command with all its arguments
        execvp(argv[1], &argv[1]);
        
        // If we get here, execvp failed
        perror("execvp failed");
        exit(1);
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status)) {
            printf("Command '%s' exited with status %d\n", 
                   argv[1], WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Command '%s' killed by signal %d\n", 
                   argv[1], WTERMSIG(status));
        }
    }
    
    return 0;
}
```

### Implementing a Simple Shell

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

void parse_input(char *input, char **args) {
    char *token;
    int i = 0;
    
    // Remove trailing newline
    input[strcspn(input, "\n")] = 0;
    
    // Tokenize input
    token = strtok(input, " ");
    while (token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;  // Null-terminate the argument list
}

int main() {
    char input[MAX_INPUT];
    char *args[MAX_ARGS];
    pid_t pid;
    int status;
    
    while (1) {
        // Display prompt
        printf("myshell> ");
        fflush(stdout);
        
        // Read command
        if (fgets(input, MAX_INPUT, stdin) == NULL) {
            perror("fgets error");
            exit(1);
        }
        
        // Parse the input into arguments
        parse_input(input, args);
        
        // Check for empty command
        if (args[0] == NULL) {
            continue;
        }
        
        // Check for exit command
        if (strcmp(args[0], "exit") == 0) {
            printf("Exiting shell...\n");
            break;
        }
        
        // Fork a child process
        pid = fork();
        
        if (pid < 0) {
            perror("Fork failed");
        } else if (pid == 0) {
            // Child process executes command
            execvp(args[0], args);
            
            // If execvp returns, command not found
            printf("%s: command not found\n", args[0]);
            exit(1);
        } else {
            // Parent waits for child to complete
            waitpid(pid, &status, 0);
            
            if (WIFEXITED(status)) {
                // For debugging: printf("Command exited with status %d\n", WEXITSTATUS(status));
            }
        }
    }
    
    return 0;
}
```

## Process Groups and Sessions

Processes can be organized into process groups and sessions for management.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid, pgid;
    
    // Get the process group ID
    pgid = getpgrp();
    printf("Parent (PID: %d) is in process group: %d\n", getpid(), pgid);
    
    pid = fork();
    
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        // Child process
        printf("Child (PID: %d) initially in process group: %d\n", getpid(), getpgrp());
        
        // Create a new process group with the child as leader
        setpgid(0, 0);
        
        printf("Child now in process group: %d\n", getpgrp());
        exit(0);
    } else {
        // Parent process
        wait(NULL);
    }
    
    return 0;
}
```

## Using clone() for Fine-grained Control (Linux-specific)

The `clone()` system call provides more control over process creation than `fork()`.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/wait.h>

#define STACK_SIZE (1024 * 1024)    // 1MB stack

// Function to be executed in the new process
int child_func(void *arg) {
    printf("Child process running with argument: %s\n", (char *)arg);
    sleep(1);
    return 0;
}

int main() {
    char *stack;
    char *stack_top;
    pid_t pid;
    
    // Allocate stack for child
    stack = malloc(STACK_SIZE);
    if (stack == NULL) {
        perror("malloc failed");
        exit(1);
    }
    
    // stack grows downward on most architectures
    stack_top = stack + STACK_SIZE;
    
    // Message to pass to child process
    char *message = "Hello from parent!";
    
    // Create new process
    pid = clone(child_func, stack_top, 
                CLONE_VM | CLONE_FS | CLONE_FILES | SIGCHLD, 
                message);
    
    if (pid == -1) {
        perror("clone failed");
        free(stack);
        exit(1);
    }
    
    printf("Parent waiting for child (PID: %d)\n", pid);
    
    // Wait for child
    waitpid(pid, NULL, 0);
    printf("Child has finished\n");
    
    free(stack);
    return 0;
}
```

Note: To compile this, you'll need: `gcc -o clone_example clone_example.c`

## Summary of Process Control Functions

1. **Process Creation**:
   - `fork()`: Creates a new process by duplicating the calling process
   - `clone()` (Linux): Creates a new process with more control over shared resources

2. **Program Execution**:
   - `execl()`, `execlp()`, `execle()`: Execute a program with a list of arguments
   - `execv()`, `execvp()`, `execvpe()`: Execute a program with an array of arguments

3. **Process Synchronization**:
   - `wait()`: Wait for any child process to terminate
   - `waitpid()`: Wait for a specific child process to terminate
   - `waitid()`: Wait for a child process to change state

4. **Process Information**:
   - `getpid()`: Get the process ID of the calling process
   - `getppid()`: Get the process ID of the parent process
   - `getpgid()`: Get the process group ID of a process
   - `setpgid()`: Set the process group ID of a process

5. **Process Control**:
   - `exit()`: Terminate the calling process
   - `_exit()`: Terminate the calling process (bypasses cleanup)
   - `kill()`: Send a signal to a process or a group of processes

These functions form the foundation of process management in Unix-like operating systems. Understanding them well is crucial for system programming, writing shells, daemons, and other advanced applications.

Would you like me to elaborate on any specific aspect of process control, or would you like to see more examples of how these functions work together in real-world scenarios?