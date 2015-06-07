#include "opencvwidget.h"

// Constructor
QOpenCVWidget::QOpenCVWidget(QWidget *parent) : QWidget(parent) {
    //setFixedSize(300, 300);
    layout = new QVBoxLayout;
    imagelabel = new QLabel;
    layout->addWidget(imagelabel);
    setLayout(layout);
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
