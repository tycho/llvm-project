// Check that, for MSVC targets, the compiler embeds a default library directive
// for compiler-rt's builtins library when that library is present, so that any
// linker driving the link picks it up.

// Per-target runtime directory layout.
// RUN: %clang -### -c --target=x86_64-pc-windows-msvc \
// RUN:   -resource-dir=%S/Inputs/resource_dir_with_per_target_subdir -- %s 2>&1 \
// RUN:   | FileCheck %s --check-prefix=PER-TARGET
// RUN: %clang_cl -### -c --target=x86_64-pc-windows-msvc \
// RUN:   -resource-dir %S/Inputs/resource_dir_with_per_target_subdir -- %s 2>&1 \
// RUN:   | FileCheck %s --check-prefix=PER-TARGET
// RUN: %clang -### -c --target=x86_64-pc-windows-msvc -fms-runtime-lib=dll \
// RUN:   -resource-dir=%S/Inputs/resource_dir_with_per_target_subdir -- %s 2>&1 \
// RUN:   | FileCheck %s --check-prefix=PER-TARGET
// PER-TARGET: "-cc1"
// PER-TARGET-SAME: "--dependent-lib=clang_rt.builtins.lib"

// Legacy layout, with the architecture in the library name.
// RUN: %clang -### -c --target=x86_64-pc-windows-msvc \
// RUN:   -resource-dir=%S/Inputs/resource_dir -- %s 2>&1 \
// RUN:   | FileCheck %s --check-prefix=LEGACY
// RUN: %clang_cl -### -c --target=x86_64-pc-windows-msvc \
// RUN:   -resource-dir %S/Inputs/resource_dir -- %s 2>&1 \
// RUN:   | FileCheck %s --check-prefix=LEGACY
// LEGACY: "-cc1"
// LEGACY-SAME: "--dependent-lib=clang_rt.builtins-x86_64.lib"

// No directive when the library for the target is not installed, so that
// toolchains without compiler-rt don't fail every link.
// RUN: %clang -### -c --target=i686-pc-windows-msvc \
// RUN:   -resource-dir=%S/Inputs/resource_dir_with_per_target_subdir -- %s 2>&1 \
// RUN:   | FileCheck %s --check-prefix=NONE
// RUN: %clang -### -c --target=aarch64-pc-windows-msvc \
// RUN:   -resource-dir=%S/Inputs/resource_dir -- %s 2>&1 \
// RUN:   | FileCheck %s --check-prefix=NONE
// RUN: %clang_cl -### -c --target=x86_64-pc-windows-msvc \
// RUN:   -resource-dir %S/Inputs -- %s 2>&1 \
// RUN:   | FileCheck %s --check-prefix=NONE

// Not for other Windows environments.
// RUN: %clang -### -c --target=x86_64-w64-windows-gnu \
// RUN:   -resource-dir=%S/Inputs/resource_dir -- %s 2>&1 \
// RUN:   | FileCheck %s --check-prefix=NONE

// Opt-outs: -fno-rtlib-defaultlib, /Zl (-fms-omit-default-lib), -nostdlib and
// -nodefaultlibs.
// RUN: %clang -### -c --target=x86_64-pc-windows-msvc -fno-rtlib-defaultlib \
// RUN:   -resource-dir=%S/Inputs/resource_dir_with_per_target_subdir -- %s 2>&1 \
// RUN:   | FileCheck %s --check-prefix=NONE
// RUN: %clang_cl -### -c --target=x86_64-pc-windows-msvc -fno-rtlib-defaultlib \
// RUN:   -resource-dir %S/Inputs/resource_dir_with_per_target_subdir -- %s 2>&1 \
// RUN:   | FileCheck %s --check-prefix=NONE
// RUN: %clang_cl -### -c --target=x86_64-pc-windows-msvc /Zl \
// RUN:   -resource-dir %S/Inputs/resource_dir_with_per_target_subdir -- %s 2>&1 \
// RUN:   | FileCheck %s --check-prefix=NONE
// RUN: %clang -### -c --target=x86_64-pc-windows-msvc -fms-omit-default-lib \
// RUN:   -resource-dir=%S/Inputs/resource_dir_with_per_target_subdir -- %s 2>&1 \
// RUN:   | FileCheck %s --check-prefix=NONE
// RUN: %clang -### -c --target=x86_64-pc-windows-msvc -nostdlib \
// RUN:   -resource-dir=%S/Inputs/resource_dir_with_per_target_subdir -- %s 2>&1 \
// RUN:   | FileCheck %s --check-prefix=NONE
// RUN: %clang -### -c --target=x86_64-pc-windows-msvc -nodefaultlibs \
// RUN:   -resource-dir=%S/Inputs/resource_dir_with_per_target_subdir -- %s 2>&1 \
// RUN:   | FileCheck %s --check-prefix=NONE
// NONE: "-cc1"
// NONE-NOT: "--dependent-lib=clang_rt.builtins
