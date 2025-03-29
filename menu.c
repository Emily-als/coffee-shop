//Simon Sherwood 318597036
//Emily Alshtein 207796533
//Ifat Vider 206865941

#define _CRT_SECURE_NO_WARNINGS
#include "menu.h"
#include "employees.h"
#include <stdio.h>
#include <stdlib.h>
#include "logger.h"

#define LOG_FILE "system_log.log"
void showMenu(Employee employee) {
    printf("\nWelcome!\n");
    printf("=================================\n");

    // Basic actions for all employees
    printf("1. Search Items\n");
    printf("2. Search Customers\n");
    printf("3. Add New Item\n");
    printf("4. Add New Customer\n");

    // Actions for Employees and Admin (Level 2+)
    if (employee.level <= 2) {
        printf("5. Update Item\n");
        printf("6. Update Customer\n");
        printf("7. Remove Item\n");
        printf("8. Remove Customer\n");
    }

    // Admin-only actions (Level 1)
    if (employee.level == 1) {
        printf("9. Add New Employee\n");
        printf("10. Remove Employee\n");
    }

    printf("0. Exit\n");
    printf("=================================\n");
    printf("Enter your choice: \n");
}

void performAction(int choice, Employee emp, Employee** employees_head, CustomerNode** customers_root, ItemNode** items_root) {
    switch (choice) {
        case 1:
            if (emp.level <= 3) {
                printf("Choose item search method: \n");
                printf("1. Search by ID.\n");
                printf("2. Search by max price.\n");
                printf("3. Search by exact price.\n");
                printf("4. Search by expiry date\n");
                printf("5. Search by name and category\n");
                printf("0. Cancel\n");

                int choice_search_method;
                scanf("%d", &choice_search_method);  // Get user's choice

                switch (choice_search_method) {
                    case 1: {
                        int item_id;
                        printf("Enter item's ID to search: ");
                        scanf("%d", &item_id);
                        findAndPrintItem(*items_root, item_id);
                        break;
                    }
                    case 2: {
                        int max_price;
                        printf("Enter max price:\n");
                        scanf("%d", &max_price);
                        search_by_max_price(*items_root, max_price);
                        if (emp.level <= 2)
                        {
                            update_item(*items_root);
                        }
                        break;
                    }
                    case 3: {
                        int exact_price;
                        printf("Enter price:\n");
                        scanf("%d", &exact_price);
                        search_by_exact_price(*items_root, exact_price);
                        if (emp.level <= 2)
                        {
                            update_item(*items_root);
                        }
                        break;
                    }
                    case 4: {
                        char input_date[11]; // YYYY-MM-DD format
                        printf("Enter expiry date (YYYY-MM-DD): ");
                        scanf("%10s", input_date);
                        search_by_expiry_date(*items_root, input_date);
                        if (emp.level <= 2)
                        {
                            update_item(*items_root);
                        }
                        break;
                    }
                    case 5: {
                        char name_search[20], category_search[20];
                        printf("Enter item name: ");
                        scanf("%19s", name_search);
                        printf("Enter item category: ");
                        scanf("%19s", category_search);
                        search_by_name_and_category(*items_root, name_search, category_search);
                        if (emp.level <= 2)
                        {
                            update_item(*items_root);
                        }
                        break;
                    }
                    case 0: {
                        printf("Exiting the system.\n");
                        break;
                    }
                    default:
                        printf("Invalid choice. Please select a valid option.\n");
                }
            }
            break;
        case 2:
            if (emp.level <= 3) {
                printf("You chose to search customers.\n");
                int id_customer_to_search;
                printf("Enter customer's ID to search: ");
                scanf("%d", &id_customer_to_search);
                find_and_print_customer(*customers_root, id_customer_to_search);
            }
            break;
        case 3:
            if (emp.level <= 3) {
                printf("You chose to add a new item.\n");
                add_new_item(items_root);
            }
            break;
        case 4:
            if (emp.level <= 3) {
                printf("You chose to add a new customer.\n");
                add_new_customer(customers_root);
            }
            break;
        case 5:
            if (emp.level <= 2) {
                printf("You chose to update an item.\n");
                update_item(*items_root);
            } else {
                printf("You do not have permission to update items.\n");
            }
            break;
        case 6:
            if (emp.level <= 2) {
                printf("You chose to update a customer.\n");
                update_customer(*customers_root);
            } else {
                printf("You do not have permission to update customers.\n");
            }
            break;
        case 7:
            if (emp.level <= 2) {
                int id_item_to_delete;
                printf("Enter item ID to delete: ");
                scanf("%d", &id_item_to_delete);
                *items_root = remove_item_from_tree(*items_root, id_item_to_delete);
            } else {
                printf("You do not have permission to remove items.\n");
            }
            break;
        case 8:
            if (emp.level <= 2) {
                int id_customer_to_remove;
                printf("Enter customer ID to remove: ");
                scanf("%d", &id_customer_to_remove);
                *customers_root = delete_customer(*customers_root, id_customer_to_remove);
            } else {
                printf("You do not have permission to remove customers.\n");
            }
            break;
        case 9:
            if (emp.level == 1) {
                printf("You chose to add a new employee.\n");
                add_employee_to_list(employees_head); // FIXED: No "&"
            } else {
                printf("You do not have permission to add new employees.\n");
            }
            break;
        case 10:
            if (emp.level == 1) {
                printf("You chose to remove an employee.\n");
                deleteEmployeeFromList(employees_head); // FIXED: No "&"
            } else {
                printf("You do not have permission to manage users.\n");
            }
            break;
        case 0:
            printf("Exiting the system.\n");
            break;
        default:
            printf("Invalid choice! Please enter a valid option.\n");
    }
}
