package(default_visibility = ["//visibility:public"])

cc_library(
    name = "singleton_manager",
    hdrs = ["singleton_manager.h"],
    deps = [
        "//sun/data_structure:hash_table",
    ],
)

cc_test(
    name = "singleton_manager_test",
    srcs = ["singleton_manager_test.cc"],
    deps = [
        "//sun/util:singleton",
        ":singleton_manager",
    ],
)