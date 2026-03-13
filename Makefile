start-frontend:
	@echo "Starting development server at http://localhost:8080"
	@echo "Open http://localhost:8080/frontend"
	python3 -m http.server 8080

start-backend:
	cd backend && docker compose up

download-releases:
	@./frontend/download-releases.sh