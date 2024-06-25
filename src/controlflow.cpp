#include "controlflow.h"
#include "shell.h"
#include <stdio.h>
#include <string.h>

enum states { NEUTRAL, WANT_THEN, THEN_BLOCK };
enum results { SUCESS, FAIL };

static int if_state = NEUTRAL;
static int if_result = SUCESS;
static int last_stat = 0;

int synErr(char *);
void fatal(char *s1, char *s2, int n);

namespace shellControlflow
{
    bool isAbleToExecute()
    {
        bool bExecute = true;

        if (if_state == WANT_THEN)
        {
            synErr("then expected");
            bExecute = false;
        }
        else if (if_state == THEN_BLOCK && if_result == SUCESS)
            bExecute = true;
        else if (if_state == THEN_BLOCK && if_result == FAIL)
            bExecute = false;
        return bExecute;
    }

    bool isControlCommand(char *s)
    {
        return (strcmp(s, "if") == 0 || strcmp(s, "then") == 0 || strcmp(s, "fi") == 0);
    }

    int doControlCommand(char **args)
    {
        char *cmd = args[0];
        std::string output;
        int rv = -1;

        if (strcmp(cmd, "if") == 0)
        {
            if (if_state != NEUTRAL)
                rv = synErr("if unexpected");
            else
            {
                last_stat = Shell::process(args + 1, output);
                if_result = (last_stat == 0 ? SUCESS : FAIL);
                if_state = WANT_THEN;
                rv = 0;
            }
        }
        else if (strcmp(cmd, "then") == 0)
        {
            if (if_state != WANT_THEN)
                rv = synErr("then unexpected");
            else
            {
                if_state = THEN_BLOCK;
                rv = 0;
            }
        }
        else if (strcmp(cmd, "fi") == 0)
        {
            if (if_state != THEN_BLOCK)
                rv = synErr("fi unexpected");
            else
            {
                if_state = NEUTRAL;
                rv = 0;
            }
        }
        else
            fatal("internal error processing:", cmd, 2);

        return rv;
    }
}

int synErr(char *msg)
{
    if_state = NEUTRAL;
    fprintf(stderr, "syntax error: %s\n", msg);
    return -1;
}

void fatal(char *s1, char *s2, int n)
{
    fprintf(stderr, "Error: %s, %s\n", s1, s2);
    exit(n);
}
