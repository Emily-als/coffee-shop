# Coffee Shop Management System

A command-line business management system developed for a cafe to help employees manage day-to-day operations efficiently.

## Description

This system allows cafe staff to manage:

- Customers  
- Products (Items)  
- Employees  

It features a menu-driven CLI interface for easy navigation of daily business operations, and uses file I/O for persistent data storage.

## Features

- Add, view, update, and delete customer records
- Manage product inventory (binary file format)
- Track employee details
- Persistent data via text and binary files
- Modular code structure using .c and .h files
- Logger for tracking actions
- Dynamic memory management and structured data

## Technologies Used

- C Programming Language
- File I/O (text and binary)
- Structs and data structures
- CLI navigation
- Dynamic memory allocation (malloc/free)


## Getting Started

### 1. Compile the project

```bash
gcc -o final_project_coffee_shop.exe main.c customers.c employees.c items.c logger.c menu.c

To run the program:

./final_project_coffee_shop.exe