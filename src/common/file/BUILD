package(default_visibility = ["//visibility:public"])

cc_library(
    name = "io",
    srcs = ["io.cpp"],
    hdrs = ["io.h"],
    deps = [
        "@com_google_protobuf//:protobuf",
    ],
)

cc_test(
    name = "io_test",
    srcs = ["io_test.cpp"],
    deps = [
        ":io",
        "//src/common/interface/proto:cc_io_test_proto",
        "//src/common/testing:google_test_main",
    ],
)
