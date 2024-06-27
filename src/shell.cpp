#include "shell.h"
#include "controlflow.h"
#include "varlib.h"

#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <iterator>
#include <cstring>
#include <QStringList>

#include <sys/wait.h>

#define is_delim(x) ((x) == ' ' || (x) == '\t')

Shell::Shell()
{
    initialization();
}

void Shell::initialization()
{
    extern char **environ;
    shellVarlib::VLenviron2table(environ);
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}

int Shell::processCommand(const QString& command, QString& output)
{
    // 将QString转化为char*
    std::string commandStr = command.toStdString();
    std::string stringOutput;
    char* commandCStr = new char[commandStr.length() + 1];  // +1 for the null terminator
    strcpy(commandCStr, commandStr.c_str());

    int result = process(splitCommand(commandCStr), stringOutput);

    output = QString::fromStdString(stringOutput);

    return result;
}

char** Shell::splitCommand(char* line)
{
    char **args;
    int spots = 0;
    int bufspace = 0;
    int argnum = 0;
    char *cp = line;
    char *start;
    int len;

    if (line == NULL)
        return NULL;
    args = (char **)malloc(bufspace = spots = 8 * sizeof(char *));
    while (*cp != '\0')
    {
        while (is_delim(*cp))
            cp++;
        if (*cp == '\0')
            break;

        if (argnum + 1 >= spots)
        {
            args = (char **)realloc(args, (bufspace += 8 * sizeof(char *)));
            spots += 8;
        }
        start = cp;
        len = 1;
        while (*++cp != '\0' && !(is_delim(*cp)))
            len++;
        args[argnum++] = strndup(start, len);
    }
    args[argnum] = NULL;
    return args;
}

int Shell::executeCommand(char *argv[], std::string &output)
{
    int pid;
    int child_info = -1;
    extern char **environ;

    if (argv[0] == NULL)
        return 0;

    int pipefd[2];
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return -1;
    }

    if ((pid = fork()) == -1)
    {
        perror("fork");
    }
    else if (pid == 0)
    {
        // 子进程
        close(pipefd[0]);   // 关闭读端
        dup2(pipefd[1], STDOUT_FILENO);     // 将标准输出重定向到写端
        dup2(pipefd[1], STDERR_FILENO);     // 将标准错误重定向到写端
        close(pipefd[1]);

        environ = shellVarlib::VLtable2environ();
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        execvp(argv[0], argv);
        perror("cannot execute command");
        exit(1);
    }
    else
    {   // 父进程
        close(pipefd[1]);   // 关闭写端
        char buffer[4096];
        ssize_t count;

        while ((count = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0)
        {
            buffer[count] = '\0';
            output += buffer;
        }

        close(pipefd[0]);

        if (wait(&child_info) == -1)
            perror("wait");
    }

    return child_info;
}

bool Shell::builtinCommand(char **args, int *resultp)
{
    bool bBuiltin = false;

    if (strcmp(args[0], "set") == 0)
    {
        shellVarlib::VLlist();   // 列出所有变量
        *resultp = 0;
        bBuiltin = true;
    }
    else if (strchr(args[0], '=') != NULL)
    {
        *resultp = assign(args[0]);     // 处理变量赋值
        if (*resultp != -1)
            bBuiltin = true;
    }
    else if (strcmp(args[0], "export") == 0)
    {
        if (args[1] != NULL && okName(args[1]))
        {
            *resultp = shellVarlib::VLexport(args[1]);   // 将变量标记为全局变量
        }
        else
            *resultp = 1;
        bBuiltin = true;
    }
    return bBuiltin;
}

int Shell::assign(char* str)
{
    char *cp;
    int rv;

    cp = strchr(str, '=');  // 找到等号位置
    *cp = '\0';     // 将等号替换为字符串终止符，分隔变量名和变量值
    rv = (okName(str) ? shellVarlib::VLstore(str, cp + 1) : -1);     // 存储变量
    *cp = '=';      // 恢复等号
    return rv;
}

int Shell::okName(char* str)
{
    char *cp;

    for (cp = str; *cp; cp++)
    {
        if ((isdigit(*cp) && cp == str) || !(isalnum(*cp)) || *cp == '_')
            return 0;
    }

    return (cp != str);
}

int Shell::process(char** args, std::string &output)
{
    // 初始化退出为0,表示成功
    int rv = 0;

    // 如果为空，则退出为0
    if (args[0] == NULL)
        rv = 0;
    // 判断是否是流程控制命令
    else if (shellControlflow::isControlCommand(args[0]))
        rv = shellControlflow::doControlCommand(args);
    // 判断当前是否可执行命令
    else if (shellControlflow::isAbleToExecute())
    {
        if (!builtinCommand(args, &rv))
            rv = executeCommand(args, output);     // 执行命令
    }

    return rv;
}
