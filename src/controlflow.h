#ifndef CONTROLFLOW_H
#define CONTROLFLOW_H

/**
 * Shell流程控制
 */
namespace shellControlflow
{
    /**
     * @brief 是否是流程控制命令
     * @param s 命令
     */
    bool isControlCommand(char *s);

    /**
     * @brief 执行流程控制命令
     * @param args 命令
     * @return 命令退出exit结果
     */
    int doControlCommand(char **args);

    /**
     * @brief 当前是否可执行
     * @return 0为可执行，非0不可执行
     */
    bool isAbleToExecute();
}

#endif // CONTROLFLOW_H
