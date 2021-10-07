#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int a, b;
    printf("Enter 2 numbers: ");
    scanf("%d %d", &a, &b);
    printf("%d + %d = %d\n\n", a, b, a + b);

    FILE *f = fopen("anotherDirectory/src/hello.c", "r");
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *buf = malloc(size);
    fread(buf, 1, size, f);
    fclose(f);

    printf("%s\n", buf);

    return 0;
}