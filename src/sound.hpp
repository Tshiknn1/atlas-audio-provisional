#pragma once

#include "engine.hpp"
#include "miniaudio.h"
#include <string>

namespace audio {

    class sound {
    public:
        sound(engine* pEngine);
        sound(engine* pEngine, const std::string filename);
        ~sound();

        void on_play();
        void on_stop();

        bool is_playing();
        void seek_time(float seconds);

        bool init();
        bool reinit();
        void uninit();

        bool file_loaded() const { return m_file_loaded; }

        void set_filename(const std::string& filename) { m_filename = filename; }
        const std::string filename() const { return m_filename; }

        void set_volume(float volume);
        float volume() const { return m_volume; }
        void set_db(float db);
        float db() const;

        void set_decode_on_load(bool decode) { m_decode_on_load = decode; }
        bool decode_on_load() const { return m_decode_on_load; }

    private:
        void stop_internal();

        ma_sound m_sound;

        engine* m_pEngine;

        bool m_file_loaded;
        bool m_initialized;
        float m_volume;
        bool m_decode_on_load;

        std::string m_filename;
    };
}