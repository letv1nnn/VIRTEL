#ifndef VIRTEL_VIRTUALVACHINE_H
#define VIRTEL_VIRTUALVACHINE_H

#include "common.h"
#include "chunk.h"
#include "values.h"
#include "debug.h"

#define MAXSTACK 256
//#define DEBUG_TRACE_EXECUTION

typedef enum {
	INTERPRET_OK,
	INTERPRET_COMPILE_ERROR,
	INTERPRET_RUNTIME_ERROR
} InterpretResult;

typedef struct {
	Chunk* chunk;
	uint8_t* ip;
	Number stack[MAXSTACK];
	Number* stackTop;
} VirtualMachine;

void initVM(VirtualMachine* vm);
void freeVM(VirtualMachine* vm);
InterpretResult interpret(VirtualMachine* vm, const char *source);

void push(VirtualMachine* vm, Number num);
Number pop(VirtualMachine* vm);

#endif