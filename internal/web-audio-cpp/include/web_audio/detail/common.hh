#pragma once

#ifdef WEB_AUDIO_TEST
#define WEB_AUDIO_PRIVATE public
#define WEB_AUDIO_PROTECTED public
#else
#define WEB_AUDIO_PRIVATE private
#define WEB_AUDIO_PROTECTED protected
#endif