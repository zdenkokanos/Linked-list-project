#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct id_mer_modulu
{
    char oznacenie;
    int cislovanie;
    char druh;
    struct id_mer_modulu *next_id;
} ID_MOD;

typedef struct pos_mer_modulu
{
    double latitude;
    double longitude;
    struct pos_mer_modulu *next_pos;
} POS_MOD;

typedef struct mer_modul
{
    ID_MOD id_mer_modulu;
    POS_MOD pos_mer_modulu;
    char typ[3];
    double hodnota;
    int cas;
    int date;
    struct mer_modul *next;
} ZAZNAM;

FILE *f_n(FILE *file_data, bool *n_was_started, int *data_count, ZAZNAM **modul)
{
    char data;
    if (*n_was_started == false)
    {
        file_data = fopen("dataloger_V2.txt", "r");
        if (file_data == NULL)
        {
            printf("Neotvorený súbor.\n");
            exit(-1);
        }
        fseek(file_data, 0, SEEK_SET);
        for (data = fgetc(file_data); data != EOF; data = fgetc(file_data))
        {
            if (data == '$')
            {
                (*data_count)++;
            }
        }
        (*data_count) /= 3;
        fseek(file_data, 0, SEEK_SET);

        *modul = (ZAZNAM *)malloc((*data_count) * sizeof(ZAZNAM));
        if (*modul == NULL)
        {
            printf("Alokácia štruktúry zlyhala.\n");
            exit(-1);
        }

        char line[100];
        ZAZNAM *current = *modul;
        for (int i = 0; i < (*data_count); i++)
        {
            ZAZNAM *novy_zaznam = (ZAZNAM *)malloc(sizeof(ZAZNAM));
            if (novy_zaznam == NULL)
            {
                printf("Alokácia uzlu zlyhala.\n");
                exit(-1);
            }
            current->next = novy_zaznam;
            current = current->next;
            current->next = NULL;

            char line[100];
            fgets(line, sizeof(line), file_data);
            fscanf(file_data, " %c%d%c", &current->id_mer_modulu.oznacenie, &current->id_mer_modulu.cislovanie, &current->id_mer_modulu.druh);
            fscanf(file_data, "%lf %lf ", &current->pos_mer_modulu.latitude, &current->pos_mer_modulu.longitude);
            fgets(line, sizeof(line), file_data);
            strncpy(current->typ, line, 2);
            fscanf(file_data, "%lf ", &current->hodnota);
            fscanf(file_data, "%d ", &current->cas);
            fscanf(file_data, "%d ", &current->date);
        }

        printf("Načítalo sa %d záznamov.\n", (*data_count));
        *n_was_started = true;
        return file_data;
    }
    else
    {
        free(*modul);
        *modul = NULL;
        fseek(file_data, 0, SEEK_SET);
        *data_count = 0;
        for (data = fgetc(file_data); data != EOF; data = fgetc(file_data))
        {
            if (data == '$')
            {
                (*data_count)++;
            }
        }
        (*data_count) /= 3;
        fseek(file_data, 0, SEEK_SET);

        *modul = (ZAZNAM *)malloc((*data_count) * sizeof(ZAZNAM));
        if (*modul == NULL)
        {
            printf("Alokácia štruktúry zlyhala.\n");
            exit(-1);
        }

        char line[100];
        ZAZNAM *current = *modul;
        for (int i = 0; i < (*data_count); i++)
        {
            ZAZNAM *novy_zaznam = (ZAZNAM *)malloc(sizeof(ZAZNAM));
            if (novy_zaznam == NULL)
            {
                printf("Alokácia uzlu zlyhala.\n");
                exit(-1);
            }
            current->next = novy_zaznam;
            current = current->next;
            current->next = NULL;

            char line[100];
            fgets(line, sizeof(line), file_data);
            fscanf(file_data, " %c%d%c", &current->id_mer_modulu.oznacenie, &current->id_mer_modulu.cislovanie, &current->id_mer_modulu.druh);
            fscanf(file_data, "%lf %lf ", &current->pos_mer_modulu.latitude, &current->pos_mer_modulu.longitude);
            fgets(line, sizeof(line), file_data);
            strncpy(current->typ, line, 2);
            fscanf(file_data, "%lf ", &current->hodnota);
            fscanf(file_data, "%d ", &current->cas);
            fscanf(file_data, "%d ", &current->date);
        }

        printf("Načítalo sa %d záznamov.\n", (*data_count));
        return file_data;
    }
}

void f_v(FILE *file_data, bool *n_was_started, int *data_count, ZAZNAM **modul)
{
    ZAZNAM *current = *modul;
    int i = 1;
    current = current->next;
    while (current != NULL)
    {
        printf("%d:\n", i);
        printf("ID: %c%d%c\t%s\t%.2lf\n", current->id_mer_modulu.oznacenie, current->id_mer_modulu.cislovanie, current->id_mer_modulu.druh, current->typ, current->hodnota);
        printf("Poz: +%.4lf\t+%.4lf\n", current->pos_mer_modulu.latitude, current->pos_mer_modulu.longitude);
        printf("DaC: %d\t%d\n", current->date, current->cas);
        current = current->next;
        i++;
    }
}
void f_r()
{
}

void f_z()
{
}

void f_u()
{
}

void f_p()
{
}

int main()
{
    FILE *file_data = NULL;
    char input;
    int data_count = 0;
    ZAZNAM *modul = NULL;
    bool n_was_started = false;
    while (1)
    {
        scanf(" %c", &input);
        switch (input)
        {
        case 'n':
            file_data = f_n(file_data, &n_was_started, &data_count, &modul);
            break;
        case 'v':
            f_v(file_data, &n_was_started, &data_count, &modul);
            break;
        case 'u':
            f_u();
            break;
        case 'p':
            f_p();
            break;
        case 'z':
            f_z();
            break;
        case 'r':
            f_r();
            break;
        case 'k':
            if (n_was_started == true)
            {
                free(modul);
                modul = NULL;
                fclose(file_data);
                return 0;
            }
            else
            {
                return 0;
            }
        default:
            printf("Bolo zadané nesprávne písmeno, skúste znova:\n");
            break;
        }
    }

    return 0;
}