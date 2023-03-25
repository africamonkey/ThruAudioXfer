load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def third_party_repositories():
    http_archive(
        name = "com_github_gflags_gflags",
        urls = ["https://github.com/gflags/gflags/archive/v2.2.2.tar.gz"],
        sha256 = "34af2f15cf7367513b352bdcd2493ab14ce43692d2dcd9dfc499492966c64dcf",
        strip_prefix = "gflags-2.2.2",
    )

    http_archive(
        name = "com_google_googletest",
        patches = [
            "//third_party/patches/com_google_googletest/no-fopen.diff",
            "//third_party/patches/com_google_googletest/no-stdout.diff",
            "//third_party/patches/com_google_googletest/program_main.diff",
        ],
        sha256 = "99dddcd1a17c786a06bb9a8da55bfa51f08aa146c877c3643d80a36047c715c7",
        strip_prefix = "googletest-0599a7b8410dc5cfdb477900b280475ae775d7f9",
        urls = ["https://github.com/google/googletest/archive/0599a7b8410dc5cfdb477900b280475ae775d7f9.tar.gz"],
    )

    http_archive(
        name = "com_google_protobuf",
        patches = [
            "//third_party/patches/com_google_protobuf/endian.diff",
            "//third_party/patches/com_google_protobuf/no-filesystem-namespace.diff",
            "//third_party/patches/com_google_protobuf/no-stdout.diff",
            "//third_party/patches/com_google_protobuf/unsafe-string-functions.diff",
        ],
        sha256 = "73fdad358857e120fd0fa19e071a96e15c0f23bb25f85d3f7009abfd4f264a2a",
        strip_prefix = "protobuf-3.6.1.3",
        urls = ["https://github.com/protocolbuffers/protobuf/archive/v3.6.1.3.tar.gz"],
    )

    http_archive(
        name = "com_google_glog",
        sha256 = "8a83bf982f37bb70825df71a9709fa90ea9f4447fb3c099e1d720a439d88bad6",
        strip_prefix = "glog-0.6.0",
        urls = ["https://github.com/google/glog/archive/v0.6.0.tar.gz"],
    )
