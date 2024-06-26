#ifndef LCDWIDGET_H
#define LCDWIDGET_H

#include <QtWidgets/QWidget>
#include <QtCore/QTimer>
#include <QtCore/QMutex>
#include <chrono>

#include "../../core/lcd.h"

class LCDWidget : public QWidget {
  Q_OBJECT

public:
    enum UpscaleMethod {
        NearestNeighbor,
        Bilinear,
        SharpBilinear
    };

    enum FullscreenArea {
        StretchToFill,
        PreserveAspectRatio,
        IntegerUpscale
    };

    explicit LCDWidget(QWidget *p = Q_NULLPTR);
    QImage getImage();
    void setMain();
    bool draw();
    void setFrameskip(int value);
    void setResponseMode(bool state);
    void setUpscaleMethod(int method);
    void setFullscreenArea(int area);
    void disableBlend();

signals:
    void updateLcd(double emuFps);

public slots:
    double refresh();

protected:
    virtual void paintEvent(QPaintEvent*) Q_DECL_OVERRIDE;
    virtual void dropEvent(QDropEvent*) Q_DECL_OVERRIDE;
    virtual void dragEnterEvent(QDragEnterEvent*) Q_DECL_OVERRIDE;
    virtual void dragLeaveEvent(QDragLeaveEvent*) Q_DECL_OVERRIDE;
    virtual void dragMoveEvent(QDragMoveEvent*) Q_DECL_OVERRIDE;
    virtual void mousePressEvent(QMouseEvent*) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QMouseEvent*) Q_DECL_OVERRIDE;

signals:
    void sendROM(const QString& romPath);

private:
    enum {
        LcdLeft = 0,
        LcdRight
    };

    static constexpr size_t PerfArraySize = 30;

    unsigned int m_side;
    bool m_transferDrag = false;
    bool m_screenshotDrag = false;
    QRect m_left, m_right;
    QImage m_interlaceAlpha;
    QImage *m_currFrame;
    QImage m_renderedFrame;
    QImage m_blendedFrame;
    QMutex m_mutex;

    // for dragable roms
    QString m_dragRom;
    bool m_isSendingRom;

    std::chrono::steady_clock::time_point m_perfArray[PerfArraySize];
    size_t m_perfIndex = 0;
    bool m_responseMode = false;
    int m_skip = 0;
    int m_frameskip = 0;

    enum UpscaleMethod m_upscale = SharpBilinear;
    enum FullscreenArea m_fullscreenArea = PreserveAspectRatio;
};

#endif
