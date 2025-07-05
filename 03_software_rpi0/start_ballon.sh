#!/bin/bash
sleep 30
/home/ballon/stop/stop >> /home/ballon/log/stop.log 2>&1 &
sleep 1
/home/ballon/lora_files/lora_files >> /home/ballon/log/lora.log 2>&1 &
/home/ballon/telemetrie/telemetrie >> /home/ballon/log/telemetrie.log 2>&1 &
/home/ballon/sstv/sstv >> /home/ballon/log/sstv.log 2>&1 &
