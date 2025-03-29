//Simon Sherwood 318597036
//Emily Alshtein 207796533
//Ifat Vider 206865941

#define _CRT_SECURE_NO_WARNINGS
#include "customers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logger.h"

#define LOG_FILE "system_log.log"
// Function to create a new customer node (BST node)
CustomerNode* create_customer(int id, const char* name) {
    CustomerNode* new_customer = (CustomerNode*)malloc(sizeof(CustomerNode));
    if (!new_customer) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    new_customer->data.id = id;
    strncpy(new_customer->data.name, name, sizeof(new_customer->data.name) - 1);
    new_customer->data.name[sizeof(new_customer->data.name) - 1] = '\0';  // Null terminate
    new_customer->data.purchase = NULL;  // No purchases initially
    new_customer->left = NULL;
    new_customer->right = NULL;

    return new_customer;
}

// Function to add a purchase to a customer (searches for the customer in the BST)
void add_purchase_to_customer(CustomerNode* root) {

    int customer_id, payment;
    char date[20];

    printf("Enter customer ID: ");
    if (scanf("%d", &customer_id) != 1) { // Validate input
        printf("Invalid input. Please enter a valid number.\n");
        while (getchar() != '\n'); // Clear input buffer
        return;
    }

    printf("Enter payment amount: ");
    if (scanf("%d", &payment) != 1) { // Validate input
        printf("Invalid input. Please enter a valid number.\n");
        while (getchar() != '\n');
        return;
    }

    printf("Enter purchase date (YYYY-MM-DD): ");
    while (getchar() != '\n'); // Clear input buffer before fgets()
    fgets(date, sizeof(date), stdin);
    date[strcspn(date, "\n")] = '\0'; // Remove trailing newline

    // Search for the customer in the BST
    CustomerNode* customer = find_customer(root, customer_id);
    if (customer == NULL) {
        printf("Error: Customer with ID %d not found.\n", customer_id);
        return;
    }

    // Check if there are already 3 purchases on the same date
    Purchase* temp = customer->data.purchase;
    int count = 0;

    // Traverse through the list of purchases and count purchases on the same date
    while (temp != NULL) {
        if (strcmp(temp->purchase_date, date) == 0) {
            count++;
        }
        temp = temp->next;
    }

    if (count >= 3) {
        printf("Error: Cannot add more than 3 purchases on the same date for customer ID %d.\n", customer_id);
        return;
    }

    // Allocate memory for a new purchase
    Purchase* new_purchase = (Purchase*)malloc(sizeof(Purchase));
    if (!new_purchase) {
        printf("Error: Memory allocation failed for purchase.\n");
        return;
    }

    // Assign values to the new purchase
    new_purchase->total_payment = payment;

    // **Fix:** Ensure the string is properly null-terminated before copying
    memset(new_purchase->purchase_date, 0, sizeof(new_purchase->purchase_date));
    strncpy(new_purchase->purchase_date, date, sizeof(new_purchase->purchase_date) - 1);

    new_purchase->next = NULL; // This will be added to the end or start, depending on the case

    // Add the purchase to the customer's purchase list
    if (customer->data.purchase == NULL) {
        // If this is the first purchase for the customer
        customer->data.purchase = new_purchase;
    }
    else {
        // Otherwise, append the purchase to the end of the list
        temp = customer->data.purchase;
        while (temp->next != NULL) {
            temp = temp->next; // Traverse to the last purchase
        }
        temp->next = new_purchase; // Append the new purchase
    }

    printf("Purchase successfully added for customer ID %d.\n", customer_id);
    log_message(LOG_FILE,"Purchase successfully added for customer ID");
}


// Function to add a purchase to an existing customer (no scanf inside)
void add_purchase_to_customer_to_tree(CustomerNode* customer, int payment, const char* date) {
    if (customer == NULL) {
        printf("Error: Customer not found.\n");
        return;
    }

    // Allocate memory for a new purchase node
    Purchase* new_purchase = (Purchase*)malloc(sizeof(Purchase));
    if (!new_purchase) {
        printf("Error: Memory allocation failed for purchase.\n");
        return;
    }

    // Assign values safely to the new purchase
    new_purchase->total_payment = payment;
    strncpy(new_purchase->purchase_date, date, sizeof(new_purchase->purchase_date) - 1);
    new_purchase->purchase_date[sizeof(new_purchase->purchase_date) - 1] = '\0';  // Ensure null termination
    new_purchase->next = NULL;  // This will be added to the end or start, depending on the case

    // Add purchase to the customer's list
    if (customer->data.purchase == NULL) {
        // If it's the first purchase for the customer
        customer->data.purchase = new_purchase;
    } else {
        // Otherwise, append the purchase to the end of the list
        Purchase* temp = customer->data.purchase;
        while (temp->next != NULL) {
            temp = temp->next;  // Traverse to the last purchase
        }
        temp->next = new_purchase;  // Append the new purchase
    }

}


CustomerNode* find_customer(CustomerNode* root, int id) {
    if (root == NULL || root->data.id == id) return root;
    if (id < root->data.id) return find_customer(root->left, id);
    return find_customer(root->right, id);
}


// Function to insert a customer into the binary search tree
CustomerNode* insert_customer(CustomerNode* root, CustomerNode* new_customer) {
    if (root == NULL) {
        return new_customer;
    }

    if (new_customer->data.id < root->data.id) {
        root->left = insert_customer(root->left, new_customer);
    }
    else if (new_customer->data.id > root->data.id) {
        root->right = insert_customer(root->right, new_customer);
    }

    return root;
}
// Function to load customers from a file into a BST
CustomerNode* load_customers_from_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    CustomerNode* root = NULL;

    if (file == NULL) {
        printf("File not found. Creating an empty customer tree.\n");
        log_message(LOG_FILE,"File not found. Creating an empty customer tree");
        return root;
    }

    int id, payment;
    char name[20], date[20];

    // Read file line by line
    while (fscanf(file, "%d %19s %d %19s", &id, name, &payment, date) == 4) {
        // Search for existing customer
        CustomerNode* existing = find_customer(root, id);

        if (existing != NULL) {
            // Add purchase to the existing customer
            add_purchase_to_customer_to_tree(existing, payment, date);  // Corrected this line
        } else {
            // Create a new customer
            CustomerNode* new_customer = create_customer(id, name);
            if (new_customer) {
                // Add the first purchase for this new customer
                add_purchase_to_customer_to_tree(new_customer, payment, date);  // Corrected this line
                // Insert the new customer into the BST
                root = insert_customer(root, new_customer);
            }
        }
    }

    fclose(file);
    return root;
}


// Function to find the minimum node in the BST
CustomerNode* find_minimum(CustomerNode* root) {
    while (root && root->left != NULL) {
        root = root->left;
    }
    return root;
}

// Function to delete a customer from the BST
CustomerNode* delete_customer(CustomerNode* root,int id_customer_to_remove) {

    if (root == NULL) {
        printf("Customer with ID %d not found.\n", id_customer_to_remove);
        return root;
    }

    // Find the node to be deleted
    if (id_customer_to_remove < root->data.id) {
        // Customer to be deleted is in the left subtree
        root->left = delete_customer(root->left,id_customer_to_remove);
    } else if (id_customer_to_remove > root->data.id) {
        // Customer to be deleted is in the right subtree
        root->right = delete_customer(root->right,id_customer_to_remove);
    } else {
        // This is the node to be deleted
        // Case 1: Node has no children (leaf node)
        if (root->left == NULL && root->right == NULL) {
            free(root);
            printf("Customer with ID %d has been deleted.\n", id_customer_to_remove);
            log_message(LOG_FILE,"Customer has been deleted.");
            return NULL;
        }
        // Case 2: Node has one child
        else if (root->left == NULL) {
            CustomerNode* temp = root->right;
            free(root);
            printf("Customer with ID %d has been deleted.\n", id_customer_to_remove);
            log_message(LOG_FILE,"Customer has been deleted.");
            return temp;
        } else if (root->right == NULL) {
            CustomerNode* temp = root->left;
            free(root);
            printf("Customer with ID %d has been deleted.\n", id_customer_to_remove);
            log_message(LOG_FILE,"Customer has been deleted.");
            return temp;
        }
        // Case 3: Node has two children
        else {
            // Find the in-order successor (smallest in the right subtree)
            CustomerNode* temp = find_minimum(root->right);

            // Replace the data of the current node with the in-order successor
            root->data = temp->data;

            // Delete the in-order successor
            root->right = delete_customer(root->right,id_customer_to_remove);
        }
    }

    return root;
}


void update_customer(CustomerNode* root) {
    if (root == NULL) {
        printf("Error: Customer tree is empty.\n");
        log_message(LOG_FILE,"Error: Customer tree is empty.");

        return;
    }

    int customer_id;
    printf("Enter the ID of the customer to update: ");
    if (scanf("%d", &customer_id) != 1) {
        printf("Invalid input. Please enter a valid number.\n");
        while (getchar() != '\n');
        return;
    }

    // Search for the customer by ID
    CustomerNode* customer_node = find_customer(root, customer_id);
    if (customer_node == NULL) {
        printf("Error: Customer with ID %d not found.\n", customer_id);
        return;
    }

    // Customer found, now ask the user what to update
    int choice;
    printf("Customer found: %s\n", customer_node->data.name);
    printf("What would you like to update?\n");
    printf("1. Name\n");
    printf("2. Add Purchase\n");
    printf("3. Cancel\n");
    printf("Enter your choice: ");

    if (scanf("%d", &choice) != 1) {
        printf("Invalid input. Please enter a valid number.\n");
        while (getchar() != '\n');
        return;
    }

    while (getchar() != '\n'); // Clear input buffer

    // Update based on user choice
    switch (choice) {
    case 1: {
        // Update the name of the customer
        char new_name[20];

        printf("Enter the new name of the customer: \n");
        fgets(new_name, sizeof(new_name), stdin);

        // Remove trailing newline (`\n`) if present
        size_t len = strlen(new_name);
        if (len > 0 && new_name[len - 1] == '\n') {
            new_name[len - 1] = '\0';
        }

        // Ensure safe copying with null termination
        strncpy(customer_node->data.name, new_name, sizeof(customer_node->data.name) - 1);
        customer_node->data.name[sizeof(customer_node->data.name) - 1] = '\0'; // Null-terminate manually

        printf("Name updated successfully.\n");
        log_message(LOG_FILE,"Customer name updated successfully.");
        break;
    }
    case 2: {
        add_purchase_to_customer(root);
        break;
    }
    case 3: {
        printf("Update canceled.\n");
        break;
    }
    default:
        printf("Invalid choice.\n");
        break;
    }
}

// Function to add a new customer 
void add_new_customer(CustomerNode** root) {
    int id;
    char name[20];

    printf("Enter new customer ID: ");
    scanf("%d", &id);

    if (find_customer(*root, id) != NULL) {
        printf("Error: Customer with ID %d already exists.\n", id);
        return;
    }

    printf("Enter customer name: ");
    scanf(" %19[^\n]", name);

    CustomerNode* new_customer = create_customer(id, name);
    if (!new_customer) {
        printf("Error: Memory allocation failed for new customer.\n");
        return;
    }

    *root = insert_customer(*root, new_customer); // Modify root in caller

    printf("Customer added successfully!\n");
    log_message(LOG_FILE,"Customer added successfully.");
}

void find_and_print_customer(CustomerNode* root, int id) {
    // Search for the customer
    CustomerNode* customer = find_customer(root, id);

    // Check if customer exists
    if (customer == NULL) {
        printf("Customer with ID %d not found.\n", id);
        return;
    }

    // Print customer details
    printf("Customer ID: %d\n", customer->data.id);
    printf("Customer Name: %s\n", customer->data.name);
    printf("Purchase History:\n");

    // Print purchase history
    Purchase* purchase = customer->data.purchase;
    if (purchase == NULL) {
        printf("  No purchases found.\n");
    } else {
        while (purchase != NULL) {
            printf("  - Payment: %d, Date: %s\n", purchase->total_payment, purchase->purchase_date);
            purchase = purchase->next;
        }
    }
}


// Recursive function to write customer data to file
void save_customers_to_file(CustomerNode* root, FILE* file) {
    if (root == NULL) {
        return; // Base case: if node is null, return
    }

    // Traverse the left subtree
    save_customers_to_file(root->left, file);

    // Write the customer ID and name once
    if (root->data.purchase == NULL) {
        // If no purchases, write only the customer info
        fprintf(file, "%d %s\n", root->data.id, root->data.name);
    } else {
        // If the customer has purchases, write each purchase on a separate row
        Purchase* purchase = root->data.purchase;
        while (purchase != NULL) {
            fprintf(file, "%d %s %d %s\n", root->data.id, root->data.name, purchase->total_payment, purchase->purchase_date);
            purchase = purchase->next; // Move to the next purchase
        }
    }

    // Traverse the right subtree
    save_customers_to_file(root->right, file);
}
// Function to save the customers and their purchases to a file (overwrites the file)
void save_customers_to_file_end(CustomerNode* root, const char* file_name) {
    // Open the file in write mode ('w') to overwrite the content
    FILE* file = fopen(file_name, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        log_message(LOG_FILE,"Error opening file for writing.");
        return;
    }

    // Call the recursive function to save customers and their purchases
    save_customers_to_file(root, file);

    // Close the file after writing
    fclose(file);
    printf("Customers and purchases saved successfully to %s\n", file_name);
    log_message(LOG_FILE,"Customers and purchases saved successfully to file");
}


void free_purchases(Purchase* purchase) {
    Purchase* temp;
    while (purchase != NULL) {
        temp = purchase;
        purchase = purchase->next;
        free(temp);
    }
}

void free_customers(CustomerNode* root) {
    if (root == NULL)
        return;

    // Recursively free left and right subtrees
    free_customers(root->left);
    free_customers(root->right);

    // Free purchases linked list
    free_purchases(root->data.purchase);

    // Free customer node
    free(root);
}