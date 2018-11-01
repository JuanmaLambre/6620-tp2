#include <stdlib.h>
#include <stdio.h>

#include "Cache.c"
#include "Memory.c"
#include "Command.c"


char* read_line(FILE* input) {
    char* str = (char*) malloc(sizeof(char));
    *str = '\0';
    int len = 0;
    char c;

    if (feof(input)) return NULL;

    while (EOF != (c=fgetc(input)) && c != '\n') {
        ++len;
        str = (char*) realloc(str, sizeof(char)*(len+1));
        str[len-1] = c;
        str[len] = '\0';
    }   

    if (c == EOF && len == 0) {
        free(str);
        return NULL;
    }   

    return str;
}

int load_commands(FILE* input, Command** ptr) {
    Command* cmds = NULL;
    int count = 0;
    char* line = NULL;

    while (line = read_line(input)) {
        ++count;
        cmds = (Command*) realloc(cmds, sizeof(Command)*count);
        Command_init(&cmds[count-1], line);
        free(line);
    }

    *ptr = cmds;
    return count;
}


int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "No file specified\n");
        return 1;
    }

    init();

    // Get commands
    Command* commands;
    int cmd_count = 0;
    FILE* input = fopen(argv[1], "r");
    if (0) {
        fprintf(stderr, "Error opening file %s", argv[1]);
        return 1;
    }
    cmd_count = load_commands(input, &commands);

    // Execute commands
    for (int i = 0; i < cmd_count; ++i) {
        Command_execute(commands+i);
    }

    for (int i = 0; i < cmd_count; ++i) {
        Command_destroy(commands+i);
    }
    
    return 0;
}

