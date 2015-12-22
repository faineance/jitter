#include <stdio.h>
#include <stdint.h>
#include <sys/user.h>
#include <string.h>
#include "sys/mman.h"


struct asm_buffer {
    uint8_t code[PAGE_SIZE];
};

struct asmbuffer *asm_create(void) {
    int prot = PROT_READ | PROT_WRITE;
    int flags = MAP_ANONYMOUS | MAP_PRIVATE;
    return mmap(NULL, PAGE_SIZE, prot, flags, -1, 0);
}

void asm_free(struct asmbuffer *buf) {
    munmap(buf, PAGE_SIZE);
}

void asm_finalize(struct asmbuffer *buf) {
    mprotect(buf, PAGE_SIZE, PROT_READ | PROT_EXEC);
}


int main() {
    struct asmbuffer *buf = asm_create();

    unsigned char code [] = {
            0x55,                   // push rbp
            0x48, 0x89, 0xe5,       // mov  rbp, rsp
            0x89, 0x7d, 0xfc,       // mov  DWORD PTR [rbp-0x4],edi
            0x89, 0x75, 0xf8,       // mov  DWORD PTR [rbp-0x8],esi
            0x8b, 0x75, 0xfc,       // mov  esi,DWORD PTR [rbp-04x]
            0x0f, 0xaf, 0x75, 0xf8, // imul esi,DWORD PTR [rbp-0x8]
            0x89, 0xf0,             // mov  eax,esi
            0x5d,                   // pop  rbp
            0xc3                    // ret
    };
    memcpy(buf, code, sizeof(code));

    asm_finalize(buf);


    int (*func)(int i, int i1) = (int (*)(int, int)) buf;

    // call function pointer
    printf("%d * %d = %d\n", 5, 11, func(5, 11));


    asm_free(buf);
    return 0;

}
