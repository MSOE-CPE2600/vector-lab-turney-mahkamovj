# Vector Calculator Project

## Overview
This program stores vectors and calculates simple vector math, while also having memory allocation and file I/O. 

There are two major versions included in this repository:

- **Version 1 (v1.0)** – Original vector calculator (Lab 5)
- **Version 2 (v2.0 – v2.2)** – Updated calculator with dynamic memory allocation and file I/O (Lab 7)

---

## How to build
Compile: make
Run: ./test

## How It Works
The Vector Calculator reads user commands from the terminal in an interactive prompt (like a mini command-line interpreter).  
Users can define vectors, perform arithmetic operations, list, clear, save, and load vectors from files.


---

## Supported Commands

| Command | Description |
|----------|-------------|
| `var = x y z` | Assign a new vector |
| `var` | Display an existing vector |
| `var1 + var2` | Add two vectors |
| `var1 - var2` | Subtract two vectors |
| `var * num` or `num * var` | Multiply a vector by a scalar |
| `clear` | Clears all vectors from memory |
| `list` | Displays all active vectors |
| `load <file>` | Loads vectors from a CSV file *(v2.x only)* |
| `save <file>` | Saves all vectors to a CSV file *(v2.x only)* |
| `-h` | Displays the help screen |
| `quit` | Exits the program |

---

## Dynamic Memory Usage (v2.x)
Version 2 replaces static arrays with **linked list nodes** dynamically allocated using `malloc()`.  
Each node stores:
- a vector name,
- its components (x, y, z),
- and a pointer to the next node.

Memory is freed using `free()` whenever vectors are cleared or the program exits.

This design allows:
- flexible storage of an arbitrary number of vectors, and  
- efficient add/clear operations without fixed-size array limits.

---

## File I/O (v2.x)
Version 2 supports reading and writing vector data in CSV format:

Example file (`vector_test.csv`):


