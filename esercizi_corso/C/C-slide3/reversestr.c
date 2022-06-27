#include <stdio.h>
#define MEMSIZE 32
char mem[MEMSIZE];
char *stringrev(char *str) {
char *i=str;
int p=MEMSIZE;
mem[--p]=0;
// printf("p=%d, str='%s'\n", p, str);
while (*i != 0) {
mem[--p]=*i;
// printf("p=%d, i=%d:%c\n", p, *i, *i);
i++;
};
return mem+p;
}
void main() {
char *txt="Hello world!";
printf("txt='%s', reversed='%s', mem='%s'\n", txt, stringrev(txt));