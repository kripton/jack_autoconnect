#include <QCoreApplication>
#include "jackautoconnect.h"

int main(int argc, char *argv[])
{
    int i;
    QHash<QRegExp*, QRegExp*>* connectionsToDo = new QHash<QRegExp*, QRegExp*>();

    QCoreApplication a(argc, argv);

    if (argc < 3)
    {
        qCritical() << "Not enough parameters";
    }

    // Iterate over the command line arguments to create Output/Input-port RegEx pairs that shall be automatically connected
    for (i = 1; i < (argc - 1); i = i + 2)
    {
        connectionsToDo->insert(new QRegExp(QString(argv[i]), Qt::CaseInsensitive, QRegExp::Wildcard), new QRegExp(QString(argv[i + 1]), Qt::CaseInsensitive, QRegExp::Wildcard));
    }

    // Create the object that connects to jack and does all the work
    JackAutoconnect* worker = new JackAutoconnect(connectionsToDo);
    Q_UNUSED(worker);

    // Simply enter the event loop and wait for the things to come
    return a.exec();
}
