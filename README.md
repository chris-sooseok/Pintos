# Pintos — CSE 421/521 Operating Systems

An implementation of the [Pintos](https://web.stanford.edu/class/cs140/projects/pintos/pintos.html)
instructional operating system, completed for CSE 421/521 (Operating Systems) at the University at
Buffalo. Pintos is a small OS kernel for the 80x86 architecture that boots on real hardware and on
the Bochs and QEMU emulators.

The work is organized into three phases, each building on the last:

| Phase | Area | Status |
|-------|------|--------|
| Phase 1 | Threads — alarm clock, priority scheduling | Complete |
| Phase 2 | User programs — argument passing, process control (`exec`/`wait`) | Complete |
| Phase 3 | User programs — system call interface | In progress |

### What was implemented

**Phase 1 — Threads (`src/threads`, `src/devices`)**
- Reworked `timer_sleep()` to be non-busy-waiting: sleeping threads are parked on a dedicated
  sleep list with a wake-up tick and unblocked from the timer interrupt handler.
- Priority scheduling: the ready list is kept in priority order and the scheduler preempts the
  running thread whenever a higher-priority thread becomes runnable (on creation, unblock, or a
  `thread_set_priority()` that lowers the current thread below a ready one).

**Phase 2 — User programs (`src/userprog`)**
- Program loading with argument passing: the command line is tokenized and `argc`/`argv` are laid
  out on the user stack per the 80x86 calling convention before the process starts.
- Process lifecycle: `process_execute()` / `process_wait()` synchronize a parent with its child's
  load result and exit status.

**Phase 3 — System calls (`src/userprog/syscall.c`)**
- User/kernel boundary handling and the syscall dispatch table — under active development.

### Repository layout

```
src/
  threads/     # kernel threads, scheduler, synchronization
  userprog/    # user program loading, syscalls
  devices/     # timer, keyboard, disk, VGA drivers
  filesys/     # basic file system
  lib/         # freestanding C library (kernel + user)
  tests/       # the course test suite
  utils/       # pintos run script and helpers
docs/          # Pintos manual, Docker setup, per-assignment instructions
```

### Building and running

A toolchain (i386 cross-compiler + Bochs/QEMU) is required. A ready-made environment is provided
in `docs/Dockerfile`; see `docs/03_Docker_Setup_Guide.pdf`.

```bash
# Phase 1 — threads
cd src/threads
make
pintos -- -q run alarm-multiple      # run a single test
make check                            # run the phase's test suite

# Phase 2 / 3 — user programs
cd src/userprog
make check
```

### Reference

`docs/01_Pintos_Manual.pdf` and the per-phase instruction PDFs under `docs/p1` and `docs/p2`.

### Acknowledgements

Pintos was developed at Stanford University. This repository contains coursework built on the
UB CSE 421/521 distribution and was completed as a team assignment.
