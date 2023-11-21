#!/bin/bash

if [ "$#" -eq 0 ]; then
  echo "usage: $0 <diskFileName>"
  exit 1
fi

echo "Creating a 128KB file in $1"
echo "This file will act as a dummy disk and will hold your filesystem"

# Open the disk file for writing
exec 3<> "$1"
echo "Formatting your filesystem..."

# Write super block
printf "\x01" > /dev/fd/3

# Write out the remaining 127 data blocks, all zeroed out
for ((i=0; i<127; i++)); do
  printf "\x00" > /dev/fd/3
done

exec 3>&-
echo "File system creation complete."

exit 0
