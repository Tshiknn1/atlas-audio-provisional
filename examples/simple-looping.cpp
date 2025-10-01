#include <string>
#include <vector>
#include <iostream>
#include <memory>
#ifdef WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

#include "../src/miniaudio.c"

#include "../src/engine.hpp"
#include "../src/sound.hpp"

int main(int argc, const char** argv) {
    const std::string filename(argv[1]);

    audio::engine engine;
    engine.init();

    std::vector<std::string> devices = engine.get_devices_out();
    printf("devices\n");
    for (std::string& s : devices) {
        printf("%s\n", s.c_str());
    }

    std::vector<std::unique_ptr<audio::sound>> sounds;

    std::string buf;
    while (buf != "exit") {
        std::unique_ptr<audio::sound> new_sound = 
            std::make_unique<audio::sound>(&engine, filename);
        new_sound->init();
        new_sound->set_volume(1.f / (sounds.size() + 1));
        new_sound->on_play();
        sounds.push_back(std::move(new_sound));
        printf("created sound %d\n", sounds.size());

        // for (std::unique_ptr<audio::sound>& sound: sounds) {
        //     sound->set_volume(1.f / sounds.size());
        // }

        std::getline(std::cin, buf);
    }

    printf("out of loop\n");

    for (std::unique_ptr<audio::sound>& sound : sounds) {
        sound->on_stop();
    }

    return 0;
}