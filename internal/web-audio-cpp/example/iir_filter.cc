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
  auto iir = web_audio::IIRFilterNode::create(
      context, {
                   .feedforward = {0.1, -4, 0, 1, 0.5, 4},
                   .feedback = {4, -0.5, 0.5, 0, 0, 0},
               });
  oscillator->connect(iir);
  iir->connect(context->getDestination());
  oscillator->start();
  std::this_thread::sleep_for(std::chrono::seconds(10));

  SDL_Quit();
  return 0;
}