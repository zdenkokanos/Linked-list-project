#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct id_mer_modulu
{
    char oznacenie;
    int cislovanie;
    char druh;
} ID_MOD;

typedef struct pos_mer_modulu
{
    double latitude;
    double longitude;
} POS_MOD;

typedef struct mer_modul
{
    ID_MOD *id_mer_modulu;
    POS_MOD *pos_mer_modulu;
    char typ;
    double hodnota;
    int cas_num;
    int date;
} MODUL;

FILE f_n(FILE *file_data, bool *n_was_started, int *data_count)
{
    if (n_was_started == false)
    {
        file_data = fopen("dataloger_V2.txt", "r");
        if (file_data == NULL)
        {
            printf("Neotvorený súbor.\n");
        }
        char data;
        for (data = fgetc(file_data); data != EOF; data = fgetc(file_data))
        {
            if (data == '$')
            {
                (*data_count)++;
            }
        }
        (*data_count) /= 3;
        fseek(file_data, 0, SEEK_SET);

        ID_MOD *id_mer_modulu = malloc((*data_count) * sizeof(ID_MOD));
        POS_MOD *pos_mer_modulu = malloc((*data_count) * sizeof(POS_MOD));
        MODUL *mer_modul = malloc((*data_count) * sizeof(MODUL));

        printf("%d", (*data_count));
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
    int data_count = 0;
    bool *n_was_started = false;
    while (1)
    {
        scanf("%c", &input);
        switch (input)
        {
        case 'n':
            f_n(file_data, n_was_started, &data_count);
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