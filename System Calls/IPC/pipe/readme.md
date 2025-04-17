# **Pipes in Linux (Complete Guide)**
---
## **1️⃣ What is a Pipe?**
A **pipe (`pipe()`)** is a unidirectional **Inter-Process Communication (IPC)** mechanism in Linux that allows data transfer **from one process to another** in a **first-in, first-out (FIFO)** manner.

🔹 **Key Characteristics:**
- **Unidirectional**: Data flows in **one** direction (one end writes, the other reads).
- **Uses File Descriptors (FDs)**: Every pipe has two FDs: **`fd[0]` for reading** and **`fd[1]` for writing**.
- **Parent-Child Communication**: Pipes are mostly used between **related processes** (e.g., parent-child).
- **Blocking Behavior**:
  - Reading from an **empty** pipe blocks until data is available.
  - Writing to a **full** pipe blocks until space is available.

---

## **2️⃣ Creating a Pipe: `pipe()` System Call**
### **Syntax:**
```c
int pipe(int fd[2]);
```
- **`fd[0]`**: Read end
- **`fd[1]`**: Write end
- Returns `0` on success, `-1` on failure.

### **Example: Creating a Pipe**
```c
#include <stdio.h>
#include <unistd.h>

int main() {
    int fd[2];  // File descriptors: fd[0] for read, fd[1] for write
    if (pipe(fd) == -1) {
        perror("pipe failed");
        return 1;
    }
    printf("Pipe created: Read FD = %d, Write FD = %d\n", fd[0], fd[1]);
    return 0;
}
```
✅ **Output:** Pipe is created with read and write ends.

---

## **3️⃣ Parent-Child Communication Using Pipe**
### **Parent writes → Child reads**
```c
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd[2];
    pid_t pid;
    char buffer[100];

    if (pipe(fd) == -1) {
        perror("pipe failed");
        return 1;
    }

    pid = fork();  // Create child process
    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {  // Child process
        close(fd[1]); // Close write end
        read(fd[0], buffer, sizeof(buffer));
        printf("Child received: %s\n", buffer);
        close(fd[0]);
    } else {  // Parent process
        close(fd[0]); // Close read end
        char msg[] = "Hello from Parent!";
        write(fd[1], msg, strlen(msg) + 1);
        close(fd[1]);
    }
    return 0;
}
```
✅ **Explanation:**
- The parent writes **"Hello from Parent!"** to the pipe.
- The child reads the message and prints it.

---

## **4️⃣ Pipes and `dup2()` (Redirect Output)**
Pipes can be used with `dup2()` to redirect standard input/output.

### **Example: Simulating `ls | wc -l`**
```c
#include <stdio.h>
#include <unistd.h>

int main() {
    int fd[2];
    pipe(fd);
    if (fork() == 0) { // Child: `ls`
        close(fd[0]);           // Close unused read end
        dup2(fd[1], STDOUT_FILENO); // Redirect stdout to pipe write end
        execlp("ls", "ls", NULL);
    } else { // Parent: `wc -l`
        close(fd[1]);           // Close unused write end
        dup2(fd[0], STDIN_FILENO); // Redirect stdin to pipe read end
        execlp("wc", "wc", "-l", NULL);
    }
    return 0;
}
```
✅ **Explanation:**
- `ls` writes its output to the pipe.
- `wc -l` reads from the pipe and counts lines.

---

## **5️⃣ Non-Blocking Pipes (`fcntl()`)**
Pipes are **blocking by default**, meaning:
- Reading an **empty** pipe **blocks**.
- Writing to a **full** pipe **blocks**.

### **Making a Pipe Non-Blocking**
```c
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int fd[2];
    pipe(fd);

    int flags = fcntl(fd[0], F_GETFL);  // Get current flags
    fcntl(fd[0], F_SETFL, flags | O_NONBLOCK);  // Set non-blocking mode

    char buffer[100];
    int bytes = read(fd[0], buffer, sizeof(buffer));

    if (bytes == -1)
        perror("Non-blocking read failed");

    return 0;
}
```
✅ **Use Case:** Prevents the process from getting stuck while waiting.

---

## **6️⃣ Named Pipes (FIFOs)**
Regular pipes work **only between related processes**. For unrelated processes, use **Named Pipes (FIFOs)**.

### **Create a FIFO**
```sh
mkfifo my_fifo
```

### **Process 1 (Writer)**
```sh
echo "Hello, FIFO!" > my_fifo
```

### **Process 2 (Reader)**
```sh
cat my_fifo
```
✅ **Use Case:** IPC between **separate programs**.

### **FIFO in C**
```c
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    char *fifo = "/tmp/my_fifo";
    mkfifo(fifo, 0666);  // Create FIFO

    char msg[] = "Hello, FIFO!";
    int fd = open(fifo, O_WRONLY);
    write(fd, msg, sizeof(msg));
    close(fd);

    return 0;
}
```
✅ **Use Case:** **Two-way** communication between **unrelated processes**.

---

## **7️⃣ When to Use Pipes?**
| **Use Case** | **Solution** |
|-------------|-------------|
| Parent-Child Communication | Use `pipe()` |
| One-way communication | Pipe works well |
| Two-way communication | Use two pipes (one for each direction) |
| Unrelated processes | Use **Named Pipes (FIFO)** |

---

## **8️⃣ Pipe vs Other IPC Mechanisms**
| **IPC Mechanism** | **Pros** | **Cons** |
|------------------|---------|---------|
| **Pipes** | Simple, Fast | Parent-child only, unidirectional |
| **Named Pipes (FIFO)** | Works across processes | Still unidirectional |
| **Sockets** | Works over a network | Complex |
| **Message Queues** | Supports multiple receivers | Harder to manage |
| **Shared Memory** | Very fast | Requires synchronization |

---

## **9️⃣ Summary**
| **Feature** | **Regular Pipe** | **Named Pipe (FIFO)** |
|------------|---------------|------------------|
| Scope | Parent-Child | Any process |
| Direction | Unidirectional | Unidirectional |
| Lifetime | Ends with process | Persistent |
| File Descriptor? | Yes | Yes |

### **Key Takeaways**
✅ Use **pipes** for simple, fast **parent-child** communication.  
✅ Use **named pipes (FIFOs)** for **separate processes**.  
✅ Combine pipes with `dup2()` for **command chaining (`ls | wc -l`)**.  
✅ Use `fcntl()` to **make pipes non-blocking**.

**🚀 Want more examples? Let me know!**
