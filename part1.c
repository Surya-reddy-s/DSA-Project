
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define a structure for files and folders
typedef struct Node {
    char name[100];
    int isFile;
    struct Node* children;
    struct Node* sibling;
} Node;

// Function to create a new file or folder
Node* createNode(const char* name, int isFile) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode != NULL) {
        strncpy(newNode->name, name, sizeof(newNode->name));
        newNode->isFile = isFile;
        newNode->children = NULL;
        newNode->sibling = NULL;
    }
    return newNode;
}

// Function to add a child (file or folder) to a parent node
void addChild(Node* parent, Node* child) {
    if (parent == NULL || child == NULL) {
        return;
    }
    child->sibling = parent->children;
    parent->children = child;
}

// Function to print the file-folder structure using a recursive approach
void printStructure(Node* root, int level) {
    if (root == NULL) {
        return;
    }

    for (int i = 0; i < level; i++) {
        printf("  ");
    }

    if (root->isFile) {
        printf("- %s (File)\n", root->name);
    } else {
        printf("- %s (Folder)\n", root->name);
        Node* child = root->children;
        while (child != NULL) {
            printStructure(child, level + 1);
            child = child->sibling;
        }
    }
}



// Function to change the current folder (similar to cd and cd.. in Linux)
Node* changeDirectory(Node* currentFolder, const char* folderName, Node* root) {
    if (currentFolder == NULL) {
        printf("Error: Current folder is not set.\n");
        return currentFolder;
    }

    if (strcmp(folderName, "..") == 0) {
        // Move up to the parent folder
        if (currentFolder != root) {
            Node* parent = root;
            Node* child = root->children;
            while (child != NULL) {
                if (child->children == currentFolder) {
                    return parent;
                }
                parent = child->children;
                child = child->sibling;
            }
            return currentFolder;  // Already at the root folder
        } else {
            printf("Error: Already at the root folder.\n");
            return currentFolder;
        }
    } else if (strcmp(folderName, ".") == 0) {
        // Stay in the current folder (no change)
        return currentFolder;
    } else {
        // Move to a child folder with the specified name
        Node* child = currentFolder->children;
        while (child != NULL) {
            if (strcmp(child->name, folderName) == 0 && !child->isFile) {
                return child;
            }
            child = child->sibling;
        }
        printf("Error: Folder '%s' not found.\n", folderName);
        return currentFolder;
    }
}

void freeStructure(Node* root) {
    if (root == NULL) {
        return;
    }

    Node* child = root->children;
    while (child != NULL) {
        Node* next = child->sibling;
        freeStructure(child);
        child = next;
    }

    free(root);
}

int main() {
 
  Node* root = createNode("Root", 0);

  Node* currentFolder = root;

  printf("Welcome to File Folder Structure made to manage files and folders using Trees as a data structure in C\n");
  printf("We are on the Root Directory now\n\n");
  int choice;

  do {
    printf("1. Add a file to the current folder\n");
    printf("2. Add a sub folder inside the current folder\n");
    printf("3. Change the Directory (Equivalent to 'cd' or 'cd ..' in Unix)\n");
    printf("4. Print the current File Structure from root\n");
    printf("5. Exit (Free Structure)\n\n"); 
    printf("Enter your choice:");
    scanf("%d",&choice); printf("\n");
    switch (choice) {
      case 1:
        printf("Enter the Name of the File (Less than 100 characters)\n");
        char name[100];
        scanf("%s",name);
        Node* file = createNode(name, 1);
        addChild(currentFolder, file);
        printf("The file has been added to the current folder\n\n");
        break;

      case 2:
        printf("Enter the name of the folder (Less than 100 characters)\n");
        char names[100];
        scanf("%s",names);
        Node* folder = createNode(names, 0);
        addChild(currentFolder, folder);
        printf("The folder has been added\n\n"); break;
      
      case 3:
        printf("Enter the folder name or '..' to move up\n"); 
        char namef[100];
        scanf("%s",namef);
        currentFolder = changeDirectory(currentFolder, namef, root);
        printf("In folder: %s", currentFolder->name);
        printf("\n\n"); break;

      case 4:
        printf("The Current file-folder structure is:\n\n");
        printStructure(root, 0); 
        printf("\n\n");
        break;
      
      case 5:
        freeStructure(root);
        exit(0);
      printf("\n\n");
    }
  }while (choice < 6); 

  return 0;
}
