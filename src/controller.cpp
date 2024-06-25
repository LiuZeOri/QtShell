#include "controller.h"
#include <QStringList>

Controller::Controller(QObject* parent) : QObject(parent)
{
    shell = new Shell();
}

Controller::~Controller()
{
    delete shell;
}

int Controller::executeCommand(const QString& command)
{
    int result = -1;

    if (shell != nullptr)
    {
        QString output = "";
        result = shell->processCommand(command, output);
        emit commandOutput(output);
    }

    return result;
}
