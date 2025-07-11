#include "vsynth/Effects.h"
#include <algorithm>
#include <cmath>

// DelayEffect Implementation
DelayEffect::DelayEffect(int sampleRate, float maxDelayTime)
    : m_sampleRate(sampleRate)
    , m_delayTime(0.3f)
    , m_feedback(0.3f)
    , m_mix(0.3f)
    , m_writeIndex(0)
    , m_readIndex(0)
{
    m_bufferSize = static_cast<int>(maxDelayTime * sampleRate);
    m_delayBuffer.resize(m_bufferSize, 0.0f);
}

float DelayEffect::process(float input)
{
    // Calculate read index based on delay time
    int delaySamples = static_cast<int>(m_delayTime * m_sampleRate);
    m_readIndex = m_writeIndex - delaySamples;
    if (m_readIndex < 0) {
        m_readIndex += m_bufferSize;
    }
    
    // Read delayed sample
    float delayedSample = m_delayBuffer[m_readIndex];
    
    // Write new sample with feedback
    m_delayBuffer[m_writeIndex] = input + (delayedSample * m_feedback);
    
    // Advance write index
    m_writeIndex = (m_writeIndex + 1) % m_bufferSize;
    
    // Mix dry and wet signals
    return input * (1.0f - m_mix) + delayedSample * m_mix;
}

void DelayEffect::setDelayTime(float delayTime)
{
    m_delayTime = std::max(0.001f, std::min(1.0f, delayTime));
}

void DelayEffect::setFeedback(float feedback)
{
    m_feedback = std::max(0.0f, std::min(0.95f, feedback));
}

void DelayEffect::setMix(float mix)
{
    m_mix = std::max(0.0f, std::min(1.0f, mix));
}

// ReverbEffect Implementation
ReverbEffect::ReverbEffect(int sampleRate)
    : m_sampleRate(sampleRate)
    , m_roomSize(0.5f)
    , m_damping(0.5f)
    , m_mix(0.3f)
{
    // Comb filter delays (in samples)
    std::vector<int> combDelays = {1116, 1188, 1277, 1356};
    std::vector<int> allpassDelays = {556, 441};
    
    // Scale delays based on sample rate
    float scale = static_cast<float>(sampleRate) / 44100.0f;
    
    m_combBuffers.resize(NUM_COMBS);
    m_combIndices.resize(NUM_COMBS, 0);
    m_combFeedback.resize(NUM_COMBS);
    
    for (int i = 0; i < NUM_COMBS; ++i) {
        int delaySize = static_cast<int>(combDelays[i] * scale);
        m_combBuffers[i].resize(delaySize, 0.0f);
        m_combFeedback[i] = 0.84f;
    }
    
    m_allpassBuffers.resize(NUM_ALLPASS);
    m_allpassIndices.resize(NUM_ALLPASS, 0);
    m_allpassFeedback.resize(NUM_ALLPASS);
    
    for (int i = 0; i < NUM_ALLPASS; ++i) {
        int delaySize = static_cast<int>(allpassDelays[i] * scale);
        m_allpassBuffers[i].resize(delaySize, 0.0f);
        m_allpassFeedback[i] = 0.5f;
    }
}

float ReverbEffect::process(float input)
{
    float output = 0.0f;
    
    // Process through comb filters
    for (int i = 0; i < NUM_COMBS; ++i) {
        float& delayLine = m_combBuffers[i][m_combIndices[i]];
        float combOutput = delayLine;
        delayLine = input + (delayLine * m_combFeedback[i] * m_roomSize);
        
        m_combIndices[i] = (m_combIndices[i] + 1) % m_combBuffers[i].size();
        output += combOutput;
    }
    
    output *= 0.25f; // Scale down after combining
    
    // Process through allpass filters
    for (int i = 0; i < NUM_ALLPASS; ++i) {
        float& delayLine = m_allpassBuffers[i][m_allpassIndices[i]];
        float allpassOutput = -output + delayLine;
        delayLine = output + (delayLine * m_allpassFeedback[i]);
        output = allpassOutput;
        
        m_allpassIndices[i] = (m_allpassIndices[i] + 1) % m_allpassBuffers[i].size();
    }
    
    // Apply damping (simple low-pass filter)
    static float lastOutput = 0.0f;
    output = output * (1.0f - m_damping) + lastOutput * m_damping;
    lastOutput = output;
    
    // Mix dry and wet signals
    return input * (1.0f - m_mix) + output * m_mix;
}

void ReverbEffect::setRoomSize(float roomSize)
{
    m_roomSize = std::max(0.0f, std::min(1.0f, roomSize));
}

void ReverbEffect::setDamping(float damping)
{
    m_damping = std::max(0.0f, std::min(1.0f, damping));
}

void ReverbEffect::setMix(float mix)
{
    m_mix = std::max(0.0f, std::min(1.0f, mix));
}

// Effects Implementation
Effects::Effects(int sampleRate)
    : m_sampleRate(sampleRate)
{
    m_delay = std::make_unique<DelayEffect>(sampleRate);
    m_reverb = std::make_unique<ReverbEffect>(sampleRate);
}

float Effects::process(float input)
{
    float output = input;
    
    // Process through delay first, then reverb
    output = m_delay->process(output);
    output = m_reverb->process(output);
    
    return output;
}

void Effects::setReverbAmount(float amount)
{
    m_reverb->setMix(amount);
}

void Effects::setDelayAmount(float amount)
{
    m_delay->setMix(amount);
}

void Effects::setDelayTime(float time)
{
    m_delay->setDelayTime(time);
}

void Effects::setDelayFeedback(float feedback)
{
    m_delay->setFeedback(feedback);
}
