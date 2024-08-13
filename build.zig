const std = @import("std");

const version = "1.1.1";

pub fn build(b: *std.Build) !void {
    const target = b.standardTargetOptions(.{ .default_target = .{
        .cpu_arch = .aarch64,
        .os_tag = .linux,
        .abi = .gnu,
        .glibc_version = .{ .major = 2, .minor = 31, .patch = 0 },
    } });

    const semver = try std.SemanticVersion.parse(version);

    const exe = b.addExecutable(.{
        .name = "go-auto-shutdown",
        .target = target,
        .version = semver,
        .optimize = .ReleaseSmall,
        .link_libc = true,
        .strip = false,
    });
    exe.root_module.addCMacro("VERSION", version); //add executable version as a #define
    exe.addCSourceFile(.{ .file = .{ .cwd_relative = "main.c" } });
    exe.linkSystemLibrary("libiio");
    exe.dead_strip_dylibs = true;

    b.installArtifact(exe);
}
