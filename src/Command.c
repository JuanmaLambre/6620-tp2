#ifndef COMMAND_C
#define COMMAND_C

#include <string.h>

typedef unsigned int uint;
typedef unsigned char uchar;

typedef struct {
    char* op;
    uint* argv;
    int argc;
} Command;


int is_eoarg_char(char c) {
    return c == ' ' || c == '\n' || c == '\t' || c == ',';
}


void Command_init(Command* self, char* cmdstr) {
    int start = 0, end = 0, cmdlen = strlen(cmdstr);

    // Default values
    self->op = NULL;
    self->argv = NULL;
    self->argc = 0;

    // Get the operation
    int eoarg = 0;
    while (end < cmdlen && !eoarg) {
        eoarg = is_eoarg_char(cmdstr[++end]);
    }
    self->op = malloc(end-start+1);
    strncpy(self->op, cmdstr, end-start);
    self->op[end-start] = '\0';

    // Now we get the arguments
    while (end < cmdlen) {
        start = end;
        while (start < cmdlen && eoarg) {
            eoarg = is_eoarg_char(cmdstr[++start]);
        }
        end = start;
        while (end < cmdlen && !eoarg) {
            eoarg = is_eoarg_char(cmdstr[++end]);
        }
        if (start < cmdlen) {
            ++self->argc;
            self->argv = (uint*) realloc(self->argv, sizeof(uint)*self->argc);
            char buffer[16];
            memset(buffer, '\0', 16);
            strncpy(buffer, cmdstr+start, end-start);
            self->argv[self->argc-1] = atoi(buffer);
        }
    }
}

void Command_execute(Command* self) {
    if (strcmp(self->op, "R") == 0) {
        int value = (uchar) read_byte(self->argv[0]);
        if (value >= 0)
            printf("%d\n", value);
    } else if (strcmp(self->op, "W") == 0) {
        int res = write_byte(self->argv[0], self->argv[1]);
        printf("%d\n", res);
    } else if (strcmp(self->op, "MR") == 0) {
        printf("MissRate %d%%\n", get_miss_rate());
    } else {
        fprintf(stderr, "Unknown command: %s\n", self->op);
    }
}

void Command_destroy(Command* self) {
    free(self->op);
    if (self->argc > 0)
        free(self->argv);
}


#endif
