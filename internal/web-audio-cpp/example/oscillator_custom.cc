#include "web_audio.hh"

#include <thread>

#include <SDL3/SDL.h>

int main() {
  SDL_Init(SDL_INIT_AUDIO);

  auto context = web_audio::AudioContext::create();

  std::vector<float> real = {0, 1, 0.5, 0.3, 0.2};
  std::vector<float> imag = {0, 0, 0, 0, 0};

  auto oscillator = web_audio::OscillatorNode::create(
      context, {
                   .type = web_audio::OscillatorType::eCustom,
                   .periodicWave = web_audio::PeriodicWave::create(
                       context, {.real = real, .imag = imag}),
               });
  oscillator->connect(context->getDestination());
  oscillator->start();
  std::this_thread::sleep_for(std::chrono::seconds(10));

  SDL_Quit();
  return 0;
}