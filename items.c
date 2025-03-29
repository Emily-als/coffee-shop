//Simon Sherwood 318597036
//Emily Alshtein 207796533
//Ifat Vider 206865941

#define _CRT_SECURE_NO_WARNINGS
#include "items.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "logger.h"

#define LOG_FILE "system_log.log"

// Function to create a new Item node
ItemNode* createItemNode(Item item) {
    ItemNode* newNode = (ItemNode*)malloc(sizeof(ItemNode));
    if (!newNode) {
        printf("Memory allocation failed.\n");
        return NULL;
    }
    newNode->data = item;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Function to insert an item into the BST
ItemNode* insertItem(ItemNode* root, Item item) {
    if (root == NULL) {
        return createItemNode(item);
    }
    if (item.id < root->data.id) {
        root->left = insertItem(root->left, item);
    }
    else if (item.id > root->data.id) {
        root->right = insertItem(root->right, item);
    }
    return root;
}

// Function to load items from file into BST
ItemNode* loadItemsFromFile( const char* filename) {
    ItemNode* root = NULL;

  FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Error opening file.\n");
        return root;
    }

    Item temp;
    while (fread(&temp, sizeof(Item), 1, file)) {
        root = insertItem(root, temp);
    }

    fclose(file);
    //printf("Items loaded from file into BST.\n");
    return root;
}


// Function to display BST in sorted order
void inOrderTraversal(ItemNode* root) {
    if (root == NULL) return;

    inOrderTraversal(root->left);
    printf("ID: %d | Name: %s | Category: %s | Price: %.2f | InStock: %s | Expiration: %s | Inventory: %d\n",
        root->data.id, root->data.name, root->data.category, root->data.price,
        root->data.inStock ? "Yes" : "No", root->data.exp_date, root->data.inventory);
    inOrderTraversal(root->right);
}


// Helper function to find the minimum item node in the BST
ItemNode* find_min(ItemNode* root) {
    while (root && root->left != NULL) {
        root = root->left;
    }
    return root;
}

ItemNode* remove_item_from_tree_internal(ItemNode* root, int id, int* deleted) {
    if (root == NULL)
        return NULL;  // Item not found in this branch

    if (id < root->data.id) {
        root->left = remove_item_from_tree_internal(root->left, id, deleted);
        return root;
    } else if (id > root->data.id) {
        root->right = remove_item_from_tree_internal(root->right, id, deleted);
        return root;
    } else {  // Found the node to delete
        *deleted = 1;  // Mark that we did delete something
        // Case 1: Node with no child or one child
        if (root->left == NULL) {
            ItemNode* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            ItemNode* temp = root->left;
            free(root);
            return temp;
        }
        // Case 2: Node with two children: Get the inorder successor (smallest in the right subtree)
        ItemNode* temp = find_min(root->right);
        // Shallow copy: directly copy the content of temp's data to root->data
        root->data = temp->data;
        // Delete the inorder successor
        root->right = remove_item_from_tree_internal(root->right, temp->data.id, deleted);
        return root;
    }
}

ItemNode* remove_item_from_tree(ItemNode* root, int id) {
    int deleted = 0;  // Flag to track whether a deletion occurred
    root = remove_item_from_tree_internal(root, id, &deleted);

    if (deleted) {
        printf("Item with id %d removed successfully.\n", id);
        log_message(LOG_FILE,"Item removed successfully");
    }
    else {
        printf("Item with id %d not found.\n", id);
        log_message(LOG_FILE,"Item not found");
    }

    return root;
}


ItemNode* find_item(ItemNode* root, int id) {
    if (root == NULL) {
        return NULL;
    }

    if (id == root->data.id) {
        return root;
    }

    if (id < root->data.id) {
        return find_item(root->left, id);
    } else {
        return find_item(root->right, id);
    }
}

// Function to update an item based on user input
void update_item(ItemNode* root) {
    if (root == NULL) {
        printf("Error: Tree is empty.\n");
        log_message(LOG_FILE,"Error: Tree is empty.");

        return;
    }

    int item_id;
    printf("Enter the ID of the item to update: ");
    if (scanf("%d", &item_id) != 1) { // Validate input
        printf("Invalid input. Please enter a valid number.\n");
        while (getchar() != '\n');
        return;
    }

    // Search for the item by ID
    ItemNode* item_node = find_item(root, item_id);
    if (item_node == NULL) {
        printf("Error: Item with ID %d not found.\n", item_id);
        return;
    }

    // Item found, now ask the user what to update
    int choice;
    printf("Item found: %s\n", item_node->data.name);
    printf("What would you like to update?\n");
    printf("1. Name\n2. Category\n3. Price\n4. In Stock\n5. Expiration Date\n6. Inventory\n7. Cancel\n");
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
        char new_name[20];
        printf("Enter new name: ");
        fgets(new_name, sizeof(new_name), stdin);
        new_name[strcspn(new_name, "\n")] = '\0'; // Remove newline if present

        strncpy(item_node->data.name, new_name, sizeof(item_node->data.name) - 1);
        item_node->data.name[sizeof(item_node->data.name) - 1] = '\0';
        printf("Name updated successfully.\n");
        log_message(LOG_FILE,"Item name updated successfully");
        break;
    }
    case 2: {
        char new_category[20];
        printf("Enter new category: ");
        fgets(new_category, sizeof(new_category), stdin);
        new_category[strcspn(new_category, "\n")] = '\0';

        strncpy(item_node->data.category, new_category, sizeof(item_node->data.category) - 1);
        item_node->data.category[sizeof(item_node->data.category) - 1] = '\0';
        printf("Category updated successfully.\n");
        log_message(LOG_FILE,"Item category updated successfully");

        break;
    }
    case 3: {
        float new_price;
        printf("Enter new price: ");
        if (scanf("%f", &new_price) != 1) {
            printf("Invalid input. Please enter a valid number.\n");
            while (getchar() != '\n');
            return;
        }
        item_node->data.price = new_price;
        printf("Price updated successfully.\n");
        log_message(LOG_FILE,"Item price updated successfully");

        break;
    }
    case 4: {
        int temp_in_stock;
        printf("Is the item in stock? (1 for yes, 0 for no): ");
        if (scanf("%d", &temp_in_stock) != 1 || (temp_in_stock != 0 && temp_in_stock != 1)) {
            printf("Invalid input. Enter 1 (yes) or 0 (no).\n");
            while (getchar() != '\n');
            return;
        }
        item_node->data.inStock = (bool)temp_in_stock;
        printf("Stock status updated successfully.\n");
        log_message(LOG_FILE,"Item stock status updated successfully");

        break;
    }
    case 5: {
        char new_exp_date[11];
        printf("Enter new expiration date (YYYY-MM-DD): ");
        fgets(new_exp_date, sizeof(new_exp_date), stdin);
        new_exp_date[strcspn(new_exp_date, "\n")] = '\0';

        strncpy(item_node->data.exp_date, new_exp_date, sizeof(item_node->data.exp_date) - 1);
        item_node->data.exp_date[sizeof(item_node->data.exp_date) - 1] = '\0';
        printf("Expiration date updated successfully.\n");
        log_message(LOG_FILE,"Item expiration date updated successfully");

        break;
    }
    case 6: {
        int new_inventory;
        printf("Enter new inventory: ");
        if (scanf("%d", &new_inventory) != 1) {
            printf("Invalid input. Please enter a valid number.\n");
            while (getchar() != '\n');
            return;
        }
        item_node->data.inventory = new_inventory;
        printf("Inventory updated successfully.\n");
        log_message(LOG_FILE,"Inventory updated successfully");

        break;
    }
    case 7: {
        printf("Update canceled.\n");
        break;
    }
    default:
        printf("Invalid choice.\n");
        break;
    }
}

void add_new_item(ItemNode** root) 
{ 
    Item newItem;

    // Get item details from the user
    printf("Enter item ID: ");
    scanf("%d", &newItem.id);

    // Check if the item already exists
    if (find_item(*root, newItem.id) != NULL) { // Dereference root
        printf("Error: Item with ID %d already exists.\n", newItem.id);
        return;
    }

    printf("Enter item name: ");
    scanf(" %19[^\n]", newItem.name);

    printf("Enter item category: ");
    scanf(" %19[^\n]", newItem.category);

    printf("Enter item price: ");
    if (scanf("%f", &newItem.price) != 1) { // Input validation
        printf("Invalid price. Please enter a valid number.\n");
        while (getchar() != '\n');
        return;
    }

    printf("Enter inventory count: ");
    if (scanf("%d", &newItem.inventory) != 1) { // Input validation
        printf("Invalid input. Please enter a valid number.\n");
        while (getchar() != '\n');
        return;
    }

    printf("Is the item in stock? (1 for yes, 0 for no): ");
    if (scanf("%d", (int*)&newItem.inStock) != 1) { // Input validation
        printf("Invalid input. Please enter 1 or 0.\n");
        while (getchar() != '\n');
        return;
    }

    printf("Enter expiration date (YYYY-MM-DD): ");
    scanf("%10s", newItem.exp_date);

    // Insert the new item into the BST
    *root = insertItem(*root, newItem); // Ensure root is updated
    printf("Item added successfully!\n");
    log_message(LOG_FILE,"Item added successfully");

}


// Function to search for an item by ID and print its details
void findAndPrintItem(ItemNode* root, int id) {
    if (root == NULL) {
        printf("Item with ID %d not found.\n", id);
        return;
    }

    if (root->data.id == id) {
        // Item found, print details
        printf("Item found:\n");
        printf("Item ID: %d\n", root->data.id);
        printf("Name: %s\n", root->data.name);
        printf("Category: %s\n", root->data.category);
        printf("Price: %.2f\n", root->data.price);
        printf("In Stock: %s\n", root->data.inStock ? "Yes" : "No");
        printf("Expiration Date: %s\n", root->data.exp_date);
        printf("Inventory: %d\n", root->data.inventory);
        return;
    }

    // Search in the left or right subtree
    if (id < root->data.id) {
        findAndPrintItem(root->left, id);
    } else {
        findAndPrintItem(root->right, id);
    }
}


// Function to save the items tree to a binary file, with filename as an argument
void save_items_to_file_end(ItemNode* root, const char* filename) {
    if (root == NULL) {
        return;
    }

    // Open the file in binary write mode
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    // In-order traversal to save items to the file
    if (root->left != NULL) {
        save_items_to_file_end(root->left, filename);  // Traverse left subtree
    }

    // Write the current node's data to the file
    fwrite(&root->data, sizeof(Item), 1, file);

    if (root->right != NULL) {
        save_items_to_file_end(root->right, filename);  // Traverse right subtree
    }

    fclose(file);  // Close the file after the write operation is complete
    printf("Items saved successfully to %s\n", filename);
    log_message(LOG_FILE,"Items added successfully to file");
}

// Helper function to traverse BST and print items within price range
void search_by_max_price(ItemNode* root, float max_price) {
    if (root == NULL) {
        return; // Base case: Reached a leaf node
    }

    // First, check items in the left subtree (smaller values)
    search_by_max_price(root->left, max_price);

    // Process the current node (print if price is within range)
    if (root->data.price <= max_price) {
        printf("ID: %d | Name: %s | Category: %s | Price: %.2f | In Stock: %s | Expiry Date: %s | Inventory: %d\n",
               root->data.id, root->data.name, root->data.category, root->data.price,
               root->data.inStock ? "Yes" : "No", root->data.exp_date, root->data.inventory);
    }

    // Then, check items in the right subtree (larger values)
    search_by_max_price(root->right, max_price);
}
void search_by_exact_price(ItemNode* root, float exact_price) {
    if (root == NULL) {
        return; // Base case: Reached a leaf node
    }

    // Traverse left subtree
    search_by_exact_price(root->left, exact_price);

    // Process the current node (print if price matches)
    if (root->data.price == exact_price) {
        printf("ID: %d | Name: %s | Category: %s | Price: %.2f | In Stock: %s | Expiry Date: %s | Inventory: %d\n",
               root->data.id, root->data.name, root->data.category, root->data.price,
               root->data.inStock ? "Yes" : "No", root->data.exp_date, root->data.inventory);
    }

    // Traverse right subtree
    search_by_exact_price(root->right, exact_price);
}

// Function to compare two dates in "YYYY-MM-DD" format
int is_after_date(const char* item_date, const char* input_date) {
    return strcmp(item_date, input_date) > 0; // Returns positive if item_date is later than input_date
}

// Recursive function to search items with expiry date after the given date
void search_by_expiry_date(ItemNode* root, const char* input_date) {
    if (root == NULL) {
        return; // Base case: No more nodes to check
    }

    // Traverse left subtree
    search_by_expiry_date(root->left, input_date);

    // Check current node's expiry date
    if (is_after_date(root->data.exp_date, input_date)) {
        printf("ID: %d | Name: %s | Category: %s | Price: %.2f | In Stock: %s | Expiry Date: %s | Inventory: %d\n",
               root->data.id, root->data.name, root->data.category, root->data.price,
               root->data.inStock ? "Yes" : "No", root->data.exp_date, root->data.inventory);
    }

    // Traverse right subtree
    search_by_expiry_date(root->right, input_date);
}
// Recursive function to search for items by name and category
void search_by_name_and_category(ItemNode* root, const char* name, const char* category) {
    if (root == NULL) {
        printf("Not found\n");
        return; // Base case: no more nodes to check
    }

    // Traverse the left subtree
    search_by_name_and_category(root->left, name, category);

    // Check if current node matches both name and category
    if (strcmp(root->data.name, name) == 0 && strcmp(root->data.category, category) == 0) {
        printf("ID: %d | Name: %s | Category: %s | Price: %.2f | In Stock: %s | Expiry Date: %s | Inventory: %d\n",
               root->data.id, root->data.name, root->data.category, root->data.price,
               root->data.inStock ? "Yes" : "No", root->data.exp_date, root->data.inventory);
    }

    // Traverse the right subtree
    search_by_name_and_category(root->right, name, category);
}

void free_items(ItemNode* root) {
    if (root == NULL)
        return;

    // Recursively free left and right subtrees
    free_items(root->left);
    free_items(root->right);

    // Free item node
    free(root);
}