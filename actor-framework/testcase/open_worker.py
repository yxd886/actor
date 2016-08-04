import os
import sys
import string
import psutil
import re
import time
import signal
def killmaster():
    process_list = psutil.get_process_list()
    a = "master"
    regex = "pid=(\d+),\sname=\'" + a + "\'"
    pid = 0
    for line in process_list:
        process_info = str(line)
        ini_regex =re.compile(regex)
        result = ini_regex.search(process_info)
        if result !=None:
            pid = string.atoi(result.group(1))
            os.kill(pid, signal.SIGKILL)
def killworker():
    process_list = psutil.get_process_list()
    a = "client"
    regex = "pid=(\d+),\sname=\'" + a + "\'"
    pid = 0
    for line in process_list:
        process_info = str(line)
        ini_regex =re.compile(regex)
        result = ini_regex.search(process_info)
        if result !=None:
            pid = string.atoi(result.group(1))
            os.kill(pid, signal.SIGKILL)
def findworkernumber():
    list=[]
    with open("/home/net/xiaodong/actor/actor-framework/examples/heartbeat_log.txt","r") as file:
        count =0
        for line in file:
            id=string.atoi(line)
            if id not in list:
                list.append(id)
            count = len(list)
        return count
def clearlog():
    with open("/home/net/xiaodong/actor/actor-framework/examples/heartbeat_log.txt","w") as file:
        file.truncate()




num1 = findworkernumber()
with open("/home/net/xiaodong/actor/actor-framework/examples/command.txt","w") as file:
    file.write("open")
time.sleep(8)
with open("/home/net/xiaodong/actor/actor-framework/examples/command.txt","w") as file:
 time.sleep(10)
num2=findworkernumber()
if num1==(num2-1):
    print "\033[1;32;40m PASS!"
    print '\033[0m'
    print "\n"
else:
    print "\033[1;31;40m FAIL!"
    print '\033[0m'
    print "\n"

killworker()
killmaster()
