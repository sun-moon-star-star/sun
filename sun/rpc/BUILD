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

cc_library(
    name = "buffer",
    hdrs = ["buffer.h"],
)

cc_test(
    name = "buffer_test",
    srcs = ["buffer_test.cc"],
    deps = [
        ":buffer",
        "//sun/util:common",
    ],
)