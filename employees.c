//Simon Sherwood 318597036
//Emily Alshtein 207796533
//Ifat Vider 206865941

#define _CRT_SECURE_NO_WARNINGS
#include "employees.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "logger.h"

#define LOG_FILE "system_log.log"
int check_if_employee_file_exists() {
    FILE* file = fopen("our_employees.txt", "r");
    if (file == NULL) {
        file = fopen("our_employees.txt", "w"); // Create the file if it doesn't exist
        if (file == NULL) {
            printf("Error creating employees file.\n");
            log_message(LOG_FILE,"Error creating employees file");
            return 0;
        }
        fclose(file);
        return 0;
    }
    fclose(file);
    return 1;
}
    // Function to create the default admin and return the address of the admin node
    Employee* createDefaultAdmin() {

        // Create the default admin employee
        Employee* defaultAdmin = (Employee*)malloc(sizeof(Employee));
        if (defaultAdmin == NULL) {
            perror("Failed to allocate memory");
            return NULL;
        }

        // Initialize the default admin
        strcpy(defaultAdmin->username, "admin");
        strcpy(defaultAdmin->firstname, "manager");
        strcpy(defaultAdmin->password, "1234");
        defaultAdmin->level = 1; // Admin level
        defaultAdmin->next = NULL; // No next node yet

        printf("Default admin created successfully.\n");
        log_message(LOG_FILE,"Default admin created successfully");

        // Return the address of the admin node
        return defaultAdmin;
    }

Employee* login(Employee* head) 
{
    char input_username[50];
    char input_password[50];
    int attempts = 0;

    printf("Welcome to coffee shop managing system. \n");
    while (attempts < 3) {
        printf("Enter username: \n");
        scanf("%19s", input_username);
        printf("Enter password: \n");
        scanf("%19s", input_password);

        Employee* current = head;
        // Traverse the linked list to check if the credentials match
        while (current != NULL) {
            if (strcmp(current->username, input_username) == 0 &&
                strcmp(current->password, input_password) == 0) {
                printf("Login successful!\n");
                return current; // Return the pointer to the logged-in employee
            }
            current = current->next;
        }

        // If credentials are incorrect
        printf("Incorrect username or password. Please try again.\n");
        log_message(LOG_FILE,"Incorrect username or password. Please try again");
        attempts++;
    }

    // If the user reaches the max attempts
    printf("Incorrect login attempts exceeded. Exiting system.\n");
    log_message(LOG_FILE,"Incorrect login attempts exceeded. Exiting system.");
    return NULL; // Return NULL to indicate failed login
}

// Function to create a new employee node
Employee* createEmployee(const char* username, const char* firstname, const char* password, unsigned int level) {
    Employee* newEmployee = (Employee*)malloc(sizeof(Employee));
    if (!newEmployee) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    strcpy(newEmployee->username, username);
    strcpy(newEmployee->firstname, firstname);
    strcpy(newEmployee->password, password);
    newEmployee->level = level;
    newEmployee->next = NULL;
    return newEmployee;
}

// Function to load employees from a file into a linked list
Employee* loadEmployeesFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file: %s\n", filename);
        return NULL;
    }

    Employee* head = NULL;
    Employee* tail = NULL;
    char username[20], firstname[10], password[10];
    unsigned int level;

    while (fscanf(file, "%19s %9s %9s %u", username, firstname, password, &level) == 4) {
        Employee* newEmployee = createEmployee(username, firstname, password, level);
        if (!newEmployee) {
            fclose(file);
            return head;
        }

        if (!head)
        {
            head = newEmployee; // First node
            tail = newEmployee; //last node
        }
        else
        {
            tail->next = newEmployee;
            tail = newEmployee; // Update tail
        }
       
    }

    fclose(file);
    return head;
}


void deleteEmployeeFromList(Employee** head) {
    if (*head == NULL) {
        printf("The employee list is empty.\n");
        log_message(LOG_FILE,"The employee list is empty");
        return;
    }

    char username[20];
    printf("\nEnter the username of the employee to delete: ");
    scanf("%19s", username); // Input protection (alternative: fgets)

    Employee* temp = *head;
    Employee* prev = NULL;

    // Explicit check in case of NULL pointer access
    if (temp == NULL) {
        printf("Employee list is empty.\n");
        log_message(LOG_FILE,"Employee list is empty");
        return;
    }

    // Check if the first node itself is to be deleted
    if (strcmp(temp->username, username) == 0) {
        *head = temp->next; // Update head
        free(temp); // Free memory
        printf("Employee '%s' deleted successfully.\n", username);
        log_message(LOG_FILE,"Employee ' deleted successfully.");
        return;
    }

    // Traverse the list to find the employee
    while (temp != NULL && strcmp(temp->username, username) != 0) {
        prev = temp;
        temp = temp->next;
    }

    // If the employee was not found
    if (temp == NULL) {
        printf("Employee with username '%s' not found.\n", username);
        log_message(LOG_FILE,"Employee not found.");
        return;
    }

    // Remove the employee from the list
    if (prev != NULL) { // Ensure `prev` is valid before accessing `prev->next`
        prev->next = temp->next;
    }

    free(temp);
    printf("Employee '%s' deleted successfully.\n", username);
    log_message(LOG_FILE,"Employee deleted.");

}

// Function to add a new employee to the linked list
void add_employee_to_list(Employee** head) {
    Employee* new_employee = (Employee*)malloc(sizeof(Employee));
    if (!new_employee) {
        printf("Memory allocation failed!\n");
        return;
    }

    printf("Enter username: ");
    scanf("%19s", new_employee->username);

    printf("Enter first name: ");
    scanf("%9s", new_employee->firstname);

    printf("Enter password: ");
    scanf("%9s", new_employee->password);

    printf("Enter access level (1 - admin, 2 - employee, 3 - practitioner): ");
    scanf("%u", &new_employee->level);

    new_employee->next = NULL;

    // If the list is empty, set the new employee as the head
    if (*head == NULL) {
        *head = new_employee;
    } else {
        // Traverse to the end and insert the new employee
        Employee* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_employee;
    }

    printf("Employee added successfully!\n");
    log_message(LOG_FILE,"Employee added successfully");

}


// Function to save employees linked list to a file (overwrite the file)
void save_employees_to_file_end(Employee* head, const char* filename) {
    // Open the file in write mode ('w') to overwrite the content
    FILE* file = fopen(filename, "w");  // 'w' for write mode (overwrites the file)
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    Employee* current = head;

    // Traverse the linked list and save each employee's data to the file
    while (current != NULL) {
        // Write each employee's data to the file
        fprintf(file, "%s %s %s %u\n", current->username, current->firstname, current->password, current->level);
        current = current->next;
    }

    fclose(file);  // Close the file after writing
    printf("Employees saved successfully to %s\n", filename);
    log_message(LOG_FILE,"Employees saved successfully to file");
}
void free_employees(Employee* head) {
    Employee* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}