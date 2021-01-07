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

cc_test(
    name = "singleton_test",
    srcs = ["singleton_test.cc"],
    deps = [
        ":singleton",
    ],
)

cc_library(
    name = "singleton_manager",
    hdrs = ["singleton_manager.h"],
    deps = [
        ":singleton",
        "//sun/data_structure:hash_table",
    ],
)

cc_test(
    name = "singleton_manager_test",
    srcs = ["singleton_manager_test.cc"],
    deps = [
        ":singleton_manager",
    ],
)