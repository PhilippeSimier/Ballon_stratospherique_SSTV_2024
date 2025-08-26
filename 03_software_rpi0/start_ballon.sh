#!/bin/bash
sleep 30
/home/ballon/stop/stop >> /home/ballon/log/stop.log 2>&1 &
sleep 1
/home/ballon/lora_files/lora_files >> /home/ballon/log/lora.log 2>&1 &
/home/ballon/weather/weather >> /home/ballon/log/weather.log 2>&1 &
/home/ballon/reception/reception >> /home/ballon/log/reception.log 2>&1 &
#/home/ballon/sstv/sstv >> /home/ballon/log/sstv.log 2>&1 &
