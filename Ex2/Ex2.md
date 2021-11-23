### Aufgabe 1
a)
bandwidth: Total amount of work done in a given time. 

* Maximum rate at which information can be transfered
* Aggregate bandwidth - total data bandwidth supplied by network
* Effective bandwidth - fraction of aggregate bandwidth delivered to an application

Latency: Time between start and completion of an event

Sending overhead + time of flight + (Packet size / Bandwidth) + receiving overhead \
b)TODO

Bandwidth: Shared-media networks

Latency: Switched-media networks

### Aufgabe 2

| Kategorie | Ring | 2D-Mesh | 2D-Torus | Vollverbunden |
| --- | --- | --- | --- | --- |
| Bisection width | 2 | 8 |  16| 1024 |
| Anzahl der Verbindungen zwischen allen Switches |64 | 112 | 128 | 2016 |

### Aufgabe 3
![](1.png) \

a) \
Diameter: 9 \
Knotenpaar: e.g. 0-21, 1-21, 2-21, 3-21, 4-21

Bisection Bandwidth: 300MB/s \
Group1: 0,1,2,3,4,5,21,23,24,30 \
Group2: 10,11,12,13,14,15,20,22,31,32 

Network Degree: 7 \
node: 5, 14

Node connectivity: 1 \
Node: 20 or 22 or 23 or 24 or 11 or 13 or 5

b) TODO 
1. Bisection bandwidth is too low(300 MB/s), but several links are 1 Gb/s, which means the throughput is low. 


2. Diameter is high, so that there is big distance for message transfer, which means high latency.
3. Node connectivity is low, which means the network is easy to disconnected.

### Aufgabe 4
a) \
Data dependence exists. \
Example: \
a[1] = a[0]+1 = 1 \
a[3] = a[1]+1 = 2 

a[7] is computed using a[3]'s new value: a[7] = a[3]+1 = 3 \
a[7] is computed using a[3]'s old value: a[7] = a[3]+1 = 4 \
The read of a[3] depends on the write of a[3] in parallel.

change :

```c
pragma omp parallel for redction(+:sum)
```

b) \
Data dependence exists. \
Example: 

| i = 0| i = 1|
|----|----|
|j = 0||
|...||
|j = 1||
|a[i][j]=a[i][j]+1=1|j = 0|
|j++||
|j=1|
|a[i][j]=a[i][j]+1=2||

use collapse clause:
```c
#pragma omp parallel for collapse(2)
     for(i = 0;i<100;i++)
         for(j = 0;j<100;j++)
             a[i][j] = a[i][j] + 1;
```

c) \
no data dependence


### Aufgabe 5
a) 
A thread includes: Thread ID, Program counter, Register set, Stack

b) 
1. Each process has its own address space, but threads in the same process share the address space.

2. For this reason, only inter-process communication is actually required. The threads of the same process share the address space. Synchronization/mutual exclusion is required to protect shared global variables.

3. The inter-process communication, whether it is a signal, a pipe, or a shared memory, is guaranteed by the operating system.

4. And network in Communication library also can be used for communication between different devices and processes.  E.g. Socket.

c) Firstly, we need to know:\
1.Process have its own address space, and other processs can not direct access. Threads in same process have the same address space.\
2.Communication between processes requires specific means for interprocss communication (IPC) to allow processes to exchange data(pipe, shared-mem,sockets, etc). But threads can communicates via shared variables.\
When considering process running on distinct and potentially distant processors, parallelization with the help of processes is larged used, which is more scalabale. For instance, client-server applications. \
But when process run on a single processor, parallelization with the help of processes has an important overhead and a communication may require several time consuming data copies. Then we should use parallelization with the help of threads, because it is more space efficient and switch context cheaper.

d) Because MPI is suitable for data exchange between multiple address spaces, the current mixed programming model is generally MPI+X, that is, MPI is used for inter-node communication, and within the node (single address space), a programming model suitable for single address space is adopted. Corresponds to the hierarchies within and between nodes (multiple address spaces) of high-performance computer nodes.