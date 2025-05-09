# Plan
## May 6
  1. Created Bytecode Chunks for Storing Constant Values.
  2. Implemented a Run-length encoding for line information
## May 7
  1. Implemented a Virtual machine that reads bytecode instructions and then execute some basic operations like (+ - * / ~).
  2. Inside the virtual machine I've added the stack to store constants.
## May 8
  1. Changed my main function, so now, a user can use two methods, loading a file or using a one line scripting, like python(3) command in terminal.
  2. Created a lexer, that scans the code from the file or scripted lines and creates a different tokens from them, that in consequence will send to a compiler.
