#ifndef OCRTHREAD
#define OCRTHREAD

#include <QThread>
#include <QString>

class OCRThread : public QThread
{
public:
    // constructor
    // set name using initializer
    OCRThread();

    // overriding the QThread's run() method
    void run();
};

#endif // OCRTHREAD
