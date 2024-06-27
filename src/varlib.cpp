#include "varlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXVARS 200

/** 存储变量的字符串和是否为全局变量的标志 */
struct var
{
    char *str;
    int global;
};

static struct var tab[MAXVARS];

static struct var *find_item(char *name, int first_blank);
static char *new_string(char *name, char *val);

namespace shellVarlib
{
    int VLstore(char *name, char *val)
    {
        struct var *itemp;
        char *s;
        int rv = 1;

        if ((itemp = find_item(name, 1)) != NULL && (s = new_string(name, val)) != NULL)
        {
            if (itemp->str)
                free(itemp->str);
            itemp->str = s;
            rv = 0;
        }
        return rv;
    }

    char *VLlookup(char *name)
    {
        struct var *itemp;

        if ((itemp = find_item(name, 0)) != NULL)
            return itemp->str + 1 + strlen(name);
        return "";
    }

    int VLexport(char *name)
    {
        struct var *itemp;
        int rv = 1;

        if ((itemp = find_item(name, 0)) != NULL)
        {
            itemp->global = 1;
            rv = 0;
        }
        else if (VLstore(name, "") == 1)
            rv = VLexport(name);
        return rv;
    }

    void VLlist()
    {
        int i;
        for (i = 0; i < MAXVARS && tab[i].str != NULL; i++)
        {
            if (tab[i].global)
                printf("*%s\n", tab[i].str);
            else
                printf(" %s\n", tab[i].str);
        }
    }

    int VLenviron2table(char *env[])
    {
        int i;

        for (i = 0; env[i] != NULL; i++)
        {
            if (i == MAXVARS)
                return 0;
            tab[i].str = new_string(env[i], "");
            tab[i].global = 1;
        }
        while (i < MAXVARS)
        {
            tab[i].str = NULL;
            tab[i++].global = 0;
        }
        return 1;
    }

    char **VLtable2environ()
    {
        int i, j, n = 0;
        char **envtab;

        for (i = 0; i < MAXVARS && tab[i].str != NULL; i++)
        {
            if (tab[i].global == 1)
                n++;
        }
        envtab = (char **)malloc((n + 1) * sizeof(char *));
        for (i = 0, j = 0; i < MAXVARS && tab[i].str != NULL; i++)
        {
            if (tab[i].global == 1)
                envtab[j++] = tab[i].str;
        }
        envtab[j] = NULL;
        return envtab;
    }
}

/** 创建一个新的字符串 */
static char *new_string(char *name, char *val)
{
    char *retval;

    retval = (char *)malloc(strlen(name) + strlen(val) + 2);
    if (retval != NULL)
        sprintf(retval, "%s=%s", name, val);
    return retval;
}

/**
 * 在 tab 中查找名字为 name 的变量
 * 如果找到，返回该变量的指针；否则，如果 first_blank 为真，并且存在空位，则返回一个空位的指针
 */
static struct var *find_item(char *name, int first_blank)
{
    int i;
    int len = strlen(name);
    char *s;

    for (i = 0; i < MAXVARS && tab[i].str != NULL; i++)
    {
        s = tab[i].str;
        if (strncmp(s, name, len) == 0 && s[len] == '=')
            return &tab[i];
    }
    if (i < MAXVARS && first_blank)
        return &tab[i];
    return NULL;
}
