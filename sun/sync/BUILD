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

cc_library(
    name = "heap",
    hdrs = ["heap.h"],
    deps = [
        ":spinlock",
        "//sun/data_structure:heap",
    ],
)

cc_library(
    name = "list",
    hdrs = ["list.h"],
    deps = [
        ":spinlock",
    ],
)

cc_test(
    name = "list_test",
    srcs = ["list_test.cc"],
    deps = [
        ":list",
    ],
)

cc_library(
    name = "task_queue",
    hdrs = ["task_queue.h"],
    deps = [
        ":list",
        ":heap",
        ":queue",
    ],
)