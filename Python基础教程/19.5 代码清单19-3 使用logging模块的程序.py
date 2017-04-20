# coding=utf-8
import logging

logging.basicConfig(level=logging.INFO, filename=r"杂物\mylog.log".decode("utf-8"))
logging.info("Trying to divide 1 by 0")
print 1/0
logging.info("The division succeeded")