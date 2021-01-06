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
    name = "task_queue",
    hdrs = ["task_queue.h"],
    deps = [
        ":spinlock",
        "//sun/data_structure:array",
    ],
)