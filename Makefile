all:
	+$(MAKE) -C net
	+$(MAKE) -C nmap
	+$(MAKE) -C smbpasswd

# vim:noexpandtab
