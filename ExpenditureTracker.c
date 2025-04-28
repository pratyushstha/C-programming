#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EXPENSES 100
#define MAX_CATEGORY_LEN 50
#define MAX_DATE_LEN 15

struct Expense {
    char category[MAX_CATEGORY_LEN];
    float amount;
    char date[MAX_DATE_LEN];
};

struct Expense expenses[MAX_EXPENSES];
int expenseCount = 0;
float budget = 0.0;

void clearInputBuffer() {
    while (getchar() != '\n');
}

void setBudget() {
    printf("\n--- Set Budget ---\n");
    printf("Enter your monthly budget (Rs.): ");
    scanf("%f", &budget);
    clearInputBuffer();
    printf("✅ Budget set to Rs. %.2f\n", budget);
}

void addExpense() {
    if (expenseCount >= MAX_EXPENSES) {
        printf("\n❌ Expense list is full!\n");
        return;
    }
    printf("\n--- Add Expense ---\n");
    printf("Enter category (use underscores if needed): ");
    scanf("%49s", expenses[expenseCount].category);

    printf("Enter amount (Rs.): ");
    scanf("%f", &expenses[expenseCount].amount);

    printf("Enter date (YYYY-MM-DD): ");
    scanf("%14s", expenses[expenseCount].date);

    clearInputBuffer();
    expenseCount++;
    printf("✅ Expense added successfully!\n");

    if (budget > 0) {
        float total = 0;
        for (int i = 0; i < expenseCount; i++) {
            total += expenses[i].amount;
        }
        if (total > budget) {
            printf("⚠️ Warning: You have exceeded your budget of Rs. %.2f!\n", budget);
        }
    }
}

void viewExpenses() {
    if (expenseCount == 0) {
        printf("\nℹ️ No expenses recorded.\n");
        return;
    }

    printf("\n======= Expense List =======\n");
    float total = 0;
    for (int i = 0; i < expenseCount; i++) {
        printf("%2d. %-20s Rs. %-10.2f Date: %s\n", i + 1, expenses[i].category, expenses[i].amount, expenses[i].date);
        total += expenses[i].amount;
    }
    printf("=============================\n");
    printf("Total Spent: Rs. %.2f\n", total);
    if (budget > 0) {
        printf("Remaining Budget: Rs. %.2f\n", budget - total);
    }
}

void deleteAllExpenses() {
    if (expenseCount == 0) {
        printf("\nℹ️ No expenses to delete.\n");
        return;
    }

    char choice;
    printf("\nAre you sure you want to delete all expenses? (y/n): ");
    scanf(" %c", &choice);
    clearInputBuffer();

    if (choice == 'y' || choice == 'Y') {
        expenseCount = 0;
        printf("✅ All expenses have been deleted.\n");
    } else {
        printf("❌ Deletion cancelled.\n");
    }
}

void saveExpensesToFile() {
    FILE *file = fopen("expenses.txt", "w");
    if (file == NULL) {
        printf("\n❌ Error saving expenses.\n");
        return;
    }

    fprintf(file, "%.2f\n", budget);
    for (int i = 0; i < expenseCount; i++) {
        fprintf(file, "%s,%.2f,%s\n", expenses[i].category, expenses[i].amount, expenses[i].date);
    }

    fclose(file);
    printf("\n💾 Expenses saved successfully!\n");
}

void loadExpensesFromFile() {
    FILE *file = fopen("expenses.txt", "r");
    if (file == NULL) {
        return;
    }

    fscanf(file, "%f\n", &budget);

    while (expenseCount < MAX_EXPENSES &&
           fscanf(file, "%49[^,],%f,%14s\n",
                  expenses[expenseCount].category,
                  &expenses[expenseCount].amount,
                  expenses[expenseCount].date) == 3) {
        expenseCount++;
    }

    fclose(file);
}

void deleteExpense() {
    int index;
    viewExpenses();
    if (expenseCount == 0) return;

    printf("\nEnter the expense number to delete: ");
    scanf("%d", &index);
    clearInputBuffer();

    if (index < 1 || index > expenseCount) {
        printf("❌ Invalid selection!\n");
        return;
    }

    for (int i = index - 1; i < expenseCount - 1; i++) {
        expenses[i] = expenses[i + 1];
    }
    expenseCount--;
    printf("✅ Expense deleted successfully!\n");
}

void mainMenu() {
    int choice;
    loadExpensesFromFile();

    do {
        printf("\n========= Expense Tracker =========\n");
        printf("1. Set Budget\n");
        printf("2. Add Expense\n");
        printf("3. View Expenses\n");
        printf("4. Delete Single Expense\n");
        printf("5. Delete All Expenses\n");
        printf("6. Save and Exit\n");
        printf("====================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1: setBudget(); break;
            case 2: addExpense(); break;
            case 3: viewExpenses(); break;
            case 4: deleteExpense(); break;
            case 5: deleteAllExpenses(); break;
            case 6: saveExpensesToFile(); printf("👋 Exiting...\n"); break;
            default: printf("❌ Invalid choice! Please try again.\n");
        }
    } while (choice != 6);
}

int main() {
    mainMenu();
    return 0;
}
