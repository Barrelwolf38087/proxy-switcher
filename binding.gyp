{
    "targets": [{
        "target_name": "proxy-switcher",
        "cflags": ["-fno-exceptions", "-pthread"],
        "cflags_cc": ["-fno-exceptions", "-pthread"],
        "cflags_cc!": ["-fno-rtti"],
        "sources": [
            "cppsrc/main.cpp",
        ],
        "include_dirs": [
            "<!@(node -p \"require('node-addon-api').include\")",
            '<!@(pkg-config --cflags-only-I giomm-2.4 | sed s\/-I//g)'
        ],
        "libraries": [
            "<!@(pkg-config --libs-only-l giomm-2.4)"
        ],
        "dependencies": [
            "<!(node -p \"require('node-addon-api').gyp\")"
        ],
        "defines": ["NAPI_DISABLE_CPP_EXCEPTIONS"]
    }]
}