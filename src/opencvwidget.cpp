#include "opencvwidget.h"

//Init the GUI
QOpenCVWidget::QOpenCVWidget(QWidget *parent) : QWidget(parent) {
    imagelabel = new QLabel;

    _mainLayout = new QHBoxLayout;
    _controlLayout = new QVBoxLayout;
        _invert = new QPushButton();
        _invert->setIcon(QIcon("rsc/invert.png"));
        _invert->setIconSize(QSize(150,150));
        _invert->setMaximumWidth(150);
        _controlLayout->addWidget(_invert);
        QObject::connect(_invert, SIGNAL(clicked()), this, SLOT(invertSlot()));
        _zoomP = new QPushButton();
        _zoomP->setIcon(QIcon("rsc/zoomPlus.png"));
        _zoomP->setIconSize(QSize(150,150));
        _zoomP->setMaximumWidth(150);
        _controlLayout->addWidget(_zoomP);
        QObject::connect(_zoomP, SIGNAL(clicked()), this, SLOT(zoomPSlotl()));
        _zoomM = new QPushButton();
        _zoomM->setIcon(QIcon("rsc/zoomMoins.png"));
        _zoomM->setIconSize(QSize(150,150));
        _zoomM->setMaximumWidth(150);
        _controlLayout->addWidget(_zoomM);
        QObject::connect(_zoomM, SIGNAL(clicked()), this, SLOT(zoomMSlot()));
        _ocr = new QPushButton("");
        _ocr->setIcon(QIcon("rsc/ocr.png"));
        _ocr->setIconSize(QSize(150,150));
        _ocr->setMaximumWidth(150);
        _controlLayout->addWidget(_ocr);
        QObject::connect(_ocr, SIGNAL(clicked()), this, SLOT(ocrSlot()));
        _save = new QPushButton();
        _save->setIcon(QIcon("rsc/save.png"));
        _save->setIconSize(QSize(150,150));
        _save->setMaximumWidth(150);
        _controlLayout->addWidget(_save);
        QObject::connect(_save, SIGNAL(clicked()), this, SLOT(saveSlot()));

    _mainLayout->addWidget(imagelabel);
    _mainLayout->addLayout(_controlLayout);
    setLayout(_mainLayout);
    setStyleSheet("QPushButton {background:transparent;}");
}

QOpenCVWidget::~QOpenCVWidget(void) 
{}

void QOpenCVWidget::resizeEvent(QResizeEvent *event)
{
    int w = this->height()/5;

    _invert->setIconSize(QSize(w,w));
    _zoomM->setIconSize(QSize(w,w));
    _zoomP->setIconSize(QSize(w,w));
    _ocr->setIconSize(QSize(w,w));
    _save->setIconSize(QSize(w,w));
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

//When we click on buttons
void QOpenCVWidget::invertSlot()
{
    emit invertSignal();
}

void QOpenCVWidget::zoomPSlotl()
{
    emit zoomPSignal();
}

void QOpenCVWidget::zoomMSlot()
{
    emit zoomMSignal();
}

void QOpenCVWidget::saveSlot()
{
    emit saveSignal();
}

void QOpenCVWidget::ocrSlot()
{
    emit ocrSignal();
}
