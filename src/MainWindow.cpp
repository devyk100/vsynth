#include "vsynth/MainWindow.h"
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_centralWidget(nullptr)
    , m_mainLayout(nullptr)
    , m_topLayout(nullptr)
    , m_bottomLayout(nullptr)
    , m_audioEngine(nullptr)
    , m_fftAnalyzer(nullptr)
    , m_fftTimer(nullptr)
{
    setupUI();
    
    // Initialize audio engine
    m_audioEngine = new AudioEngine();
    if (!m_audioEngine->initialize()) {
        QMessageBox::critical(this, "Error", "Failed to initialize audio engine");
        return;
    }
    
    // Initialize FFT analyzer
    m_fftAnalyzer = new FFTAnalyzer();
    
    // Setup FFT update timer
    m_fftTimer = new QTimer(this);
    connect(m_fftTimer, &QTimer::timeout, this, &MainWindow::updateFFTDisplay);
    m_fftTimer->start(50); // Update at 20 FPS
    
    // Start audio engine
    m_audioEngine->start();
}

MainWindow::~MainWindow()
{
    if (m_audioEngine) {
        m_audioEngine->stop();
        delete m_audioEngine;
    }
    
    if (m_fftAnalyzer) {
        delete m_fftAnalyzer;
    }
}

void MainWindow::setupUI()
{
    m_centralWidget = new QWidget();
    setCentralWidget(m_centralWidget);
    
    m_mainLayout = new QVBoxLayout(m_centralWidget);
    m_topLayout = new QHBoxLayout();
    m_bottomLayout = new QHBoxLayout();
    
    // Create control groups
    m_adsrGroup = new QGroupBox("ADSR Envelope");
    m_oscillatorGroup = new QGroupBox("Oscillators");
    m_effectsGroup = new QGroupBox("Effects");
    m_recordingGroup = new QGroupBox("Recording");
    m_fftGroup = new QGroupBox("Frequency Analysis");
    
    setupADSRControls(m_adsrGroup);
    setupOscillatorControls(m_oscillatorGroup);
    setupEffectsControls(m_effectsGroup);
    setupRecordingControls(m_recordingGroup);
    
    // Setup FFT display
    QVBoxLayout* fftLayout = new QVBoxLayout(m_fftGroup);
    QHBoxLayout* barsLayout = new QHBoxLayout();
    
    for (int i = 0; i < 32; ++i) {
        m_fftDisplay[i] = new QProgressBar();
        m_fftDisplay[i]->setOrientation(Qt::Vertical);
        m_fftDisplay[i]->setRange(0, 100);
        m_fftDisplay[i]->setValue(0);
        m_fftDisplay[i]->setMaximumWidth(15);
        barsLayout->addWidget(m_fftDisplay[i]);
    }
    
    fftLayout->addLayout(barsLayout);
    
    // Create keyboard
    m_keyboard = new KeyboardWidget();
    connect(m_keyboard, &KeyboardWidget::notePressed, this, [this](int note, float velocity) {
        if (m_audioEngine) {
            m_audioEngine->noteOn(note, velocity);
        }
    });
    connect(m_keyboard, &KeyboardWidget::noteReleased, this, [this](int note) {
        if (m_audioEngine) {
            m_audioEngine->noteOff(note);
        }
    });
    
    // Layout arrangement
    m_topLayout->addWidget(m_adsrGroup);
    m_topLayout->addWidget(m_oscillatorGroup);
    m_topLayout->addWidget(m_effectsGroup);
    
    m_bottomLayout->addWidget(m_recordingGroup);
    m_bottomLayout->addWidget(m_fftGroup);
    
    m_mainLayout->addLayout(m_topLayout);
    m_mainLayout->addLayout(m_bottomLayout);
    m_mainLayout->addWidget(m_keyboard);
    
    setWindowTitle("VSynth - Simple Synthesizer");
    resize(1000, 600);
}

void MainWindow::setupADSRControls(QGroupBox* parent)
{
    QGridLayout* layout = new QGridLayout(parent);
    
    // Attack
    layout->addWidget(new QLabel("Attack:"), 0, 0);
    m_attackSlider = new QSlider(Qt::Horizontal);
    m_attackSlider->setRange(1, 5000); // 0.001 to 5.0 seconds
    m_attackSlider->setValue(100); // 0.1 seconds
    m_attackLabel = new QLabel("0.1s");
    layout->addWidget(m_attackSlider, 0, 1);
    layout->addWidget(m_attackLabel, 0, 2);
    connect(m_attackSlider, &QSlider::valueChanged, this, &MainWindow::onAttackChanged);
    
    // Decay
    layout->addWidget(new QLabel("Decay:"), 1, 0);
    m_decaySlider = new QSlider(Qt::Horizontal);
    m_decaySlider->setRange(1, 5000);
    m_decaySlider->setValue(200); // 0.2 seconds
    m_decayLabel = new QLabel("0.2s");
    layout->addWidget(m_decaySlider, 1, 1);
    layout->addWidget(m_decayLabel, 1, 2);
    connect(m_decaySlider, &QSlider::valueChanged, this, &MainWindow::onDecayChanged);
    
    // Sustain
    layout->addWidget(new QLabel("Sustain:"), 2, 0);
    m_sustainSlider = new QSlider(Qt::Horizontal);
    m_sustainSlider->setRange(0, 100);
    m_sustainSlider->setValue(70); // 0.7 level
    m_sustainLabel = new QLabel("0.7");
    layout->addWidget(m_sustainSlider, 2, 1);
    layout->addWidget(m_sustainLabel, 2, 2);
    connect(m_sustainSlider, &QSlider::valueChanged, this, &MainWindow::onSustainChanged);
    
    // Release
    layout->addWidget(new QLabel("Release:"), 3, 0);
    m_releaseSlider = new QSlider(Qt::Horizontal);
    m_releaseSlider->setRange(1, 5000);
    m_releaseSlider->setValue(500); // 0.5 seconds
    m_releaseLabel = new QLabel("0.5s");
    layout->addWidget(m_releaseSlider, 3, 1);
    layout->addWidget(m_releaseLabel, 3, 2);
    connect(m_releaseSlider, &QSlider::valueChanged, this, &MainWindow::onReleaseChanged);
}

void MainWindow::setupOscillatorControls(QGroupBox* parent)
{
    QGridLayout* layout = new QGridLayout(parent);
    
    // Waveform selection
    layout->addWidget(new QLabel("Waveform:"), 0, 0);
    m_waveformCombo = new QComboBox();
    m_waveformCombo->addItems({"Sine", "Square", "Sawtooth", "Triangle", "Noise"});
    layout->addWidget(m_waveformCombo, 0, 1);
    connect(m_waveformCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &MainWindow::onWaveformChanged);
    
    // Oscillator count
    layout->addWidget(new QLabel("Oscillators:"), 1, 0);
    m_oscillatorCountSpin = new QSpinBox();
    m_oscillatorCountSpin->setRange(1, 3);
    m_oscillatorCountSpin->setValue(2);
    layout->addWidget(m_oscillatorCountSpin, 1, 1);
    connect(m_oscillatorCountSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MainWindow::onOscillatorCountChanged);
    
    // Vibrato rate
    layout->addWidget(new QLabel("Vibrato Rate:"), 2, 0);
    m_vibratoRateSlider = new QSlider(Qt::Horizontal);
    m_vibratoRateSlider->setRange(0, 200); // 0 to 20 Hz
    m_vibratoRateSlider->setValue(50); // 5 Hz
    m_vibratoRateLabel = new QLabel("5.0 Hz");
    layout->addWidget(m_vibratoRateSlider, 2, 1);
    layout->addWidget(m_vibratoRateLabel, 2, 2);
    connect(m_vibratoRateSlider, &QSlider::valueChanged, this, &MainWindow::onVibratoRateChanged);
    
    // Vibrato depth
    layout->addWidget(new QLabel("Vibrato Depth:"), 3, 0);
    m_vibratoDepthSlider = new QSlider(Qt::Horizontal);
    m_vibratoDepthSlider->setRange(0, 100); // 0 to 0.1 (10%)
    m_vibratoDepthSlider->setValue(20); // 0.02 (2%)
    m_vibratoDepthLabel = new QLabel("2.0%");
    layout->addWidget(m_vibratoDepthSlider, 3, 1);
    layout->addWidget(m_vibratoDepthLabel, 3, 2);
    connect(m_vibratoDepthSlider, &QSlider::valueChanged, this, &MainWindow::onVibratoDepthChanged);
}

void MainWindow::setupEffectsControls(QGroupBox* parent)
{
    QGridLayout* layout = new QGridLayout(parent);
    
    // Reverb
    layout->addWidget(new QLabel("Reverb:"), 0, 0);
    m_reverbSlider = new QSlider(Qt::Horizontal);
    m_reverbSlider->setRange(0, 100);
    m_reverbSlider->setValue(30);
    m_reverbLabel = new QLabel("30%");
    layout->addWidget(m_reverbSlider, 0, 1);
    layout->addWidget(m_reverbLabel, 0, 2);
    connect(m_reverbSlider, &QSlider::valueChanged, this, &MainWindow::onReverbChanged);
    
    // Delay
    layout->addWidget(new QLabel("Delay:"), 1, 0);
    m_delaySlider = new QSlider(Qt::Horizontal);
    m_delaySlider->setRange(0, 100);
    m_delaySlider->setValue(30);
    m_delayLabel = new QLabel("30%");
    layout->addWidget(m_delaySlider, 1, 1);
    layout->addWidget(m_delayLabel, 1, 2);
    connect(m_delaySlider, &QSlider::valueChanged, this, &MainWindow::onDelayChanged);
}

void MainWindow::setupRecordingControls(QGroupBox* parent)
{
    QVBoxLayout* layout = new QVBoxLayout(parent);
    
    m_recordButton = new QPushButton("Record");
    m_recordButton->setCheckable(true);
    connect(m_recordButton, &QPushButton::toggled, this, &MainWindow::onRecordToggled);
    
    m_playButton = new QPushButton("Play");
    m_playButton->setCheckable(true);
    connect(m_playButton, &QPushButton::toggled, this, &MainWindow::onPlayToggled);
    
    m_exportButton = new QPushButton("Export...");
    connect(m_exportButton, &QPushButton::clicked, this, &MainWindow::onExportClicked);
    
    layout->addWidget(m_recordButton);
    layout->addWidget(m_playButton);
    layout->addWidget(m_exportButton);
}

// Slot implementations
void MainWindow::onAttackChanged(int value)
{
    float attack = value / 1000.0f; // Convert to seconds
    m_attackLabel->setText(QString("%1s").arg(attack, 0, 'f', 3));
    if (m_audioEngine) {
        m_audioEngine->setAttack(attack);
    }
}

void MainWindow::onDecayChanged(int value)
{
    float decay = value / 1000.0f;
    m_decayLabel->setText(QString("%1s").arg(decay, 0, 'f', 3));
    if (m_audioEngine) {
        m_audioEngine->setDecay(decay);
    }
}

void MainWindow::onSustainChanged(int value)
{
    float sustain = value / 100.0f;
    m_sustainLabel->setText(QString("%1").arg(sustain, 0, 'f', 2));
    if (m_audioEngine) {
        m_audioEngine->setSustain(sustain);
    }
}

void MainWindow::onReleaseChanged(int value)
{
    float release = value / 1000.0f;
    m_releaseLabel->setText(QString("%1s").arg(release, 0, 'f', 3));
    if (m_audioEngine) {
        m_audioEngine->setRelease(release);
    }
}

void MainWindow::onWaveformChanged(int index)
{
    if (m_audioEngine) {
        m_audioEngine->setWaveform(index);
    }
}

void MainWindow::onOscillatorCountChanged(int count)
{
    if (m_audioEngine) {
        m_audioEngine->setOscillatorCount(count);
    }
}

void MainWindow::onVibratoRateChanged(int value)
{
    float rate = value / 10.0f; // 0 to 20 Hz
    m_vibratoRateLabel->setText(QString("%1 Hz").arg(rate, 0, 'f', 1));
    if (m_audioEngine) {
        m_audioEngine->setVibratoRate(rate);
    }
}

void MainWindow::onVibratoDepthChanged(int value)
{
    float depth = value / 1000.0f; // 0 to 0.1
    m_vibratoDepthLabel->setText(QString("%1%").arg(depth * 100, 0, 'f', 1));
    if (m_audioEngine) {
        m_audioEngine->setVibratoDepth(depth);
    }
}

void MainWindow::onReverbChanged(int value)
{
    float reverb = value / 100.0f;
    m_reverbLabel->setText(QString("%1%").arg(value));
    if (m_audioEngine) {
        m_audioEngine->setReverb(reverb);
    }
}

void MainWindow::onDelayChanged(int value)
{
    float delay = value / 100.0f;
    m_delayLabel->setText(QString("%1%").arg(value));
    if (m_audioEngine) {
        m_audioEngine->setDelay(delay);
    }
}

void MainWindow::onRecordToggled()
{
    if (m_recordButton->isChecked()) {
        if (m_audioEngine) {
            m_audioEngine->startRecording();
        }
        m_recordButton->setText("Stop Recording");
    } else {
        if (m_audioEngine) {
            m_audioEngine->stopRecording();
        }
        m_recordButton->setText("Record");
    }
}

void MainWindow::onPlayToggled()
{
    if (m_playButton->isChecked()) {
        if (m_audioEngine) {
            m_audioEngine->startPlayback();
        }
        m_playButton->setText("Stop Playback");
    } else {
        if (m_audioEngine) {
            m_audioEngine->stopPlayback();
        }
        m_playButton->setText("Play");
    }
}

void MainWindow::onExportClicked()
{
    QString filename = QFileDialog::getSaveFileName(this, 
        "Export Audio", 
        "recording.wav",
        "WAV Files (*.wav);;MIDI Files (*.mid);;Note Events (*.txt)");
    
    if (!filename.isEmpty() && m_audioEngine) {
        m_audioEngine->exportToFile(filename.toStdString());
        QMessageBox::information(this, "Export", "File exported successfully!");
    }
}

void MainWindow::updateFFTDisplay()
{
    if (!m_audioEngine || !m_fftAnalyzer) return;
    
    auto fftData = m_audioEngine->getFFTData();
    if (fftData.size() >= 1024) {
        m_fftAnalyzer->processBuffer(fftData);
        auto spectrum = m_fftAnalyzer->getMagnitudeSpectrum();
        
        // Update progress bars (logarithmic frequency spacing)
        for (int i = 0; i < 32; ++i) {
            int binIndex = static_cast<int>(std::pow(2.0, i * 9.0 / 32.0)); // Logarithmic mapping
            if (binIndex < static_cast<int>(spectrum.size())) {
                int value = static_cast<int>(spectrum[binIndex] * 100);
                m_fftDisplay[i]->setValue(value);
            }
        }
    }
}
