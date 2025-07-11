#ifndef KEYBOARDWIDGET_H
#define KEYBOARDWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QRect>
#include <vector>
#include <set>

struct Key {
    int note;
    QRect rect;
    bool isBlack;
    bool isPressed;
    
    Key(int n, const QRect& r, bool black) 
        : note(n), rect(r), isBlack(black), isPressed(false) {}
};

class KeyboardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit KeyboardWidget(QWidget *parent = nullptr);
    ~KeyboardWidget() = default;
    
    void setOctaves(int octaves);
    void setStartOctave(int startOctave);
    
signals:
    void notePressed(int note, float velocity);
    void noteReleased(int note);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void setupKeys();
    void drawKey(QPainter& painter, const Key& key);
    int findKeyAtPosition(const QPoint& pos);
    int getKeyboardNoteFromKey(int key);
    void pressNote(int note, float velocity = 0.8f);
    void releaseNote(int note);
    
    std::vector<Key> m_keys;
    std::set<int> m_pressedNotes;
    std::set<int> m_pressedKeyboardKeys;
    
    int m_octaves;
    int m_startOctave;
    int m_whiteKeyWidth;
    int m_whiteKeyHeight;
    int m_blackKeyWidth;
    int m_blackKeyHeight;
    
    // Colors
    QColor m_whiteKeyColor;
    QColor m_blackKeyColor;
    QColor m_pressedWhiteKeyColor;
    QColor m_pressedBlackKeyColor;
    QColor m_borderColor;
    
    // Computer keyboard mapping
    static const char* KEYBOARD_KEYS;
};

#endif // KEYBOARDWIDGET_H
