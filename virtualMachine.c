#include "virtualMachine.h"

//Number stack[MAXSTACK];
//Number* stackTop;

#define READ_BYTE() (*vm->ip++)
#define READ_CONSTANT() (vm->chunk->constants.numbers[READ_BYTE()])

void push(VirtualMachine* vm, Number num) {
	if (vm->stackTop >= vm->stack + MAXSTACK) {
		fprintf(stderr, "Stack overflow!\n");
		exit(1);
	}
	*vm->stackTop = num; 
	vm->stackTop++;
}

Number pop(VirtualMachine* vm) {
	if (vm->stackTop <= vm->stack) {
		fprintf(stderr, "Stack underflow!\n");
		exit(1);
	}
	vm->stackTop--;
	return *vm->stackTop;
}

void initVM(VirtualMachine* vm) {
	vm->stackTop = vm->stack;
	vm->chunk = NULL;
	vm->ip = NULL;
}

InterpretResult interpret(VirtualMachine* vm, Chunk* chunk) {
	vm->chunk = chunk;
	vm->ip = vm->chunk->bytes;
	return run(vm);
}

static InterpretResult run(VirtualMachine* vm) {
	for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
		printf("          ");
		for (Number* slot = vm->stack; slot < vm->stackTop; slot++) {
			printf("[ %g ]", *slot);
		}
		printf("\n");
#endif
		uint8_t instruction;
		switch (instruction = READ_BYTE()) {
			case OP_ADD: case OP_SUB:
			case OP_MUL: case OP_DIV: {
				Number b = pop(vm);
				Number a = pop(vm);
				if (instruction == OP_ADD) push(vm, a + b);
				else if (instruction == OP_SUB) push(vm, a - b);
				else if (instruction == OP_MUL) push(vm, a * b);
				else {
					if (b == 0) {
						fprintf(stderr, "Division by zero\n");
						return INTERPRET_RUNTIME_ERROR;
					}
					push(vm, a / b);
				}
				break;
			}
			case OP_NEG: {
				Number num = pop(vm);
				push(vm, -num);
				break;
			}
			case OP_CONSTANT: {
				Number number = READ_CONSTANT();
				push(vm, number);
				printf("constant: %lf\n", number);
				break;
			}
			case OP_RETURN:
				return INTERPRET_OK;
		}
	}
}


void freeVM(VirtualMachine* vm) {
	(void)vm;
}
