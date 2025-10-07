#include "web_audio.hh"

#include <thread>

#include <SDL3/SDL.h>
#include <SDL3/SDL_timer.h>

int main() {
  SDL_Init(SDL_INIT_AUDIO);

  auto context = web_audio::AudioContext::create();
  auto oscillator = web_audio::OscillatorNode::create(context);
  oscillator->connect(context->getDestination());
  oscillator->start();
  // SDL_Delay(10000);
  std::this_thread::sleep_for(std::chrono::seconds(10));

  SDL_Quit();
  return 0;
}