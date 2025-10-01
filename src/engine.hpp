#pragma once

#include "miniaudio.h"

#include <vector>
#include <string>
#include <map>

/**
 * TODO
 *  - initialize context and config before initializing engine
 *      - we want to know available devices before we initialize
 */

namespace audio {

    class engine {
    public:
        engine();
        ~engine();

        bool init();
        void uninit();
        bool reinit();

        bool initialized() const { return m_initialized; }
        ma_engine* raw_inst() { return &m_engine; }

        std::vector<std::string> get_devices_out();
        void set_device_out(const std::string& device_name);

        // std::vector<std::string> get_devices_in();
        // void set_device_in(const std::string& device_name);

    private:
        bool m_initialized;
        bool m_context_initialized;

        ma_engine m_engine;
        ma_context m_context;
        ma_engine_config m_engine_config;

        std::map<std::string, ma_device_id> m_device_list;

        void populate_device_list();
    };

}