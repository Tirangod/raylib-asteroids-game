import os
import hashlib

# Set the compiler and linker flags
compiler_flags = "-Wall"
linker_flags = ""

# Set the source and header file paths
src_dir = "src"
include_dir = "src"

# Set the output directory and executable name
build_dir = "build"
exe_name = "myapp"

# Get a list of source files
src_files = [os.path.join(src_dir, f) for f in os.listdir(src_dir) if f.endswith(".c")]

# Create the build directory if it doesn't exist
if not os.path.exists(build_dir):
    os.makedirs(build_dir)

# Check if the build file exists
build_file_path = os.path.join(build_dir, "build.hash")
if os.path.exists(build_file_path):
    # Read the hash of the last build
    with open(build_file_path, "rb") as f:
        last_build_hash = f.read()
else:
    # Set the hash of the last build to None
    last_build_hash = None

# Compute the hash of the current source files
current_hash = hashlib.sha256()
for src_file in src_files:
    print(src_file)
    with open(src_file, "rb") as f:
        current_hash.update(f.read())

# If the hashes match, we don't need to rebuild
if last_build_hash == current_hash.digest():
    print("No changes detected. Build skipped.")
else:
    # Build the object files
    obj_files = []
    for src_file in src_files:
        obj_file = os.path.join(build_dir, os.path.basename(src_file[:-2] + ".o"))
        obj_files.append(obj_file)
        str = f"gcc -c {compiler_flags} -I {include_dir} {src_file} -o {obj_file}"
        print(str)
        os.system(str)

    # Link the object files into an executable
    s = f"gcc {linker_flags} {' '.join(obj_files)} -o {os.path.join(build_dir, exe_name)}"
    print(s)
    os.system(s)

    # Write the hash of the current build to the build file
    with open(build_file_path, "wb") as f:
        f.write(current_hash.digest())

    print("Build completed successfully!")