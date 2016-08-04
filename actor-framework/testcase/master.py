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
def findmaster():
    list=[]
    with open("/home/net/xiaodong/actor/actor-framework/examples/heartbeat_log.txt","r") as file:
        for line in file:
            id=string.atoi(line)
            if id not in list:
                list.append(id)
            count = len(list)
            if count!=0:
                return True;
        return False;
            
def clearlog():
    with open("/home/net/xiaodong/actor/actor-framework/examples/heartbeat_log.txt","w") as file1:
        file1.truncate()
def clearprelog():
    with open("/home/net/xiaodong/actor/actor-framework/examples/after_log","wb") as file2:
        file2.truncate()
    with open("/home/net/xiaodong/actor/actor-framework/examples/before_log","wb") as file3:
        file3.truncate()
def clearcommand():
    with open("/home/net/xiaodong/actor/actor-framework/examples/command.txt","w") as file4:
        file4.truncate()
clearcommand()
clearprelog()                
print "start master and run it for 10 seconds\n"
os.system("nohup /home/net/xiaodong/actor/build/bin/master &")
time.sleep(10)
killmaster()
print "kill master, and restart it after 80 seconds\n"
time.sleep(80)
clearlog()
os.system("nohup /home/net/xiaodong/actor/build/bin/master &")
print "restart it right now,and check the state after 10 seconds\n"
time.sleep(10)
a=findmaster()
if a==True:
    print "\033[1;32;40m PASS!"
    print '\033[0m'
    print "\n"
else:
    print "\033[1;31;40m FAIL!"
    print '\033[0m'
    print "\n"
        
