#include <stdio.h>

char array[15][50];
int array_size = 0;

int main()
{
    append("+");
    append("*");
    display();
}

void append(char *s)
{
    strcpy(array[array_size], s);
    array_size++;
}

void display()
{
    int i;
    for (i = array_size - 1; i >= 0; i--)
    {
        printf("%s ", array[i]);
    }
}