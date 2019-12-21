#include <iostream>
#include <napi.h>
#include <glibmm-2.4/glibmm.h>
#include <giomm-2.4/giomm/settings.h>

typedef std::string proxy_t;
typedef uint16_t    port_t;

proxy_t proxyTypeKeyFromInt(Napi::Env& env, int num) {
    switch (num) {
        case 0:
            return "ftp";
        case 1:
            return "http";
        case 2:
            return "https";
        case 3:
            return "socks";
        default:
            Napi::RangeError::New(env, "Unexpected value: " + num)
                .ThrowAsJavaScriptException();
            return "http"; // Sane default I guess. I have no idea if
                                    // throwing a JS exception will interrupt this function call.
    }
}

void setProxyEnabled(bool value) {
    Glib::RefPtr<Gio::Settings> proxySetting =
        Gio::Settings::create("org.gnome.system.proxy");
    
    proxySetting->set_enum("mode",
        value ? 1 : 0);
}

void setProxyEnabledWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() != 1) {
        Napi::Error::New(env, "Expected one argument, got " + info.Length()).ThrowAsJavaScriptException();
        return;
    } else if (!info[0].IsBoolean()) {
        Napi::TypeError::New(env, "Expected boolean, got " + info[0].ToString().Utf8Value());
        return;
    }

    setProxyEnabled(info[0].ToBoolean());
}

void setProxy(const proxy_t& proxyType, const std::string& host, port_t port) {
    Glib::RefPtr<Gio::Settings> proxySettings =
        Gio::Settings::create("org.gnome.system.proxy." + proxyType);
    
    proxySettings->set_string("host", Glib::ustring(host));
    proxySettings->set_int("port", port);
    g_settings_sync();
}

void setProxyWrapped(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() != 3) {
        Napi::Error::New(env, "Incorrect number of arguments").ThrowAsJavaScriptException();
        return;
    } else if (!info[0].IsNumber()){
        Napi::Error::New(env, "Expected number, got " + info[0].ToString().Utf8Value()).ThrowAsJavaScriptException();
        return;
    } else if (!info[1].IsString()) {
        Napi::Error::New(env, "Expected string, got " + info[1].ToString().Utf8Value()).ThrowAsJavaScriptException();
        return;
    } else if (!info[2].IsNumber()) {
        Napi::Error::New(env, "Expected number, got " + info[1].ToString().Utf8Value()).ThrowAsJavaScriptException();
        return;
    }

    proxy_t pType = proxyTypeKeyFromInt(env, info[0].As<Napi::Number>().Int32Value());
    std::string host = info[1].As<Napi::String>();
    int argPort = info[2].As<Napi::Number>().Int32Value();

    if (argPort < 0 || argPort > UINT16_MAX) {
        Napi::RangeError::New(env, "Port number out of range: " + argPort).ThrowAsJavaScriptException();
        return;
    }

    setProxy(pType, host, static_cast<port_t>(argPort));
}

Napi::Object init(Napi::Env env, Napi::Object exports) {
    Glib::init();

    // Export ProxyType
    // This is kind of a hack, but it works.
    auto proxyType = Napi::Object::New(env);
    proxyType.Set("FTP", Napi::Number::New(env, 0));
    proxyType.Set("HTTP", Napi::Number::New(env, 1));
    proxyType.Set("HTTPS", Napi::Number::New(env, 2));
    proxyType.Set("SOCKS", Napi::Number::New(env, 3));
    exports.Set("ProxyType", proxyType);

    exports.Set("setProxyEnabled", Napi::Function::New(env, setProxyEnabledWrapped));
    exports.Set("setProxy", Napi::Function::New(env, setProxyWrapped));

    return exports;
}

NODE_API_MODULE(proxyswitcher, init)