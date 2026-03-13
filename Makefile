start-frontend:
	@echo "Starting development server at http://localhost:8080"
	@echo "Open http://localhost:8080/frontend"
	python3 -m http.server 8080

start-backend:
	cd backend && docker compose up

download-releases:
	@echo "Downloading firmware binaries from GitHub releases..."
	@cd frontend && \
	jq -r '.releases[] | .version as $$version | .parts[] | "\($$version) \(.path | split("/")[-1])"' manifest.json | while read version filename; do \
		mkdir -p "releases/$${version}"; \
		echo "Downloading $${filename} for version $${version}..."; \
		curl -L -o "releases/$${version}/$${filename}" \
			"https://github.com/jveldboom/esp32-temperature-system/releases/download/$${version}/$${filename}" \
			|| echo "Warning: Failed to download $${filename} for $${version}"; \
	done
	@echo "✓ Download complete"