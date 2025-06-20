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

### *Sequence of bytecode* 14/6/2025
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

### *Virtual Machine* 15/6/2025
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

### *Scanner* 16/6/2025 - 19/6/2025
this time want to try Rust for the first time (3 days of grinding)

1. stage, handled everything, except the keywords, and do not know what to do with the variables yet.
    ```plaintext
    Token { token_type: Noth, lexeme: "v", line: 1 }
    Token { token_type: Noth, lexeme: "a", line: 1 }
    Token { token_type: Noth, lexeme: "r", line: 1 }
    Token { token_type: Noth, lexeme: "x", line: 1 }
    Token { token_type: Equal, lexeme: "=", line: 1 }
    Token { token_type: Number, lexeme: "5", line: 1 }
    ```
2.  gonna use a hashmap for this purpose, I'll store the key = "identifier as a string"
    and value as a token type.
    ```plaintext
    Token { token_type: Var, lexeme: "var", line: 1 }
    Token { token_type: Identifier, lexeme: "x", line: 1 }
    Token { token_type: Equal, lexeme: "=", line: 1 }
    Token { token_type: Number, lexeme: "5", line: 1 }
    Token { token_type: Semicolon, lexeme: ";", line: 1 }
    Token { token_type: Identifier, lexeme: "is_equal", line: 2 }
    Token { token_type: Equal, lexeme: "=", line: 2 }
    Token { token_type: Identifier, lexeme: "x", line: 2 }
    Token { token_type: Plus, lexeme: "+", line: 2 }
    Token { token_type: Number, lexeme: "10", line: 2 }
    Token { token_type: EqualEqual, lexeme: "==", line: 2 }
    Token { token_type: Number, lexeme: "15", line: 2 }
    ```

3.  need to take a file or a one line coding and transfer it to my Rust scanner and then return Rust's tokens to my future compiler.
    So initially need to connect my rust file with main C file.

I've created two scanners one in C and one int Rust, but could not connect my rust interpreter to the main program so had to rewrite my whole scanner using C lang.

```plaintext
> for (var i = 0; i < 10; i = i + 1) print(i);
   1 25 'for'
   |  0 '('
   | 33 'var'
   | 19 'i'
   | 13 '='
   | 21 '0'
   |  8 ';'
   | 19 'i'
   | 17 '<'
   | 21 '10'
   |  8 ';'
   | 19 'i'
   | 13 '='
   | 19 'i'
   |  7 '+'
   | 21 '1'
   |  1 ')'
   | 30 'print'
   |  0 '('
   | 19 'i'
   |  1 ')'
   |  8 ';'
   2 36 ''

```

*Consequence:* both of these scanners perform the same function, but scanner written in C uses another approach to detect the keyword, "trie", I've found this approach in the book "Crafting Interpreters".


### *Parser - AST - Code Generator* 20/6/2025 - ...
    for instance we have an expression 1 + 2 * -3
    - after Scanning it we get:
```plaintext
        TOKEN_NUMBER "1"
        TOKEN_PLUS "+"
        TOKEN_NUMBER "2"
        TOKEN_STAR "*"
        TOKEN_MINUS "-"
        TOKEN_NUMBER "3"
```
    - parser builds Abstract syntax tree:
```plaintext
            +
           / \
          1   *
             / \
            2   -
                |
                3
```
    - gode generator walks AST, post-order traversal (left->right->node)
```plaintext
        1. generate code for 1
        2. generate code for 2
        3. generate code for 3
        4. generate code for -
        5. generate code for *
        6. generate code for +
```
    - resulting bytecode should look like this
```plaintext
    OP_CONSTANT 1
    OP_CONSTANT 2
    OP_CONSTANT 3
    OP_NEG
    OP_MUL
    OP_ADD
```



