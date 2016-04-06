#coding=gbk
import threading
import queue
import urllib.request, urllib.parse, urllib.error
import sys
from bs4 import BeautifulSoup

task_queue=queue.Queue()

queryUrl = 'http://qq.ip138.com/train/jiangsu/NanJing.htm'
filePath = "train.txt"
numOfThreads = 8
if(len(sys.argv) == 4):
    queryUrl = 'http://qq.ip138.com/train/' + sys.argv[1] + '/' + sys.argv[2] + '.htm'
    filePath = sys.argv[3]

if(len(sys.argv) == 5):
    queryUrl = 'http://qq.ip138.com/train/' + sys.argv[1] + '/' + sys.argv[2] + '.htm'
    filePath = sys.argv[3]
    numOfThreads = int(sys.argv[4])


page = urllib.request.urlopen(queryUrl).read().decode('gbk')
soup = BeautifulSoup(page)

f = open(filePath ,"w" , encoding='utf-8')
f.write("")
f.close()

for link in soup.find_all('a'):
    postUrl = link.get('href')
    if(postUrl and postUrl.find("htm") > 0 and postUrl.find("daishoudian") == -1 and postUrl.find("http") == -1):
        trainUrl = "http://qq.ip138.com" + postUrl
        task_queue.put(trainUrl)


def worker():
    while not task_queue.empty():
        trainUrl = task_queue.get()
        print('{} >>> {}...'.format(threading.current_thread().name, trainUrl))
        sys.stdout.flush()
        try:
            trainPage = urllib.request.urlopen(trainUrl).read().decode('gbk')
        except urllib.error.URLError:
            print('{} reports: error when open {}'.format(threading.current_thread().name, trainUrl))
            continue
        except UnicodeEncodeError:
            print('{} reports: encoding error'.format(threading.current_thread().name))
            continue
        trainSoup = BeautifulSoup(trainPage)
        if lock.acquire(1):
            f = open(filePath ,"a" , encoding='utf-8')
            f.write(trainUrl + " ")
            for train in trainSoup.find_all('td'):
                if(train.text):
                    f.write(train.text.strip() + "  ")
            f.close()
            lock.release()
    if(task_queue.empty()):
        print('{} reports: finished'.format(threading.current_thread().name))
        sys.stdout.flush()

lock = threading.Lock()

for i in range(numOfThreads):
    t = threading.Thread(target = worker, name = '***线程 ' + str(i + 1))
    t.start()
