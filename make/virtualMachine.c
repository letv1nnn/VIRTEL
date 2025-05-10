#include "virtualMachine.h"
#include "compiler.h"

#define READ_BYTE() (*vm->ip++)
#define READ_CONSTANT() (vm->chunk->constants.numbers[READ_BYTE()])

void push(VirtualMachine* vm, Number num) {
	if (vm->stackTop  - vm->stack >= MAXSTACK) {
		fprintf(stderr, "Stack overflow!\n");
		exit(1);
	}
	*vm->stackTop++ = num;
}

Number pop(VirtualMachine* vm) {
	if (vm->stackTop <= vm->stack) {
		fprintf(stderr, "Stack underflow!\n");
		exit(1);
	}
	return *--vm->stackTop;
}

void initVM(VirtualMachine* vm) {
	vm->stackTop = vm->stack;
	memset(vm->stack, 0, sizeof(vm->stack));
	vm->chunk = NULL;
	vm->ip = NULL;
}

InterpretResult interpret(VirtualMachine* vm, const char* source) {
	Chunk chunk;
	createChunk(&chunk);

	if (!compile(source, &chunk)) {
		freeChunk(&chunk);
		return INTERPRET_COMPILE_ERROR;
	}

	vm->chunk = &chunk;
	vm->ip = vm->chunk->bytes;

	InterpretResult result = run(vm);

	freeChunk(&chunk);
	return result;
}

static InterpretResult run(VirtualMachine* vm) {
	// printf("Initial stackTop: %p\n", (void*)vm->stackTop);
	for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
		printf("Current IP: %p, StackTop: %p\n",
			(void*)vm->ip, (void*)vm->stackTop);
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
			default:
				fprintf(stderr, "Unknown opcode %d\n", instruction);
				return INTERPRET_RUNTIME_ERROR;
		}
	}
}


void freeVM(VirtualMachine* vm) {
	(void)vm;
}
