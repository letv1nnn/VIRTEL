# ***VIRTEL Interpreter***
  *(Functional language)*

---

## *Main Phases*
```plaintext
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
```
---

## *Concepts I want to implement*
- string interning
- garbage collection(mark-and-sweep)
- hash table
- own memory allocators
- ...

---

## *Path*

### *Sequence of bytecode*
- **Buffer of instructions**
- **Handle constants** (storage approach):
  ```plaintext
            opcode
           constant index
  [0x1234, 0x1235, 0x1236, ...]
                     |
             |=======|
             |
             V
  [123, 3.141592, ...]
  ```

- **Run-length encoding for line info**:
  ```plaintext
  1 - line       2 - line
  3 - num of     1 - num of
    continues     continues
      bytes         bytes
  [ (1, 3), (2, 1) ]
  ```

---

### *Virtual Machine*
The virtual machine will take a chunk of bytecode and execute instructions.

```plaintext
line num
    1     42 + 3.141592  =>  (45.141592)
    2     123 / 90.909 * 45.141592  => (1448.638829)
    3     -1448.638829  => (-1448.638829)

Stack (top at 0x580f25af0050):
Stack (top at 0x580f25af0058): [ 42.000000 ]
Stack (top at 0x580f25af0060): [ 42.000000 ] [ 3.141592 ]
Stack (top at 0x580f25af0058): [ 45.141592 ]
Stack (top at 0x580f25af0060): [ 45.141592 ] [ 123.000000 ]
Stack (top at 0x580f25af0068): [ 45.141592 ] [ 123.000000 ] [ 90.909000 ]
Stack (top at 0x580f25af0060): [ 45.141592 ] [ 32.091000 ]
Stack (top at 0x580f25af0058): [ 1448.638829 ]
Stack (top at 0x580f25af0058): [ -1448.638829 ]
```


