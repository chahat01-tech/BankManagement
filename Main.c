#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Account
{
    int accNo;
    char name[50];
    int age;
    char phone[15];
    float balance;
};

void createAccount();
void displayAccounts();
void searchAccount();
void depositMoney();
void withdrawMoney();
void checkBalance();
void modifyAccount();
void deleteAccount();

int accountExists(int number);

int main()
{
    int choice;

    do
    {
        printf("\n====================================");
        printf("\n       BANK MANAGEMENT SYSTEM");
        printf("\n====================================");
        printf("\n1. Create New Account");
        printf("\n2. Display All Accounts");
        printf("\n3. Search Account");
        printf("\n4. Deposit Money");
        printf("\n5. Withdraw Money");
        printf("\n6. Check Balance");
        printf("\n7. Modify Account Details");
        printf("\n8. Delete Account");
        printf("\n9. Exit");
        printf("\n====================================");
        printf("\nEnter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("\nInvalid input!");
            while (getchar() != '\n')
                ;
            continue;
        }

        switch (choice)
        {
        case 1:
            createAccount();
            break;
        case 2:
            displayAccounts();
            break;
        case 3:
            searchAccount();
            break;
        case 4:
            depositMoney();
            break;
        case 5:
            withdrawMoney();
            break;
        case 6:
            checkBalance();
            break;
        case 7:
            modifyAccount();
            break;
        case 8:
            deleteAccount();
            break;
        case 9:
            printf("\nThank you for using our banking system!");
            break;
        default:
            printf("\nInvalid choice! Try again.");
        }

    } while (choice != 9);

    return 0;
}

// Check whether account number already exists
int accountExists(int number)
{
    FILE *fp = fopen("bank.dat", "rb");
    struct Account a;

    if (fp == NULL)
        return 0;

    while (fread(&a, sizeof(a), 1, fp))
    {
        if (a.accNo == number)
        {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

// 1. Create Account
void createAccount()
{
    FILE *fp;
    struct Account a;

    printf("\nEnter Account Number: ");
    scanf("%d", &a.accNo);

    if (accountExists(a.accNo))
    {
        printf("\nAccount number already exists!");
        return;
    }

    printf("Enter Customer Name: ");
    scanf(" %49[^\n]", a.name);

    printf("Enter Age: ");
    scanf("%d", &a.age);

    printf("Enter Phone Number: ");
    scanf("%14s", a.phone);

    printf("Enter Initial Deposit: ");
    scanf("%f", &a.balance);

    if (a.age < 18 || a.balance < 0)
    {
        printf("\nInvalid age or deposit!");
        return;
    }

    fp = fopen("bank.dat", "ab");

    if (fp == NULL)
    {
        printf("\nFile opening error!");
        return;
    }

    fwrite(&a, sizeof(a), 1, fp);
    fclose(fp);

    printf("\nAccount Created Successfully!");
}

// 2. Display All Accounts
void displayAccounts()
{
    FILE *fp = fopen("bank.dat", "rb");
    struct Account a;

    if (fp == NULL)
    {
        printf("\nNo account records found!");
        return;
    }

    printf("\n========== ALL ACCOUNTS ==========");

    while (fread(&a, sizeof(a), 1, fp))
    {
        printf("\n\nAccount Number: %d", a.accNo);
        printf("\nName: %s", a.name);
        printf("\nAge: %d", a.age);
        printf("\nPhone: %s", a.phone);
        printf("\nBalance: Rs. %.2f", a.balance);
    }

    fclose(fp);
}

// 3. Search Account
void searchAccount()
{
    FILE *fp = fopen("bank.dat", "rb");
    struct Account a;
    int number, found = 0;

    if (fp == NULL)
    {
        printf("\nNo records found!");
        return;
    }

    printf("\nEnter Account Number: ");
    scanf("%d", &number);

    while (fread(&a, sizeof(a), 1, fp))
    {
        if (a.accNo == number)
        {
            printf("\nAccount Found!");
            printf("\nName: %s", a.name);
            printf("\nAccount Number: %d", a.accNo);
            printf("\nBalance: Rs. %.2f", a.balance);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("\nAccount Not Found!");

    fclose(fp);
}

// 4. Deposit Money
void depositMoney()
{
    FILE *fp = fopen("bank.dat", "rb+");
    struct Account a;
    int number, found = 0;
    float amount;

    if (fp == NULL)
    {
        printf("\nNo records found!");
        return;
    }

    printf("\nEnter Account Number: ");
    scanf("%d", &number);

    while (fread(&a, sizeof(a), 1, fp))
    {
        if (a.accNo == number)
        {
            printf("Enter Deposit Amount: ");
            scanf("%f", &amount);

            if (amount <= 0)
            {
                printf("\nInvalid amount!");
                found = 1;
                break;
            }

            a.balance += amount;

            fseek(fp, -(long)sizeof(a), SEEK_CUR);
            fwrite(&a, sizeof(a), 1, fp);

            printf("\nMoney Deposited Successfully!");
            printf("\nNew Balance: Rs. %.2f", a.balance);

            found = 1;
            break;
        }
    }

    if (!found)
        printf("\nAccount Not Found!");

    fclose(fp);
}

// 5. Withdraw Money
void withdrawMoney()
{
    FILE *fp = fopen("bank.dat", "rb+");
    struct Account a;
    int number, found = 0;
    float amount;

    if (fp == NULL)
    {
        printf("\nNo records found!");
        return;
    }

    printf("\nEnter Account Number: ");
    scanf("%d", &number);

    while (fread(&a, sizeof(a), 1, fp))
    {
        if (a.accNo == number)
        {
            printf("Enter Withdrawal Amount: ");
            scanf("%f", &amount);

            if (amount <= 0 || amount > a.balance)
            {
                printf("\nInvalid amount or Insufficient Balance!");
                found = 1;
                break;
            }

            a.balance -= amount;

            fseek(fp, -(long)sizeof(a), SEEK_CUR);
            fwrite(&a, sizeof(a), 1, fp);

            printf("\nWithdrawal Successful!");
            printf("\nRemaining Balance: Rs. %.2f", a.balance);

            found = 1;
            break;
        }
    }

    if (!found)
        printf("\nAccount Not Found!");

    fclose(fp);
}

// 6. Check Balance
void checkBalance()
{
    FILE *fp = fopen("bank.dat", "rb");
    struct Account a;
    int number, found = 0;

    if (fp == NULL)
    {
        printf("\nNo records found!");
        return;
    }

    printf("\nEnter Account Number: ");
    scanf("%d", &number);

    while (fread(&a, sizeof(a), 1, fp))
    {
        if (a.accNo == number)
        {
            printf("\nAccount Holder: %s", a.name);
            printf("\nAvailable Balance: Rs. %.2f", a.balance);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("\nAccount Not Found!");

    fclose(fp);
}

// 7. Modify Account Details
void modifyAccount()
{
    FILE *fp = fopen("bank.dat", "rb+");
    struct Account a;
    int number, found = 0;

    if (fp == NULL)
    {
        printf("\nNo records found!");
        return;
    }

    printf("\nEnter Account Number: ");
    scanf("%d", &number);

    while (fread(&a, sizeof(a), 1, fp))
    {
        if (a.accNo == number)
        {
            printf("\nEnter New Name: ");
            scanf(" %49[^\n]", a.name);

            printf("Enter New Age: ");
            scanf("%d", &a.age);

            printf("Enter New Phone Number: ");
            scanf("%14s", a.phone);

            if (a.age < 18)
            {
                printf("\nInvalid age!");
                found = 1;
                break;
            }

            fseek(fp, -(long)sizeof(a), SEEK_CUR);
            fwrite(&a, sizeof(a), 1, fp);

            printf("\nAccount Updated Successfully!");
            found = 1;
            break;
        }
    }

    if (!found)
        printf("\nAccount Not Found!");

    fclose(fp);
}

// 8. Delete Account
void deleteAccount()
{
    FILE *fp = fopen("bank.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");

    struct Account a;
    int number, found = 0;

    if (fp == NULL || temp == NULL)
    {
        printf("\nFile opening error!");
        if (fp != NULL)
            fclose(fp);
        if (temp != NULL)
            fclose(temp);
        return;
    }

    printf("\nEnter Account Number to Delete: ");
    scanf("%d", &number);

    while (fread(&a, sizeof(a), 1, fp))
    {
        if (a.accNo == number)
        {
            found = 1;
        }
        else
        {
            fwrite(&a, sizeof(a), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    if (found)
    {
        if (remove("bank.dat") == 0 &&
            rename("temp.dat", "bank.dat") == 0)
        {
            printf("\nAccount Deleted Successfully!");
        }
        else
        {
            printf("\nFile update error!");
        }
    }
    else
    {
        remove("temp.dat");
        printf("\nAccount Not Found!");
    }
}