#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

void listFilesAndFolders(const char *path) {
    struct dirent *entry;
    DIR *dir = opendir(path);

    if (dir == NULL) {
        perror("opendir");
        exit(1);
    }

    while ((entry = readdir(dir)) != NULL) {
        char fullpath[1024]; // Adjust the size as needed
        struct stat info;

        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

        if (lstat(fullpath, &info) == -1) {
            perror("lstat");
            continue;
        }

        if (S_ISDIR(info.st_mode)) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                printf("Folder: %s\n", entry->d_name);
            }
        } else if (S_ISREG(info.st_mode)) {
            printf("File: %s\n", entry->d_name);
        }
    }

    closedir(dir);
}

void createFile(const char *path, const char *filename) {
    char fullpath[1024]; // Adjust the size as needed
    snprintf(fullpath, sizeof(fullpath), "%s/%s", path, filename);
    FILE *file = fopen(fullpath, "w");
    if (file != NULL) {
        fclose(file);
        printf("File created: %s\n", fullpath);
    } else {
        perror("fopen");
    }
}

void createFolder(const char *path, const char *foldername) {
    char fullpath[1024]; // Adjust the size as needed
    snprintf(fullpath, sizeof(fullpath), "%s/%s", path, foldername);
    if (mkdir(fullpath, 0755) == 0) {
        printf("Folder created: %s\n", fullpath);
    } else {
        perror("mkdir");
    }
}

int main() {
    char currentDirectory[1024];
    getcwd(currentDirectory, sizeof(currentDirectory));

    printf("Welcome to the File and Folder Manager!\n");
    printf("Current Directory: %s\n\n", currentDirectory);

    while (1) {
        printf("Menu:\n");
        printf("1. List files and folders\n");
        printf("2. Create a new file\n");
        printf("3. Create a new folder\n");
        printf("4. Change directory (cd)\n");
        printf("5. Go up one level (cd ..)\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");

        int choice;
        scanf("%d", &choice);
        while (getchar() != '\n'); // Clear the input buffer

        switch (choice) {
            case 1:
                listFilesAndFolders(currentDirectory);
                break;
            case 2:
                printf("Enter the name of the file to create: ");
                char newFile[256];
                fgets(newFile, sizeof(newFile), stdin);
                newFile[strcspn(newFile, "\n")] = '\0';
                createFile(currentDirectory, newFile);
                break;
            case 3:
                printf("Enter the name of the folder to create: ");
                char newFolder[256];
                fgets(newFolder, sizeof(newFolder), stdin);
                newFolder[strcspn(newFolder, "\n")] = '\0';
                createFolder(currentDirectory, newFolder);
                break;
            case 4:
                printf("Enter the name of the folder to enter: ");
                char folderToEnter[256];
                fgets(folderToEnter, sizeof(folderToEnter), stdin);
                folderToEnter[strcspn(folderToEnter, "\n")] = '\0';
                if (chdir(folderToEnter) == 0) {
                    getcwd(currentDirectory, sizeof(currentDirectory));
                } else {
                    perror("chdir");
                }
                break;
            case 5:
                if (chdir("..") == 0) {
                    getcwd(currentDirectory, sizeof(currentDirectory));
                } else {
                    perror("chdir");
                }
                break;
            case 6:
                printf("Exiting the program.\n");
                exit(0);
            default:
                printf("Invalid choice. Please choose a valid option.\n");
        }

        printf("\n");
    }

    return 0;
}
