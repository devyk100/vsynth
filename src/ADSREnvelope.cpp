#include "vsynth/ADSREnvelope.h"
#include <algorithm>

ADSREnvelope::ADSREnvelope(int sampleRate)
    : m_sampleRate(sampleRate)
    , m_deltaTime(1.0f / static_cast<float>(sampleRate))
    , m_attack(0.1f)
    , m_decay(0.2f)
    , m_sustain(0.7f)
    , m_release(0.5f)
    , m_state(EnvelopeState::IDLE)
    , m_currentLevel(0.0f)
    , m_targetLevel(0.0f)
    , m_rate(0.0f)
{
    calculateRates();
}

void ADSREnvelope::trigger()
{
    m_state = EnvelopeState::ATTACK;
    m_targetLevel = 1.0f;
    m_rate = m_attackRate;
}

void ADSREnvelope::release()
{
    if (m_state != EnvelopeState::IDLE) {
        m_state = EnvelopeState::RELEASE;
        m_targetLevel = 0.0f;
        m_rate = m_releaseRate;
    }
}

float ADSREnvelope::process()
{
    switch (m_state) {
        case EnvelopeState::IDLE:
            m_currentLevel = 0.0f;
            break;
            
        case EnvelopeState::ATTACK:
            m_currentLevel += m_rate;
            if (m_currentLevel >= m_targetLevel) {
                m_currentLevel = m_targetLevel;
                m_state = EnvelopeState::DECAY;
                m_targetLevel = m_sustain;
                m_rate = -m_decayRate;
            }
            break;
            
        case EnvelopeState::DECAY:
            m_currentLevel += m_rate;
            if (m_currentLevel <= m_targetLevel) {
                m_currentLevel = m_targetLevel;
                m_state = EnvelopeState::SUSTAIN;
                m_rate = 0.0f;
            }
            break;
            
        case EnvelopeState::SUSTAIN:
            m_currentLevel = m_sustain;
            break;
            
        case EnvelopeState::RELEASE:
            m_currentLevel += m_rate;
            if (m_currentLevel <= 0.0f) {
                m_currentLevel = 0.0f;
                m_state = EnvelopeState::IDLE;
                m_rate = 0.0f;
            }
            break;
    }
    
    return std::max(0.0f, std::min(1.0f, m_currentLevel));
}

void ADSREnvelope::setAttack(float attack)
{
    m_attack = std::max(0.001f, attack);
    calculateRates();
}

void ADSREnvelope::setDecay(float decay)
{
    m_decay = std::max(0.001f, decay);
    calculateRates();
}

void ADSREnvelope::setSustain(float sustain)
{
    m_sustain = std::max(0.0f, std::min(1.0f, sustain));
}

void ADSREnvelope::setRelease(float release)
{
    m_release = std::max(0.001f, release);
    calculateRates();
}

bool ADSREnvelope::isActive() const
{
    return m_state != EnvelopeState::IDLE;
}

void ADSREnvelope::calculateRates()
{
    m_attackRate = 1.0f / (m_attack * static_cast<float>(m_sampleRate));
    m_decayRate = (1.0f - m_sustain) / (m_decay * static_cast<float>(m_sampleRate));
    m_releaseRate = -m_sustain / (m_release * static_cast<float>(m_sampleRate));
}
