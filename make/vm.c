#include "vm.h"
#include "compiler.h"

#define READ_BYTE() (*vm.ip++)

VM vm;

// stack methods
static Value pop() { vm.stackTop--; return *vm.stackTop; }
static void push(Value val) { *vm.stackTop = val; vm.stackTop++; }

static void resetStack() {
    vm.stackTop = vm.stack;
}

void createVM() {
    resetStack();
}

void freeVM() {

}

static InterpretResult run() {
#if 1 // chunk testing purposes
    testChunk(vm.chunk);
#endif
    for (;;) {
#if 1 // stack testing purposes
        for (Value *slot = vm.stack; slot < vm.stackTop; slot++) {
            printf(" [ ");
            printf("%lf", *slot);
            printf(" ] ");
        }
        printf("\n");
#endif
        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
            Value b;
            case OP_ADD:
                b = pop();
                push(pop() + b);
                break;
            case OP_SUB:
                b = pop();
                push(pop() - b);
                break;
            case OP_MUL:
                b = pop();
                push(pop() * b);
                break;
            case OP_DIV:
                b = pop();
                if (b == 0) {
                    return INTERPRET_RUNTIME_ERROR;
                }
                push(pop() / b);
                break;
            case OP_NEG: *(vm.stackTop - 1) *= (-1); break;
            case OP_CONSTANT:
                Value val = vm.chunk->buf.values[READ_BYTE()];
                push(val);
                break;
            case OP_RETURN:
                return INTERPRET_OK;
        }
    }
}

InterpretResult interpret(const char *source) {
    compile(source);
    return INTERPRET_OK;
}


