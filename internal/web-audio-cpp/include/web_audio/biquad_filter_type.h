#pragma once

namespace web_audio {
enum class BiquadFilterType {
  eLowpass,
  eHighpass,
  eBandpass,
  eLowshelf,
  eHighshelf,
  ePeaking,
  eNotch,
  eAllpass,
};
}