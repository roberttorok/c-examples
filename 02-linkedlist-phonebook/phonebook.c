#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct phone_entry {
    char name[256];
    char number[256];
    struct phone_entry *next;
};

#define MAX_ENTRIES 50000

#define USE_LINKED_LIST

struct phone_entry entries[MAX_ENTRIES];
struct phone_entry *head = NULL;
struct phone_entry *tail = NULL;
int cnt_entries = 0;

void insert_to_phonebook_linkedlist() {
    struct phone_entry *entry = (struct phone_entry *)malloc(sizeof(struct phone_entry));
    printf("Name:\n");
    fgets(entry->name, 1024, stdin);
    entry->name[strlen(entry->name) - 1] = 0;

    printf("Number:\n");
    fgets(entry->number, 1024, stdin);
    entry->number[strlen(entry->number) - 1] = 0;

    entry->next = NULL;

    if (head == NULL) {
        head = entry;
        tail = entry;
    } else {
        tail->next = entry;
        tail = entry;
    }
}

void list_phonebook_linkedlist() {
    struct phone_entry *tmp = head;
    printf("\nEntries of the phonebook:\n");
    while (tmp) {
        printf("Name: %s\n", tmp->name);
        printf("Number: %s\n\n", tmp->number);
        tmp = tmp->next;
    }
}

void delete_from_phonebook_linkedlist() {
    char name[1024];
    struct phone_entry *tmp, *prev;

    printf("\nEnter the index of the record you want to delete:\n");
    fgets(name, 1024, stdin);
    name[strlen(name) - 1] = 0;

    tmp = head;
    prev = NULL;
    while (tmp) {
        if (!strcmp(tmp->name, name)) {            
            if (!tmp->next) {
                tail = prev;
            }
            if (prev) {
                prev->next = tmp->next;
            } else {
                head = tmp->next;
            }
            free(tmp);
            printf("Successfully deleted %s\n", name);
            break;
        }
        prev = tmp;
        tmp = tmp->next;
    }
}

void insert_to_phonebook() {
    char name[256];
    char number[256];

    if (cnt_entries > MAX_ENTRIES) {
        printf("Can't add more entires to the phonebook!\n");
        return;
    }

    printf("Name:\n");
    fgets(name, 256, stdin);
    name[strlen(name) - 1] = 0;

    printf("Number:\n");
    fgets(number, 256, stdin);
    number[strlen(number) - 1] = 0;

    strcpy(entries[cnt_entries].name, name);
    strcpy(entries[cnt_entries].number, number);

    printf("Record added!\n\n");

    cnt_entries++;
}

void list_entries_phonebook() {
    printf("\nEntries of the phonebook:\n");
    int i = 0;
    for (i = 0; i < cnt_entries; i++) {
        printf("Record %i\n", i);
        printf("Name: %s\n", entries[i].name);
        printf("Number: %s\n\n", entries[i].number);
    }
}

void delete_from_phonebook() {
    char name[1024];
    int record_index = -1;
    int i = 0;

    printf("\nEnter the name of the record you want to delete:\n");
    fgets(name, 1024, stdin);
    name[strlen(name) - 1] = 0;

    for (i = 0; i < cnt_entries; i++) {
        if (!strcmp(entries[i].name, name)) {
            record_index = i;
            break;
        }
    }

    if (record_index == -1) {
        printf("Couldn't find %s\n", name);
        return;
    }

    for (i = record_index + 1; i < cnt_entries; i++) {
        strcpy(entries[i - 1].name, entries[i].name);
        strcpy(entries[i - 1].number, entries[i].number);
    }

    printf("Record %s succesfully deleted!\n\n", name);

    cnt_entries--;
}

int main(void) {
    int choice;
    char tmp[1024];

    while (1) {
        printf("1. Create new entry\n");
        printf("2. List entries\n");
        printf("3. Delete entry\n");
        printf("4. Exit\n");
        printf("Your choice: \n");
        choice = atoi(fgets(tmp, 1024, stdin));

        switch (choice) {
            case 1: {
#ifdef USE_LINKED_LIST
                insert_to_phonebook_linkedlist();
#else                
                insert_to_phonebook();                
#endif                
                break;
            }
            case 2: {
#ifdef USE_LINKED_LIST                
                list_phonebook_linkedlist();
#else                
                list_entries_phonebook();                
#endif                
                break;
            }
            case 3: {
#ifdef USE_LINKED_LIST                
                delete_from_phonebook_linkedlist();
#else                
                delete_from_phonebook();
#endif                
                break;
            }
            case 4: {
                return 0;
                break;
            }
        }
    }

    return 0;
}
