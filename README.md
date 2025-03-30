# Memory Allocator

## Project Description
The goal of this project is to create a memory manager for managing the heap of a custom program. The project implements custom versions of the `malloc`, `calloc`, `free`, and `realloc` functions. Additionally, it provides utility functions for monitoring the state, consistency, and defragmentation of the heap.

### Main Functionalities
- Standard allocation/deallocation tasks according to the `malloc` API, ensuring compatibility with the expected behavior from the caller's perspective.
- Resetting the heap to the initial state when the program started.
- Automatic heap region expansion by generating operating system requests when necessary.
- Implementation of memory fences (guards) to detect memory access errors (e.g., buffer overflows).

### Memory Layout
The memory area managed by the allocator is organized as a continuous sequence of pages, each 4KB in size. Memory blocks include:
- Control structure (header)
- Top fence (head)
- User block (data)
- Bottom fence (tail)

Fences are positioned such that there are no empty bytes between the user block and the fences, ensuring error detection through guard violation.

### Implemented Functions
- `heap_setup()`: Initializes the heap memory area.
- `heap_clean()`: Cleans up and releases all allocated memory.
- `heap_malloc()`, `heap_calloc()`, `heap_realloc()`, `heap_free()`: Standard memory management functions following the GNU C Library (glibc) API.
- `heap_get_largest_used_block_size()`: Retrieves the size of the largest allocated block.
- `get_pointer_type()`: Determines the type of the given pointer within the heap.
- `heap_validate()`: Checks the consistency of the heap.

### File Structure
- `heap.c`, `heap.h`: Core implementation of the memory allocator.
- `custom_unistd.h`, `custom_unistd.c`: Custom implementation of system functions (e.g., `sbrk`).
- `main.c`: Test file for custom memory functions.
- `unit_helper_v2.c`, `unit_helper_v2.h`: Unit testing helpers.
- `unit_test_v2.c`: Comprehensive unit tests.
- `tested_declarations.h`: Declarations of required functions for testing.
- `rdebug.c`, `rdebug.h`: Debugging utilities.
- `memmanager.c`: Memory management utilities.
- `CMakeLists.txt`, `makefile`: Build and execution scripts.

### Build Instructions
To build the project, use the following commands:
```
mkdir build
cd build
cmake ..
make
```

### Usage
After building, run the executable:
```
./memory_allocator
```

### Testing
Unit tests are located in `unit_test_v2.c`. Run the tests as follows:
```
./unit_tests
```

### License
This project is licensed under the MIT License.

### Author
Miraslau Alkhovik

