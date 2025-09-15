#include "stdint.h"
#include "util.h"
#include "memoryx.h"
#include "stdio.h"
#include "multiboot.h"

static uint32_t pageFrameMin;
static uint32_t pageFrameMax;
static uint32_t totalAlloc;
uint32_t initial_page_dir[1024];

#define NUM_PAGES_DIRS 256
#define NUM_PAGE_FRAMES (0x100000000 / 0x1000 / 8)

uint8_t physicalMemoryBitmap[NUM_PAGE_FRAMES / 8]; //Dynamically, bit array

static uint32_t pageDirs[NUM_PAGES_DIRS][1024] __attribute__((aligned(4096)));
static uint8_t pageDirUsed[NUM_PAGES_DIRS];

void pmm_init(uint32_t memLow, uint32_t memHigh){
    pageFrameMin = CEIL_DIV(memLow, 0x1000);
    pageFrameMax = memHigh / 0x1000;
    totalAlloc = 0;

    memset(physicalMemoryBitmap, 0, sizeof(physicalMemoryBitmap));
}


void initMemory(uint32_t memHigh, uint32_t physicalAllocStart){
    initial_page_dir[0] = 0;
    invalidate(0);
    initial_page_dir[1023] = ((uint32_t) initial_page_dir - KERNEL_START) | PAGE_FLAG_PRESENT | PAGE_FLAG_WRITE;
    invalidate(0xFFFFF000);

    pmm_init(physicalAllocStart, memHigh);
    memset(pageDirs, 0, 0x1000 * NUM_PAGES_DIRS);
    memset(pageDirUsed, 0, NUM_PAGES_DIRS);
}

void invalidate(uint32_t vaddr){
    asm volatile("invlpg %0" :: "m"(vaddr));
}
