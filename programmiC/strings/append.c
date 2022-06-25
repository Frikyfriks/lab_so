// C program to Append a Character to a String

#include <stdio.h>
#include <string.h>

int main()
{
        // declare and initialize string
        char str[6] = "Geek";

        // declare and initialize char
        char ch = 's';

        // print string
        printf("Original String: %s\n", str);
        printf("Character to be appended: %c\n", ch);

        // append ch to str
        strncat(str, &ch, 1);

        // print string
        printf("Appended String: %s\n", str);

        return 0;
}
