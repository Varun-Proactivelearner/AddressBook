#include <stdio.h>
#include "contact.h"

int main() {
    int choice;
    AddressBook addressBook;
    initialize(&addressBook); // Initialize the address book

    do {
        printf(BOLD RED"\nAddress Book Menu:"RESET"\n");
        printf(BOLD"1. Create contact\n");
        printf("2. Search contact\n");
        printf("3. Edit contact\n");
        printf("4. Delete contact\n");
        printf("5. List all contacts\n");
        printf("6. Save & Exit"RESET"\n");
        printf(BOLD YELLOW"Enter your choice: "RESET);
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                createContact(&addressBook);
                break;
            case 2:
                searchContact(&addressBook);
                break;
            case 3:
                editContact(&addressBook);
                break;
            case 4:
                deleteContact(&addressBook);
                break;
            case 5:
                printf("------------------------\n");
                printf("| Select sort criteria |\n");
                printf("------------------------\n");
                printf("1. Sort by name\n");
                printf("2. Sort by phone\n");
                printf("3. Sort by email\n");
                printf("4. Without Sorting\n");
                printf("------------------------\n");
                printf(BOLD YELLOW"Enter your choice: "RESET);
                int sortChoice;
                scanf("%d", &sortChoice);
                printf("------------------------\n");
                listContacts(&addressBook, sortChoice);
                break;
            case 6:
                printf(BOLD GREEN"Saving and Exiting..."RESET"\n");
                saveContactsToFile(&addressBook);
                break;
            default:
                printf(BOLD RED"Invalid choice. Please try again."RESET"\n");
        }
    } while (choice != 6);
    
       return 0;
}
