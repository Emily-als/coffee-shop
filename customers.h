//Simon Sherwood 318597036
//Emily Alshtein 207796533
//Ifat Vider 206865941

#pragma once
#include <stdio.h>

// Customer Struct
typedef struct purchase {
    int total_payment;
    char purchase_date[20];
    struct purchase* next;
} Purchase;

typedef struct customer {
    int id;                // Unique customer ID
    char name[20];         // Customer name (editable)
    Purchase* purchase;    // Pointer to the first purchase in the list
} Customer;

// Customer Node for the BST
typedef struct customerNode {
    Customer data;              // Customer data
    struct customerNode* left;  // Pointer to left child
    struct customerNode* right; // Pointer to right child
} CustomerNode;

void add_purchase_to_customer(CustomerNode* customer);
CustomerNode* insert_customer(CustomerNode* root, CustomerNode* new_customer);
CustomerNode* load_customers_from_file(const char* filename);
CustomerNode* create_customer(int id, const char* name);
CustomerNode* find_customer(CustomerNode* root, int id);
CustomerNode* find_minimum(CustomerNode* root);
CustomerNode* delete_customer(CustomerNode* root, int id_customer_to_remove);
void update_customer(CustomerNode* root);
void add_new_customer(CustomerNode** root);
void find_and_print_customer(CustomerNode* root, int id);
void save_customers_to_file_end(CustomerNode* root, const char* file_name);
void free_customers(CustomerNode* root);
void free_purchases(Purchase* purchase);
