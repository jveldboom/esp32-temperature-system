frontend-start:
	@echo "Starting development server at http://localhost:8080"
	@echo "Open http://localhost:8080/frontend"
	python3 -m http.server 8080

backend-start:
	cd backend && docker compose up

backend-delete:
	cd backend && docker compose down -v --remove-orphans

download-releases:
	@./frontend/download-releases.sh
