load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

def third_party_repositories():
    http_archive(
        name = "com_github_gflags_gflags",
        urls = ["https://github.com/gflags/gflags/archive/v2.2.2.tar.gz"],
        sha256 = "34af2f15cf7367513b352bdcd2493ab14ce43692d2dcd9dfc499492966c64dcf",
        strip_prefix = "gflags-2.2.2",
    )

    http_archive(
        name = "com_google_googletest",
        sha256 = "99dddcd1a17c786a06bb9a8da55bfa51f08aa146c877c3643d80a36047c715c7",
        strip_prefix = "googletest-0599a7b8410dc5cfdb477900b280475ae775d7f9",
        urls = ["https://github.com/google/googletest/archive/0599a7b8410dc5cfdb477900b280475ae775d7f9.tar.gz"],
    )

    http_archive(
        name = "com_google_glog",
        sha256 = "8a83bf982f37bb70825df71a9709fa90ea9f4447fb3c099e1d720a439d88bad6",
        strip_prefix = "glog-0.6.0",
        urls = ["https://github.com/google/glog/archive/v0.6.0.tar.gz"],
    )

    # rules_cc defines rules for generating C++ code from Protocol Buffers.
    http_archive(
        name = "rules_cc",
        urls = [
            "https://github.com/bazelbuild/rules_cc/releases/download/0.0.6/rules_cc-0.0.6.tar.gz",
        ],
        sha256 = "3d9e271e2876ba42e114c9b9bc51454e379cbf0ec9ef9d40e2ae4cec61a31b40",
        strip_prefix = "rules_cc-0.0.6",
    )

    # rules_proto defines abstract rules for building Protocol Buffers.
    http_archive(
        name = "rules_proto",
        sha256 = "dc3fb206a2cb3441b485eb1e423165b231235a1ea9b031b4433cf7bc1fa460dd",
        strip_prefix = "rules_proto-5.3.0-21.7",
        urls = [
            "https://github.com/bazelbuild/rules_proto/archive/refs/tags/5.3.0-21.7.tar.gz",
        ],
    )
