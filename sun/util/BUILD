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
)

cc_library(
    name = "singleton_manager",
    hdrs = ["singleton_manager.h"],
    deps = [
        "//sun/data_structure:hash_table",
    ],
)