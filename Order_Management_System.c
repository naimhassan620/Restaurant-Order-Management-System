// Only Allah and I know how the code works
// But since its past 1 day
// Now only Allah knows
// If you ask me question, i probably won't be able to anser
// But I will try my best
// Thanks for your understanding Sir


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Product
{
    char name[50];
    int quantity;
    float price;
    struct Product *next;
};

struct Order
{
    int id;
    char customer[50];
    float total;
    struct Product *phead;
    struct Order *next;
};

struct Order *head = NULL;

void showMenu()
{
    printf("\n--- FOOD MENU ---\n");
    printf("Burger 120\n");
    printf("Pizza 300\n");
    printf("Pasta 200\n");
    printf("Coffee 80\n");
}

void addOrder()
{

    struct Order *newOrder = (struct Order *)malloc(sizeof(struct Order));
    struct Product *phead = NULL, *temp;

    int n;

    printf("Customer name: ");
    scanf(" %[^\n]", newOrder->customer);

    printf("Order id: ");
    scanf("%d", &newOrder->id);

    printf("How many products: ");
    scanf("%d", &n);

    newOrder->total = 0;

    for (int i = 1; i <= n; i++)
    {

        struct Product *newP = (struct Product *)malloc(sizeof(struct Product));

        printf("\nProduct %d name: ", i);
        scanf(" %[^\n]", newP->name);

        printf("Quantity: ");
        scanf("%d", &newP->quantity);

        printf("Price: ");
        scanf("%f", &newP->price);

        newOrder->total += newP->quantity * newP->price;

        newP->next = NULL;

        if (phead == NULL)
        {
            phead = newP;
        }
        else
        {
            temp = phead;
            while (temp->next != NULL)
                temp = temp->next;
            temp->next = newP;
        }
    }

    newOrder->phead = phead;

    newOrder->next = head;
    head = newOrder;

    printf("Order added. Total bill = %.2f\n", newOrder->total);
}

void displayOrders()
{

    struct Order *temp = head;

    if (temp == NULL)
    {
        printf("No orders\n");
        return;
    }

    while (temp != NULL)
    {

        printf("\nOrder ID: %d\n", temp->id);
        printf("Customer: %s\n", temp->customer);
        printf("Total bill: %.2f\n", temp->total);

        struct Product *p = temp->phead;

        printf("Products:\n");

        while (p != NULL)
        {
            printf(" - %s | qty:%d | price:%.2f\n", p->name, p->quantity, p->price);
            p = p->next;
        }

        temp = temp->next;
    }
}

void saveToFile()
{

    FILE *fp = fopen("orders.txt", "w");

    if (fp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    struct Order *o = head;

    while (o != NULL)
    {

        fprintf(fp, "%d\n%s\n%.2f\n", o->id, o->customer, o->total);

        struct Product *p = o->phead;

        // count products
        int count = 0;
        struct Product *tmp = p;
        while (tmp != NULL)
        {
            count++;
            tmp = tmp->next;
        }

        fprintf(fp, "%d\n", count);

        while (p != NULL)
        {
            fprintf(fp, "%s\n%d\n%.2f\n", p->name, p->quantity, p->price);
            p = p->next;
        }

        o = o->next;
    }

    fclose(fp);
    printf("Data saved to file.\n");
}

void searchOrder()
{

    int id;
    struct Order *temp = head;

    printf("Enter order id: ");
    scanf("%d", &id);

    while (temp != NULL)
    {

        if (temp->id == id)
        {

            printf("\nCustomer: %s\n", temp->customer);
            printf("Total bill: %.2f\n", temp->total);

            struct Product *p = temp->phead;

            while (p != NULL)
            {
                printf("%s qty:%d price:%.2f\n", p->name, p->quantity, p->price);
                p = p->next;
            }

            return;
        }

        temp = temp->next;
    }

    printf("Order not found\n");
}

void deleteOrder()
{

    int id, ch;
    struct Order *temp = head, *prev = NULL;

    printf("Enter order id: ");
    scanf("%d", &id);

    while (temp != NULL)
    {

        if (temp->id == id)
        {

            printf("Delete this order? (1 yes /0 no): ");
            scanf("%d", &ch);

            if(ch == 1)
            {

                if (prev == NULL)
                    head = temp->next;
                else
                    prev->next = temp->next;

                // 👇 ADD THIS BLOCK HERE
                struct Product *p = temp->phead;
                while (p != NULL)
                {
                    struct Product *del = p;
                    p = p->next;
                    free(del);
                }

                free(temp); // now safe

                printf("Order deleted\n");
            }

            return;
        }

        prev = temp;
        temp = temp->next;
    }

    printf("Order not found\n");
}

void loadFromFile()
{

    FILE *fp = fopen("orders.txt", "r");

    if (fp == NULL)
    {
        printf("No previous data found.\n");
        return;
    }

    while (1)
    {

        struct Order *newOrder = (struct Order *)malloc(sizeof(struct Order));

        if (fscanf(fp, "%d\n", &newOrder->id) != 1)
        {
            free(newOrder);
            break;
        }

        fgets(newOrder->customer, 50, fp);
        newOrder->customer[strcspn(newOrder->customer, "\n")] = 0;

        fscanf(fp, "%f\n", &newOrder->total);

        int n;
        fscanf(fp, "%d\n", &n);

        struct Product *phead = NULL, *temp;

        for (int i = 0; i < n; i++)
        {

            struct Product *newP = (struct Product *)malloc(sizeof(struct Product));

            fgets(newP->name, 50, fp);
            newP->name[strcspn(newP->name, "\n")] = 0;

            fscanf(fp, "%d\n", &newP->quantity);
            fscanf(fp, "%f\n", &newP->price);

            newP->next = NULL;

            if (phead == NULL)
            {
                phead = newP;
            }
            else
            {
                temp = phead;
                while (temp->next != NULL)
                    temp = temp->next;
                temp->next = newP;
            }
        }

        newOrder->phead = phead;

        newOrder->next = head;
        head = newOrder;
    }

    fclose(fp);
    printf("Data loaded from file.\n");
}

int main()
{
    loadFromFile();

    int mainChoice, choice, pass;

    while (1)
    {
        printf("\n--- MAIN MENU ---\n");
        printf("1. Admin Panel\n");
        printf("2. User Panel\n");
        printf("3. Exit\n");
        printf("Choice: ");
        scanf("%d", &mainChoice);

        // -------- ADMIN PANEL --------
        if (mainChoice == 1)
        {
            printf("Enter password: ");
            scanf("%d", &pass);

            if (pass != 1234)
            {
                printf("Wrong password!\n");
                continue;
            }

            while (1)
            {
                printf("\n--- ADMIN PANEL ---\n");
                printf("1 Display all orders\n");
                printf("2 Search order\n");
                printf("3 Delete order\n");
                printf("4 Save to file\n");
                printf("5 Back\n");
                printf("Choice: ");
                scanf("%d", &choice);

                if (choice == 1)
                    displayOrders();
                else if (choice == 2)
                    searchOrder();
                else if (choice == 3)
                    deleteOrder();
                else if (choice == 4)
                    saveToFile();
                else if (choice == 5)
                    break;
                else
                    printf("Invalid choice\n");
            }
        }

        // -------- USER PANEL --------
        else if (mainChoice == 2)
        {
            while (1)
            {
                printf("\n--- USER PANEL ---\n");
                printf("1 Show menu\n");
                printf("2 Place order\n");
                printf("3 Search order\n");
                printf("4 Back\n");
                printf("Choice: ");
                scanf("%d", &choice);

                if (choice == 1)
                    showMenu();
                else if (choice == 2)
                    addOrder();
                else if (choice == 3)
                    searchOrder();
                else if (choice == 4)
                    break;
                else
                    printf("Invalid choice\n");
            }
        }

        else if (mainChoice == 3)
        {
            saveToFile(); // exit er age save
            exit(0);
        }

        else
        {
            printf("Invalid choice\n");
        }
    }
}
