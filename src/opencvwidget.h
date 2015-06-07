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
 * @brief The QOpenCVWidget class: The Interface
 */
class QOpenCVWidget : public QWidget {
    Q_OBJECT
private:
    QLabel *imagelabel;
    QHBoxLayout *_mainLayout;
    QVBoxLayout *_controlLayout;

    QPushButton *_invert;
    QPushButton *_balance;
    QPushButton *_zoomP;
    QPushButton *_zoomM;
    QPushButton *_saturationP;
    QPushButton *_saturationM;
    QPushButton *_save;

    QImage image;

public:
    QOpenCVWidget(QWidget *parent = 0);
    ~QOpenCVWidget(void);
    void putImage(IplImage *);
    QImage getImageLabel();


public slots:
    void invertSlot();
    void balanceSlot();
    void zoomPSlotl();
    void zoomMSlot();
    void saturationPSlot();
    void saturationMSlot();
    void saveSlot();

signals:
    void invertSignal();
    void balanceSignal();
    void zoomPSignal();
    void zoomMSignal();
    void saturationPSignal();
    void saturationMSignal();
    void saveSignal();
};

#endif
