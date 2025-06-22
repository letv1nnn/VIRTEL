#include "include/vm.h"
#include "include/debug.h"

VM vm;

static void resetStack() {
    vm.stack_top = vm.stack;
}

void constructVM(Chunk *chunk) {
    vm.chunk = chunk;
    resetStack();
}

void deconstructVM() {

}

void push(double val) {
    if (vm.stack_top >= vm.stack + STACK_MAX) {
        printf("Stack underflow!\n");
        exit(1);
    }
    *vm.stack_top = val;
    vm.stack_top++;
}

double pop() {
    if (vm.stack_top == vm.stack) {
        printf("Stack underflow!\n");
        exit(1);
    }
    vm.stack_top--;
    return *vm.stack_top;
}

InterpretResult interpret(const char * source) {
    Chunk *chunk = constructChunk();

    if (!compile(source, chunk)) {
        deconstructChunk(chunk);
        return COM_ERR;
    }

    vm.chunk = chunk;
    vm.ip = vm.chunk->opcodes;

    InterpretResult result = run();

    deconstructChunk(chunk);

    return result;
}

InterpretResult run() {
    for (;;) {
#ifdef DEBUG
        vmStack(&vm);
#endif
        uint8_t instruction = *vm.ip++;
        switch (instruction) {
            case OP_RETURN:
                return OK;
            case OP_CONSTANT:
                uint8_t constant_index = *vm.ip++;
                double val = vm.chunk->constants->nums[constant_index];
                push(val);
                break;
            case OP_ADD: case OP_SUB:
            case OP_MUL: case OP_DIV:
                double b = pop(), a = pop();
                if (instruction == OP_ADD) push(a + b);
                else if (instruction == OP_SUB) push(a - b);
                else if (instruction == OP_MUL) push(a * b);
                else {
                    if (b != 0.0)
                        push(a / b);
                    else {
                        printf("Zero division error!");
                        return RUN_ERR;
                    }
                }
                break;
            case OP_NEG:
                double num = pop();
                push(-num);
                break;
            default:
                printf("Unknown opcode.\n");
                return COM_ERR;
        }
    }
    return OK;
}

