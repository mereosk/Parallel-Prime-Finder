# Project Structure and Procedures

The program is divided into several modules: `include`, `modules`, and `programs`. Below is an overview of their purpose and components:

## Folder Descriptions
### 1. `programs`
Contains the source files responsible for the hierarchical structure of the project:
- **Root Program:** `myprime.c`  
  Calls the intermediate programs.
- **Intermediate Program:** `inter.c`  
  Acts as a bridge and invokes the worker programs.
- **Worker Programs:** `prime1.c`, `prime2.c`, `prime3.c`  
  Perform computations assigned by the intermediates.

### 2. `modules`
Contains reusable components and utilities:
- **Sorted List Structure:** Implemented in `ADTBList.c` for handling results efficiently.  
  This data structure is used for storing prime number ranges in sorted order for efficient processing.  
- **Utility Functions:** Implemented in `func.c` to provide common functionality across the project.  
  Functions here handle tasks such as range generation and process communication.  

### 3. `include`
Holds headers and type definitions:
- **Module Initializations:** Headers for `ADTBList` and `func`.
- **Common Types:** `common_types.h` includes various `typedefs` used throughout the project, such as for ranges, linked list elements, and worker metadata.

---

## Execution Flow
### **1. Root Program (`myprime.c`)**  
The root program is responsible for initializing the project and coordinating the other modules. It:  
- Divides the interval into smaller ranges.  
- Passes those ranges to the intermediate program(s).  
- Collects results back from intermediates and processes them for final output.  

### **2. Intermediate Program (`inter.c`)**  
This module serves as the communication and control layer between the root and the worker programs. It:  
- Receives ranges from the root program.  
- Further splits them into smaller intervals if needed.  
- Assigns tasks to worker programs.  
- Collects and organizes results from the workers.

### **3. Worker Programs (`prime1.c`, `prime2.c`, `prime3.c`)**  
These programs are tasked with the actual computation of prime numbers within the assigned range. They:  
- Receive a range from the intermediate program.  
- Use a prime-checking algorithm to identify and store primes.  
- Communicate the results back to the intermediate program.  

---

## Implementation Features
1. **Concurrency & Parallelism:**  
   - The project uses **multiprocessing** to divide computational work across multiple processes.  
   - Workers handle different sub-ranges of the interval concurrently for improved performance.  

2. **Inter-Process Communication (IPC):**  
   - Signals and **pipes** are used to coordinate communication between processes efficiently.  

3. **Data Handling:**  
   - A **sorted linked list** (via `ADTBList`) ensures results are ordered and stored without duplicates.  

4. **Error Handling:**  
   - Signal handling mechanisms are implemented to manage unexpected events or process termination.  

---

## Makefile
The `Makefile` in the `programs` directory automates common tasks like compilation, execution, and cleanup.

### Commands
- **Build:**  
  Compile all necessary files into executables:
  ```bash
  make
- **Run:**  
  Execute the `myprime` program:
  ```bash
  make run
- **Clean:**
  Remove all compiled files and clean up:
  ```bash
  make clean
