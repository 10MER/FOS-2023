#include <inc/memlayout.h>
#include <kern/kheap.h>
#include <kern/memory_manager.h>

//NOTE: All kernel heap allocations are multiples of PAGE_SIZE (4KB)

struct kHeapData{
    int ksize;
    uint32 *address;
}
kheap_data_arr[(KERNEL_HEAP_MAX - KERNEL_HEAP_START) / PAGE_SIZE];
int index = 0;

void* kmalloc(unsigned int size)
{
	//TODO: [PROJECT 2023 - MS1 - [1] Kernel Heap] kmalloc()

	size = ROUNDUP(size, PAGE_SIZE);
    int freeSpace = 0;
    int pageCounter = 0;
    int startingAddress = -1;

    for (int i = KERNEL_HEAP_START; i < KERNEL_HEAP_MAX; i += PAGE_SIZE){
        uint32 *ptr_page_table = NULL;
        struct Frame_Info *frame_info_ptr = get_frame_info(ptr_page_directory, (void*)i, &ptr_page_table);
        if (frame_info_ptr == NULL){
            freeSpace += PAGE_SIZE;
            pageCounter++;
            if (freeSpace >= size){
                startingAddress = i - (pageCounter - 1) * PAGE_SIZE;
                break;
            }
        }
        else{
            freeSpace = 0;
            pageCounter = 0;
        }
    }

    if (startingAddress == -1){
        return NULL;
    }

    kheap_data_arr[index].address = (uint32*)startingAddress;
    kheap_data_arr[index].ksize = size;
    index++;

    int endingAddress = startingAddress + size;

    for (int i = startingAddress; i < endingAddress ; i += PAGE_SIZE)
    {
        struct Frame_Info *frame_info_ptr = NULL;
        allocate_frame(&frame_info_ptr);
        map_frame(ptr_page_directory, frame_info_ptr, (void*)i, PERM_PRESENT|PERM_WRITEABLE);
    }

    return (void*)startingAddress;
}

void kfree(void* virtual_address){
	//TODO: [PROJECT 2023 - MS1 - [1] Kernel Heap] kfree()

	int allocatedPages = -1;
	int required_index;
	for(int i = 0; i < index; i++) {
		if(virtual_address == kheap_data_arr[i].address) {
			allocatedPages = kheap_data_arr[i].ksize;
			required_index = i;
			break;
		}
	}

	if(allocatedPages==-1)
		return;

	for(int i = 0; i < (allocatedPages/PAGE_SIZE) ; i++){
		unmap_frame(ptr_page_directory, (void *)virtual_address);
		virtual_address += PAGE_SIZE;
	}

	kheap_data_arr[required_index].address = (uint32*)-1;
	kheap_data_arr[required_index].ksize = -1;
}


unsigned int kheap_virtual_address(unsigned int physical_address){
	//TODO: [PROJECT 2023 - MS1 - [1] Kernel Heap] kheap_virtual_address()

	uint32 *ptr_page_table = NULL;
	uint32 frameNum = physical_address >> 12;
	uint32 tableEntry;

	for (uint32 virtA = KERNEL_HEAP_START; virtA < KERNEL_HEAP_MAX; virtA=virtA+PAGE_SIZE){
		get_page_table(ptr_page_directory, (uint32 *)virtA, &ptr_page_table);
		if (ptr_page_table != NULL){
			tableEntry = ptr_page_table[PTX(virtA)];
			if ((tableEntry&(~PERM_PRESENT)) != tableEntry){
				uint32 cFrame = tableEntry >> 12;
				if (cFrame == frameNum){
					return virtA;
				}
			}
		}
	}

	return 0;
}


unsigned int kheap_physical_address(unsigned int virtual_address)
{
	//TODO: [PROJECT 2023 - MS1 - [1] Kernel Heap] kheap_physical_address()

	uint32 *ptr_page_table = NULL;
	uint32 physAddress;
    struct Frame_Info *frame_info_ptr = get_frame_info(ptr_page_directory, (void *)virtual_address, &ptr_page_table);
    physAddress = to_physical_address(frame_info_ptr);
	if ((ptr_page_table[PTX(virtual_address)]&PERM_PRESENT) == 0){
		return 0;
	}
	return physAddress;
}


//=================================================================================//
//============================== BONUS FUNCTION ===================================//
//=================================================================================//
// krealloc():

//	Attempts to resize the allocated space at "virtual_address" to "new_size" bytes,
//	possibly moving it in the heap.
//	If successful, returns the new virtual_address, in which case the old virtual_address must no longer be accessed.
//	On failure, returns a null pointer, and the old virtual_address remains valid.

//	A call with virtual_address = null is equivalent to kmalloc().
//	A call with new_size = zero is equivalent to kfree().

void *krealloc(void *virtual_address, uint32 new_size)
{
	//panic("krealloc() is not implemented yet...!!");

	return NULL;

}
