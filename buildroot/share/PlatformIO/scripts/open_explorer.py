#
# open_explorer.py
#
def open_file_explorer():
    import subprocess
    from pathlib import Path
    from SCons.Script import Import
    from SCons.Script import DefaultEnvironment
    env = DefaultEnvironment()
    Import("env")
    BUILD_PATH = Path(env['PROJECT_BUILD_DIR'], env['PIOENV'])
    script = f'Explorer.exe "{BUILD_PATH}"'
    try:
        subprocess.run(["bash", "-c", script])
    except Exception as e:
        print(f"Could not open File Explorer, an error occurred: {e}")
