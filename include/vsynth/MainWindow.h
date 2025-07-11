#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSlider>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QSpinBox>
#include <QGroupBox>
#include <QProgressBar>
#include <QTimer>
#include "AudioEngine.h"
#include "KeyboardWidget.h"
#include "FFTAnalyzer.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAttackChanged(int value);
    void onDecayChanged(int value);
    void onSustainChanged(int value);
    void onReleaseChanged(int value);
    void onWaveformChanged(int index);
    void onOscillatorCountChanged(int count);
    void onVibratoRateChanged(int value);
    void onVibratoDepthChanged(int value);
    void onReverbChanged(int value);
    void onDelayChanged(int value);
    void onRecordToggled();
    void onPlayToggled();
    void onExportClicked();
    void updateFFTDisplay();

private:
    void setupUI();
    void setupADSRControls(QGroupBox* parent);
    void setupOscillatorControls(QGroupBox* parent);
    void setupEffectsControls(QGroupBox* parent);
    void setupRecordingControls(QGroupBox* parent);

    // UI Components
    QWidget* m_centralWidget;
    QVBoxLayout* m_mainLayout;
    QHBoxLayout* m_topLayout;
    QHBoxLayout* m_bottomLayout;
    
    // Control groups
    QGroupBox* m_adsrGroup;
    QGroupBox* m_oscillatorGroup;
    QGroupBox* m_effectsGroup;
    QGroupBox* m_recordingGroup;
    QGroupBox* m_fftGroup;
    
    // ADSR controls
    QSlider* m_attackSlider;
    QSlider* m_decaySlider;
    QSlider* m_sustainSlider;
    QSlider* m_releaseSlider;
    QLabel* m_attackLabel;
    QLabel* m_decayLabel;
    QLabel* m_sustainLabel;
    QLabel* m_releaseLabel;
    
    // Oscillator controls
    QComboBox* m_waveformCombo;
    QSpinBox* m_oscillatorCountSpin;
    QSlider* m_vibratoRateSlider;
    QSlider* m_vibratoDepthSlider;
    QLabel* m_vibratoRateLabel;
    QLabel* m_vibratoDepthLabel;
    
    // Effects controls
    QSlider* m_reverbSlider;
    QSlider* m_delaySlider;
    QLabel* m_reverbLabel;
    QLabel* m_delayLabel;
    
    // Recording controls
    QPushButton* m_recordButton;
    QPushButton* m_playButton;
    QPushButton* m_exportButton;
    
    // Keyboard and visualization
    KeyboardWidget* m_keyboard;
    QProgressBar* m_fftDisplay[32]; // Simple FFT visualization
    
    // Core components
    AudioEngine* m_audioEngine;
    FFTAnalyzer* m_fftAnalyzer;
    QTimer* m_fftTimer;
};

#endif // MAINWINDOW_H
