# SENG21213-OS

## Course

SENG 21213 - Computer Architecture & Operating Systems

## Project

x86 Operating System Development

This repository contains the source code developed for the SENG 21213 Operating Systems assignment.

## Project Stages

| Stage | Topic |
|---|---|
| Stage 0 | Bootloader, VGA, keyboard and kernel foundations |
| Stage 1 | Process management and scheduling |
| Stage 2 | Threads and synchronization |
| Stage 3 | Physical and virtual memory management |
| Stage 4 | Final kernel development |

## Project Structure

```text
SENG-21213-OS/
├── boot/
│   └── boot.asm
├── include/
│   ├── types.h
│   ├── process.h
│   ├── scheduler.h
│   └── kernel/
├── kernel/
│   ├── kernel.c
│   ├── kernel_entry.asm
│   ├── process.c
│   ├── scheduler.c
│   ├── thread.c
│   ├── mutex.c
│   ├── pmm.c
│   ├── vmm.c
│   ├── idt.c
│   ├── irq_stub.asm
│   ├── switch.asm
│   ├── timer.c
│   ├── keyboard.c
│   └── vga.c
├── Makefile
├── linker.ld
├── .gitignore
└── README.md