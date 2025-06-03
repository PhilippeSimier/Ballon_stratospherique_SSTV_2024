#!/bin/bash
sleep 5
/home/ballon/stop/stop &
sleep 1
/home/ballon/sstv/sstv &
sleep 1
/home/ballon/Telemetrie/Telemetrie &
sleep 1
/home/ballon/Reception/reception &
sleep 1
/home/ballon/lora_files/lora_files >> /home/ballon/log.txt 2>&1 &
sleep 1
/home/ballon/lora_files/lora_files >> /home/ballon/log.txt 2>&1 &
