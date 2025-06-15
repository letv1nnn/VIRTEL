# ***VIRTEL Interpreter***
  *(Functional language)*

---

## *Main Phases*

1. Source code
       |
       V
2.  Scanner
       |
       V
3.   Tokens
       |
       V
4.  Compiler
       |
       V
5.  Bytecode
     chunk
       |
       V
6.  Virtual
    machine

---

## *Concepts I want to implement*
- string interning
- garbage collection(mark-and-sweep)
- hash table
- own memory allocators
- ...

---

## *Path*

##### *Sequence of bytecode*
- buffer of instructions
- handle constants, how and where to store them
             opcode
            constant index
    [0x1234, 0x1235, 0x1236, ...]
                       \|
                       \|
             ==========\|
            \|
             V
    [123, 3,141592, ...]]

- implement run-length encoding to store a line information
    1 - line       2 - line
    3 - num of     1 - num of
      continues     continues
        bytes         bytes
    [ ( 1, 3 ) ,  ( 2, 1 ) ]

##### *Virtual Machine*
- virtual machine gonna take a chunk of bytecode and execute instructions
