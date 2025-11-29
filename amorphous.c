#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_VARS 2048
#define MAX_NAME 64
#define MAX_LINE 512

// variable table
char  names[MAX_VARS][MAX_NAME];
long double values[MAX_VARS];
char  types[MAX_VARS];   // i=int, f=float, s=sci, c=char
int   used[MAX_VARS];
int   var_count = 0;

// find or create variable ID
int get_var_id(char *name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(names[i], name) == 0)
            return i;
    }
    strcpy(names[var_count], name);
    return var_count++;
}

static inline void skip(char **p) {
    while (**p==' ' || **p=='\t') (*p)++;
}

long double read_number(char **p) {
    char buf[128];
    int bi = 0;

    while (isdigit(**p) || **p=='.' || **p=='e' || **p=='E'
           || **p=='+' || **p=='-') {
        buf[bi++] = **p;
        (*p)++;
    }
    buf[bi] = 0;

    return strtold(buf, NULL);
}

// read full var name: letters, digits, underscore
void read_varname(char **p, char *buf) {
    int bi = 0;
    while (isalnum(**p) || **p=='_') {
        buf[bi++] = **p;
        (*p)++;
    }
    buf[bi] = 0;
}

// FULL RPN FOLDER
long double hyperfold(char *expr) {
    long double stack[256];
    int sp = 0;
    char *p = expr;

    while (*p) {

        if (p[0]=='.' && p[1]=='.')
            break;

        if (*p==' ' || *p=='\t') {
            p++;
            continue;
        }

        // NUMBER
        if (isdigit(*p) || (*p=='-' && isdigit(*(p+1)))) {
            stack[sp++] = read_number(&p);
            continue;
        }

        // VARIABLE (.name, %name, &name)
        if (*p=='.' || *p=='%' || *p=='&') {
            p++;
            char varname[MAX_NAME];
            read_varname(&p, varname);
            int id = get_var_id(varname);
            stack[sp++] = values[id];
            continue;
        }

        // CHAR VAR
        if (*p=='"' && isalpha(p[1])) {
            p++;
            char varname[2] = { p[0], 0 };
            p++;
            int id = get_var_id(varname);
            stack[sp++] = values[id];
            continue;
        }

        // OPERATORS
        if (*p=='+' || *p=='-' || *p=='*' || *p=='/') {
            if (sp < 2) { p++; continue; }
            long double b = stack[--sp];
            long double a = stack[--sp];

            switch (*p) {
                case '+': stack[sp++] = a + b; break;
                case '-': stack[sp++] = a - b; break;
                case '*': stack[sp++] = a * b; break;
                case '/': stack[sp++] = a / b; break;
            }
            p++;
            continue;
        }

        p++;
    }

    return sp ? stack[sp-1] : 0;
}

int main(int argc, char **argv) {

    if (argc < 3) {
        printf("usage: optimizer <in.cryo> <out.c>\n");
        return 1;
    }

    FILE *in  = fopen(argv[1], "r");
    FILE *out = fopen(argv[2], "w");

    if (!in || !out) {
        printf("file error\n");
        return 1;
    }

    fprintf(out, "#include <stdio.h>\nint main(){\n");

    char line[MAX_LINE];

    while (fgets(line, MAX_LINE, in)) {

        // clean CR
        for (int i=0; line[i]; i++)
            if (line[i]=='\r') line[i] = ' ';

        if (strlen(line) < 2)
            continue;

        // OUTPUT
        if (strncmp(line, "out", 3)==0 || strncmp(line, "print", 5)==0) {

            char fmt[512]={0};
            char args[512]={0};
            int fi=0, ai=0;

            char *p = line + (line[0]=='o' ? 3 : 5);
            skip(&p);

            while (*p && !(p[0]=='.' && p[1]=='.')) {

                if (*p=='.' || *p=='%' || *p=='&') {
                    p++;
                    char varname[MAX_NAME];
                    read_varname(&p, varname);

                    int id = get_var_id(varname);
                    char t = types[id];
                    long double val = values[id];

                    if (t=='i') { fmt[fi++]='%'; fmt[fi++]='d'; ai+=sprintf(args+ai,", %d",(int)val); }
                    if (t=='f') { fmt[fi++]='%'; fmt[fi++]='f'; ai+=sprintf(args+ai,", %Lf",val); }
                    if (t=='s') { fmt[fi++]='%'; fmt[fi++]='e'; ai+=sprintf(args+ai,", %Le",val); }

                    continue;
                }

                // CHAR VAR
                if (*p=='"' && isalpha(p[1])) {
                    p++;
                    char varname[2]={p[0],0};
                    p++;
                    int id = get_var_id(varname);
                    fmt[fi++]='%'; fmt[fi++]='c';
                    ai += sprintf(args+ai,", '%c'", (char)values[id]);
                    continue;
                }

                fmt[fi++] = *p;
                p++;
            }

            fprintf(out, "printf(\"%s\"%s);\n", fmt, args);
            continue;
        }

        // VARIABLE DEF
        char *p = line;

        char typechar = *p;     // . % & "
        p++;

        char varname[MAX_NAME];
        read_varname(&p, varname);
        int id = get_var_id(varname);

        // get expression
        char *expr = strchr(line, ' ');
        if (!expr) continue;

        expr++;
        skip(&expr);

        if (typechar=='"') {
            values[id] = *expr;
            types[id] = 'c';
            used[id] = 1;
            continue;
        }

        long double val = hyperfold(expr);

        values[id] = val;
        used[id] = 1;

        if (typechar=='.') types[id] = 'i';
        if (typechar=='%') types[id] = 'f';
        if (typechar=='&') types[id] = 's';
    }

    fprintf(out, "return 0;\n}\n");
    return 0;
}
