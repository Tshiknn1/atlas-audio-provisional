#include "sound.hpp"
#include <stdexcept>
#include <cmath>

namespace audio {

    sound::sound(engine* pEngine) 
        : m_pEngine(pEngine) 
        , m_initialized(false)
        , m_decode_on_load(false)
        , m_volume(1.f)
    { }

    sound::sound(engine* pEngine, const std::string filename)
        : sound(pEngine)
    {
        m_filename = filename;
    }

    sound::~sound() {
        if (m_initialized) {
            uninit();
        }
    }

    void sound::on_play() {
        ma_sound_start(&m_sound);
    }

    void sound::on_stop() {
        stop_internal();
    }

    bool sound::is_playing() {
        return ma_sound_is_playing(&m_sound);
    }

    bool sound::init() {
        if (m_initialized) {
            return false;
        }

        if (m_filename.empty()) {
            return false;
        }

        int cfg_flags = 0;
        if (m_decode_on_load) {
            cfg_flags |= MA_SOUND_FLAG_DECODE;
        }

        ma_result result;
        result = ma_sound_init_from_file(
            m_pEngine->raw_inst(),
            m_filename.c_str(),
            cfg_flags,
            NULL,
            NULL,
            &m_sound
        );

        if (result != MA_SUCCESS) {
            return false;
        }

        m_initialized = true;
        return true;
    }

    void sound::uninit() {
        if (is_playing()) {
            stop_internal();
        }
        ma_sound_uninit(&m_sound);
        m_initialized = false;
    }

    bool sound::reinit() {
        uninit();
        return init();
    }

    void sound::seek_time(float seconds) {
        ma_sound_seek_to_second(&m_sound, seconds);
    }

    void sound::stop_internal() {
        ma_sound_stop(&m_sound);
    }

    void sound::set_volume(float volume) {
        ma_sound_set_volume(&m_sound, volume);
        m_volume = volume;
    }

    void sound::set_db(float db) {
        float volume = std::pow(10, db / 20);
        set_volume(volume);
    }

    float sound::db() const {
        return 20 * std::log10(m_volume);
    }
}