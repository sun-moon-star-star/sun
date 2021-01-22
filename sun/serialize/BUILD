package(default_visibility = ["//visibility:public"])

cc_library(
    name = "serialize",
    hdrs = ["serialize.h"],
)

cc_test(
    name = "serialize_test",
    srcs = ["serialize_test.cc"],
    deps = [
        ":serialize",
        "//sun/util:common",
    ],
)