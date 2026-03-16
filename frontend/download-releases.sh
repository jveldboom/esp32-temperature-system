#!/bin/bash
set -euo pipefail

# Download firmware binaries from GitHub releases based on manifest.json

REPO="jveldboom/esp32-temperature-system"

cd "$(dirname "$0")"

echo "Downloading firmware binaries from GitHub releases..."

# Extract version/file pairs from manifest (deduplicated)
FILES=$(jq -r '.releases[] | .version as $v | .parts[] | "\($v) \(.path | split("/")[-1])"' manifest.json | sort -u)

# Check if we have any files to download
if [ -z "$FILES" ]; then
  echo "Error: No files found in manifest.json" >&2
  exit 1
fi

# Download each file
echo "$FILES" | while read -r version filename; do
  mkdir -p "releases/${version}"
  echo "  ${filename} (${version})..."
  curl --fail --silent --show-error --location \
    -o "releases/${version}/${filename}" \
    "https://github.com/${REPO}/releases/download/${version}/${filename}"
done

echo "✓ Download complete"
