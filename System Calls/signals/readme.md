# **Signals in Linux (Complete Guide)**
---
## **1️⃣ What Are Signals?**
Signals are **asynchronous notifications** sent to a process by the operating system, another process, or itself to notify it about events like:
- **Interrupts** (e.g., pressing `Ctrl+C`)
- **Errors** (e.g., division by zero)
- **Termination Requests**
- **User-defined notifications**

🔹 **Key Characteristics:**
- **Asynchronous**: Signals occur unexpectedly.
- **Interrupt-driven**: A process doesn't check for signals; they arrive forcefully.
- **Minimal Information**: A signal conveys only its number and type, not additional data.

---

## **2️⃣ Signal Types in Linux**
Signals are identified by **signal numbers** and **names**. Common signals include:

| **Signal Name** | **Number** | **Description** | **Default Action** |
|---------------|------------|-----------------|------------------|
| `SIGHUP`     | 1  | Terminal closed | Terminate |
| `SIGINT`     | 2  | Interrupt (Ctrl+C) | Terminate |
| `SIGQUIT`    | 3  | Quit (Ctrl+\) | Terminate + Core Dump |
| `SIGKILL`    | 9  | Force kill | Terminate (cannot be ignored) |
| `SIGTERM`    | 15 | Graceful termination | Terminate |
| `SIGSTOP`    | 19 | Stop process | Stop |
| `SIGCONT`    | 18 | Continue stopped process | Continue |
| `SIGSEGV`    | 11 | Segmentation fault | Terminate + Core Dump |
| `SIGALRM`    | 14 | Alarm from `alarm()` | Terminate |
| `SIGUSR1`    | 10 | User-defined signal 1 | Terminate (default) |

📌 **Key Notes:**
- **`SIGKILL` and `SIGSTOP` cannot be caught, blocked, or ignored.**
- **User-defined signals (`SIGUSR1` and `SIGUSR2`) are for custom notifications.**
- **Core Dump**: A process snapshot saved for debugging.

---

## **3️⃣ Sending Signals**
### **A. Using `kill` Command**
The `kill` command sends signals to a process by PID.

```sh
kill -SIGTERM <PID>
kill -9 <PID>     # Force kill using SIGKILL
kill -STOP <PID>  # Stop process
kill -CONT <PID>  # Resume process
```

You can also use signal numbers:
```sh
kill -15 <PID>  # Same as SIGTERM
```

### **B. Using `kill()` System Call**
Processes can send signals programmatically using `kill()`.

```c
#include <signal.h>
#include <unistd.h>

int main() {
    pid_t pid = 1234;  // Replace with actual PID
    kill(pid, SIGTERM);
    return 0;
}
```

### **C. Using `raise()` (Self-Signaling)**
A process can send a signal to itself using `raise()`.

```c
#include <signal.h>
#include <stdio.h>

int main() {
    printf("Raising SIGTERM\n");
    raise(SIGTERM);
    return 0;
}
```

---

## **4️⃣ Handling Signals (`signal()` and `sigaction()`)**
By default, most signals **terminate** a process, but we can **handle** them using:
- `signal()`
- `sigaction()`

### **A. Using `signal()`**
```c
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void handle_signal(int signum) {
    printf("Received signal: %d\n", signum);
}

int main() {
    signal(SIGINT, handle_signal); // Handle Ctrl+C
    while (1) {
        printf("Running... Press Ctrl+C\n");
        sleep(1);
    }
    return 0;
}
```
**How it works:**
1. The process registers a handler for `SIGINT`.
2. When `Ctrl+C` is pressed, `handle_signal()` runs instead of terminating.

### **B. Using `sigaction()` (Recommended)**
`sigaction()` is more powerful than `signal()`, allowing finer control.

```c
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void handle_signal(int signum) {
    printf("Handled signal: %d\n", signum);
}

int main() {
    struct sigaction sa;
    sa.sa_handler = handle_signal;
    sigaction(SIGINT, &sa, NULL);

    while (1) {
        printf("Running... Press Ctrl+C\n");
        sleep(1);
    }
    return 0;
}
```

✅ **Advantages of `sigaction()` over `signal()`**
- Works reliably across UNIX/Linux.
- Supports more features like blocking signals.

---

## **5️⃣ Ignoring and Blocking Signals**
### **A. Ignoring Signals**
You can **ignore** a signal using `SIG_IGN`.

```c
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    signal(SIGINT, SIG_IGN);  // Ignore Ctrl+C
    while (1) {
        printf("Can't stop me with Ctrl+C!\n");
        sleep(1);
    }
    return 0;
}
```
Now, pressing `Ctrl+C` does nothing.

---

### **B. Blocking Signals (`sigprocmask()`)**
Processes can **block signals** using `sigprocmask()`.

```c
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    sigset_t block_set;
    sigemptyset(&block_set);
    sigaddset(&block_set, SIGINT);

    sigprocmask(SIG_BLOCK, &block_set, NULL);  // Block SIGINT

    printf("SIGINT blocked for 5 seconds\n");
    sleep(5);

    sigprocmask(SIG_UNBLOCK, &block_set, NULL);  // Unblock SIGINT
    printf("SIGINT unblocked\n");

    while (1) sleep(1);  // Keep running
    return 0;
}
```

📌 **Blocking a signal prevents the process from receiving it until unblocked.**

---

## **6️⃣ Alarm Signals (`SIGALRM`)**
The `alarm()` function sends `SIGALRM` after a specified time.

```c
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void alarm_handler(int signum) {
    printf("Alarm received!\n");
}

int main() {
    signal(SIGALRM, alarm_handler);
    alarm(3);  // Send SIGALRM after 3 seconds
    pause();   // Wait for signal
    return 0;
}
```

📌 **Use Case**: Timeouts, periodic tasks.

---

## **7️⃣ Handling Zombie Processes (`SIGCHLD`)**
When a child terminates, it becomes a **zombie** until the parent collects its exit status. `SIGCHLD` prevents zombies.

```c
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void reap_child(int signum) {
    wait(NULL);
    printf("Child reaped!\n");
}

int main() {
    signal(SIGCHLD, reap_child);
    if (fork() == 0) {
        printf("Child process\n");
        return 0;
    }
    sleep(5);
    return 0;
}
```
📌 **Fixing zombies** by handling `SIGCHLD`.

---

## **8️⃣ Signal Handling Best Practices**
1. **Use `sigaction()` instead of `signal()`** for portability.
2. **Handle critical operations safely** (use `sig_atomic_t` for variables modified in signal handlers).
3. **Avoid system calls inside handlers**, except async-safe ones (`write()`, `_exit()`).
4. **Use blocking instead of ignoring signals** if temporary masking is needed.
5. **Handle `SIGCHLD`** to prevent zombie processes.

---

## **Conclusion**
| **Feature** | **Function** |
|------------|-------------|
| Send Signals | `kill()`, `raise()` |
| Handle Signals | `signal()`, `sigaction()` |
| Block Signals | `sigprocmask()` |
| Ignore Signals | `SIG_IGN` |
| Alarm Signal | `alarm()`, `SIGALRM` |
| Prevent Zombies | `SIGCHLD` |

**Want to explore more?** Let me know! 🚀
