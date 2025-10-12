#include "web_audio.hh"

#include <thread>

#include <SDL3/SDL.h>

int main() {
  SDL_Init(SDL_INIT_AUDIO);

  auto context = web_audio::AudioContext::create();
  auto oscillator = web_audio::OscillatorNode::create(
      context, {
                   .type = web_audio::OscillatorType::eSawtooth,
               });
  auto gain = web_audio::GainNode::create(context, {
                                                       .gain = 0.1f,
                                                   });
  oscillator->connect(gain);
  gain->connect(context->getDestination());
  oscillator->start();
  std::this_thread::sleep_for(std::chrono::seconds(10));

  SDL_Quit();
  return 0;
}