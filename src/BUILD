package(default_visibility = ["//visibility:public"])

cc_library(
    name = "math_utils",
    hdrs = ["math_utils.h"],
    deps = [
        "@com_google_glog//:glog",
    ],
)

cc_library(
    name = "wav_header",
    hdrs = ["wav_header.h"],
)

cc_library(
    name = "wav_io",
    srcs = ["wav_io.cpp"],
    hdrs = ["wav_io.h"],
    deps = [
        ":math_utils",
        ":wav_header",
    ],
)

cc_binary(
    name = "demo",
    srcs = ["demo.cpp"],
    deps = [
        ":wav_io",
        "@com_google_glog//:glog",
    ],
)

cc_binary(
    name = "hello_world",
    srcs = ["hello_world.cpp"],
)
