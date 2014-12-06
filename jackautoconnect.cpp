#include "jackautoconnect.h"

JackAutoconnect::JackAutoconnect(QHash<QRegExp *, QRegExp *>* connectionsToDo, QObject *parent) :
    QObject(parent)
{
    if ((client = jack_client_open("autoconnect", JackNoStartServer, 0)) == 0)
    {
        qCritical() << "Unable to connect to jack server :( Exiting ...";
    }

    this->connectionsToDo = connectionsToDo;

    // We explicitely want a QueuedConnection since we cannot connect ports in the callback/notification-thread
    connect(this, SIGNAL(newPort()), this, SLOT(doNewPort()), Qt::QueuedConnection);

    jack_set_port_registration_callback(client, &myRegCallback_static, (void*)this);

    jack_activate(client);
}

void JackAutoconnect::myRegCallback_static(jack_port_id_t port, int action, void *arg)
{
    Q_UNUSED(port);

    if (action == 0)
    {
        // A port has been unregistered. Nothing to do for us
        return;
    }

    // Emit the signal so that the main thread will check the ports and connect them
    emit ((JackAutoconnect*) arg)->newPort();
}

void JackAutoconnect::doNewPort()
{
    const char ** outPorts;
    const char ** inPorts;
    long unsigned int i = 0;

    qDebug() << "Port registered";

    QList<QString> outputPorts;
    QList<QString> inputPorts;

    // Iterate over all output ports and save them to our QList
    outPorts = jack_get_ports(client, NULL, NULL, JackPortIsOutput);
    for (i = 0; outPorts[i]; ++i)
    {
        qDebug() << "Found output port:" << QString(outPorts[i]);
        outputPorts.append(QString(outPorts[i]));
    }
    free(outPorts);

    // Iterate over all output ports and save them to our QList
    inPorts = jack_get_ports(client, NULL, NULL, JackPortIsInput);
    for (i = 0; inPorts[i]; ++i)
    {
        qDebug() << "Found input port:" << QString(inPorts[i]);
        inputPorts.append(QString(inPorts[i]));
    }
    free(inPorts);

    // now check all our regexes to see if a pair of existing jack ports matches
    foreach(QRegExp* expr, connectionsToDo->keys())
    {
        qDebug() << "Checking RegEx pair" << expr->pattern() << "->" << connectionsToDo->value(expr)->pattern();
        foreach (QString existingOutPort, outputPorts)
        {
            if ((expr->indexIn(existingOutPort)) != -1)
            {
                qDebug() << "Got matching outputPort:" << existingOutPort;
                foreach (QString existingInPort, inputPorts)
                {
                    if ((connectionsToDo->value(expr)->indexIn(existingInPort)) != -1)
                    {
                        qDebug() << "Got matching inputPort:" << existingInPort << "CONNECT!";

                        // We don't even check if the ports are connected yet, we just tell jack to connect them and even ignore the return value
                        jack_connect(client, existingOutPort.toUtf8().constData(), existingInPort.toUtf8().constData());
                    }
                }
            }
        }
    }
}
