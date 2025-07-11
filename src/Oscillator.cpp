#include "vsynth/Oscillator.h"
#include <random>

Oscillator::Oscillator(float frequency, int sampleRate)
    : m_frequency(frequency)
    , m_amplitude(1.0f)
    , m_phase(0.0f)
    , m_sampleRate(sampleRate)
    , m_waveform(WaveformType::SINE)
    , m_lastNoise(0.0f)
{
    m_phaseIncrement = (TWO_PI * m_frequency) / static_cast<float>(m_sampleRate);
}

float Oscillator::process()
{
    float output = 0.0f;
    
    switch (m_waveform) {
        case WaveformType::SINE:
            output = generateSine();
            break;
        case WaveformType::SQUARE:
            output = generateSquare();
            break;
        case WaveformType::SAWTOOTH:
            output = generateSawtooth();
            break;
        case WaveformType::TRIANGLE:
            output = generateTriangle();
            break;
        case WaveformType::NOISE:
            output = generateNoise();
            break;
    }
    
    // Advance phase
    m_phase += m_phaseIncrement;
    if (m_phase >= TWO_PI) {
        m_phase -= TWO_PI;
    }
    
    return output * m_amplitude;
}

void Oscillator::setFrequency(float frequency)
{
    m_frequency = frequency;
    m_phaseIncrement = (TWO_PI * m_frequency) / static_cast<float>(m_sampleRate);
}

void Oscillator::setWaveform(WaveformType waveform)
{
    m_waveform = waveform;
}

void Oscillator::setAmplitude(float amplitude)
{
    m_amplitude = amplitude;
}

void Oscillator::setPhase(float phase)
{
    m_phase = phase;
}

float Oscillator::generateSine()
{
    return std::sin(m_phase);
}

float Oscillator::generateSquare()
{
    return (m_phase < M_PI) ? 1.0f : -1.0f;
}

float Oscillator::generateSawtooth()
{
    return (2.0f * m_phase / TWO_PI) - 1.0f;
}

float Oscillator::generateTriangle()
{
    if (m_phase < M_PI) {
        return (2.0f * m_phase / M_PI) - 1.0f;
    } else {
        return 3.0f - (2.0f * m_phase / M_PI);
    }
}

float Oscillator::generateNoise()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
    
    // Simple low-pass filtered noise
    float noise = dis(gen);
    m_lastNoise = m_lastNoise * 0.99f + noise * 0.01f;
    return m_lastNoise;
}
