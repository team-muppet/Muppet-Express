#!/bin/bash

# Directory to save images
DOWNLOADS_DIR=~/Downloads/pokemon_images

# Range of Pokémon IDs to fetch
START_ID=1
END_ID=20

# Base URL for Pokémon images
BASE_URL="https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/pokemon/other/official-artwork"

# Create the directory if it doesn't exist
mkdir -p "$DOWNLOADS_DIR"

echo "Fetching Pokémon images..."

# Loop through the range of Pokémon IDs
for i in $(seq $START_ID $END_ID); do
  # Construct the image URL
  IMAGE_URL="$BASE_URL/$i.png"
  
  # Construct the output file path
  OUTPUT_FILE="$DOWNLOADS_DIR/$i.png"
  
  # Download the image
  curl -s -o "$OUTPUT_FILE" "$IMAGE_URL"
  
  # Check if the download was successful
  if [[ $? -eq 0 ]]; then
    echo "Downloaded Pokémon ID $i"
  else
    echo "Failed to download Pokémon ID $i"
  fi
done

echo "Images saved in $DOWNLOADS_DIR"
