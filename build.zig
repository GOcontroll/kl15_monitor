const std = @import("std");

pub fn build(b: *std.Build) !void {
    const exe = b.addExecutable(.{
        .name = "kl15_monitor",
        .root_source_file = .{ .path = "main.c" },
        .target = std.zig.CrossTarget{
            .os_tag = .linux,
            .cpu_arch = .aarch64,
            .abi = .gnu,
            .glibc_version = std.SemanticVersion{ .major = 2, .minor = 31, .patch = 0 },
        },
        .version = .{ .major = 1, .minor = 1, .patch = 0 },
        .optimize = std.builtin.OptimizeMode.ReleaseSmall,
    });

    exe.addLibraryPath(std.build.LazyPath{ .cwd_relative = "lib/" });
    try exe.include_dirs.append(std.Build.Step.Compile.IncludeDir{ .path = std.build.LazyPath{ .cwd_relative = "lib/" } });

    exe.linkSystemLibrary("libiio");
    exe.dead_strip_dylibs = true;
    exe.linkLibC();

    b.installArtifact(exe);
}
