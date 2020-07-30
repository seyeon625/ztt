from selenium import webdriver
from selenium.webdriver.common.by import By

'''
	Example:
	
	fps_driver = FPSDriver("./chromedriver")
	aquarium_url = "https://webglsamples.org/aquarium/aquarium.html"
	fps_driver.open_page(aquarium_url)
	while True:
		fps = fps_driver.get_fps()
		print("FPS: " + fps)
		time.sleep(1)


'''

class FPSDriver:

	def __init__(self, driver_path):
		self.driver_path = driver_path
		self.driver = webdriver.Chrome(self.driver_path)
	
	def open_page(self, url):
		self.driver.get(url)
	
	def get_fps(self):
		fps_box = self.driver.find_element(By.ID, "fps")
		return fps_box.text


