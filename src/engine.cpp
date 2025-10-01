#include "engine.hpp"

#include <stdexcept>

namespace audio {

    engine::engine() {
        m_initialized = false;

        ma_result result;
        result = ma_context_init(NULL, 0, NULL, &m_context);
        if (result != MA_SUCCESS) {
            return;
        }

        m_engine_config = ma_engine_config_init();
        m_engine_config.pContext = &m_context;
    }

    engine::~engine() {
        if (m_initialized) {
            uninit();
        }
        ma_context_uninit(&m_context);
    }

    bool engine::init() {
        // we do not want to init twice
        if (m_initialized) {
            return false;
        }

        ma_result result;

        result = ma_engine_init(&m_engine_config, &m_engine);
        if (result != MA_SUCCESS) {
            return false;
        }

        m_initialized = true;
        return true;
    }

    void engine::uninit() {
        ma_engine_uninit(&m_engine);
        m_initialized = false;
    }

    bool engine::reinit() {
        uninit();
        return init();
    }

    void engine::populate_device_list() {
        ma_uint32 device_count;
        ma_device_info* playback_devices;
        ma_result result;

        result = ma_context_get_devices(&m_context, &playback_devices, &device_count, NULL, NULL);
        if (result != MA_SUCCESS) {
            throw std::runtime_error("error getting devices");
        }

        m_device_list.clear();
        for (int i = 0; i < device_count; i++) {
            ma_device_info device = playback_devices[i];
            m_device_list.insert(std::make_pair(std::string(device.name), device.id));
        }
    }

    std::vector<std::string> engine::get_devices_out() {
        populate_device_list();

        std::vector<std::string> r;
        for (std::map<std::string, ma_device_id>::iterator it = m_device_list.begin();
            it != m_device_list.end(); it++)
        {
            r.push_back(it->first);
        }
        
        return r;
    }

    void engine::set_device_out(const std::string& device_name) {
        ma_device_id* new_id = &m_device_list[device_name];

        /* do this to poke library to reinitialize device */
        ma_device_uninit(m_engine_config.pDevice);
        m_engine_config.pDevice = NULL;
        m_engine_config.pPlaybackDeviceID = new_id;
    }
}