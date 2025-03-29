//Simon Sherwood 318597036
//Emily Alshtein 207796533
//Ifat Vider 206865941

#pragma once
#include <stdbool.h>
//item struct
typedef struct item {
    int id;
    char name[20]; // Editable
    char category[20]; // Editable
    float price;
    bool inStock;
    char exp_date[11];
    int inventory;
} Item;

// Define BST Node for Items
typedef struct itemNode {
    Item data;
    struct itemNode* left;
    struct itemNode* right;
} ItemNode;

ItemNode* createItemNode(Item item);
ItemNode* insertItem(ItemNode* root, Item item);
ItemNode* loadItemsFromFile(const char* filename);
ItemNode* loadOrCreateItemTree(const char* filename);
void inOrderTraversal(ItemNode* root);
ItemNode* find_item(ItemNode* root, int id);
void update_item(ItemNode* root);
void add_new_item(ItemNode** root);
void findAndPrintItem(ItemNode* root, int id);
ItemNode* remove_item_from_tree_internal(ItemNode* root, int id, int* deleted);
ItemNode* remove_item_from_tree(ItemNode* root, int id);
void save_items_to_file_end(ItemNode* root, const char* filename);
void search_by_max_price(ItemNode* root, float max_price);
void search_by_exact_price(ItemNode* root, float exact_price);
int is_after_date(const char* item_date, const char* input_date);
void search_by_expiry_date(ItemNode* root, const char* input_date);
void search_by_name_and_category(ItemNode* root, const char* name, const char* category);
void free_items(ItemNode* root);
