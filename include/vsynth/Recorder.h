#ifndef RECORDER_H
#define RECORDER_H

#include <vector>
#include <string>
#include <fstream>
#include <memory>

struct NoteEvent {
    float timestamp;
    int note;
    float velocity;
    bool isNoteOn;
    
    NoteEvent(float t, int n, float v, bool on) 
        : timestamp(t), note(n), velocity(v), isNoteOn(on) {}
};

class Recorder
{
public:
    Recorder(int sampleRate);
    ~Recorder() = default;
    
    void startRecording();
    void stopRecording();
    bool isRecording() const { return m_isRecording; }
    
    void startPlayback();
    void stopPlayback();
    bool isPlaying() const { return m_isPlaying; }
    
    void recordNoteEvent(int note, float velocity, bool isNoteOn);
    void recordAudioSample(float sample);
    
    // Export functions
    void exportToWAV(const std::string& filename);
    void exportToMIDI(const std::string& filename);
    void exportNoteEvents(const std::string& filename);
    
    // Playback
    void processPlayback(float deltaTime);
    std::vector<NoteEvent> getEventsToPlay();
    
    void clear();
    
private:
    void writeWAVHeader(std::ofstream& file, int dataSize);
    void writeMIDIFile(const std::string& filename);
    
    int m_sampleRate;
    bool m_isRecording;
    bool m_isPlaying;
    
    // Note events recording
    std::vector<NoteEvent> m_noteEvents;
    float m_recordingTime;
    
    // Audio recording
    std::vector<float> m_audioBuffer;
    
    // Playback state
    float m_playbackTime;
    size_t m_playbackIndex;
    std::vector<NoteEvent> m_eventsToPlay;
};

#endif // RECORDER_H
