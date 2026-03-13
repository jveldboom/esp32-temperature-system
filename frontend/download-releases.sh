#!/bin/bash
set -euo pipefail

# Download firmware binaries from GitHub releases based on manifest.json
# This script is used by both local development (Makefile) and GitHub Actions

# GitHub repository configuration
REPO_OWNER="jveldboom"
REPO_NAME="esp32-temperature-system"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

if [ ! -f "manifest.json" ]; then
  echo "Error: manifest.json not found" >&2
  exit 1
fi

echo "Downloading firmware binaries from GitHub releases..."
echo "Repository: ${REPO_OWNER}/${REPO_NAME}"

FAILED=0

# Extract unique version/file combinations from manifest and download each binary
while IFS=' ' read -r version filename; do
  mkdir -p "releases/${version}"

  echo "Downloading ${filename} for version ${version}..."

  if curl --fail --show-error --silent --location \
    -o "releases/${version}/${filename}" \
    "https://github.com/${REPO_OWNER}/${REPO_NAME}/releases/download/${version}/${filename}"; then
    echo "  ✓ ${filename}"
  else
    echo "  ✗ Failed to download ${filename} for ${version}" >&2
    FAILED=1
  fi
done < <(jq -r '.releases[] | .version as $version | .parts[] | "\($version) \(.path | split("/")[-1])"' manifest.json | sort -u)

if [ $FAILED -eq 1 ]; then
  echo ""
  echo "Error: Some downloads failed" >&2
  exit 1
fi

echo ""
echo "✓ All binaries downloaded successfully"
