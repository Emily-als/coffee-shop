//Simon Sherwood 318597036
//Emily Alshtein 207796533
//Ifat Vider 206865941

#pragma once
#include "customers.h"
#include "employees.h"
#include "items.h"

void showMenu(Employee employee);
void performAction(int choice, Employee emp, Employee** employees_head, CustomerNode** customers_root, ItemNode** items_root);