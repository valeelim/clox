#include <stdlib.h>
#include <stdio.h>

#include "chunk.h"
#include "memory.h"

void initChunk(Chunk* chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    initValueArray(&chunk->constants);

    chunk->lineCount = 0;
    chunk->lineCapacity = 0;
    chunk->lines = NULL;
}


void freeChunk(Chunk* chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    FREE_ARRAY(LineInfo, chunk->lines, chunk->lineCapacity);
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}

void writeChunk(Chunk* chunk, uint8_t byte, int line) {
    if (chunk->capacity < chunk->count + 1) {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
    }

    chunk->code[chunk->count] = byte;
    chunk->count++;

    if (chunk->lineCount == 0 || chunk->lines[chunk->lineCount - 1].line != line) {
        if (chunk->lineCapacity < chunk->lineCount + 1) {
            int oldLineCapacity = chunk->lineCapacity;
            chunk->lineCapacity = GROW_CAPACITY(oldLineCapacity);
            chunk->lines = GROW_ARRAY(LineInfo, chunk->lines, oldLineCapacity, chunk->lineCapacity);
        }
        
        chunk->lines[chunk->lineCount].line = line;
        chunk->lines[chunk->lineCount].runLength = 1;
        chunk->lineCount++;
    }
    else {
        chunk->lines[chunk->lineCount].runLength++;
    }
}

int addConstant(Chunk* chunk, Value value) {
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}

int getLine(Chunk* chunk, int offset) {
    int accumulatedLength = 0;
    for (int i = 0; i < chunk->lineCount; i++) {
        accumulatedLength += chunk->lines[i].runLength;
        if (offset < accumulatedLength) {
            return chunk->lines[i].line;
        }
    }
    return -1;
}
