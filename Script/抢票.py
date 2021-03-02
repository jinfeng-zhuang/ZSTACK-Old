from selenium import webdriver
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.common.by import By

class TicketGrab(object):
    def __init__(self):
        self.login_url = 'https://kyfw.12306.cn/otn/login/init'
        self.driver = webdriver.Chrome()
    
    def wait_input(self):
        self.from_station = input('出发地：')
    
    def _login(self):
        self.driver.get(self.login_url)
        WebDriverWait(self.driver, 1000).util(EC.url_to_be(self.initmy_url))
        print('登录成功')
    
    def _order_ticket(self):
        