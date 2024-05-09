#ifndef SLEEPER_H
#define SLEEPER_H

#include <QThread>

class Sleeper: public QThread
{
public:
    static void msleep(unsigned short int msecs)
    {
        QThread::msleep(msecs);
    }
};

#endif // SLEEPER_H
