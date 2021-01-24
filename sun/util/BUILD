package(default_visibility = ["//visibility:public"])

cc_library(
    name = "common",
    hdrs = ["common.h"],
)

cc_test(
    name = "common_test",
    srcs = ["common_test.cc"],
    deps = [
        ":common",
    ],
)

cc_library(
    name = "hash",
    hdrs = ["hash.h"],
)

cc_test(
    name = "hash_test",
    srcs = ["hash_test.cc"],
    deps = [
        ":hash",
    ],
)

cc_library(
    name = "singleton",
    hdrs = ["singleton.h"],
    deps = [
        "//sun/util/include:singleton_manager",
    ],
)

cc_test(
    name = "singleton_test",
    srcs = ["singleton_test.cc"],
    deps = [
        ":singleton",
    ],
)

cc_library(
    name = "status",
    hdrs = ["status.h"],
    srcs = ["status.cc"],
)

cc_library(
    name = "decorator",
    hdrs = ["decorator.h"],
)
