import subprocess
import os
import sys


def find_file_root(filename, folder_path):
    for root, _, files in os.walk(folder_path):
        if filename in files:
            # If the target file is found, print its full path
            bat_file_path = root
            return bat_file_path
    return None


def setup():
    if sys.platform.startswith("win"):
        root = find_file_root("Setup-Windows.bat", os.getcwd())
        path = os.path.join(root, "Setup-Windows.bat")
        if path is not None and os.path.exists(root) and os.path.isfile(path):
            try:
                subprocess.run(path, shell=True, cwd=root)
            except Exception as e:
                print(f"Error running the command: {e}")
        else:
            raise Exception(f"Setup bat file for {sys.platform} not found.")
    else:
        raise Exception(f"Not support platform {sys.platform}")


if __name__ == "__main__":
    setup()
