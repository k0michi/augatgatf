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
  auto biquad = web_audio::BiquadFilterNode::create(
      context, {
                   .type = web_audio::BiquadFilterType::eHighpass,
                   .Q = 1,
                   .frequency = 1000,
               });
  oscillator->connect(biquad);
  biquad->connect(context->getDestination());
  oscillator->start();
  std::this_thread::sleep_for(std::chrono::seconds(10));

  SDL_Quit();
  return 0;
}