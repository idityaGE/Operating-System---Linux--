# **FIFO (Named Pipes) in Linux**
---
## **1️⃣ What is a FIFO?**
A **FIFO (First-In, First-Out)**, also called a **Named Pipe**, is a type of **Inter-Process Communication (IPC)** mechanism in Linux. It allows two **unrelated** processes (not necessarily parent-child) to communicate with each other.

### **🔹 Key Characteristics:**
- **Bidirectional** communication (unlike regular pipes, which are unidirectional).
- **Persistent**: Unlike regular pipes, FIFOs **exist beyond process execution**.
- **Uses the filesystem**: FIFO appears as a file and can be accessed by multiple processes.
- **Blocking Behavior**:
  - A process reading from an **empty** FIFO **blocks** until data is available.
  - A process writing to a **full** FIFO **blocks** until space is available.

---

## **2️⃣ Creating a FIFO**
### **Using `mkfifo` Command**
You can create a FIFO using:
```sh
mkfifo my_fifo
```
This creates a **special file** `my_fifo` that can be used for communication.

#### **Check the FIFO**
```sh
ls -l my_fifo
```
✅ **Output:**
```
prw-r--r-- 1 user user 0 Mar 24 10:30 my_fifo
```
- The `p` at the beginning (`prw-r--r--`) indicates a **pipe**.

---

## **3️⃣ FIFO in Action (Shell Example)**
### **Process 1 (Writer)**
```sh
echo "Hello FIFO!" > my_fifo
```
### **Process 2 (Reader)**
```sh
cat my_fifo
```
✅ **Output:**  
```
Hello FIFO!
```
The message is sent from **Process 1** to **Process 2** through the FIFO.

---

## **4️⃣ Creating a FIFO in C**
The `mkfifo()` system call is used to create a FIFO.

### **Syntax**
```c
#include <sys/types.h>
#include <sys/stat.h>

int mkfifo(const char *pathname, mode_t mode);
```
- `pathname`: Path to the FIFO file.
- `mode`: File permissions (e.g., `0666` for read/write access).

---

## **5️⃣ C Program: Writing to FIFO**
```c
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    char *fifo = "/tmp/my_fifo";
    mkfifo(fifo, 0666); // Create FIFO with read-write permission

    int fd = open(fifo, O_WRONLY); // Open FIFO for writing
    char message[] = "Hello from FIFO!";
    write(fd, message, sizeof(message)); // Write message
    close(fd); // Close FIFO

    return 0;
}
```
✅ **Explanation:**
- Creates a FIFO at `/tmp/my_fifo`.
- Opens it for writing (`O_WRONLY`).
- Sends a message **"Hello from FIFO!"**.
- Closes the FIFO.

---

## **6️⃣ C Program: Reading from FIFO**
```c
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    char *fifo = "/tmp/my_fifo";
    mkfifo(fifo, 0666); // Create FIFO

    int fd = open(fifo, O_RDONLY); // Open FIFO for reading
    char buffer[100];
    read(fd, buffer, sizeof(buffer)); // Read message
    printf("Received: %s\n", buffer);
    close(fd); // Close FIFO

    return 0;
}
```
✅ **Output:**
```
Received: Hello from FIFO!
```

✅ **Steps to Run:**
1. **Open two terminals**.
2. **Run the Reader first** (`./reader`).
3. **Then run the Writer** (`./writer`).

---

## **7️⃣ Two-Way Communication Using FIFO**
We can use **two FIFOs** for **bidirectional** communication.

### **Create Two FIFOs**
```sh
mkfifo fifo1 fifo2
```

### **C Program for Two-Way FIFO Communication**
#### **Process 1 (Client)**
```c
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    char *fifo1 = "/tmp/fifo1"; // Client writes, Server reads
    char *fifo2 = "/tmp/fifo2"; // Server writes, Client reads

    mkfifo(fifo1, 0666);
    mkfifo(fifo2, 0666);

    char message[100], response[100];
    int fd1 = open(fifo1, O_WRONLY); // Open FIFO1 for writing
    printf("Enter message: ");
    fgets(message, 100, stdin);
    write(fd1, message, sizeof(message)); // Send message
    close(fd1);

    int fd2 = open(fifo2, O_RDONLY); // Open FIFO2 for reading
    read(fd2, response, sizeof(response));
    printf("Server response: %s\n", response);
    close(fd2);

    return 0;
}
```

#### **Process 2 (Server)**
```c
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    char *fifo1 = "/tmp/fifo1"; // Client writes, Server reads
    char *fifo2 = "/tmp/fifo2"; // Server writes, Client reads

    mkfifo(fifo1, 0666);
    mkfifo(fifo2, 0666);

    char message[100], response[] = "Message received by Server!";
    int fd1 = open(fifo1, O_RDONLY); // Open FIFO1 for reading
    read(fd1, message, sizeof(message));
    printf("Received from Client: %s\n", message);
    close(fd1);

    int fd2 = open(fifo2, O_WRONLY); // Open FIFO2 for writing
    write(fd2, response, sizeof(response)); // Send response
    close(fd2);

    return 0;
}
```

✅ **Steps to Run:**
1. Open **two terminals**.
2. **Run the Server first** (`./server`).
3. **Then run the Client** (`./client`).
4. The Client sends a message, and the Server responds.

---

## **8️⃣ FIFO vs Regular Pipes**
| Feature | Regular Pipe | FIFO (Named Pipe) |
|---------|-------------|------------------|
| **Scope** | Parent-Child Only | Any Process |
| **Direction** | Unidirectional | Unidirectional (Can be bidirectional with 2 FIFOs) |
| **File System** | No | Yes (`ls -l` shows FIFO) |
| **Persistence** | Dies with process | Exists until deleted |
| **Usage** | `pipe()` | `mkfifo()` |

---

## **9️⃣ When to Use FIFO?**
| Use Case | Solution |
|----------|----------|
| Parent-Child IPC | **Regular Pipe (`pipe()`)** |
| IPC between **unrelated processes** | **FIFO (`mkfifo()`)** |
| **Inter-machine communication** | **Sockets** |
| **Multiple clients, one server** | **Message Queues or Shared Memory** |

---

## **🔟 Summary**
✔ **FIFO is a Named Pipe** that enables **IPC between unrelated processes**.  
✔ **`mkfifo()`** creates a FIFO, and it behaves like a **file**.  
✔ **Used for one-way or two-way IPC** by using multiple FIFOs.  
✔ **Blocking behavior**: A process **waiting for data blocks** until another writes.  
✔ **Alternative IPCs**: Shared Memory, Message Queues, or Sockets for advanced use cases.  

---

🚀 **Want more examples? Let me know!**
