# Function to list files with a given extension in a directory
list_files_with_extension() {
    local directory=$1
    local extension=$2

    # Ensure the extension starts with a dot
    if [[ $extension != .* ]]; then
        extension=".$extension"
    fi

    # Loop through all files in the directory
    for file in "$directory"/*"$extension"; do
        # Check if the file exists (handles case with no matching files)
        if [[ -e $file ]]; then
            filename=$(basename "$file" "$extension")
            echo "$filename"
            /usr/bin/clang++ -c -fPIC ./sharedlib/$filename.cpp -o ./sharedlib/$filename.o
        fi
    done
}

directory="./sharedlib/"
extension="cpp"  # or any other extension, e.g., "sh", "jpg", etc.
shared_lib_name="shrsub.so"
shared_lib_path="$directory/$shared_lib_name"

list_files_with_extension "$directory" "$extension"

/usr/bin/clang++ -shared -o "$shared_lib_path" "$directory"/*.o