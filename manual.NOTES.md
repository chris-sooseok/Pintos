# The Loader and kernel initialization
PC BIOS loads the loader -> Loader finds the kernel on disk and reads it into memory at physical address 128KB -> The loader's final job is to extract the entry point, which is `start()` in `threads/start.S` from the kernel image and tranfer control to it

The job of start() is to switch the CPU from legacy 16-bit 'real-mode' into the 32-bit 'protected-mode' used by all modern 80x86 OSs.

start() does these in step
1. CPU initialization
2. Creates a basic page table for startup code at virtual address `LOADER_PHYS_BASE`, which defaults to 3 GB that is equivalent to the kernel virtual memory starting point, that maps to the base of physical address
3. After the basic page table is initialized, we loads the CPU's control registers to turn on protected mode and paging, and set up the segment registers. The final step is to call `main()`

When `main()` starts, we're in 32-bit protected mode with paging enabled, and hardly anything else is ready. 

main() does these in step
1. bss_init()
2. read_command_line()
3. thread_init() that creates an early initial thread
4. palloc_init() that enables the kernel page allocator
5. paging_init() that set ups a page table for the kernel
6. tss_init() and gdt_init() for project 2
7. intr_init(), timer_init(), and kdb_init() which initializes the interrupt system, prepares timer interrupts, and keyboard interrupts
- intr_init() sets up the CPU's IDT, as well as initializes the PICs for interrupt handling which controls external handling
8. input_init() that merges serial and keyboard into into one stream
9. exeception_init() and syscall_init() for project 2
10. Now that interrupts are set up, we can start the scheduler with `thread_start()`, which creates the idle thread and enables interrupts.
 - The idle thread is scheduled when no other thread is ready
11. With interrupts enabled, interrupt-driven seial port I/O becomes possible, so use `serial_init_queue()` to switch to that mode
12. Finally, `timer_calibrate()` calibrates the timer for accurate short delays

Boot is complete, and `main()` calls `thread_exit()` which allows any other running threads to continue.

# Thread
`struct thread` will be used for both a thread itself and user process.

In creation of a thread, every `struct thread` occupies the begining of its own page of memory. The rest is for the thread's stack.

In creation of a process, `struct thread` allocates a new page directory and page table for the process, which `pagedir` points to its page directory.

# Page Table, Virtual Memory, and Physical Memory
The page allocator divides the memory into two pools, called the kernel and user pools. Each pool's usage is tracked with a bitmap, one bit per page in the pool. A request to allocate n pages scans the bitmap for n consecutive bits set to false.

A 32-bit virtual address can be devided into a 20-bit page number and a 12-bit page offset. Virtual memory in Pintos is divided into user virtual memory and kernel virtual memory. The boundary between them is `PHYS_BASE`.

User virtual memory: 0 up to `PHYS_BASE` (~3GB) by default \
Kernel virtual memory: `PHYS_BASE` up to 4GB

## Pintos uses the normal 32-bit x86 paging structure
1. Page Directory \
A single 4KB page that contains 1024 PDEs (page directroy entries). A single 4KB page that contains 1024 entries, each entry = 4bytes, called PDEs (page directory entries). Each PDE can point to 4MB of virtual address space (thus total 4GB of memory space)

2. Page Table \
Another 4KB page that contains 1024 entries called PTEs (page table entries). Each PTE contains a physical frame base (the starting physical address in a 4KB frame), and plus flags (present, writable, user/kernel, etc). This maps one 4 KB virtual page to a physical frame.

3. Entries (PDE / PTE) \
An 'entry' is just one slot in those arrays:
- a PDE either points to a page table (PT) or is 'not present'.
- a PTE either points to a physical frame base or is 'not present'.

## How one virtual address gets translated into physical memory
A 32-bit virtual address is split into 3 fields:
- bits 31..22 = Page Directory index (10 bits)
- bits 21..12 = Page Table index (10 bits)
- bits 11..0 = offset within the 4KB page (12 bits)

And the CPU does this exact walk:
1. Use bits 22..31 to pick a `PDE`. If `PDE` not present -> page fault.
2. PDE points to a `PT`. Use bits 12..21 to pick a `PTE`. If `PTE` not present -> page fault
3. Take the physical frame base from the PTE and add the offset to get the final physical address

Thus, Physical address = frame_base + offset. PD index and PT index exist to help find the right PTE (virtual-side lookup)

Each user process has its own user mappings (its own PD/PT setup). The CPU switches to another process by switching the CPU's page-directory base register (via `pagedir_activate()` - for project2).


# Extra Notes
Pages and blocks may not be allocated from interrupt context, but they may be freed.
When a page or block is freed, all of its bytes are cleared to 0xcc for debugging purpose.

Blocks returned by `malloc()` are obtained from the kernel pool, whereas `palloc()` can allocate pages either from the kernel or user pool. Also, `malloc()` is layered on top of `palloc()`, meaning its blocks come from pages. Most small block allocations don't require a new page at all because they are satisfied using part of a page already allocated.

`PD` and `PT` live in the kernel memory.

## QEMU and Bochs
Both are machine emulators that simulate hardware environment. They are used to boot the Pintos kernel.

QEMU is a fast x86 emulator. It is usually faster, great for automation, and GDB debugging

Bochs is an x86 PC emulator that focuses on correctness and detailed instrumentation (it simulates hardware very precisely). It is useful when you want every detailed debugging (because it's slower but can be more faitful and verbose).
Instrucmentation: extra internal tracing/logging support that helps you observe what the emulated CPU/devices are doing

## Testing

`make check` in `build` for complete testing

`make check` will select the faster simulator (Bochs or QEMU) by default. You can override this by specifying `SIMULATOR=--bochs` or `SIMULATOR=--qemu`.

A given test `t` writes its output to `t.output`. and writes the verdict to `t.result`. To run and grade a single test, make the ‘.result’ file explicitly from the
`build` directory, e.g. make tests/threads/alarm-multiple.result.

`make check VERBOSE=1` to observe test feedback during runtime

## Design Document

`Data Structus` Any new or change declarations in design within 25 words description

`Algorithms` Explains how your code works to implement the requirements

`Synchonization` How you chose to synchronize kernel's complex, multithreaded program.

`Rationale` Justify your design decisions