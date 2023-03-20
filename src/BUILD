package(default_visibility = ["//visibility:public"])

cc_library(
    name = "wav_header",
    hdrs = ["wav_header.h"],
)

cc_binary(
    name = "demo",
    srcs = ["demo.cpp"],
    deps = [
        ":wav_header",
    ],
)

cc_binary(
    name = "hello_world",
    srcs = ["hello_world.cpp"],
)
