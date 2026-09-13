#include <kernel/vmm.h>
#include <kernel/pmm.h>

#define PAGE_PRESENT 0x1
#define PAGE_WRITE   0x2

// Page directory aligned to 4KB boundary
__attribute__((aligned(4096))) static uint32_t page_directory[1024];
__attribute__((aligned(4096))) static uint32_t first_page_table[1024];

void vmm_init(void) {
    // Identity map the first 4MB of memory
    for (int i = 0; i < 1024; i++) {
        first_page_table[i] = (i * 0x1000) | PAGE_PRESENT | PAGE_WRITE;
    }

    // Set the first entry of the page directory to point to our page table
    page_directory[0] = ((uint32_t)first_page_table) | PAGE_PRESENT | PAGE_WRITE;

    // Clear the rest of the page directory entries
    for (int i = 1; i < 1024; i++) {
        page_directory[i] = 0 | PAGE_WRITE; // supervisor, read/write, not present
    }

    // Load page directory into CR3 and enable paging in CR0
    __asm__ volatile (
        "mov %0, %%cr3\n\t"
        "mov %%cr0, %%eax\n\t"
        "or $0x80000000, %%eax\n\t"
        "mov %%eax, %%cr0"
        : : "r"(page_directory) : "eax"
    );
}
