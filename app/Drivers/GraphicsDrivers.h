#ifndef HL_SDL_H
#define HL_SDL_H

#include <QMainWindow>

class QtWindow : public QMainWindow {
    public:
        QtWindow(QString &title);
};

class QtRenderer : public QWidget {
    public:
    QtRenderer(QWidget* parent = nullptr);
    void setDrawColor(QColor color);
    void drawPixel(int x, int y, int width, int height);
    void clear();

    protected:
    void paintEvent(QPaintEvent *event) override;

    private:
    QColor color;
    QVector<QRect> pixels;
};


#endif //HL_SDL_H
