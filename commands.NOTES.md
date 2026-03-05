

### GDB
pintos --gdb -- run [testname] \
pintos --bochs -- run [testname] \g
pintos-gdb kernel.o \
target remote localhost:1234 \

### GDB commands
c: continue \
n: next line \
s: step in \
finish: out of step in \
list: print lines of code \
ex. `list 0xsome_address`
dumplist list(name of list) type element: print types of elements in list \
ex.`dumplist &all_list thread allelem` prints all threads \
p: prints \
ex. `p thread_current()-> name` prints the name of current thread \
ex. `p thread_current()-> priority` prints the priority of current thread \
backtrace: backtrace stack that shows how we reached the current position \
btthreadlist list(name of list) element: trace a particular thread \
ex. `btthreadlist &all_list allmen`: prints trace of all threads \

info: shows info commands \
info registers: show details of registers \ 

