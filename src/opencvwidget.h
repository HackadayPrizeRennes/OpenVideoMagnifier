#ifndef QOPENCVWIDGET_H
#define QOPENCVWIDGET_H

#include <opencv/cv.h>
#include <QPixmap>
#include <QLabel>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QImage>
#include <QPushButton>

/**
 * @brief The QOpenCVWidget class: The GUI
 */
class QOpenCVWidget : public QWidget {
    Q_OBJECT
private:
    QLabel *imagelabel;
    QHBoxLayout *_mainLayout;
    QVBoxLayout *_controlLayout;

    QPushButton *_invert;
    QPushButton *_zoomP;
    QPushButton *_zoomM;
    QPushButton *_ocr;
    QPushButton *_save;

    QImage image;

public:
    QOpenCVWidget(QWidget *parent = 0);
    ~QOpenCVWidget(void);
    void putImage(IplImage *);
    QImage getImageLabel();

protected:
    void resizeEvent(QResizeEvent* event);

public slots:
    //When we click on buttons
    void invertSlot();
    void zoomPSlotl();
    void zoomMSlot();
    void ocrSlot();
    void saveSlot();

signals:
    void invertSignal();
    void zoomPSignal();
    void zoomMSignal();
    void ocrSignal();
    void saveSignal();
};

#endif
