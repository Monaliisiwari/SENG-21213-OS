CC = gcc
ASM = nasm
LD = ld

CFLAGS = -m32 -ffreestanding -fno-stack-protector -fno-pie -nostdlib -Wall -Wextra -O2 -I./include
LDFLAGS = -m elf_i386 -nostdlib -T linker.ld

C_SOURCES = kernel/kernel.c kernel/vga.c kernel/keyboard.c kernel/process.c kernel/scheduler.c kernel/thread.c kernel/mutex.c kernel/pmm.c kernel/vmm.c kernel/idt.c
C_OBJS = $(patsubst kernel/%.c, build/%.o, $(C_SOURCES))

ASM_SOURCES = kernel/kernel_entry.asm kernel/switch.asm kernel/irq_stub.asm
ASM_OBJS = $(patsubst kernel/%.asm, build/%.o, $(ASM_SOURCES))

all: seng21213-os.img

build/%.o: kernel/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

build/%.o: kernel/%.asm
	@mkdir -p build
	$(ASM) -f elf32 $< -o $@

build/kernel.elf: $(C_OBJS) $(ASM_OBJS)
	$(LD) $(LDFLAGS) $(C_OBJS) $(ASM_OBJS) -o $@

build/kernel.bin: build/kernel.elf
	objcopy -O binary build/kernel.elf build/kernel.bin

boot/boot.bin: boot/boot.asm
	nasm -f bin boot/boot.asm -o boot/boot.bin

seng21213-os.img: boot/boot.bin build/kernel.bin
	dd if=/dev/zero bs=512 count=2880 of=seng21213-os.img 2>/dev/null
	dd if=boot/boot.bin conv=notrunc bs=512 count=1 of=seng21213-os.img 2>/dev/null
	dd if=build/kernel.bin conv=notrunc bs=512 seek=1 of=seng21213-os.img 2>/dev/null
	@echo "[IMG] seng21213-os.img ready"

run: seng21213-os.img
	qemu-system-i386 -drive format=raw,file=seng21213-os.img -m 32M

clean:
	rm -rf build seng21213-os.img
