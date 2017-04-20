# coding=utf-8

from subprocess import Popen,PIPE

text = open(r"C:\Users\Administrator.SKY-20151226PGL\Desktop\123.htm").read()
tidy = Popen('tidy',stdin=PIPE,stdout=PIPE,stderr=PIPE)

tidy.stdin.write(text)
tidy.stdin.close()

print tidy.stdout.read()