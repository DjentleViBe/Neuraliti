#!/bin/bash
echo "Running script!"
pwd
# Set the destination folder
DESTINATION_FOLDER="bin"

# Copy the executable
cp "${BUILT_PRODUCTS_DIR}"/* "$DESTINATION_FOLDER"
