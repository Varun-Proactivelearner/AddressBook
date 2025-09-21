#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "contact.h"

// Validating Name and location
int validateName(char *name) 
{
    for(int i = 0; name[i] != '\0'; i++)    //run loop until null character
    {
        if(!isalpha(name[i]) && name[i] != ' ')     //check only alphabets and space are present
        {
            return 0;   //if any other characters present return 0
        }
    }
    return 1;   //else return 1;
}

// Validating Mobile number
int validateMobile(char *mobile) 
{
    if((strlen(mobile) != 10) && (strlen(mobile) != 11))    //check number length should be 10 or 11
    { 
        return 0;   //if not there return 0
    }
    for(int i = 0; mobile[i] != '\0'; i++)  //run loop until null character
    {
        if(!isdigit(mobile[i]))     //check only digits is present
        {
            return 0;   //if other charcters present return 0
        }
    }
    return 1;   //else return 1
}

int mobileExists(AddressBook *addressBook, char *mobile) 
{
    for (int i = 0; i < addressBook->contactCount; i++) 
    {
        if (strcmp(addressBook->contacts[i].mobile, mobile) == 0) 
        {
            return 1; // Duplicate found
        }
    }
    return 0; // No duplicate
}

// Validating Email
int validateEmail(char *email) 
{
    char *pos1 = strchr(email, '@');    //store '@' address
    char *pos2 = strstr(email, ".com"); //store ".com" address

    if(!pos1 || !pos2 || pos1 == email || (pos2 - pos1 <= 1))   //check if email is valid (should contain '@' and end with '.com')
    {
        return 0;
    }
    for(int i = 0; email[i] != '\0'; i++)   //run loop until null character 
    {
        if(isupper(email[i]) || isspace(email[i]))  //check any uppercase or space is present
        {
            return 0;   //if true return 0
        }
    }
    return 1;   //else return 1
}



// function definition for list contacts
void listContacts(AddressBook *addressBook, int sortCriteria) 
{
    if(addressBook->contactCount == 0)  //if no contacts present, return
    {
        printf("No contacts to display.\n");
        return;
    }
    // Bubble sort algorithm to sort contacts based on selected criteria
    for(int i = 0; i < addressBook->contactCount - 1; i++) 
    {
        for(int j = 0; j < addressBook->contactCount - i - 1; j++)  // Inner loop to compare adjacent contacts
        {
            Contact *a = &addressBook->contacts[j];     // store address of current contact
            Contact *b = &addressBook->contacts[j + 1]; // store address of next contact
            int compare = 0;
            switch(sortCriteria) 
            {
                case 1:
                compare = strcmp(a->name, b->name);
                break;

                case 2: 
                compare = strcmp(a->mobile, b->mobile);
                break;

                case 3:
                compare = strcmp(a->email, b->email);
                break;

                case 4:
                compare = 0;
                break;

                default:
                printf(BOLD RED"Invalid sorting criteria."RESET"\n");
                return;
            }

            if(compare > 0) // If value > 0, then swap them to sort in ascending order
            {
                Contact temp = *a;
                *a = *b;
                *b = temp;
            }
        }
    }

    printf("--------------------------------------------------------------------------------------------------\n");
    printf("|"BOLD CYAN"%-6s"RESET"|"BOLD CYAN" %-21s"RESET"|"BOLD CYAN" %-21s"RESET"|"BOLD CYAN" %-25s"RESET"|"BOLD CYAN" %-15s"RESET"|\n", "S.No", "Name", "Mobile", "Email", "Location");
    printf("--------------------------------------------------------------------------------------------------\n");
    for(int i = 0; i < addressBook->contactCount; i++) 
    {
        Contact *c = &addressBook->contacts[i];     //pointer that holds address of contacts
        printf("|"GREEN"%-6d"RESET"|"GREEN" %-21s"RESET"|"GREEN" %-21s"RESET"|"GREEN" %-25s"RESET"|"GREEN" %-15s"RESET"|\n", i + 1, c->name, c->mobile, c->email, c->loc);
        printf("--------------------------------------------------------------------------------------------------\n");
        
    }
}

void initialize(AddressBook *addressBook) 
{
    addressBook->contactCount = 0;
    //populateAddressBook(addressBook);
    
    // Load contacts from file during initialization (After files)
    FILE *fp = fopen("contacts.txt", "r");
    if (fp == NULL) 
    {
        printf("Could not open contacts.txt.\n");
        return;
    }

    while (fscanf(fp, " %[^,],%[^,],%[^,],%[^\n]\n", addressBook->contacts[addressBook->contactCount].name, 
        addressBook->contacts[addressBook->contactCount].mobile, addressBook->contacts[addressBook->contactCount].email, 
        addressBook->contacts[addressBook->contactCount].loc) == 4)
    {
        addressBook->contactCount++;
        if (addressBook->contactCount >= MAX_CONTACTS) 
        {
            printf(BOLD RED"Address book is full. Cannot load more contacts."RESET"\n");
            break;
        }
    }

    fclose(fp);

    //loadContactsFromFile(addressBook);
}

void saveContactsToFile(AddressBook *addressBook) 
{
    FILE *fp = fopen("contacts.txt", "w");
    if (fp == NULL) 
    {
        printf(BOLD RED"Could not open contacts.txt for writing."RESET"\n");
        return;
    }

    for (int i = 0; i < addressBook->contactCount; i++) 
    {
        Contact *c = &addressBook->contacts[i];
        fprintf(fp, "%s,%s,%s,%s\n", c->name, c->mobile, c->email, c->loc);
    }

    fclose(fp);
    exit(EXIT_SUCCESS); // Exit the program
}


// function definition for creating contacts
void createContact(AddressBook *addressBook)
{
	/* Define the logic to create a Contacts */
     
    Contact *c = &addressBook->contacts[addressBook->contactCount]; //pointer that holds address of array of structure

    while(1) 
    {
        printf(BOLD BLUE"Enter name: "RESET);
        scanf(" %[^\n]", c->name);  //store name from user
        if(validateName(c->name))   //based on return value validate name
        { 
            break;  //if valid break the loop
        }
        else
        {
            printf(BOLD RED"Only Characters are allowed!"RESET"\n");
            continue;   //else ask again
        }
    }

    while(1) 
    {
        printf(BOLD BLUE"Enter mobile: "RESET);   
        scanf(" %[^\n]", c->mobile);    //store number from user
        if(!validateMobile(c->mobile))   //if not valid
        { 
            printf(BOLD RED"Invalid mobile number!"RESET"\n");
            continue;  //ask again
        }

        if (mobileExists(addressBook, c->mobile))   //check number exists or not
        {
            printf(BOLD RED"This mobile number already exists!"RESET"\n");
            continue;
        }
        break;
    }

    while(1) 
    {
        printf(BOLD BLUE"Enter email: "RESET);
        scanf(" %[^\n]", c->email);     //store mail from user
        if(validateEmail(c->email))     //based on return value validate email
        {
            break;  //if valid break the loop
        }
        else
        {
            printf(BOLD RED"Invalid email!"RESET"\n");
            continue;   //else ask again
        }
    }

    while(1) 
    {
        printf(BOLD BLUE"Enter location: "RESET);     
        scanf(" %[^\n]", c->loc);       //store location from user
        if(validateName(c->loc))        //based on return value validate location
        {
            break;  //if valid break the loop
        }
        else
        {
            printf(BOLD RED"Invalid location!"RESET"\n");
            continue;   //else ask again
        }
    }

    addressBook->contactCount++;    //increment count if contact saved
    printf(BOLD GREEN"-------------------------------\n");
    printf("| Contact added successfully! |\n");
    printf("-------------------------------"RESET"\n");
}

//Function definition for searching contact

int searchContact(AddressBook *addressBook)
{
    int choice;
    char search[100];
    int count = 0;
    int Index[MAX_CONTACTS]; // store actual indices of matches

    printf(BOLD"\nSearch by:\n1. Name\n2. Mobile\n3. Email\nEnter choice: "RESET);
    scanf("%d", &choice);

    switch(choice)
    {
        case 1:
        printf(BOLD BLUE"Enter name to search: "RESET);
        scanf(" %[^\n]", search);
        break;

        case 2:
        printf(BOLD BLUE"Enter mobile to search: "RESET);
        scanf(" %[^\n]", search);
        break;

        case 3:
        printf(BOLD BLUE"Enter email to search: "RESET);
        scanf(" %[^\n]", search);
        break;
        
        default:
        printf(BOLD RED"Invalid choice."RESET"\n");
        return -1;
    }

    printf(BOLD YELLOW"\nSearch results"RESET"\n");
    printf("--------------------------------------------------------------------------------------------------\n");
    printf("|"BOLD CYAN"%-6s"RESET"|"BOLD CYAN" %-21s"RESET"|"BOLD CYAN" %-21s"RESET"|"BOLD CYAN" %-25s"RESET"|"BOLD CYAN" %-15s"RESET"|\n", "S.No", "Name", "Mobile", "Email", "Location");
    printf("--------------------------------------------------------------------------------------------------\n");

    for(int i = 0; i < addressBook->contactCount; i++)
    {
        Contact *c = &addressBook->contacts[i];
        int match = 0;
        if(choice == 1 && strcasestr(c->name, search))
            match = 1;
        else if(choice == 2 && strcasestr(c->mobile, search))
            match = 1;
        else if(choice == 3 && strcasestr(c->email, search))
            match = 1;

        if(match)
        {
            count++;
            Index[count - 1] = i; // store actual index
            printf("|"GREEN"%-6d"RESET"|"GREEN" %-21s"RESET"|"GREEN" %-21s"RESET"|"GREEN" %-25s"RESET"|"GREEN" %-15s"RESET"|\n", count, c->name, c->mobile, c->email, c->loc);
            printf("--------------------------------------------------------------------------------------------------\n");
        }
    }

    if(count == 0)
    {
        printf(BOLD RED"No matches found."RESET"\n");
        return -1;
    }

    int select;
    printf(BOLD YELLOW"\nEnter serial number of contact to view details: "RESET);
    scanf("%d", &select);

    if(select > 0 && select <= count)
    {
        int actualIndex = Index[select - 1];
        Contact c = addressBook->contacts[actualIndex];
        printf(BOLD YELLOW"\nSelected Contact"RESET"\n");
        printf("--------------------------------------------------------------------------------------------------\n");
        printf("|"BOLD CYAN"%-6s"RESET"|"BOLD CYAN" %-21s"RESET"|"BOLD CYAN" %-21s"RESET"|"BOLD CYAN" %-25s"RESET"|"BOLD CYAN" %-15s"RESET"|\n", "S.No", "Name", "Mobile", "Email", "Location");
        printf("--------------------------------------------------------------------------------------------------\n");
        printf("|"GREEN"%-6d"RESET"|"GREEN" %-21s"RESET"|"GREEN" %-21s"RESET"|"GREEN" %-25s"RESET"|"GREEN" %-15s"RESET"|\n", select, c.name, c.mobile, c.email, c.loc);
        printf("--------------------------------------------------------------------------------------------------\n");
        return actualIndex; // return actual index in address book
    }
    else
    {
        printf(BOLD RED"Invalid serial number."RESET"\n");
        return -1;
    }
}


//Function definition for edit contact
void editContact(AddressBook *addressBook)
{
	/* Define the logic for Editcontact */
    if(addressBook->contactCount == 0) 
    {
        printf(BOLD RED"No contacts available to edit."RESET"\n");
        return;
    }

    int index = searchContact(addressBook); // Returns index of chosen contact
    if(index == -1) 
    {
        printf(BOLD RED"No matching contact found."RESET"\n");
        return;
    }

    int choice;
    Contact *c = &addressBook->contacts[index];
    do
    {
        printf(BOLD"\nWhat do you want to edit?\n1. Name\n2. Mobile\n3. Email\n4. Location\n5. Exit"RESET"\n");
        printf(BOLD YELLOW"Enter choice: "RESET);
        scanf("%d", &choice);
        
        switch(choice) 
        {
            case 1:
            while(1)
            {
                printf(BOLD BLUE"Enter new name: "RESET);
                scanf(" %[^\n]", c->name); 
                if(validateName(c->name))   //based on return value validate name
                { 
                    break;  //if valid break the loop
                }
                else
                {
                    printf(BOLD RED"Only Characters are allowed!"RESET"\n");
                    continue;   //else ask again
                }
            }
            break;

            case 2:
            while(1) 
            {
                printf(BOLD BLUE"Enter new mobile number: "RESET);   
                scanf(" %[^\n]", c->mobile);    //store number from user
                if(validateMobile(c->mobile))   //based on return value validate number
                { 
                    break;  //if valid break the loop
                }
                else
                {
                    printf(BOLD RED"Invalid mobile number!"RESET"\n");
                    continue;   //else ask again
                }
            }
            break;
            
            case 3:
            while(1) 
            {
                printf(BOLD BLUE"Enter new email: "RESET);
                scanf(" %[^\n]", c->email);     //store mail from user
                if(validateEmail(c->email))     //based on return value validate email
                {
                    break;  //if valid break the loop
                }
                else
                {
                    printf(BOLD RED"Invalid email!"RESET"\n");
                    continue;   //else ask again
                }
            }
            break;

            case 4:
            while(1) 
            {
                printf(BOLD BLUE"Enter new location: "RESET);     
                scanf(" %[^\n]", c->loc);       //store location from user
                if(validateName(c->loc))        //based on return value validate location
                {
                    break;  //if valid break the loop
                }
                else
                {
                    printf(BOLD RED"Invalid location!"RESET"\n");
                    continue;   //else ask again
                }
            }
            break;

            case 5:
            break;

            default:
            printf(BOLD RED"Invalid choice. No changes made."RESET"\n");
            return;
        }
    }while(choice != 5);

    printf(BOLD GREEN"---------------------------------\n");
    printf("| Contact updated successfully! |\n");
    printf("---------------------------------"RESET"\n");
}


//Function definition for delete contact
void deleteContact(AddressBook *addressBook) 
{
    int index = searchContact(addressBook); // search first
    if(index == -1) 
    {
        printf(BOLD RED"No contact found."RESET"\n");
        return;
    }

    char confirm;
    printf(BOLD YELLOW"Are you sure you want to delete this contact? (y/n): "RESET);
    scanf(" %c", &confirm);

    if(confirm == 'y' || confirm == 'Y') 
    {
        for(int i = index; i < addressBook->contactCount - 1; i++) 
        {
            strcpy(addressBook->contacts[i].name, addressBook->contacts[i + 1].name);
            strcpy(addressBook->contacts[i].mobile, addressBook->contacts[i + 1].mobile);
            strcpy(addressBook->contacts[i].email, addressBook->contacts[i + 1].email);
            strcpy(addressBook->contacts[i].loc, addressBook->contacts[i + 1].loc);
        }
        addressBook->contactCount--;
        printf(BOLD GREEN"-------------------------------\n");
        printf("|Contact deleted successfully.|\n");
        printf("-------------------------------"RESET"\n");
    } 
    else if(confirm == 'n' || confirm == 'N') 
    {
        printf(BOLD RED"-----------------------------\n");
        printf("|Delete operation cancelled.|\n");
        printf("-----------------------------"RESET"\n");
    }
    else
    {
        printf(BOLD RED"Invalid Choice!."RESET"\n");
    }
}

