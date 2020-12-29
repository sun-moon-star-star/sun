cc_library(
    name = "common",
    hdrs = ["common.h"],
)

cc_test(
    name = "common_test",
    srcs = ["common_test.cc"],
    deps = [
        ":common",
    ],
)

cc_library(
    name = "struct",
    hdrs = ["struct.h"],
)

cc_test(
    name = "struct_test",
    srcs = ["struct_test.cc"],
    deps = [
        ":struct",
    ],
)

cc_library(
    name = "hash",
    hdrs = ["hash.h"],
)

cc_test(
    name = "hash_test",
    srcs = ["hash_test.cc"],
    deps = [
        ":hash",
    ],
)

cc_library(
    name = "json",
    hdrs = ["json.h"],
    linkopts = ["-ljsoncpp"],
)

cc_test(
    name = "json_test",
    srcs = ["json_test.cc"],
    deps = [
        ":json",
    ],
)