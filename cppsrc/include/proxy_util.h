#pragma once
#include <napi.h>
#include <string>

namespace ProxySwitcher {
    enum class ProxyType {
        FTP, HTTP, HTTPS, SOCKS
    };

    ProxyType pTypeFromNumber(Napi::Env& env, Napi::Number& num) {
        int32_t i = num.Int32Value();
        switch (i) {
            case 0:
                return ProxyType::FTP;
            case 1:
                return ProxyType::HTTP;
            case 2:
                return ProxyType::HTTPS;
            case 3:
                return ProxyType::SOCKS;
            default:
                Napi::RangeError::New(env, "Unexpected value: " + i)
                    .ThrowAsJavaScriptException();
                return ProxyType::HTTP; // Sane default I guess. I have no idea if
                                        // throwing a JS exception will interrupt this function call.
        }
    }

    void setProxyEnabled(bool value);
    void setProxy(ProxyType proxyType, std::string proxy);
}
