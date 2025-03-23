# **Inter-Process Communication (IPC) in Linux**
**Inter-Process Communication (IPC)** allows processes to communicate and share data. Since processes run in separate memory spaces, IPC mechanisms help them exchange information efficiently.

---

## **1️⃣ Why is IPC Needed?**
- **Processes are isolated**: They don’t share memory.
- **Coordination**: Processes working on a shared task must exchange data.
- **Performance**: Instead of writing data to disk, IPC provides fast communication.

---

## **2️⃣ IPC Mechanisms in Linux**
Linux provides several IPC mechanisms:
| IPC Mechanism | Description | Use Case |
|--------------|------------|----------|
| **Pipes** | Unidirectional communication between related processes | Parent-child processes |
| **Named Pipes (FIFOs)** | Like pipes, but work between unrelated processes | Logging, producer-consumer model |
| **Message Queues** | Exchange structured messages | Banking transactions, message brokers |
| **Shared Memory** | Fastest IPC, memory shared between processes | Real-time data sharing (e.g., game engines) |
| **Semaphores** | Synchronization mechanism to control access to resources | Prevent race conditions |
| **Sockets** | Bi-directional communication, even over a network | Networked applications (e.g., client-server) |

---

## **3️⃣ Pipes**
Pipes allow **one-way communication** between a parent and child process.

### **Example: Using `pipe()`**
```c
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd[2];  // fd[0] for reading, fd[1] for writing
    char message[] = "Hello from parent!";
    char buffer[100];

    if (pipe(fd) == -1) {
        perror("Pipe failed");
        return 1;
    }

    if (fork() == 0) {  // Child process
        close(fd[1]);  // Close write end
        read(fd[0], buffer, sizeof(buffer));
        printf("Child received: %s\n", buffer);
        close(fd[0]);
    } else {  // Parent process
        close(fd[0]);  // Close read end
        write(fd[1], message, strlen(message) + 1);
        close(fd[1]);
    }

    return 0;
}
```
**How it works:**
1. `pipe(fd)` creates a communication channel.
2. The **parent writes** to `fd[1]`, the **child reads** from `fd[0]`.

🔹 **Limitation**: Only works between parent-child processes.

---

## **4️⃣ Named Pipes (FIFOs)**
Unlike normal pipes, **FIFOs (Named Pipes)** allow communication between **unrelated processes**.

### **Example: Using `mkfifo()`**
**Process 1: Writer**
```c
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    char *fifo = "/tmp/myfifo";
    mkfifo(fifo, 0666); // Create named pipe

    int fd = open(fifo, O_WRONLY);
    write(fd, "Hello from FIFO!", 16);
    close(fd);

    return 0;
}
```

**Process 2: Reader**
```c
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    char buffer[100];
    char *fifo = "/tmp/myfifo";

    int fd = open(fifo, O_RDONLY);
    read(fd, buffer, sizeof(buffer));
    printf("Received: %s\n", buffer);
    close(fd);

    return 0;
}
```
🔹 **Limitation**: Slower than shared memory.

---

## **5️⃣ Message Queues**
Message Queues allow **structured message passing** with priorities.

### **Example: Using `msgget()`, `msgsnd()`, and `msgrcv()`**
**Sender Process**
```c
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

struct message {
    long msg_type;
    char msg_text[100];
};

int main() {
    key_t key = ftok("progfile", 65);
    int msgid = msgget(key, 0666 | IPC_CREAT);

    struct message msg;
    msg.msg_type = 1;
    strcpy(msg.msg_text, "Hello Message Queue!");

    msgsnd(msgid, &msg, sizeof(msg.msg_text), 0);
    printf("Message Sent!\n");

    return 0;
}
```

**Receiver Process**
```c
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct message {
    long msg_type;
    char msg_text[100];
};

int main() {
    key_t key = ftok("progfile", 65);
    int msgid = msgget(key, 0666 | IPC_CREAT);

    struct message msg;
    msgrcv(msgid, &msg, sizeof(msg.msg_text), 1, 0);
    printf("Received: %s\n", msg.msg_text);

    msgctl(msgid, IPC_RMID, NULL); // Remove queue

    return 0;
}
```
🔹 **Use Case**: Chat applications, task queues.

---

## **6️⃣ Shared Memory**
Shared memory allows multiple processes to access the **same memory segment**, making it the fastest IPC.

### **Example: Using `shmget()` and `shmat()`**
**Writer Process**
```c
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

int main() {
    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);

    char *shared_mem = (char *)shmat(shmid, NULL, 0);
    strcpy(shared_mem, "Shared Memory Example");
    
    printf("Data written to shared memory\n");

    shmdt(shared_mem);
    return 0;
}
```

**Reader Process**
```c
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main() {
    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);

    char *shared_mem = (char *)shmat(shmid, NULL, 0);
    printf("Data read from shared memory: %s\n", shared_mem);

    shmdt(shared_mem);
    shmctl(shmid, IPC_RMID, NULL); // Remove shared memory segment

    return 0;
}
```
🔹 **Fastest IPC**, but requires synchronization.

---

## **7️⃣ Semaphores (Synchronization)**
Semaphores **control access** to shared resources.

### **Example: Using `semget()`, `semop()`**
**Initialization**
```c
#include <sys/sem.h>
sem_t mutex;
sem_init(&mutex, 1, 1);
```
**Locking and Unlocking**
```c
sem_wait(&mutex);  // Lock
// Critical Section
sem_post(&mutex);  // Unlock
```
🔹 **Use Case**: Preventing race conditions.

---

## **8️⃣ Sockets (Network Communication)**
Sockets enable **bi-directional** communication, even over networks.

### **Example: TCP Server & Client**
**Server**
```c
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr = {AF_INET, htons(8080), INADDR_ANY};

    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_fd, 5);

    int client_fd = accept(server_fd, NULL, NULL);
    char message[] = "Hello Client!";
    send(client_fd, message, strlen(message), 0);
    close(server_fd);

    return 0;
}
```

**Client**
```c
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr = {AF_INET, htons(8080), inet_addr("127.0.0.1")};

    connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

    char buffer[100];
    recv(sock, buffer, sizeof(buffer), 0);
    printf("Server says: %s\n", buffer);
    close(sock);

    return 0;
}
```
🔹 **Use Case**: Web servers, database connections.

---

## **Conclusion**
- **Fastest IPC?** → **Shared Memory**  
- **Networked IPC?** → **Sockets**  
- **Process Coordination?** → **Semaphores**  

Would you like more details on any specific mechanism? 🚀
