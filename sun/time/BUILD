package(default_visibility = ["//visibility:public"])

cc_library(
    name = "time",
    hdrs = ["time.h"],
    deps = [
        "//sun/util:common",
    ],
)

cc_test(
    name = "time_test",
    srcs = ["time_test.cc"],
    deps = [
        ":time",
    ],
)