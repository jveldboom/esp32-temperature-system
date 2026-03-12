.PHONY: serve build-firmware

serve:
	@echo "Starting development server at http://localhost:8080"
	@echo "Open http://localhost:8080/frontend"
	python3 -m http.server 8080

build-firmware:
	cd firmware && pio run
