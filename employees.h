//Simon Sherwood 318597036
//Emily Alshtein 207796533
//Ifat Vider 206865941

#pragma once
#ifndef EMPLOYEES_H
#define EMPLOYEES_H
#endif
typedef struct employee {
    char username[20];
    char firstname[10];
    char password[10];
    unsigned int level;
    struct employee* next;
    // 1 - admin, 2 - employee, 3 - practicioner
} Employee;

int check_if_employee_file_exists();
Employee* createDefaultAdmin();
Employee* login(Employee* head);
Employee* loadEmployeesFromFile(const char* filename);
void deleteEmployeeFromList(Employee** head);
void add_employee_to_list(Employee** head);
void save_employees_to_file_end(Employee* head, const char* filename);
void free_employees(Employee* head);