# C++ Systems Programming Learning Journey

A structured, hands-on learning path focused on systems-level C++ programming. This repository documents my progress through multithreading, networking, design patterns, and inter-process communication.

## About

This project follows a mentor-guided roadmap covering:
- Parallel programming with threads and synchronization primitives
- Systems-level networking (TCP/UDP sockets, epoll, concurrent I/O)
- Software design patterns and MVC architecture with Qt
- Inter-process communication (shared memory, IceORYX)

## Roadmap

### Phase 1 - Multithreading 
- [ ] std::thread basics (create, join, detach)
- [ ] Mutex, lock_guard, unique_lock
- [ ] Condition variables and producer-consumer pattern
- [ ] std::async, std::future, std::promise
- [ ] Custom thread pool implementation
- [ ] Atomic operations and memory ordering
- [ ] Qt QThread integration

### Phase 2 - Networking
- [ ] TCP echo server (single and multi-client)
- [ ] Custom protocol design (message framing)
- [ ] UDP log collector
- [ ] Non-blocking I/O with select() and epoll
- [ ] Data pipeline project: Producer + Consumer + Monitor

### Phase 3 - Design Patterns + MVC
- [ ] Singleton, Observer, Factory, Strategy, Command patterns
- [ ] Builder and Adapter patterns
- [ ] Qt MVC application with SQLite backend
- [ ] Config file reader and logging system

### Phase 4 - IPC + Optimization
- [ ] POSIX shared memory and message queues
- [ ] Named pipes (FIFO)
- [ ] IceORYX zero-copy communication
- [ ] Profiling with perf, valgrind, gprof
- [ ] Memory management and custom allocators

## Tech Stack
- **Language:** C++17/20
- **Build System:** CMake
- **GUI Framework:** Qt6
- **OS:** Linux (Mint)
- **Tools:** g++ 13.3, Git, Valgrind, perf

## Resources
- *C++ Concurrency in Action* - Anthony Williams
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/)
- [cppreference.com](https://en.cppreference.com/)
- [learncpp.com](https://www.learncpp.com/)

## License
This project is for educational purposes.