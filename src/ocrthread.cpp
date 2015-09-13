#include "ocrthread.h"
#include <QProcess>

OCRThread::OCRThread()
{

}

void OCRThread::run()
{
    QProcess::execute("gocr ocr.png -o gocrOutput");
    QProcess::execute("python autocorrect.py gocrOutput");
    QProcess::execute("espeak -f gocrOutput -vfr+f4");
}
