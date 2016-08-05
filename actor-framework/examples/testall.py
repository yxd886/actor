import os
import sys
import string
import psutil
import re
import time
import signal


print "\033[1;31;40m"
print "testing master.py\t\t"
print '\033[0m'
os.system("python /home/net/xiaodong/actor/actor-framework/testcase/master.py")
print "\033[1;31;40m"
print "testing worker.py\t\t"
print '\033[0m'
os.system("python /home/net/xiaodong/actor/actor-framework/testcase/worker.py")
print "\033[1;31;40m"
print "testing close_worker.py\t\t"
print '\033[0m'
os.system("python /home/net/xiaodong/actor/actor-framework/testcase/close_worker.py")
print "\033[1;31;40m"
print "testing open_worker.py\t\t"
print '\033[0m'
os.system("python /home/net/xiaodong/actor/actor-framework/testcase/open_worker.py")
