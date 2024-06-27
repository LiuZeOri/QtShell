#ifndef SHELL_H
#define SHELL_H

#include <string>
#include <QString>

class Shell
{
public:
    Shell();

    /**
     * @brief 执行处理命令
     * @param command 命令
     * @param output 输出
     * @return 命令执行退出结果
     */
    int processCommand(const QString& command, QString& output);

private:
    /** 初始化 */
    void initialization();

    /**
     * @brief 分割命令
     * @param line 命令
     * @return 分割后的命令字符串数组
     */
    char** splitCommand(char* line);

    /**
     * @brief 执行命令
     * @param argv 命令
     * @param output 运行结果
     * @return 执行退出结果
     */
    static int executeCommand(char *argv[], std::string &output);

private:
    /**
     * @brief 检查输入的命令是否是内建命令，如果是则执行相应的操作并返回结果
     * @param args 命令
     * @param resultp 执行退出结果
     * @return 是否为内建命令
     */
    static bool builtinCommand(char** args, int* resultp);

    /** 处理变量赋值操作，将变量名和值存储在变量表中 */
    static int assign(char* str);

    /** 检查变量名是否合法。变量名必须由字母和数字组成，且不能以数字开头，也不能包含下划线 */
    static int okName(char* str);

public:
    /**
     * @brief 处理命令
     * @param args 具体命令
     * @param output 运行结果
     * @return 退出exit状态，0表示成功，非0为失败
     */
    static int process(char** args, std::string &output);
};

#endif // SHELL_H
