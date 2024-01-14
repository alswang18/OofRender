import platform
import subprocess
import os
import argparse

def is_command_available(command):
    try:
        subprocess.check_call([command, "--version"], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        return True
    except subprocess.CalledProcessError:
        return False

def build(build_type="Release"):
    if platform.system() == "Windows":
        required_commands = ["msbuild"]
        for command in required_commands:
            if not is_command_available(command):
                raise Exception(f"{command} is not available.")
        if platform.system() == "Windows":
            if not is_file_exists("OofRender.sln"):
                raise Exception("OofRender.sln not found.")
        if build_type not in ["Debug", "Release", "Dist"]:
            raise Exception(f"Invalid build type {build_type}. Should be Debug, Release, or Dist")
        
        subprocess.run(["msbuild", "OofRender.sln", f"/p:Configuration={build_type}"], shell=True)
    else:
        raise Exception(f"Not support platform {platform.system()}")
    
def is_file_exists(file_path):
    return os.path.exists(file_path) and os.path.isfile(file_path)
    
def main():
    parser = argparse.ArgumentParser(description="Build OofRender")
    parser.add_argument("--command", type=str, help="Command to run: build", default="build")
    parser.add_argument("--build-type", type=str, help="Build type: Debug, Release, Dist", default="Release")

    args = parser.parse_args()
    if args.command == "build":
        build(args.build_type)

    else:
        raise Exception(f"Invalid command {args.command}")

if __name__ == "__main__":
    main()