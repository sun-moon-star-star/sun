package(default_visibility = ["//visibility:public"])

cc_library(
    name = "statistics",
    hdrs = ["statistics.h"],
)

cc_test(
    name = "statistics_test",
    srcs = ["statistics_test.cc"],
    deps = [
        ":statistics",
        "//sun/util:common",
    ],
)