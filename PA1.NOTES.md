Start from `main()` in `init.c`.

`thread_init()` - creates a thread struct for the initial thread via `init_thread`.
>`init_thread` does
1. initialize `thread struct`
2. add thread into `all_list`

This initial thread exits in the end of main() calling `thread_exit`, which calls `schedule` that will lead to scheduling the idle thread. This main thread will be 'destroyed' once the code reaches `thread_schedule_tail` at which point the idle thread is running

`thread_start()` - creates the idle thread via `thread_create`, and enables preemptive scheduling
> `thread_create` does
1. allocate thread page & initialize `thread struct`
2. decides the function of this thread
3. creates fake 'context' [1] used for new thread when running the first time
4. calls `thread_unblock`
>> `thread_unblock` does
> 1. push thread into `ready_list` and set it ready

At this point, `schedule` called by the main thread will `thread_switch` from the main thread to the idle thread. The idle thread will call `idle` function, whichs calls `thread_block` in a loop to let someone else run


[1] fake 'context'
When a new thread is created, `thread_create` creates a fake 'context' used for that thread to run for the first time. When `thread_switch` happens, it first saves the `CUR` thread's registers and set `cur->stack = ESP`[2]. Then, it restores the `NEXT` registers and set `EIP` = `next->stack` [3]. However, when a new thread runs, its `ESP` doesn't have the return address because it has never gone through context switch. Thus, when `thread_create` is called, it assigns `ef->eip = kernel_thread` and `sf->eip = switch_entry` for the new thread, so that it can reach to `switch_thread_tail()` to finalize context switching. These fake context frames are used only once for the first time running thread.

When non-first time running thread resumes, we already have stored its `ESP` in the stack, so that `switch_thread` can always set the `ESP` to `EIP` for the CPU to resume the thread.

[2,3] EIP and ESP 
EIP is the CPU's instruction pointer, and ESP is the CPU's thread stack pointer.
EIP points to whatever instruction the CPU is to execute, and ESP points to the stack of the current thread. Since the state of thread context is saved in threads so that the CPU can pick up and resume, each thread 'owns' its ESP pointing to the top of the stack.

[2] cur thread
When the current thread is at `switch_threads` entry, the stack looks like this
ESP ->  [ RET_ADDR_into_schedule ]   <-- pushed by CALL (4 bytes)
        [ cur ]                      <-- pushed by caller (4 bytes)
        [ next ]                     <-- pushed by caller (4 bytes)
        ...
EIP = switch_threads

> When the CPU executes `call switch_threads` at the machine level, `call` pushes return address (next instruction after the call), that is RET_ADDR_into_schedule below, onto the stack and set `EIP = switch_threads`

Then, `switch_threads` saves the current thread state
ESP ->  [ saved_edi ]
        [ saved_esi ]
        [ saved_ebp ]
        [ saved_ebx ]
        [ RET_ADDR_into_schedule ]
        [ cur ]
        [ next ]
        -- schedule() stack frame --
        -- thread_yield() stack frame --
        -- foo() stack frame --         <-- assuming this thread was running foo()

Then, `cur->stack = ESP` so that the CPU can resume this thread from this stack later
(by restoring the registers again and return from schedule) [4]. Thus, when it resumes it returns from `switch_threads`, that will trace back to `foo()`, resuming whatever work it has been doing.

[3] next thread (first time running)
When a new thread is to be scheduled, their stack looks like this
(LOW addr)  <-- t->stack points here (and later becomes ESP)
ESP ->  [ sf.edi  ]   (4 bytes)  placeholder
        [ sf.esi  ]   (4 bytes)  placeholder
        [ sf.ebp  ]   (4 bytes)  placeholder
        [ sf.ebx  ]   (4 bytes)  placeholder
        siwtch_threads_frame
        [ sf.eip  ]   (4 bytes)  = switch_entry  <-- ret target 1
        [ sf.cur  ]   (4 bytes)  fake/unused
        [ sf.next ]   (4 bytes)  fake/unused
        switch_entry_frame
        [ ef.eip  ]   (4 bytes)  = kernel_thread <-- ret target 2
        kernel_thread_frame
        [ kf.eip      ] (4 bytes) = NULL         <-- “return addr” for kernel_thread 
        [ kf.function ] (4 bytes) = function
        [ kf.aux      ] (4 bytes) = aux
(HIGH addr)

`switch_threads` restores the registers. Then, when it returns `ret`, that jumps to `switch_entry` which calls `thread_schedule_tail` and runs the `kf.function`. After this new thread runs, and gets switched out, its stack will resemble to the diagram in the current thread case.


[4] `ret`
Once we have restored registers, ESP points at the return address that was pushed earlier by the CPU's `call switch_threads`. Say `ESP = 0xABCD1234` and memory at ESP contains `0x1234ABCD`, meaning 'return to schedule()' is at `0x1234ABCD`. 

ESP = 0xABCD1234

ESP -> +------------------+  0xABCD1234
       | 0x1234ABCD       |  <-- return address (into schedule)
       +------------------+  0xABCD1238
       | 0xDEADBEEF       |  cur arg
       +------------------+  0xABCD123C
       | 0xCAFEBABE       |  next arg
       +------------------+  0xABCD1240
       | ...              |
       +------------------+

At this moment, EIP is still executing inside `switch_threads` and at the `ret`. Then, when `ret` returns, it does `EIP = *(unit32_t*)ESP` and `ESP = ESP + 4`.

After ret:

EIP = 0x1234ABCD (now executing schedule() at that instruction)
ESP = 0xABCD1238

ESP -> +------------------+  0xABCD1238
       | 0xDEADBEEF       |  cur arg (still there until cleaned)
       +------------------+  0xABCD123C
       | 0xCAFEBABE       |  next arg
       +------------------+  0xABCD1240
       | ...              |
       +------------------+