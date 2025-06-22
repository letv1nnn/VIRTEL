# **VIRTEL Interpreter**
*(Functional Language Implementation)*

---

## **Main Phases**
```plaintext
1. Source Code
       |
       V
2. Scanner (Lexer)
       |
       V
3. Tokens
       |
       V
4. Compiler
       |
       V
5. Bytecode (Chunk)
       |
       V
6. Virtual Machine
```

---

## **Future Features to Implement**
- String interning
- Garbage collection (mark-and-sweep)
- Hash table optimizations
- Custom memory allocators
- ...

---

## **Implementation Notes**

### **Bytecode Design** *(14/06/2025)*
- **Instruction buffer**: Sequential opcode storage.
- **Constants handling**:
  ```plaintext
            OPCODE
       CONSTANT_INDEX
  [0x1234, 0x1235, 0x1236, ...]
                     |
             |=======|
             V
  [123, 3.141592, ...]  (Constants pool)
  ```
- **Line tracking**: Run-length encoding (RLE) for efficient debugging:
  ```plaintext
  [(Line_Number, Byte_Count), ...]
  Example: [(1, 3), (2, 1)]
  ```

---

### **Virtual Machine** *(15/06/2025)*
Executes bytecode chunks with stack-based operations:
```plaintext
1     42 + 3.141592    → [45.141592]
2     123 / 90.909 * 45.141592 → [1448.638829]
3     -1448.638829     → [-1448.638829]

Stack Trace:
[0x580f25af0050]: [42.000000]
[0x580f25af0058]: [42.000000, 3.141592]
[0x580f25af0060]: [45.141592]
...
```

---

### **Scanner Development** *(16/06/2025 - 19/06/2025)*
**Initial Approach (Rust Prototype)**:
```rust
Token { token_type: Noth, lexeme: "var", line: 1 }
Token { token_type: Equal, lexeme: "=", line: 1 }
```
**Optimized (C Implementation)**:
- Used **trie structure** for keyword detection (inspired by *Crafting Interpreters*).
- Example output:
  ```plaintext
  1 25 'for'
   |  0 '('
   | 33 'var'
   | 19 'i'
   | 13 '='
  ```

---

### **Parser** *(20/06/2025 - 22/06/2025)*
This is the point where I've created a calculater that is ultra overcomplicated under the hood.
Implemented **Pratt parsing** for expressions:
```plaintext
> (-1 + 8) * 3.5 / 6 - -1
Stack (top at 0x641a47b4e3f0):
Stack (top at 0x641a47b4e3f8): [ 1.000000 ]
Stack (top at 0x641a47b4e3f8): [ -1.000000 ]
Stack (top at 0x641a47b4e400): [ -1.000000 ] [ 8.000000 ]
Stack (top at 0x641a47b4e3f8): [ 7.000000 ]
Stack (top at 0x641a47b4e400): [ 7.000000 ] [ 3.500000 ]
Stack (top at 0x641a47b4e3f8): [ 24.500000 ]
Stack (top at 0x641a47b4e400): [ 24.500000 ] [ 6.000000 ]
Stack (top at 0x641a47b4e3f8): [ 4.083333 ]
Stack (top at 0x641a47b4e400): [ 4.083333 ] [ 1.000000 ]
Stack (top at 0x641a47b4e400): [ 4.083333 ] [ -1.000000 ]
Stack (top at 0x641a47b4e3f8): [ 5.083333 ]

Final Result: [5.083333]
```
```
