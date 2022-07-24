#if 0
gcc -o ${0}.out $0  -lm && ${0}.out
rm -f *.out
exit ${?}
#endif

// wap to store array of integers in a file
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct leaves
{
    char id[50];
    int lv[12][10];
};

struct employe
{
    char id[100];
    char name[100];
    char dob[100];
    char phone[100];
    char email[100];
    char password[100];
    char type[100];
    int leaves;
    long int doj;
};

int main()
{
    struct leaves var;
    FILE *fp, *fp1;
    struct employe e;
    int i, j;
    fp = fopen("array.txt", "a");
    if (fp == NULL)
    {
        printf("Error opening file");
        exit(1);
    }
    strcpy(var.id, "1658582969d");
    for (i = 0; i < 12; i++)
    {
        for (j = 0; j < 10; j++)
        {
            var.lv[i][j] = i * j;
        }
    }
    fwrite(&var, sizeof(var), 1, fp);
    fclose(fp);

    fp1 = fopen("user.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening file");
        exit(1);
    }
    while (fscanf(fp1, "%s %s %s %s %s %s %s %d %ld", e.id, e.name, e.dob, e.phone, e.email, e.password, e.type, &e.leaves, &e.doj) != EOF)
    {
        fp = fopen("array.txt", "r");
        while (fread(&var, sizeof(var), 1, fp) != 0)
        {
            if (strcmp(e.id, var.id) == 0)
            {
                printf("%s %s %s %s %s %s %s %d %ld\n", e.id, e.name, e.dob, e.phone, e.email, e.password, e.type, e.leaves, e.doj);
            }
        }
        fclose(fp);
    }
    fclose(fp1);
    return 0;
}
