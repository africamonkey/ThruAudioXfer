package(default_visibility = ["//visibility:public"])

cc_library(
    name = "wav_header",
    hdrs = ["wav_header.h"],
)

cc_library(
    name = "wav_io",
    srcs = ["wav_io.cpp"],
    hdrs = ["wav_io.h"],
    deps = [
        ":wav_header",
        "//src/common/interface/proto:cc_wav_params_proto",
        "//src/common/math:math_utils",
    ],
)
