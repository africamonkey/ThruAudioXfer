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

cc_test(
    name = "wav_io_test",
    srcs = ["wav_io_test.cpp"],
    data = [
        "//params:wav_params",
    ],
    deps = [
        ":wav_io",
        "//src/common/file:io",
        "//src/common/math:math_utils",
        "//src/common/testing:google_test_main",
    ],
)
