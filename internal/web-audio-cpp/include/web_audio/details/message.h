#pragma once

#include <variant>

namespace web_audio::details {
class BeginRenderingMessage {};
// TODO: other messages

using Message = std::variant<BeginRenderingMessage>;
} // namespace web_audio::details