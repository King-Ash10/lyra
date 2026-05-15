#include <atomic>
#include <chrono>
#include <cstdio>
#include <ftxui/dom/node.hpp>
#include <iostream>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <thread>

using namespace ftxui;

// global variables <MAKE STRUCT LATER>

std::string current_artist;
std::string current_title;

std::mutex music_mutex;

ScreenInteractive* screen_ptr = nullptr;

std::string get_artist(){
    FILE* pipe = popen("playerctl metadata xesam:artist", "r");
    if (pipe == nullptr){
        return "";
    }
    char buffer[256];
    char* result = fgets(buffer, 256, pipe);

    if (result == nullptr){
        pclose(pipe);
        return "";
    }

    std::string artist_name(buffer);

    if (!artist_name.empty()){
        if (artist_name.back() == '\n'){
            artist_name.pop_back();
        }
    }

    pclose(pipe);

    return artist_name;
}
std::string get_title(){
    FILE* pipe = popen("playerctl metadata xesam:title", "r");
    if (pipe == nullptr){
        return "";
    }
    char buffer[256];
    char* result = fgets(buffer, 256, pipe);

    if (result == nullptr){
        pclose(pipe);
        return "";
    }

    std::string song_title(buffer);

    if (!song_title.empty()){
        if (song_title.back() == '\n'){
            song_title.pop_back();
        }
    }

    pclose(pipe);

    return song_title;
}

std::string get_album_art(){
    FILE* pipe = popen("playerctl metadata mpris:artUrl", "r");
    char buffer[256];
    char* result = fgets(buffer, 256, pipe);
    if (pipe == nullptr){
        return "";
    }
    if (result == nullptr){
        pclose(pipe);
        return "";
    }
    std::string album_art(buffer);
    if (!album_art.empty()){
        if (album_art.back() == '\n'){
            album_art.pop_back();
        }
    }
    pclose(pipe);

    return album_art;
    
}

// this thread is gunna be  SUPER hard at work
void music_update_loop(){
    std::string last_track = "";
    
    while (true){
        std::string artist = get_artist();
        std::string title = get_title();
        {
            std::lock_guard<std::mutex> lock(music_mutex);

            current_artist = artist;
            current_title = title;
        }
        std::string current_track = artist + " - " + title;
        
        if (screen_ptr) {
            screen_ptr->PostEvent(Event::Custom);
        }
        

        // make sure no music is playing
        if (!current_track.empty() && current_track != " - "){
            if (current_track != last_track){
                std::cout << current_track << std::endl;
                last_track = current_track;
            }
        }
        
        

        std::this_thread::sleep_for((std::chrono::milliseconds(500)));

    }
}
// reder the tui
Element render_now_playing() {
    std::string artist;
    std::string title;

    {
        std::lock_guard<std::mutex> lock(music_mutex);
        artist = current_artist;
        title = current_title;
    }

    return vbox({
        text(" NOW PLAYING ") | bold | center,
        separator(),
        filler(),
        text(artist) | color(Color::Blue) | bold | center,
        text(title) | color(Color::Magenta) | center,
        filler(),
    }) | border;
}

Element render_lyrics() {
    return vbox({
        text(" LYRICS ") | bold | center,
        separator(),
        filler(),
        text(" Placeholder, wow im so amazing!!!! ") | center,
        text("IMPORTANT!!!! SET UP SYNCING AND LYRIC FETCH ONE DAY!!!!") | dim | center,
        filler(),
    }) | border;
}

Element render_visualizer() {
    return vbox({
        text(" VISUALIZER ") | bold | center,
        separator(),
        filler(),
        text("▁ ▂ ▃ ▄ ▅ ▆ ▇ █ ▇ ▆ ▅ ▄ ▃ ▂ ▁") | center,
        filler(),
    }) | border;
}

// layout things
auto renderer = Renderer([]{
    return vbox({
    hbox({
        render_now_playing() | flex,
        render_lyrics() | flex
    }) | flex,
    render_visualizer() | flex
    });
});





int main() {
    // run command, get song, be awesome
    auto screen = ScreenInteractive::Fullscreen();
    screen_ptr = &screen;
    std::thread update_loop(music_update_loop);
    update_loop.detach();
    
    


    screen.Loop(renderer);

    return 0;
}