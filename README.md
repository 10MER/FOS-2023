# FOS - Educational OS for Ain Shams University Operating Systems Course

FOS is an educational OS for Ain Shams University Operating Systems Course CSW355, forked and refactored from MIT Operating Systems Lab 6.828. It was created by Dr. Mahmoud Hossam and currently maintained by Dr. Ahmed Salah.

## Milestones

The FOS project is divided into two milestones, each building upon the previous one:

## Milestone 1
In Milestone 1, the focus was on implementing the following functionalities within the FOS operating system:

1. **kmalloc()**: This function dynamically allocates space using the FIRST FIT strategy and maps it according to the memory management requirements.

2. **kfree()**: The kfree() function is responsible for deleting a previously allocated space from the Kernel Heap. It should remove all the pages associated with the allocated space.

3. **kheap_virtual_address()**: This function finds the kernel virtual address corresponding to a given physical address.

4. **kheap_physical_address()**: The kheap_physical_address() function enables finding the physical address corresponding to a given kernel virtual address. 

These functionalities are essential for memory management within the FOS operating system, allowing dynamic allocation, deallocation, and mapping of memory spaces.

---

## Milestone 2

1. **page_fault_handler_with_buffering()**: The fault handler is responsible for handling page faults that occur in the FOS kernel. The page fault handling process includes the following steps; allocating a new page in the main memory for the faulted page, loading the faulted page back into the main memory from the page file, if the page exists. If it's a new page, adding it to the page file. Implementing the MODIFIED CLOCK algorithm for page replacement in the fault handler.

2. **Malloc() [USER SIDE]**: Implementing the BEST FIT strategy to search the heap for suitable space to allocate based on the required allocation size. The space allocation should be aligned on a 4 KB boundary.

3. **allocateMem() [KERNEL SIDE]**: When allocating pages, they should not be added to the working set (i.e., not placed in main memory). Instead, they should be added to the page file. This allows the page fault mechanism to load the pages into memory when they are accessed.

4. **Free() [USER SIDE]**: Responsible for freeing the allocation of the given virtual address.

5. **freeMem_with_buffering() [KERNEL SIDE]**: The freeMem() function in the kernel is responsible for freeing each buffered page within the given range for a specific environment.

**BONUS: __env_free_with_buffering()**: Frees the entire environment  
