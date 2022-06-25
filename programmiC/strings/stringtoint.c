// C program to demonstrate
// the working of SSCANF() to
// convert a string into a number

#include <stdio.h>
int main()
{
        const char* str = "12345";
        int x;
        sscanf(str, "%d", &x);
        printf("\nThe value of x : %d\n", x);
        return 0;
}
