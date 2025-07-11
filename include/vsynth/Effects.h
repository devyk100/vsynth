#ifndef EFFECTS_H
#define EFFECTS_H

#include <vector>
#include <memory>

class DelayEffect
{
public:
    DelayEffect(int sampleRate, float maxDelayTime = 1.0f);
    ~DelayEffect() = default;
    
    float process(float input);
    void setDelayTime(float delayTime);
    void setFeedback(float feedback);
    void setMix(float mix);
    
private:
    std::vector<float> m_delayBuffer;
    int m_bufferSize;
    int m_writeIndex;
    int m_readIndex;
    float m_delayTime;
    float m_feedback;
    float m_mix;
    int m_sampleRate;
};

class ReverbEffect
{
public:
    ReverbEffect(int sampleRate);
    ~ReverbEffect() = default;
    
    float process(float input);
    void setRoomSize(float roomSize);
    void setDamping(float damping);
    void setMix(float mix);
    
private:
    // Simple Schroeder reverb implementation
    std::vector<std::vector<float>> m_combBuffers;
    std::vector<std::vector<float>> m_allpassBuffers;
    std::vector<int> m_combIndices;
    std::vector<int> m_allpassIndices;
    std::vector<float> m_combFeedback;
    std::vector<float> m_allpassFeedback;
    
    float m_roomSize;
    float m_damping;
    float m_mix;
    int m_sampleRate;
    
    static const int NUM_COMBS = 4;
    static const int NUM_ALLPASS = 2;
};

class Effects
{
public:
    Effects(int sampleRate);
    ~Effects() = default;
    
    float process(float input);
    
    void setReverbAmount(float amount);
    void setDelayAmount(float amount);
    void setDelayTime(float time);
    void setDelayFeedback(float feedback);
    
private:
    std::unique_ptr<DelayEffect> m_delay;
    std::unique_ptr<ReverbEffect> m_reverb;
    int m_sampleRate;
};

#endif // EFFECTS_H
