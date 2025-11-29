/* LIBRARY BOOK ISSUE TRACKER. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Book
{
    int id;
    char title[50];
    char author[50];
    int isIssued;
    char issuedTo[50];
};

void addBook()
{
    struct Book b;
    FILE *fp = fopen("library.dat", "ab");
    if( !fp )
    {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter Book ID: ");
    scanf("%d",&b.id);
    getchar();
    
    printf("Enter the Title: ");
    fgets(b.title, 50, stdin);
    b.title[strcspn(b.title,"\n")] = 0;

    printf("Enter Author: ");
    fgets(b.author, 50, stdin);
    b.author[strcspn(b.author, "\n")] = 0;

    b.isIssued = 0;
    strcpy(b.issuedTo, "None");

    fwrite(&b, sizeof(b), 1, fp);
    fclose(fp);

    printf("Book added successfully!\n");
}

void displayBooks()
{
    struct Book b;
    FILE *fp = fopen("library.dat", "rb");
    if ( !fp )
    {
        printf("No records found!\n");
        return;
    }

    printf("\n%-5s %-45s %-25s %-12s %-15s\n", "ID", "Title", "Author", "Status", "Issued To");
    printf("----------------------------------------------------------------------------------------------------\n");

    while (fread(&b, sizeof(b), 1, fp))
    {
        printf("%-5d %-45s %-25s %-12s %-15s\n",
       b.id, b.title, b.author,
       b.isIssued?"Issued":"Available", b.issuedTo);
    }

    fclose(fp);
}

void issueBook()
{
    int id;
    int found = 0;
    char name[50];
    struct Book b;
    FILE *fp = fopen("library.dat", "rb+");

    if( !fp )
    {
        printf("No record found!\n");
        return;
    }

    printf("Enter Book ID to issue: ");
    scanf("%d", &id);
    getchar();

    while( fread(&b, sizeof(b), 1, fp) )
    {
        if(b.id == id && b.isIssued == 0)
        {
            found = 1;
            printf("Enter name of person issuing the book: ");
            fgets(name, 50, stdin);
            name[strcspn(name, "\n")] = 0;

            b.isIssued = 1;
            strcpy(b.issuedTo, name);

            fseek(fp, -sizeof(b), SEEK_CUR); //Moving the file pointer backwards by the size of one Book structure.
            fwrite(&b, sizeof(b), 1, fp);
            printf("Book issued successfully to %s!\n", name);
            break;
        }
    }

    if( !found )
    {
        printf("Book not available, Already issued");
    }

    fclose(fp);
}

void returnBook()
{
    int id;
    int found = 0;
    struct Book b;
    FILE *fp = fopen("library.dat", "rb+");

    if( !fp )
    {
        printf("No record found!\n");
        return;
    }

    printf("Enter Book ID to return: ");
    scanf("%d", &id);

    while( fread(&b, sizeof(b), 1, fp) )
    {
        if( b.id == id && b.isIssued == 1 )
        {
            found = 1;
            b.isIssued = 0;
            strcpy(b.issuedTo,"none");

            fseek(fp, -sizeof(b), SEEK_CUR); //Moving the file pointer backwards by the size of one Book structure.
            fwrite(&b, sizeof(b), 1, fp);
            printf("Book returned successfully!\n");
            break;
        }
    }

    if (!found)
        printf("Book not found or not issued!\n");

    fclose(fp);
}

int main()
{
    int choice;

    do
    {
        printf("\n====== LIBRARY BOOK ISSUE TRACKER ======\n");
        printf("1. Add New Book\n");
        printf("2. Display All Books\n");
        printf("3. Issue Book\n");
        printf("4. Return Book\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
        case 1:
            addBook();
            break;
        case 2:
            displayBooks();
            break;
        case 3:
            issueBook();
            break;
        case 4:
            returnBook();
            break;
        case 5:
            printf("Exiting program...\n");
            break;
        default:
            printf("Invalid choice! Try again.\n");
        }
    }while(choice != 5);

    return 0;
}