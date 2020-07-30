#Please install selenium
#get FPS for webGL application running on chrome(or chromium) browser

	ex: pip3 install selenium

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
