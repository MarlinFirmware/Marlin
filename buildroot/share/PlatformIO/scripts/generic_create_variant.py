#
# generic_create_variant.py
#
# Copy one of the variants from buildroot/platformio/variants into
# the appropriate framework variants folder, so that its contents
# will be picked up by PlatformIO just like any other variant.
#
import pioutil, re
marlin_variant_pattern = re.compile("marlin_.*")
if pioutil.is_pio_build():
    import shutil,marlin
    from pathlib import Path

    #
    # Get the platform name from the 'platform_packages' option,
    # or look it up by the platform.class.name.
    #
    env = marlin.env
    platform = env.PioPlatform()

    from platformio.package.meta import PackageSpec
    platform_packages = env.GetProjectOption('platform_packages')

    # Remove all tool items from platform_packages
    platform_packages = [x for x in platform_packages if not x.startswith("platformio/tool-")]

    if len(platform_packages) == 0:
        framewords = {
            "Ststm32Platform": "framework-arduinoststm32",
            "AtmelavrPlatform": "framework-arduino-avr"
        }
        platform_name = framewords[platform.__class__.__name__]
    else:
        spec = PackageSpec(platform_packages[0])
        if spec.uri and '@' in spec.uri:
            platform_name = re.sub(r'@.+', '', spec.uri)
        else:
            platform_name = spec.name

    FRAMEWORK_DIR = Path(platform.get_package_dir(platform_name))
    assert FRAMEWORK_DIR.is_dir()

    board = env.BoardConfig()

    #mcu_type = board.get("build.mcu")[:-2]
    variant = board.get("build.variant")
    #series = mcu_type[:7].upper() + "xx"

    # Only prepare a new variant if the PlatformIO configuration provides it (board_build.variant).
    # This check is important to avoid deleting official board config variants.
    if marlin_variant_pattern.match(str(variant).lower()):
        # Prepare a new empty folder at the destination
        variant_dir = FRAMEWORK_DIR / "variants" / variant
        if variant_dir.is_dir():
            shutil.rmtree(variant_dir)
        if not variant_dir.is_dir():
            variant_dir.mkdir()

        # Source dir is a local variant sub-folder
        source_dir = Path("buildroot/share/PlatformIO/variants", variant)
        assert source_dir.is_dir()

        print("Copying variant " + str(variant) + " to framework directory...")

        marlin.copytree(source_dir, variant_dir)