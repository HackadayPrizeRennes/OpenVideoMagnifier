#include "opencvwidget.h"

QOpenCVWidget::QOpenCVWidget(QWidget *parent) : QWidget(parent) {
    imagelabel = new QLabel;

    _mainLayout = new QHBoxLayout;
    _controlLayout = new QVBoxLayout;
        _invert = new QPushButton("Inverser");
        _invert->setMaximumWidth(200);
        _controlLayout->addWidget(_invert);
        QObject::connect(_invert, SIGNAL(clicked()), this, SLOT(invertSlot()));
        _balance = new QPushButton("Balance des blancs");
        _balance->setMaximumWidth(200);
        _controlLayout->addWidget(_balance);
        QObject::connect(_balance, SIGNAL(clicked()), this, SLOT(balanceSlot()));
        _zoomP = new QPushButton("Zoom x2");
        _zoomP->setMaximumWidth(200);
        _controlLayout->addWidget(_zoomP);
        QObject::connect(_zoomP, SIGNAL(clicked()), this, SLOT(zoomPSlotl()));
        _zoomM = new QPushButton("Zoom /2");
        _zoomM->setMaximumWidth(200);
        _controlLayout->addWidget(_zoomM);
        QObject::connect(_zoomM, SIGNAL(clicked()), this, SLOT(zoomMSlot()));
        _saturationP = new QPushButton("Saturation +1");
        _saturationP->setMaximumWidth(200);
        _controlLayout->addWidget(_saturationP);
        QObject::connect(_saturationP, SIGNAL(clicked()), this, SLOT(saturationPSlot()));
        _saturationM = new QPushButton("Saturation -1");
        _saturationM->setMaximumWidth(200);
        _controlLayout->addWidget(_saturationM);
        QObject::connect(_saturationM, SIGNAL(clicked()), this, SLOT(saturationMSlot()));
        _save = new QPushButton("Sauvegarde");
        _save->setMaximumWidth(200);
        _controlLayout->addWidget(_save);
        QObject::connect(_save, SIGNAL(clicked()), this, SLOT(saveSlot()));

    _mainLayout->addWidget(imagelabel);
    _mainLayout->addLayout(_controlLayout);
    setLayout(_mainLayout);
}

QOpenCVWidget::~QOpenCVWidget(void) {

}

void QOpenCVWidget::putImage(IplImage *cvimage) {
    int cvIndex, cvLineStart;
    // switch between bit depths
    switch (cvimage->depth) {
        case IPL_DEPTH_8U:
            switch (cvimage->nChannels) {
                case 3:
                    if ( (cvimage->width != image.width()) || (cvimage->height != image.height()) ) {
                        QImage temp(cvimage->width, cvimage->height, QImage::Format_RGB32);
                        image = temp;
                    }
                    cvIndex = 0; cvLineStart = 0;
                    for (int y = 0; y < cvimage->height; y++) {
                        unsigned char red,green,blue;
                        cvIndex = cvLineStart;
                        for (int x = 0; x < cvimage->width; x++) {
                            // DO it
                            red = cvimage->imageData[cvIndex+2];
                            green = cvimage->imageData[cvIndex+1];
                            blue = cvimage->imageData[cvIndex+0];

                            image.setPixel(x,y,qRgb(red, green, blue));
                            cvIndex += 3;
                        }
                        cvLineStart += cvimage->widthStep;
                    }
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    imagelabel->setPixmap(QPixmap::fromImage(image).scaled(imagelabel->width(), imagelabel->height(), Qt::KeepAspectRatio));
}
QImage QOpenCVWidget::getImageLabel()
{
    return imagelabel->pixmap()->toImage();
}

void QOpenCVWidget::invertSlot()
{
    emit invertSignal();
}

void QOpenCVWidget::balanceSlot()
{
    emit balanceSignal();
}

void QOpenCVWidget::zoomPSlotl()
{
    emit zoomPSignal();
}

void QOpenCVWidget::zoomMSlot()
{
    emit zoomMSignal();
}

void QOpenCVWidget::saturationPSlot()
{
    emit saturationPSignal();
}

void QOpenCVWidget::saturationMSlot()
{
    emit saturationMSignal();
}

void QOpenCVWidget::saveSlot()
{
    emit saveSignal();
}

