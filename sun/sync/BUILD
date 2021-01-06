package(default_visibility = ["//visibility:public"])

cc_library(
    name = "spinlock",
    hdrs = ["spinlock.h"],
    srcs = ["spinlock.cc"],
    deps = [
        "//sun/util:common",
    ],
)

cc_library(
    name = "queue",
    hdrs = ["queue.h"],
    deps = [
        ":spinlock",
        "//sun/data_structure:array",
    ],
)

cc_test(
    name = "queue_test",
    srcs = ["queue_test.cc"],
    deps = [
        ":queue",
    ],
)