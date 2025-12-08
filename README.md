*This project has been created as part of the 42 curriculum by amtan.*

# get_next_line

## Description

`get_next_line` is a small C library whose goal is to read a text stream **one line at a time** from a file descriptor. It is part of the 42 core curriculum and is designed to:

* Re‑implement a convenient line‑reading function on top of the low‑level `read(2)` system call.
* Practice manual memory management with `malloc`/`free`.
* Learn how to keep state between function calls using **static variables** instead of global variables.
* Work correctly with different buffer sizes and both regular files and standard input.

The mandatory part provides:

* `char *get_next_line(int fd);`
* A minimal set of utility functions in `get_next_line_utils.c` (string length, join, search, memory copy, etc.).

On each successful call, `get_next_line` returns the **next line** from the given file descriptor, including the trailing `\n` when present. When there is nothing left to read or an error occurs, it returns `NULL`.

---

## Instructions

### Files

Mandatory part:

* `get_next_line.c`
* `get_next_line_utils.c`
* `get_next_line.h`

Bonus part (optional):

* `get_next_line_bonus.c`
* `get_next_line_utils_bonus.c`
* `get_next_line_bonus.h`

### Compilation

The project is written in C and must be compiled with:

```sh
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
   get_next_line.c get_next_line_utils.c \
   main.c -o gnl_test
```

Notes:

* `BUFFER_SIZE` controls how many bytes are read at a time by `read(2)`.
* The project must compile **with and without** the `-D BUFFER_SIZE=...` flag. A default value is defined inside `get_next_line.h` if none is provided.
* Only the following external functions are used: `read`, `malloc`, `free`.
* `libft` is **not** used in this project.

### Usage

Example `main.c`:

```c
#include <fcntl.h>      // open
#include <stdlib.h>     // free, exit
#include <stdio.h>      // printf
#include "get_next_line.h"

int main(int argc, char **argv)
{
    int   fd;
    char *line;

    if (argc != 2)
    {
        printf("Usage: %s <file>\n", argv[0]);
        return (1);
    }
    fd = open(argv[1], O_RDONLY);
    if (fd < 0)
    {
        perror("open");
        return (1);
    }
    line = get_next_line(fd);
    while (line)
    {
        printf("%s", line);
        free(line);
        line = get_next_line(fd);
    }
    close(fd);
    return (0);
}
```

Compile and run:

```sh
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
   get_next_line.c get_next_line_utils.c main.c -o gnl_test
./gnl_test path/to/file.txt
```

You can also read from standard input:

```sh
./gnl_test /dev/stdin
```

Then type lines and press `Ctrl+D` (EOF) to stop.

---

## Algorithm

### Overview

The core idea is to **avoid reading the whole file at once**. Instead, `get_next_line`:

1. Keeps a static "stash" of characters that were read previously but not yet returned to the caller.
2. Reads more data from the file descriptor into a temporary buffer using `read(2)`.
3. Appends this buffer to the stash.
4. As soon as a newline (`'\n'`) is found in the stash, it:

   * Extracts the next line.
   * Stores any leftover characters (after the newline) back into the stash for the next call.

This process repeats until there are no more lines to return or an error occurs.

### Data structures

**Mandatory part**

* A single static pointer inside `get_next_line`:

  ```c
  static char *stash;
  ```

  * `stash` lives for the entire duration of the program.
  * It holds the bytes that were read from `fd` but not yet returned to the caller.

* Utility helpers:

  * `gnl_strlen` – length of a C string (handles `NULL`).
  * `gnl_strchr` – find the first occurrence of a character in a string.
  * `gnl_strjoin` – allocate and return the concatenation of `stash` and `buffer`, freeing the old `stash`.
  * `ft_memcpy` – simple byte copy for internal use.
  * `free_and_ret_null` – free a pointer and return `NULL` (to simplify error paths).

### Control flow

A simplified view of each call to `get_next_line(fd)`:

1. **Validate input**

   * If `fd` is negative, `BUFFER_SIZE <= 0`, or any other obvious error → return `NULL`.

2. **Read into the stash**

   * Allocate a temporary `buffer` of size `BUFFER_SIZE + 1`.
   * Loop while the latest `read` call returned `> 0` and there is **no newline** yet in `stash`:

     * Call `read(fd, buffer, BUFFER_SIZE)`.
     * On `rd < 0` (error): free `buffer`, free `stash`, return `NULL`.
     * Null‑terminate the buffer: `buffer[rd] = '\0';`.
     * Concatenate `buffer` onto `stash` using `gnl_strjoin`.

3. **Extract the next line**

   * If the stash is empty after reading (EOF with no data) → free stash and return `NULL`.
   * Find the position of the first `\n` or `\0`.
   * Allocate a new string of the right size, copy the characters, and null‑terminate it.

4. **Update the stash**

   * If there was no newline in the stash (last line without `\n`): free stash and set it to `NULL`.
   * Otherwise, allocate a new stash containing only the leftover characters **after** the newline, free the old stash, and keep it for the next call.

5. **Return the line**

   * The caller is responsible for freeing the returned pointer.

### Justification of the algorithm

* **Correctness**

  * Repeated calls return consecutive lines in order.
  * The returned line includes the terminating `\n` when it exists; the last line without a trailing newline is also supported.
  * The function works for both regular files and standard input.

* **Efficiency**

  * The code reads **only as much as needed**: the loop stops as soon as a newline is seen or EOF is reached, instead of reading the whole file.
  * Reusing and growing a `stash` avoids re‑reading previously consumed data.
  * The user can tune `BUFFER_SIZE` to balance syscall overhead vs memory usage.

* **Memory safety**

  * Every `malloc` has a matching `free` on all paths (normal or error).
  * `stash` is centralized and always updated to either a valid allocated string or `NULL`.
  * The temporary `buffer` is freed before returning from `read_to_stash`, even on early exit.

* **Static state, not globals**

  * Using a `static` local pointer keeps state between calls **without** using forbidden global variables.
  * For the bonus, `stash[OPEN_MAX]` provides a simple, predictable way to maintain independent state for many file descriptors.

---

## Resources

### Documentation & articles

* `man 2 read` – details of the low‑level system call used to read from file descriptors.
* `man 2 write` – complementary call used in test programs to print or debug.
* Documentation and tutorials on:

  * Unix file descriptors and standard streams (`stdin`, `stdout`, `stderr`).
  * Static variables in C and how they preserve state across function calls.
* The official 42 subject PDF for **get_next_line** and the 42 **Norm** (coding style guide).

### AI usage

AI (ChatGPT) was used as a **learning assistant**, not as an automatic code generator. In particular, it was used to:

* Clarify the behavior of `read(2)` (return values, EOF, and error handling).
* Understand how static local variables work and why they are suitable for keeping the stash between calls.
* Discuss different design options for the stash (single pointer vs array indexed by file descriptor) and how to keep the design flexible for future extensions.
* Review and refine helper functions such as `gnl_strlen`, `gnl_strjoin`, and the error‑handling helpers.
* Identify subtle issues such as stack overflows with very large `BUFFER_SIZE` values and move the read buffer to dynamic allocation.
* Help draft this `README.md` and structure the explanation of the algorithm.

All code was:

* Written in my own editor.
* Re‑typed, compiled, and debugged by me.
* Tested using personal tests, 42 community testers (e.g. `gnlTester`, `francinette`), and manual checks.

During exams and evaluations where AI is not allowed, this project can be fully explained and re‑implemented from scratch without external assistance.
