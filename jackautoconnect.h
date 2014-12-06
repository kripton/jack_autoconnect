#ifndef JACKAUTOCONNECT_H
#define JACKAUTOCONNECT_H

#include <QObject>
#include <QDebug>
#include <QRegExp>
#include <QHash>
#include <jack/jack.h>

class JackAutoconnect : public QObject
{
    Q_OBJECT
public:
    // ctor :D
    explicit JackAutoconnect(QHash<QRegExp*, QRegExp*>* connectionsToDo, QObject *parent = 0);

    // Method to be calles whenever a new port has been registered with jack
    static void myRegCallback_static(jack_port_id_t port, int action, void *arg);

private:
        // The RegEx pairs of port names to connect
    QHash<QRegExp*, QRegExp*>* connectionsToDo;

    // Our jack client handle
    jack_client_t* client;

signals:
    // Will be emitted every time a new port has been registered with jack
    void newPort();

public slots:
    // Main worker method that iterates through all entries in connectionsToDo
    void doNewPort();

};

#endif // JACKAUTOCONNECT_H
