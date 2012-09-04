#
# Regular cron jobs for the grub2-remenu package
#
0 4	* * *	root	[ -x /usr/bin/grub2-remenu_maintenance ] && /usr/bin/grub2-remenu_maintenance
