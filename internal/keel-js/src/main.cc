#include <napi.h>

Napi::Number Add(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() < 2 || !info[0].IsNumber() || !info[1].IsNumber()) {
    Napi::TypeError::New(env, "Two numbers expected")
        .ThrowAsJavaScriptException();
    return Napi::Number::New(env, 0);
  }

  double arg0 = info[0].As<Napi::Number>().DoubleValue();
  double arg1 = info[1].As<Napi::Number>().DoubleValue();
  Napi::Number sum = Napi::Number::New(env, arg0 + arg1);

  return sum;
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "add"), Napi::Function::New(env, Add));
  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)