[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/TrWxYekB)
# Pintos Source

This is the starter Pintos source for CSE 521 Operating Systems at UB. This will be updated for each project.

## Notes

### Applied updates:
- PA0
- PA1

### Design documentation:
* The template appears to be written in GNU Texinfo, documentation can be found [here](https://www.gnu.org/software/texinfo/manual/texinfo/html_node/Command-Syntax.html)
* The given texi2html screws up the ``@xref{}`` commands because they only gave us a snippet of the manual's source code so some node are missing

* Once compiled with ``./texi2html`` you can view the document in firefox. It's just the pintos project 1 outline

* We must have our own design document wrapped with our submission base on the given ``threads.tmpl``. Details under OUR TASK.

### UBLearns links:
* Phase1 [Designdoc submission](https://ublearns.buffalo.edu/d2l/le/content/365441/viewContent/5332044/View)
* All documents for the projects are in [`Documents`](Documents/)
* Submissions are built according to each pa's submission instructions PDF

### Docker commands:
* docker run --name <new_container_name> --mount type=bind,src=</path/to/host/folder>,dst=</path/to/container/mapping> -it <image_name>:<build>
* docker container list
* docker container start
* docker exec -it <container_name> bash
* docker container stop <container_name>
* docker rm <container_name>

### Github ssh keys:
* [Documentation](https://docs.github.com/authentication/connecting-to-github-with-ssh)
* $ ls -al ~/.ssh # Lists the files in your .ssh directory, if they exist
* h-keygen -t ed25519 -C "your_email@example.com" # Generate a new one if empty
* cat ~/.ssh/id.*.pub # To copy and paste for git settings

## First Read through
### Implementation
Alarm-clock:
* To protect shared state of the kernel threads and interrupt handlers, interrupts should be toggled before and after access with minimum distance between. See `synch.c`, `interrrupt.c`, and `interrupt.h`.
* No busy waiting
* Use pintos -r to run if things seem slow
* Re-implement ``timer_sleep()`` to avoid busy-waiting

Priority Scheduling: **THIS IS PHASE 1**
* Running threads should immediately yeild to *higher* priority threads that appear in the ready queue
* We talked of implementing the ready queue as a sorted list s.t. the head is always the highest priority thread (linked list with mutable prio for donation?)
* Highest priority threads waiting on a synch primitive should wake *first*
* Since a thread may raise and lower its priority at any time, any *decrease* should result in an imediate yeild
* Priority donation from higher threads to lower threads that hold a required resource
* Should account for multiple priorities to be donated
* Should allow for nested donations, within a reasonable limit (~8 levels)
* Priority donation for locks (others not needed)
* Implement ``thread_set_priority()`` and ``thread_get_priority`` to get and set thread priorities

Should pass [`priority-change`](src/tests/threads/priority-change.c), [`priority-fifo`](src/tests/threads/priority-fifo.c), and [`priority-preempt`](src/tests/threads/priority-preempt.c)

Advanced Scheduler:
* Implement a multi-level feedback queue like [4.4BSD](https://web.stanford.edu/class/cs140/projects/pintos/pintos_7.html)
* Should be selectable with ``-mlfqs`` as a kernel option see ``threads/thread.h``
* Threads will no longer control their own priorities, ignoring calls to the getter, setter, and priority argument of ``thread_create()``

### Design documentation:
Front matter:
* Group names and emails ; DONE
* List of all outside references / any comments for graders

For each of ALARM, PRIO-SCHED, and ADVANCED-SCHED:
* Declarations and Brief descriptions of each new type, struct, global/static variable, etc
* Answer the embedded questions about the behavior of the project
* Our rationale for the final structure and implementation

Plus the survey at the end



