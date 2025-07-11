#include "vsynth/KeyboardWidget.h"
#include <QResizeEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QBrush>
#include <QPen>

const char* KeyboardWidget::KEYBOARD_KEYS = "awsedftgyhujkolp;'";

KeyboardWidget::KeyboardWidget(QWidget *parent)
    : QWidget(parent)
    , m_octaves(2)
    , m_startOctave(4)
    , m_whiteKeyWidth(40)
    , m_whiteKeyHeight(120)
    , m_blackKeyWidth(25)
    , m_blackKeyHeight(80)
    , m_whiteKeyColor(Qt::white)
    , m_blackKeyColor(Qt::black)
    , m_pressedWhiteKeyColor(Qt::lightGray)
    , m_pressedBlackKeyColor(Qt::darkGray)
    , m_borderColor(Qt::black)
{
    setFocusPolicy(Qt::StrongFocus);
    setupKeys();
}

void KeyboardWidget::setOctaves(int octaves)
{
    m_octaves = octaves;
    setupKeys();
    update();
}

void KeyboardWidget::setStartOctave(int startOctave)
{
    m_startOctave = startOctave;
    setupKeys();
    update();
}

void KeyboardWidget::setupKeys()
{
    m_keys.clear();
    
    // Pattern for one octave: C, C#, D, D#, E, F, F#, G, G#, A, A#, B
    bool blackKeyPattern[] = {false, true, false, true, false, false, true, false, true, false, true, false};
    
    int whiteKeyIndex = 0;
    
    for (int octave = 0; octave < m_octaves; ++octave) {
        for (int note = 0; note < 12; ++note) {
            int midiNote = (m_startOctave + octave) * 12 + note;
            bool isBlack = blackKeyPattern[note];
            
            QRect keyRect;
            
            if (isBlack) {
                // Black key positioning
                int whiteKeyPos = whiteKeyIndex * m_whiteKeyWidth;
                keyRect = QRect(whiteKeyPos - m_blackKeyWidth/2, 0, m_blackKeyWidth, m_blackKeyHeight);
            } else {
                // White key positioning
                keyRect = QRect(whiteKeyIndex * m_whiteKeyWidth, 0, m_whiteKeyWidth, m_whiteKeyHeight);
                whiteKeyIndex++;
            }
            
            m_keys.emplace_back(midiNote, keyRect, isBlack);
        }
    }
    
    // Update widget size
    int totalWidth = whiteKeyIndex * m_whiteKeyWidth;
    setMinimumSize(totalWidth, m_whiteKeyHeight);
}

void KeyboardWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Draw white keys first
    for (const auto& key : m_keys) {
        if (!key.isBlack) {
            drawKey(painter, key);
        }
    }
    
    // Draw black keys on top
    for (const auto& key : m_keys) {
        if (key.isBlack) {
            drawKey(painter, key);
        }
    }
}

void KeyboardWidget::drawKey(QPainter& painter, const Key& key)
{
    QColor fillColor;
    
    if (key.isPressed) {
        fillColor = key.isBlack ? m_pressedBlackKeyColor : m_pressedWhiteKeyColor;
    } else {
        fillColor = key.isBlack ? m_blackKeyColor : m_whiteKeyColor;
    }
    
    painter.setBrush(QBrush(fillColor));
    painter.setPen(QPen(m_borderColor, 1));
    painter.drawRect(key.rect);
    
    // Draw note name for white keys
    if (!key.isBlack && !key.isPressed) {
        painter.setPen(QPen(Qt::black));
        QString noteName = QString("C%1").arg((key.note / 12) - 1);
        if (key.note % 12 == 0) { // C note
            painter.drawText(key.rect, Qt::AlignBottom | Qt::AlignHCenter, noteName);
        }
    }
}

void KeyboardWidget::mousePressEvent(QMouseEvent *event)
{
    int keyIndex = findKeyAtPosition(event->pos());
    if (keyIndex >= 0) {
        pressNote(m_keys[keyIndex].note);
    }
}

void KeyboardWidget::mouseReleaseEvent(QMouseEvent *event)
{
    // Release all pressed notes from mouse
    for (auto& key : m_keys) {
        if (key.isPressed && m_pressedNotes.count(key.note)) {
            releaseNote(key.note);
        }
    }
}

void KeyboardWidget::mouseMoveEvent(QMouseEvent *event)
{
    // Handle mouse drag
    int keyIndex = findKeyAtPosition(event->pos());
    
    // Release notes that are no longer under the mouse
    for (auto& key : m_keys) {
        if (key.isPressed && (keyIndex < 0 || m_keys[keyIndex].note != key.note)) {
            if (m_pressedNotes.count(key.note)) {
                releaseNote(key.note);
            }
        }
    }
    
    // Press note under mouse if not already pressed
    if (keyIndex >= 0) {
        int note = m_keys[keyIndex].note;
        if (!m_pressedNotes.count(note)) {
            pressNote(note);
        }
    }
}

void KeyboardWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) return;
    
    int note = getKeyboardNoteFromKey(event->key());
    if (note >= 0 && !m_pressedKeyboardKeys.count(event->key())) {
        m_pressedKeyboardKeys.insert(event->key());
        pressNote(note);
    }
}

void KeyboardWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) return;
    
    int note = getKeyboardNoteFromKey(event->key());
    if (note >= 0 && m_pressedKeyboardKeys.count(event->key())) {
        m_pressedKeyboardKeys.erase(event->key());
        releaseNote(note);
    }
}

void KeyboardWidget::resizeEvent(QResizeEvent *event)
{
    // Recalculate key sizes based on new widget size
    if (!m_keys.empty()) {
        int whiteKeyCount = 0;
        for (const auto& key : m_keys) {
            if (!key.isBlack) whiteKeyCount++;
        }
        
        if (whiteKeyCount > 0) {
            m_whiteKeyWidth = width() / whiteKeyCount;
            m_blackKeyWidth = m_whiteKeyWidth * 0.6f;
            m_whiteKeyHeight = height();
            m_blackKeyHeight = height() * 0.67f;
            
            setupKeys();
        }
    }
    
    QWidget::resizeEvent(event);
}

int KeyboardWidget::findKeyAtPosition(const QPoint& pos)
{
    // Check black keys first (they're on top)
    for (int i = 0; i < static_cast<int>(m_keys.size()); ++i) {
        if (m_keys[i].isBlack && m_keys[i].rect.contains(pos)) {
            return i;
        }
    }
    
    // Then check white keys
    for (int i = 0; i < static_cast<int>(m_keys.size()); ++i) {
        if (!m_keys[i].isBlack && m_keys[i].rect.contains(pos)) {
            return i;
        }
    }
    
    return -1;
}

int KeyboardWidget::getKeyboardNoteFromKey(int key)
{
    const char* keyStr = KEYBOARD_KEYS;
    int keyIndex = -1;
    
    // Convert Qt key to character
    char ch = static_cast<char>(key);
    if (ch >= 'A' && ch <= 'Z') {
        ch = ch - 'A' + 'a'; // Convert to lowercase
    }
    
    // Find key in mapping
    for (int i = 0; keyStr[i] != '\0'; ++i) {
        if (keyStr[i] == ch) {
            keyIndex = i;
            break;
        }
    }
    
    if (keyIndex >= 0) {
        // Map to MIDI note (starting from C4)
        int baseNote = m_startOctave * 12; // C of start octave
        int noteOffsets[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17}; // C to F of next octave
        
        if (keyIndex < 18) {
            return baseNote + noteOffsets[keyIndex];
        }
    }
    
    return -1;
}

void KeyboardWidget::pressNote(int note, float velocity)
{
    if (!m_pressedNotes.count(note)) {
        m_pressedNotes.insert(note);
        
        // Update key visual state
        for (auto& key : m_keys) {
            if (key.note == note) {
                key.isPressed = true;
                break;
            }
        }
        
        emit notePressed(note, velocity);
        update();
    }
}

void KeyboardWidget::releaseNote(int note)
{
    if (m_pressedNotes.count(note)) {
        m_pressedNotes.erase(note);
        
        // Update key visual state
        for (auto& key : m_keys) {
            if (key.note == note) {
                key.isPressed = false;
                break;
            }
        }
        
        emit noteReleased(note);
        update();
    }
}
