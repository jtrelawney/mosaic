#/bin/bash

this_dir="$1"
echo "send dir : $this_dir"

rsync -rvh --exclude 'build*'  $this_dir pi@192.168.0.100:/home/pi/Projects/network/$this_dir
