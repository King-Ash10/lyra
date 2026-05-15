#lyra

---
## rundown
its a live real-time TUI that displays whatsong you're listening to, as far as im aware it only works with spotify but i havent tested it so idk
---


## features:
- live music tracking via the ever useful 'playerctl'
- a tui, built with 'FTXUI'
- its multi threaded for the state updates
- its modular i think
- its low laency, atleast it should be
- its unoptimised

---

## features coming soon
- an audio visualiser (FFT cus it sounds cool)
- synced lyrics
- album art support (by ASCII conversion cus i dont know how to get images to render)
- progress bar WITH track timing
- playback controlls

## dependancies
- c++23 compiler (clang++ is reccomended)
- CMake
- FTXUI
- playerctl (Linux MPRIS control)
- pthreads

---

## build instructions
```bash
git clone https://github.com/<your-username>/lyra.git
cd lyra

cmake -B build -G Ninja
cmake --build build

./build/lyra
```
---
## things to note
im on nixos, it isn't tested on other distros and also im a student and this is just a hobby so don't expect constant updates

so uhhh this is called lyra i guess, if you care about where the name came from for whatever reason it was a typo i couldnt be bothered to change
