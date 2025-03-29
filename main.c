//Simon Sherwood 318597036
//Emily Alshtein 207796533
//Ifat Vider 206865941

#define _CRT_SECURE_NO_WARNINGS
#include "employees.h"
#include "menu.h"
#include <stdio.h>
#include "items.h"
#include "customers.h"
#include <string.h>
#include <stdlib.h>
#include "logger.h"
#define LOG_FILE "system_log.log"

int main()
{
    // Initialize log file at the start
   log_message(LOG_FILE,"Managing system has been opened.");


    //setbuf(stdout, NULL);
    //load customers from file to tree
    CustomerNode* customers_root = load_customers_from_file("our_customers.txt");


    //load items from file to tree
    ItemNode* items_root= loadItemsFromFile("our_items.bin");


    Employee* employees_head = NULL;
    if (!check_if_employee_file_exists())
     {
         employees_head = createDefaultAdmin();
     }
    else
    {
        employees_head = loadEmployeesFromFile("our_employees.txt");
    }


     Employee* currentEmployee = login(employees_head);
     if (currentEmployee == NULL) {
         printf("Login failed. Exiting system.\n");
         log_message(LOG_FILE,"Login failed. Exiting system.");
         return 1;
     }


     int choice=1;
     while (choice !=0)
     {
         showMenu(*currentEmployee);  // Display menu based on employee's authorization level
         scanf("%d",&choice);  // Get user's choice
         performAction(choice,*currentEmployee,&employees_head,&customers_root,&items_root);
         // Perform the corresponding action
     }

    save_items_to_file_end(items_root, "our_items.bin");
    save_customers_to_file_end(customers_root, "our_customers.txt");
    save_employees_to_file_end(employees_head, "our_employees.txt");

    // **Free memory**
    if (items_root) free_items(items_root);
    if (customers_root) free_customers(customers_root);
    if (employees_head) free_employees(employees_head);

    printf("System exited successfully.\n");
    log_message(LOG_FILE,"System exited successfully.");
    return 0;                               
}
