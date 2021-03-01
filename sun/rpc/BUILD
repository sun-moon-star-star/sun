package(default_visibility = ["//visibility:public"])

cc_library(
    name = "protocol",
    hdrs = ["protocol.h"],
)

cc_test(
    name = "protocol_test",
    srcs = ["protocol_test.cc"],
    deps = [
        ":protocol",
        "//sun/util:common",
    ],
)