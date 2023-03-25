workspace(
    name = "TAXf",
)

load("//third_party:third_party.bzl", "third_party_repositories")

third_party_repositories()

load("@rules_cc//cc:repositories.bzl", "rules_cc_dependencies")

rules_cc_dependencies()

load("@rules_proto//proto:repositories.bzl", "rules_proto_dependencies", "rules_proto_toolchains")

rules_proto_dependencies()

rules_proto_toolchains()
