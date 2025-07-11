#ifndef ADSRENVELOPE_H
#define ADSRENVELOPE_H

enum class EnvelopeState {
    IDLE,
    ATTACK,
    DECAY,
    SUSTAIN,
    RELEASE
};

class ADSREnvelope
{
public:
    ADSREnvelope(int sampleRate);
    ~ADSREnvelope() = default;
    
    void trigger();
    void release();
    float process();
    
    void setAttack(float attack);
    void setDecay(float decay);
    void setSustain(float sustain);
    void setRelease(float release);
    
    bool isActive() const;
    EnvelopeState getState() const { return m_state; }
    
private:
    void calculateRates();
    
    int m_sampleRate;
    float m_deltaTime;
    
    // ADSR parameters (in seconds)
    float m_attack;
    float m_decay;
    float m_sustain;  // Level (0.0 to 1.0)
    float m_release;
    
    // Internal state
    EnvelopeState m_state;
    float m_currentLevel;
    float m_targetLevel;
    float m_rate;
    
    // Calculated rates (per sample)
    float m_attackRate;
    float m_decayRate;
    float m_releaseRate;
};

#endif // ADSRENVELOPE_H
