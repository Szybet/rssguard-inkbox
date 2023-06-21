#!/bin/bash

servername="root@10.42.0.28"
passwd="root"

# Debug
#sshpass -p $passwd ssh $servername "bash -c \"killall rssguard\""
#sshpass -p $passwd ssh $servername "bash -c \"rm  /kobo/mnt/rss/rss/lib/librssguard.so\""
#sshpass -p $passwd ssh $servername "bash -c \"rm  /kobo/mnt/rss/rss/rssguard\""

#sshpass -p $passwd scp build/src/librssguard/librssguard.so $servername:/kobo/mnt/rss/rss/lib/
#sshpass -p $passwd scp build/src/rssguard/rssguard $servername:/kobo/mnt/rss/rss/
#exit 0

cp build/src/rssguard/rssguard inkbox_userapp/rssguard/app-bin/rssguard.bin
cp build/src/librssguard/librssguard.so inkbox_userapp/rssguard/app-lib/
# Very important
rm -f inkbox_userapp/rssguard.isa.dgst
rm -f inkbox_userapp/rssguard.isa

mksquashfs inkbox_userapp/rssguard/* inkbox_userapp/rssguard.isa

# Yes, here are my private keys. Is providing this info a security threat? no.
openssl dgst -sha256 -sign /home/szybet/inkbox-keys/userapps.pem -out inkbox_userapp/rssguard.isa.dgst inkbox_userapp/rssguard.isa

sshpass -p $passwd ssh $servername "bash -c \"ifsctl mnt rootfs rw\""
# sshpass -p $passwd ssh $servername "bash -c \"rm -r /data/onboard/.apps/rssguard\""
sshpass -p $passwd ssh $servername "bash -c \"mkdir /data/onboard/.apps/rssguard\""
sshpass -p $passwd ssh $servername "bash -c \"rm  /data/onboard/.apps/rssguard/rssguard.isa\""
sshpass -p $passwd ssh $servername "bash -c \"rm  /data/onboard/.apps/rssguard/rssguard.isa.dgst\""
sshpass -p $passwd ssh $servername "bash -c \"rm  /data/onboard/.apps/rssguard/app.json\""

sshpass -p $passwd scp inkbox_userapp/app.json $servername:/data/onboard/.apps/rssguard/
sshpass -p $passwd scp inkbox_userapp/rssguard.isa.dgst $servername:/data/onboard/.apps/rssguard/
sshpass -p $passwd scp inkbox_userapp/rssguard.isa $servername:/data/onboard/.apps/rssguard/

sshpass -p $passwd ssh $servername "bash -c \"touch /kobo/tmp/rescan_userapps\""

sshpass -p $passwd ssh $servername "bash -c \"sync\""

sshpass -p $passwd ssh $servername "bash -c \"killall -9 rssguard.sh\"" || EXIT_CODE=0

sshpass -p $passwd ssh $servername "bash -c \"rc-service inkbox_gui restart\"" # to get logs
# sshpass -p $passwd ssh $servername "bash -c \"rc-service gui_apps restart\""

# To update main json
# sshpass -p $passwd ssh $servername "bash -c \"touch /kobo/tmp/rescan_userapps\"" # This gets deleted by service restart
#sshpass -p $passwd ssh $servername "bash -c \"killall inkbox-bin\""
#sleep 10
