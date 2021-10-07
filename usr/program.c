#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int a;
    printf("Enter: ");
    scanf("%d", &a);
    printf("%d was entered\n", a);

    return 0;
}