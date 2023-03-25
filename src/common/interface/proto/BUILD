package(default_visibility = ["//visibility:public"])

load("@rules_cc//cc:defs.bzl", "cc_library")
load("@rules_proto//proto:defs.bzl", "proto_library")

proto_library(
    name = "wav_params_proto",
    srcs = ["wav_params.proto"],
)

cc_proto_library(
    name = "cc_wav_params_proto",
    deps = [":wav_params_proto"],
)
