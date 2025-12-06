#include <napi.h>

#include <kl/version.hh>

Napi::Object getVersion(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  Napi::Object versionObj = Napi::Object::New(env);
  auto version = kl::getVersion();
  versionObj.Set("major", Napi::Number::New(env, version.major));
  versionObj.Set("minor", Napi::Number::New(env, version.minor));
  versionObj.Set("patch", Napi::Number::New(env, version.patch));
  return versionObj;
}

Napi::String getRevision(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  auto revision = kl::getRevision();
  return Napi::String::New(env, revision.data(), revision.size());
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "getVersion"),
              Napi::Function::New(env, getVersion));
  exports.Set(Napi::String::New(env, "getRevision"),
              Napi::Function::New(env, getRevision));
  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)