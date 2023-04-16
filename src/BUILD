package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "demo",
    srcs = ["demo.cpp"],
    data = [
        "//params:wav_params",
    ],
    deps = [
        "//src/common/file:io",
        "//src/common/math:math_utils",
        "//src/wav:wav_writer",
        "@com_google_glog//:glog",
    ],
)

cc_binary(
    name = "demo2",
    srcs = ["demo2.cpp"],
    deps = [
        "@com_google_glog//:glog",
    ],
)
