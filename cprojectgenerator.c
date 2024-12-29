#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

// Function declarations
void show_help();
void create_directory(const char *dir_name);
void create_file(const char *file_path, const char *content);
void create_project(const char *project_name);
void add_library(const char *project_name, const char *library_name);
void add_function(const char *project_name, const char *lib_name, const char *func_name, 
                 const char *return_type, const char *params);
void remove_function(const char *project_name, const char *lib_name, const char *func_name);
void remove_library(const char *project_name, const char *library_name);
void remove_project(const char *project_name);
void build_and_run_project(const char *project_name);
void update_makefile_for_library(const char *project_name, const char *library_name);
void add_include_to_main(const char *project_name, const char *library_name);

// Function to display help message
void show_help() {
   int line_length = 50;  // Base length for border

   // Print top border
   printf("\n\n");

   printf("\n\033[1;36m📦 C Project Generator - Command Line Tool\033[0m\n\n");

   printf("\033[1;33m🔧 Available Commands:\033[0m\n\n");

   // Create Project
   printf("  \033[1;34m📁 Create New Project\033[0m\n");
   printf("     \033[32mcpg [ProjectName]\033[0m\n\n");

   // Help
   printf("  \033[1;34m❓ Show Help\033[0m\n");
   printf("     \033[32mcpg help\033[0m\n\n");

   // Add Function
   printf("  \033[1;34m➕ Add Function\033[0m\n");
   printf("     \033[32mcpg addfunc [ProjectName] [LibName] [FuncName] [ReturnType] [Params]\033[0m\n\n");

   // Remove Function
   printf("  \033[1;34m➖ Remove Function\033[0m\n");
   printf("     \033[32mcpg removefunc [ProjectName] [LibName] [FuncName]\033[0m\n\n");

   // Add Library
   printf("  \033[1;34m📚 Add Library\033[0m\n");
   printf("     \033[32mcpg addlib [ProjectName] [LibraryName]\033[0m\n\n");

   // Remove Library
   printf("  \033[1;34m🗑️  Remove Library\033[0m\n");
   printf("     \033[32mcpg removelib [ProjectName] [LibraryName]\033[0m\n\n");

   // Remove Project
   printf("  \033[1;34m💣 Remove Project\033[0m\n");
   printf("     \033[32mcpg removeproject [ProjectName]\033[0m\n\n");

   // Build and Run
   printf("  \033[1;34m🚀 Build and Run\033[0m\n");
   printf("     \033[32mcpg buildrun [ProjectName]\033[0m\n\n");
  
   printf("\n\n");
}

// Function to create a directory
void create_directory(const char *dir_name) {
    if (mkdir(dir_name, 0777) == 0) {
        printf("Created directory: %s\n", dir_name);
    } else {
        perror("Error creating directory");
    }
}

// Function to create a file with content
void create_file(const char *file_path, const char *content) {
    FILE *file = fopen(file_path, "w");
    if (file) {
        fprintf(file, "%s", content);
        fclose(file);
        printf("Created file: %s\n", file_path);
    } else {
        perror("Error creating file");
    }
}


// Function to create a new project
void create_project(const char *project_name) {
    char path[256];

    // Create project structure
    snprintf(path, sizeof(path), "%s", project_name);
    create_directory(path);

    snprintf(path, sizeof(path), "%s/include", project_name);
    create_directory(path);

    snprintf(path, sizeof(path), "%s/src", project_name);
    create_directory(path);

    snprintf(path, sizeof(path), "%s/lib", project_name);
    create_directory(path);

    // Create main.c
    snprintf(path, sizeof(path), "%s/main.c", project_name);
    const char *main_c_content = 
        "#include <stdio.h>\n\n"
        "int main() {\n"
        "    printf(\"Hello, World!\\n\");\n"
        "    return 0;\n"
        "}\n";
    create_file(path, main_c_content);

    // Create Makefile
    snprintf(path, sizeof(path), "%s/Makefile", project_name);
    const char *makefile_content = 
        "# Project directories\n"
        "INCLUDE_DIR = include\n"
        "SRC_DIR = src\n"
        "LIB_DIR = lib\n\n"
        "# Compiler settings\n"
        "CC = gcc\n"
        "CFLAGS = -Wall -Wextra -I$(INCLUDE_DIR)\n\n"
        "# Source files\n"
        "OBJ = main.o\n"
        "TARGET = main\n\n"
        "# Build rules\n"
        "all: $(TARGET)\n\n"
        "$(TARGET): $(OBJ)\n"
        "\t$(CC) $(OBJ) -o $(TARGET)\n\n"
        "%.o: %.c\n"
        "\t$(CC) $(CFLAGS) -c $< -o $@\n\n"
        "clean:\n"
        "\trm -f $(OBJ) $(TARGET)\n\n"
        ".PHONY: all clean\n";
    create_file(path, makefile_content);

    printf("Project '%s' created successfully!\n", project_name);
}

void add_library(const char *project_name, const char *library_name) {
    char path[256];
    
    // Create .h file
    snprintf(path, sizeof(path), "%s/include/%s.h", project_name, library_name);
    const char *header_content = 
        "#ifndef %s_H\n"
        "#define %s_H\n\n"
        "// Function declarations for %s\n\n"
        "#endif // %s_H\n";
    
    FILE *header_file = fopen(path, "w");
    if (header_file) {
        fprintf(header_file, header_content, 
                library_name, library_name, library_name, library_name);
        fclose(header_file);
        printf("Created header file: %s.h\n", library_name);
    } else {
        perror("Error creating header file");
    }

    // Create .c file
    snprintf(path, sizeof(path), "%s/src/%s.c", project_name, library_name);
    const char *source_content = 
        "#include \"%s.h\"\n\n"
        "// Function implementations for %s\n";
    
    FILE *source_file = fopen(path, "w");
    if (source_file) {
        fprintf(source_file, source_content, library_name, library_name);
        fclose(source_file);
        printf("Created source file: %s.c\n", library_name);
    } else {
        perror("Error creating source file");
    }

    // Update Makefile
    update_makefile_for_library(project_name, library_name);

    // Add include to main.c
    add_include_to_main(project_name, library_name);
}

void update_makefile_for_library(const char *project_name, const char *library_name) {
    char path[256];
    char temp_path[256];
    char line[1024];
    
    snprintf(path, sizeof(path), "%s/Makefile", project_name);
    snprintf(temp_path, sizeof(temp_path), "%s/Makefile.tmp", project_name);
    
    FILE *makefile = fopen(path, "r");
    FILE *temp_file = fopen(temp_path, "w");
    
    if (!makefile || !temp_file) {
        perror("Error opening Makefile");
        return;
    }

    // Add new source file to OBJ
    int obj_line_found = 0;
    while (fgets(line, sizeof(line), makefile)) {
        if (strstr(line, "OBJ =") && !obj_line_found) {
            // Remove newline and append new object
            line[strlen(line)-1] = '\0';
            fprintf(temp_file, "%s $(SRC_DIR)/%s.o\n", line, library_name);
            obj_line_found = 1;
        } else {
            fprintf(temp_file, "%s", line);
        }
    }

    fclose(makefile);
    fclose(temp_file);

    // Replace original with temp file
    remove(path);
    rename(temp_path, path);
    
    printf("Updated Makefile for library: %s\n", library_name);
}

void add_include_to_main(const char *project_name, const char *library_name) {
    char path[256];
    char temp_path[256];
    char line[1024];
    
    snprintf(path, sizeof(path), "%s/main.c", project_name);
    snprintf(temp_path, sizeof(temp_path), "%s/main.tmp", project_name);
    
    FILE *main_file = fopen(path, "r");
    FILE *temp_file = fopen(temp_path, "w");
    
    if (!main_file || !temp_file) {
        perror("Error opening main.c");
        return;
    }

    int include_added = 0;
    while (fgets(line, sizeof(line), main_file)) {
        if (strstr(line, "#include") && !include_added) {
            fprintf(temp_file, "%s", line);
            fprintf(temp_file, "#include \"%s.h\"\n", library_name);
            include_added = 1;
        } else {
            fprintf(temp_file, "%s", line);
        }
    }

    fclose(main_file);
    fclose(temp_file);

    // Replace original with temp file
    remove(path);
    rename(temp_path, path);
    
    printf("Added include to main.c for library: %s\n", library_name);
}

void add_function(const char *project_name, const char *lib_name, const char *func_name, 
                 const char *return_type, const char *params) {
    char path[256];

    // Add to header file
    snprintf(path, sizeof(path), "%s/include/%s.h", project_name, lib_name);
    FILE *header_file = fopen(path, "r+");
    if (header_file) {
        fseek(header_file, -strlen("#endif // _H\n"), SEEK_END);
        fprintf(header_file, "%s %s(%s);\n\n#endif // %s_H\n", 
                return_type, func_name, params, lib_name);
        fclose(header_file);
        printf("Added function prototype to %s.h: %s %s(%s);\n", 
               lib_name, return_type, func_name, params);
    } else {
        perror("Error opening header file");
        return;
    }

    // Add to source file
    snprintf(path, sizeof(path), "%s/src/%s.c", project_name, lib_name);
    FILE *source_file = fopen(path, "a");
    if (source_file) {
        fprintf(source_file, "\n%s %s(%s) {\n", return_type, func_name, params);
        fprintf(source_file, "    // TODO: Implement %s\n", func_name);
        if (strcmp(return_type, "void") != 0) {
            fprintf(source_file, "    return 0; // Default return value\n");
        }
        fprintf(source_file, "}\n");
        fclose(source_file);
        printf("Added function definition to %s.c: %s %s(%s)\n", 
               lib_name, return_type, func_name, params);
    } else {
        perror("Error opening source file");
    }
}

void remove_function(const char *project_name, const char *lib_name, const char *func_name) {
    char path[256];
    char line[256];
    FILE *file;
    FILE *temp_file;

    // Remove from header file
    snprintf(path, sizeof(path), "%s/include/%s.h", project_name, lib_name);
    file = fopen(path, "r");
    temp_file = fopen("temp.h", "w");

    if (file && temp_file) {
        while (fgets(line, sizeof(line), file)) {
            if (strstr(line, func_name) == NULL) {  // Skip lines with the function name
                fputs(line, temp_file);
            }
        }
        fclose(file);
        fclose(temp_file);

        // Replace original header file with the temporary one
        remove(path);
        rename("temp.h", path);
        printf("Removed function prototype from %s.h: %s\n", lib_name, func_name);
    } else {
        perror("Error removing function from header file");
    }

    // Remove from source file
    snprintf(path, sizeof(path), "%s/src/%s.c", project_name, lib_name);
    file = fopen(path, "r");
    temp_file = fopen("temp.c", "w");

    if (file && temp_file) {
        while (fgets(line, sizeof(line), file)) {
            if (strstr(line, func_name) == NULL) {  // Skip lines with the function name
                fputs(line, temp_file);
            }
        }
        fclose(file);
        fclose(temp_file);

        // Replace original source file with the temporary one
        remove(path);
        rename("temp.c", path);
        printf("Removed function definition from %s.c: %s\n", lib_name, func_name);
    } else {
        perror("Error removing function from source file");
    }
}


void remove_library(const char *project_name, const char *library_name) {
    char path[256];
    char command[512];

    // Remove header file
    snprintf(path, sizeof(path), "%s/include/%s.h", project_name, library_name);
    remove(path);
    printf("Removed header file: %s.h\n", library_name);

    // Remove source file
    snprintf(path, sizeof(path), "%s/src/%s.c", project_name, library_name);
    remove(path);
    printf("Removed source file: %s.c\n", library_name);

    // Remove library object file
    snprintf(path, sizeof(path), "%s/src/%s.o", project_name, library_name);
    remove(path);
    printf("Removed object file: %s.o\n", library_name);

    // Update Makefile
    char temp_path[256];
    char line[1024];
    
    snprintf(path, sizeof(path), "%s/Makefile", project_name);
    snprintf(temp_path, sizeof(temp_path), "%s/Makefile.tmp", project_name);
    
    FILE *makefile = fopen(path, "r");
    FILE *temp_file = fopen(temp_path, "w");
    
    if (makefile && temp_file) {
        while (fgets(line, sizeof(line), makefile)) {
            if (strstr(line, library_name) == NULL) {
                fputs(line, temp_file);
            }
        }
        fclose(makefile);
        fclose(temp_file);
        remove(path);
        rename(temp_path, path);
    }

    // Remove include from main.c
    snprintf(path, sizeof(path), "%s/main.c", project_name);
    snprintf(temp_path, sizeof(temp_path), "%s/main.tmp", project_name);
    
    FILE *main_file = fopen(path, "r");
    temp_file = fopen(temp_path, "w");
    
    if (main_file && temp_file) {
        while (fgets(line, sizeof(line), main_file)) {
            if (strstr(line, library_name) == NULL) {
                fputs(line, temp_file);
            }
        }
        fclose(main_file);
        fclose(temp_file);
        remove(path);
        rename(temp_path, path);
    }

    printf("Library '%s' removed successfully!\n", library_name);
}

void remove_project(const char *project_name) {
    char command[256];
    snprintf(command, sizeof(command), "rm -rf %s", project_name);
    
    if (system(command) == 0) {
        printf("Project '%s' removed successfully!\n", project_name);
    } else {
        perror("Error removing project");
    }
}

void build_and_run_project(const char *project_name) {
    int name_length = strlen(project_name);
    int line_length = 20; // Base length for each side
    
    // Print top border with project name
    printf("\n");
    for(int i = 0; i < line_length; i++) printf("━");
    printf(" %s ", project_name);
    for(int i = 0; i < line_length; i++) printf("━");
    printf("\n");
    
    // Build message
    printf("\n\033[1;33m⚡ Building project...\033[0m\n\n");
    
    char command[256];
    
    // Build the project
    snprintf(command, sizeof(command), "cd %s && make", project_name);
    if (system(command) != 0) {
        printf("\n\033[1;31m✖ Error: Build failed!\033[0m\n");
        return;
    }

    printf("\n\033[1;32m✔ Build successful!\033[0m\n");
    
    // Run message
    printf("\n\033[1;33m▶ Running project...\033[0m\n\n");
    
    // Run the project
    snprintf(command, sizeof(command), "cd %s && ./main", project_name);
    if (system(command) == 0) {
        printf("\n\033[1;32m✔ Program executed successfully!\033[0m\n");
    } else {
        printf("\n\033[1;31m✖ Error: Program execution failed!\033[0m\n");
    }

    // Print bottom border with same length as top
    printf("\n");
    for(int i = 0; i < 2 * line_length + name_length + 2; i++) printf("━");
    printf("\n\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Error: No command provided. Use 'cpg help' for usage.\n");
        return 1;
    }

    if (strcmp(argv[1], "help") == 0) {
        show_help();
    } else if (strcmp(argv[1], "addfunc") == 0) {
        if (argc < 6) {
            printf("Error: Insufficient arguments for 'addfunc'.\n");
            printf("Usage: cpg addfunc [ProjectName] [LibName] [FuncName] [ReturnType] [Params]\n");
            return 1;
        }
        add_function(argv[2], argv[3], argv[4], argv[5], argv[6]);
    } else if (strcmp(argv[1], "removefunc") == 0) {
        if (argc < 5) {
            printf("Error: Insufficient arguments for 'removefunc'.\n");
            printf("Usage: cpg removefunc [ProjectName] [LibName] [FuncName]\n");
            return 1;
        }
        remove_function(argv[2], argv[3], argv[4]);
    } else if (strcmp(argv[1], "addlib") == 0) {
        if (argc < 4) {
            printf("Error: Insufficient arguments for 'addlib'.\n");
            printf("Usage: cpg addlib [ProjectName] [LibraryName]\n");
            return 1;
        }
        add_library(argv[2], argv[3]);
    } else if (strcmp(argv[1], "removelib") == 0) {
        if (argc < 4) {
            printf("Error: Insufficient arguments for 'removelib'.\n");
            printf("Usage: cpg removelib [ProjectName] [LibraryName]\n");
            return 1;
        }
        remove_library(argv[2], argv[3]);
    } else if (strcmp(argv[1], "removeproject") == 0) {
        if (argc < 3) {
            printf("Error: No project name provided to remove.\n");
            printf("Usage: cpg removeproject [ProjectName]\n");
            return 1;
        }
        remove_project(argv[2]);
    } else if (strcmp(argv[1], "buildrun") == 0) {
        if (argc < 3) {
            printf("Error: No project name provided to build and run.\n");
            printf("Usage: cpg buildrun [ProjectName]\n");
            return 1;
        }
        build_and_run_project(argv[2]);
    } else {
        create_project(argv[1]);
    }

    return 0;
}


