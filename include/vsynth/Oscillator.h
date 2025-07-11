#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include <cmath>

enum class WaveformType {
    SINE = 0,
    SQUARE,
    SAWTOOTH,
    TRIANGLE,
    NOISE
};

class Oscillator
{
public:
    Oscillator(float frequency, int sampleRate);
    ~Oscillator() = default;
    
    float process();
    
    void setFrequency(float frequency);
    void setWaveform(WaveformType waveform);
    void setAmplitude(float amplitude);
    void setPhase(float phase);
    
    float getFrequency() const { return m_frequency; }
    WaveformType getWaveform() const { return m_waveform; }
    float getAmplitude() const { return m_amplitude; }
    
private:
    float generateSine();
    float generateSquare();
    float generateSawtooth();
    float generateTriangle();
    float generateNoise();
    
    float m_frequency;
    float m_amplitude;
    float m_phase;
    float m_phaseIncrement;
    int m_sampleRate;
    WaveformType m_waveform;
    
    // For noise generation
    float m_lastNoise;
    
    static constexpr float TWO_PI = 2.0f * M_PI;
};

#endif // OSCILLATOR_H
