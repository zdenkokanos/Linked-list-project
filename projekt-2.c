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

FILE *f_n(FILE *file_data, bool *n_was_started, int *data_count, ZAZNAM **head, ZAZNAM **tail)
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

        char line[100];
        ZAZNAM *current = NULL;
        for (int i = 0; i < (*data_count); i++)
        {
            ZAZNAM *novy_zaznam = (ZAZNAM *)malloc(sizeof(ZAZNAM));
            if (novy_zaznam == NULL)
            {
                printf("Alokácia štruktúry zlyhala.\n");
                exit(-1);
            }

            if ((*head) == NULL)
            {
                *head = novy_zaznam; // udrží mi adresu začiatku linked listu
                current = novy_zaznam;
            }
            else
            {
                current->next = novy_zaznam;
                current = current->next;
            }

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
            *tail = novy_zaznam; // koniec záznamu
        }

        printf("Načítalo sa %d záznamov.\n", (*data_count));
        *n_was_started = true;
        return file_data;
    }
    else
    {
        free(*head);
        *head = NULL;
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

        char line[100];
        ZAZNAM *current = NULL;
        for (int i = 0; i < (*data_count); i++)
        {
            ZAZNAM *novy_zaznam = (ZAZNAM *)malloc(sizeof(ZAZNAM));
            if (novy_zaznam == NULL)
            {
                printf("Alokácia štruktúry zlyhala.\n");
                exit(-1);
            }

            if ((*head) == NULL)
            {
                *head = novy_zaznam; // udrží mi adresu začiatku linked listu
                current = novy_zaznam;
            }
            else
            {
                current->next = novy_zaznam;
                current = current->next;
            }

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
            *tail = novy_zaznam;
        }

        printf("Načítalo sa %d záznamov.\n", (*data_count));
        *n_was_started = true;
        return file_data;
    }
}

void f_v(FILE *file_data, bool *n_was_started, int *data_count, ZAZNAM **head)
{
    ZAZNAM *current = *head;
    int i = 1;
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

void f_p(int *data_count, ZAZNAM **head, bool *n_was_started, ZAZNAM **tail)
{
    int c1;
    scanf("%d", &c1);
    if (c1 < 0)
    {
        printf("Číslo musí byť väčšie ako 0.\n");
    }
    else
    {
        ZAZNAM *current;
        if (c1 > (*data_count))
        {

            ZAZNAM *novy_zaznam = (ZAZNAM *)malloc(sizeof(ZAZNAM));
            if (*head == NULL)
            {
                current = novy_zaznam;
                *head = current;
            }
            else
            {
                current = *tail;
                current->next = novy_zaznam;
                current = current->next;
                current->next = NULL;
                *tail = current;
            }

            scanf(" %c%d%c", &current->id_mer_modulu.oznacenie, &current->id_mer_modulu.cislovanie, &current->id_mer_modulu.druh);
            scanf("%lf %lf", &current->pos_mer_modulu.latitude, &current->pos_mer_modulu.longitude);
            scanf("%2s", current->typ);
            scanf("%lf", &current->hodnota);
            scanf("%d", &current->cas);
            scanf("%d", &current->date);
            (*data_count)++;
            *n_was_started = true;
        }
    }
}

int main()
{
    FILE *file_data = NULL;
    char input;
    int data_count = 0;
    ZAZNAM *head = NULL;
    ZAZNAM *tail = NULL;
    bool n_was_started = false;
    while (1)
    {
        scanf(" %c", &input);
        switch (input)
        {
        case 'n':
            file_data = f_n(file_data, &n_was_started, &data_count, &head, &tail);
            break;
        case 'v':
            f_v(file_data, &n_was_started, &data_count, &head);
            break;
        case 'u':
            f_u();
            break;
        case 'p':
            f_p(&data_count, &head, &n_was_started, &tail);
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
                free(head);
                head = NULL;
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