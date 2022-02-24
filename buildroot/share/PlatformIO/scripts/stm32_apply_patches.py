from genericpath import isdir
from hashlib import sha1
from os.path import join, isfile
import patch 

Import("env")

FRAMEWORK_DIR = env.PioPlatform().get_package_dir("framework-arduinoststm32")
patchflag_path = join(FRAMEWORK_DIR, ".patching-done")

# patch file only if we didn't do it before
if not isfile(join(FRAMEWORK_DIR, ".patching-done")):
    patch_file = join("buildroot", "share", "patches", "ststm32_SPI_virtual.patch")

    assert isfile(patch_file)

    patchset = patch.fromfile(patch_file)
    patchset.apply(root=FRAMEWORK_DIR)

    def _touch(path):
        with open(path, "w") as fp:
            fp.write("")

    env.Execute(lambda *args, **kwargs: _touch(patchflag_path))