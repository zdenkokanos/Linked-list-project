#include <stdio.h>
#include <stdbool.h>

FILE f_n(FILE *file_data, bool *n_was_started)
{
    if (n_was_started == false)
    {
        file_data = fopen("dataloger_V2.txt", "r");
        if (file_data == NULL)
        {
            printf("Neotvorený súbor.\n");
        }
    }
    return *file_data;
}

void f_v(FILE *file_data)
{
    if (file_data != NULL)
    {
        printf("Bol otvoreny.\n");
    }
}

int main()
{
    FILE *file_data;
    char input;
    bool *n_was_started = false;
    while (1)
    {
        scanf("%c", &input);
        switch (input)
        {
        case 'n':
            f_n(file_data, n_was_started);
            break;
        case 'v':
            f_v(file_data);
            break;
        default:
            break;
        }
    }

    return 0;
}