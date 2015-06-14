#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include <iostream>
#include <assert.h>
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include "opencvwidget.h"
#include "mycamerawindow.h"

int main(int argc, char **argv) {
    //TODO: modify the camera
    //Capture the first camera
    CvCapture * camera = cvCaptureFromCAM(0);

    //Launch the GUI
    QApplication app(argc, argv);
    MyCameraWindow *mainWin = new MyCameraWindow(camera);
    mainWin->setWindowTitle("OpenVideoMagnifier");
    mainWin->showMaximized();
    int retval = app.exec();

    //Release the camera
    cvReleaseCapture(&camera);

    return retval;
}
