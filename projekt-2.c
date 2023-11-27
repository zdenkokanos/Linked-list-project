#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

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
    ID_MOD id_mer_modulu;
    POS_MOD pos_mer_modulu;
    char typ[3];
    double hodnota;
    int cas;
    int date;
    struct mer_modul *next;
} ZAZNAM;

FILE *f_n(FILE *file_data, bool *n_was_started, int *data_count, ZAZNAM **head, ZAZNAM **tail, bool *p_was_started)
{
    char data;

    if (*n_was_started == false) // ak nebolo zapnuté
    {
        if (*p_was_started == true)
        {
            if (*head != NULL) // ak náhodou bol pridaný záznam cez funkciu p, vymazem ho
            {
                ZAZNAM *current = *head;
                ZAZNAM *previous = NULL;
                while (current != NULL)
                {
                    previous = current;
                    current = current->next;
                    free(previous);
                    *head = NULL;
                    *tail = NULL;
                }
            }
        }
        *data_count = 0; // počet záznamov dám na 0
        file_data = fopen("dataloger_V2.txt", "r");
        if (file_data == NULL)
        {
            printf("Neotvorený súbor.\n");
            exit(-1);
        }
        fseek(file_data, 0, SEEK_SET);
        for (data = fgetc(file_data); data != EOF; data = fgetc(file_data)) // spočíta záznamy v súbore
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
        for (int i = 0; i < (*data_count); i++) // vložím záznamy do štruktúry
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
    else // ak už pred tým bolo stlačené n bolo niečo načítané do štruktúry, najprv free potom vložím do structu záznamy
    {
        ZAZNAM *current = *head;
        ZAZNAM *previous = NULL;
        while (current != NULL)
        {
            previous = current;
            current = current->next;
            free(previous);
        }
        *head = NULL;
        *tail = NULL;
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
        current = NULL;
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

void f_z(ZAZNAM **head, ZAZNAM **tail, int *data_count)
{
    char input_oznacenie;
    int input_cislovanie;
    char input_druh;
    printf("Zadaj ID: ");
    scanf(" %c%3d%c", &input_oznacenie, &input_cislovanie, &input_druh);
    ZAZNAM *current = *head;
    ZAZNAM *temp = NULL;
    ZAZNAM *previous = NULL;
    bool found = false;
    while (current != NULL)
    {
        if (input_oznacenie == current->id_mer_modulu.oznacenie && input_cislovanie == current->id_mer_modulu.cislovanie && input_druh == current->id_mer_modulu.druh)
        {
            temp = current;
            if (*head == current)
            {
                *head = current->next;
                printf("Záznam pre ID: %c%d%c bol vymazaný.\n", current->id_mer_modulu.oznacenie, current->id_mer_modulu.cislovanie, current->id_mer_modulu.druh);
                current = current->next;
                free(temp);
                temp = NULL;
                (*data_count)--;
            }
            else
            {
                previous->next = current->next;
                printf("Záznam pre ID: %c%d%c bol vymazaný.\n", current->id_mer_modulu.oznacenie, current->id_mer_modulu.cislovanie, current->id_mer_modulu.druh);
                current = current->next;
                free(temp);
                temp = NULL;
                (*data_count)--;
            }
            found = true;
        }
        else
        {
            previous = current;
            current = current->next;
        }
    }
    current = *head;
    while (current != NULL) // zistím kde mám poslednú štruktúru
    {
        *tail = current;
        current = current->next;
    }

    if (found == false)
    {
        printf("Zadané ID sa nenachádza v zozname.\n");
    }
}

void f_u(ZAZNAM **head, int *data_count, ZAZNAM **tail)
{
    ZAZNAM *current = *head;
    ZAZNAM *temp = NULL;
    ZAZNAM *left = NULL;
    ZAZNAM *right = NULL;
    ZAZNAM *previous = NULL;
    bool sorted = false;
    if (*head == NULL)
    {
        printf("V zozname sa nič nenachádza.\n");
    }
    else if ((*head)->next == NULL)
    {
        printf("V zozname je len jedna štruktúra.\n");
    }
    else
    {
        while (sorted == false) // kým nie je všetko vysortované
        {
            sorted = true; // pokial je vysortovaný tak nikdy neskočí naspäť na false
            current = *head;
            previous = NULL;

            while (current->next != NULL)
            {
                left = current;
                right = current->next;
                if (left->date > right->date || (left->date == right->date && left->cas > right->cas))
                {
                    sorted = false;
                    if (previous == NULL) // ak je to prvý záznam ktorý vymieňam
                    {
                        *head = right;
                        left->next = right->next;
                        right->next = left;
                        current = *head;
                    }
                    else // ostatné
                    {
                        left->next = right->next;
                        right->next = left;
                        previous->next = right;
                        current = right;
                    }
                }
                else
                {
                    previous = current;
                    current = current->next;
                }
            }
        }
    }
    current = *head;
    while (current != NULL)
    {
        *tail = current;
        current = current->next;
    }
}

void f_r(ZAZNAM **head, ZAZNAM **tail, int *data_count)
{
    int c1;
    int c2;
    ZAZNAM *prev_c1 = NULL;
    ZAZNAM *node_c1 = NULL;
    ZAZNAM *node_c2 = NULL;
    ZAZNAM *prev_c2 = NULL;
    ZAZNAM *current = *head;
    ZAZNAM *temp = NULL;
    printf("Zadaj polohy c1 a c2: ");
    scanf("%d %d", &c1, &c2);
    if (c1 <= 0 || c1 > *data_count || c2 <= 0 || c2 > *data_count)
    {
        printf("Nejdeeeeeee.\n");
        // žiaden výstup pokiaľ nebolo stlačené n alebo zadané zlé hodnoty c1 a c2
    }
    else
    {
        if (c1 == 1 || c2 == 1) // ak c1 alebo c2 je prvá štruktúra
        {
            if (c1 == 1) // ak c1 tak sprav c2 = prvý prvok
            {
                node_c1 = *head;
                current = *head;
                for (int i = 1; i < c2; i++)
                {
                    prev_c2 = current;
                    current = current->next;
                    node_c2 = current;
                }
                *head = node_c2;
                temp = node_c2->next;
                if (abs(c1 - c2) == 1)
                {
                    node_c2->next = node_c1;
                    node_c1->next = temp;
                }
                else
                {
                    node_c2->next = node_c1->next;
                    node_c1->next = temp;
                    prev_c2->next = node_c1;
                }
            }
            else if (c2 == 1) // ak c2 tak c1 = prvý prvok
            {
                node_c2 = *head;
                current = *head;
                for (int i = 1; i < c1; i++)
                {
                    prev_c1 = current;
                    current = current->next;
                    node_c1 = current;
                }
                *head = node_c1;
                temp = node_c1->next;
                if (abs(c1 - c2) == 1)
                {
                    node_c1->next = node_c2;
                    node_c2->next = temp;
                }
                else
                {

                    node_c1->next = node_c2->next;
                    node_c2->next = temp;
                    prev_c1->next = node_c2;
                }
            }
        }
        else
        {
            for (int i = 1; i < c1; i++) // zisti ktorá adresa je pre c1
            {
                prev_c1 = current;
                current = current->next;
                node_c1 = current;
            }
            current = *head;
            for (int i = 1; i < c2; i++) // zisti ktorá adresa je pre c2
            {
                prev_c2 = current;
                current = current->next;
                node_c2 = current;
            }
            temp = node_c2->next;
            if (abs(c1 - c2) == 1) // záznamy idúce za sebou
            {
                prev_c1->next = node_c2;
                node_c2->next = node_c1;
                node_c1->next = temp;
            }
            else
            {
                prev_c1->next = node_c2;
                node_c2->next = node_c1->next;
                prev_c2->next = node_c1;
                node_c1->next = temp;
            }
        }
        if (node_c1->next == NULL) // pokial menim posledny prvok zmenim aj tail
        {
            *tail = node_c1;
        }
        if (node_c2->next == NULL)
        {
            *tail = node_c2;
        }
        printf("Záznamy boli úspešne vymenené.\n");
    }
}

void f_p(int *data_count, ZAZNAM **head, ZAZNAM **tail, bool *p_was_started)
{
    int c1;
    scanf("%d", &c1);
    if (c1 <= 0)
    {
        printf("Číslo musí byť väčšie ako 0.\n");
    }
    else
    {
        ZAZNAM *current;
        if (c1 > (*data_count)) // ak c1 je vacsie tak zapisem na koniec zoznamu
        {
            ZAZNAM *novy_zaznam = (ZAZNAM *)malloc(sizeof(ZAZNAM));
            if (*head == NULL)
            {
                current = novy_zaznam;
                *head = current;
                *tail = current;
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
        }
        else // zapisem prvok na miesto ktoré bolo zadané
        {
            ZAZNAM *previous = NULL;
            current = *head;
            for (int i = 0; i < c1 - 2; i++)
            {
                current = current->next;
            }

            ZAZNAM *novy_zaznam = (ZAZNAM *)malloc(sizeof(ZAZNAM));
            if (c1 == 1)
            {
                previous = *head;
                *head = novy_zaznam;
                current = *head;
            }
            else
            {
                previous = current->next;
                current->next = novy_zaznam;
                current = current->next;
            }

            scanf(" %c%d%c", &current->id_mer_modulu.oznacenie, &current->id_mer_modulu.cislovanie, &current->id_mer_modulu.druh);
            scanf("%lf %lf", &current->pos_mer_modulu.latitude, &current->pos_mer_modulu.longitude);
            scanf("%2s", current->typ);
            scanf("%lf", &current->hodnota);
            scanf("%d", &current->cas);
            scanf("%d", &current->date);
            (*data_count)++;
            current->next = previous;
        }
    }
    *p_was_started = true;
}

int main()
{
    FILE *file_data = NULL;
    char input;
    int data_count = 0;
    ZAZNAM *head = NULL;
    ZAZNAM *tail = NULL;
    ZAZNAM *temp = NULL;
    bool n_was_started = false;
    bool p_was_started = false;
    while (1)
    {
        scanf(" %c", &input);
        switch (input)
        {
        case 'n':
            file_data = f_n(file_data, &n_was_started, &data_count, &head, &tail, &p_was_started);
            break;
        case 'v':
            f_v(file_data, &n_was_started, &data_count, &head);
            break;
        case 'u':
            f_u(&head, &data_count, &tail);
            break;
        case 'p':
            f_p(&data_count, &head, &tail, &p_was_started);
            break;
        case 'z':
            f_z(&head, &tail, &data_count);
            break;
        case 'r':
            f_r(&head, &tail, &data_count);
            break;
        case 'k':
            if (head != NULL)
            {
                while (head != NULL)
                {
                    temp = head;
                    head = head->next;
                    free(temp);
                    temp = NULL;
                }
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