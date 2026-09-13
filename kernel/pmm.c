#include <kernel/pmm.h>

#define PMM_MAX_SIZE 0x2000000 // 32MB physical memory
#define BLOCK_SIZE 4096       // 4KB per frame
#define BLOCKS_PER_BYTE 8
#define TOTAL_BLOCKS (PMM_MAX_SIZE / BLOCK_SIZE)
#define BITMAP_SIZE (TOTAL_BLOCKS / BLOCKS_PER_BYTE)

static uint32_t pmm_bitmap[BITMAP_SIZE / sizeof(uint32_t)];
static uint32_t used_blocks_count = 0;

static void set_block(uint32_t bit) {
    pmm_bitmap[bit / 32] |= (1 << (bit % 32));
}

static void unset_block(uint32_t bit) {
    pmm_bitmap[bit / 32] &= ~(1 << (bit % 32));
}

static int test_block(uint32_t bit) {
    return pmm_bitmap[bit / 32] & (1 << (bit % 32));
}

void pmm_init(void) {
    for (uint32_t i = 0; i < (BITMAP_SIZE / sizeof(uint32_t)); i++) {
        pmm_bitmap[i] = 0; // All frames free initially
    }
    used_blocks_count = 0;
}

void *pmm_alloc_frame(void) {
    for (uint32_t i = 0; i < TOTAL_BLOCKS; i++) {
        if (!test_block(i)) {
            set_block(i);
            used_blocks_count++;
            return (void *)(i * BLOCK_SIZE);
        }
    }
    return 0; // Out of memory
}

void pmm_free_frame(void *frame) {
    uint32_t frame_addr = (uint32_t)frame;
    uint32_t bit = frame_addr / BLOCK_SIZE;
    unset_block(bit);
    used_blocks_count--;
}
