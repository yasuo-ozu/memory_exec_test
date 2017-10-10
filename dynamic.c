#include <stdio.h>                      // printf()
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

int simple_func(void){
///asm("simple_func:               ");     // simple_func entry address
asm("  movl  $123, %eax         ");     // return 123
asm("pop %rbp");
asm("ret");
asm(" .byte  0x55, 0xAA         ");     // Mark the end of function
}                  // Declare simple_func as an
// function entry address

unsigned char text[ 256 ];              // .text section holder

int main() {
  unsigned char *src, *dst, *text;
  int (*fn)(void);

  src = (unsigned char*) simple_func;
  
  int fd = open("/dev/null", O_RDONLY);
  if (fd == -1) {
	  perror("open");
	  return 1;
  }

  text = mmap(NULL, 4096, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
  if (text == MAP_FAILED) {
	  perror("mmap");
	  return 1;
  }

	printf("%p\n", text);

  dst = text;
  while ((*src != 0x55) || (*(src + 1) != 0xAA)) {
    printf("%02X \n", *src);
    *dst++ = *src++;
  }
  printf("\n");

  text[5] = 200;

  fn = (int (*)(void)) text;
  printf("res = %d\n", (simple_func)());
  printf("res = %d\n", (fn)());
}
