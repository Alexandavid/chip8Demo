#include "GraphicsDrivers.h"

#include <QPainter>

QtWindow::QtWindow(QString &title) {
    setWindowTitle(title);
}

QtRenderer::QtRenderer(QWidget *parent) : QWidget(parent), color(Qt::black) {

}

void QtRenderer::setDrawColor(QColor color) {
    this->color = color;
}

void QtRenderer::drawPixel(int x, int y, int width, int height) {
    pixels.append(QRect(x, y, width, height));
    update();
}

void QtRenderer::clear() {
    QPainter painter(this);
    painter.setBrush(QColor(0, 0, 0, 255));
    painter.setPen(Qt::NoPen);
    painter.drawRect(0, 0, width(), height());
}

void QtRenderer::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    painter.setBrush(Qt::black);
    painter.setPen(Qt::NoPen);
    painter.drawRect(0, 0, width(), height());

    painter.setPen(color);
    painter.setBrush(color);

    for (const QRect &pos : pixels) {
        painter.drawRect(pos);
    }

    pixels.clear();
}
