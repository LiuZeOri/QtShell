#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "shell.h"

/**
 * 此类为Shell的控制类，继承自QObject
 * 实现输入命令的执行，与界面的通讯
 */
class Controller : public QObject
{
    Q_OBJECT

public:
    explicit Controller(QObject* parent = nullptr);
    ~Controller();

public slots:
    /**
     * @brief 执行命令
     * @param command 命令
     * @return 命令执行退出exit结果；0成功，非0失败
     */
    int executeCommand(const QString& command);

signals:
    /** 信号，返回执行的结果 */
    void commandOutput(const QString& output);

private:
    Shell* shell;
};

#endif // CONTROLLER_H
