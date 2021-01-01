cc_library(
    name = "code",
    hdrs = ["code.h"],
)


cc_library(
    name = "config",
    hdrs = ["config.h"],
)

cc_library(
    name = "struct",
    hdrs = ["struct.h"],
    srcs = ["struct.cc"],
    deps = [
        ":code",
        "//sun/util:common",
    ],
)

cc_test(
    name = "struct_test",
    srcs = ["struct_test.cc"],
    deps = [
        ":struct",
    ],
)

cc_library(
    name = "accepter",
    hdrs = ["accepter.h"],
    srcs = ["accepter.cc"],
    deps = [
        ":struct",
    ],
)

cc_test(
    name = "accepter_test",
    srcs = ["accepter_test.cc"],
    deps = [
        ":accepter",
    ],
)

cc_library(
    name = "proposer",
    hdrs = ["proposer.h"],
    srcs = ["proposer.cc"],
    deps = [
        ":config",
        ":struct",
        ":accepter",
    ],
)
