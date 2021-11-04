#
# robin.py
#

# Apply customizations for a MKS Robin
def prepare(address, ldname, fwname):
	import pioutil
	if not pioutil.is_vscode_init():
		import marlin
		def encrypt(source, target, env):
			marlin.encrypt_mks(source, target, env, fwname)
		marlin.relocate_firmware(address)
		marlin.custom_ld_script(ldname)
		marlin.add_post_action(encrypt);
