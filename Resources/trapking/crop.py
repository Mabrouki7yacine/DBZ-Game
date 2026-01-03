import cv2
import os

# Set your directory path
folder_path = "/home/yacine/Learning/Game_Dev/Screw_it_lets_make_a_game/Game_V1/Resources/trapking/"
output_folder = os.path.join(folder_path, "cropped")
os.makedirs(output_folder, exist_ok=True)

# Loop over all .png files
for filename in os.listdir(folder_path):
    if filename.endswith(".png"):
        image_path = os.path.join(folder_path, filename)
        image = cv2.imread(image_path, cv2.IMREAD_UNCHANGED)

        if image is None:
            print(f"Failed to load {filename}")
            continue

        # Crop the image
        resized_image = cv2.resize(image, (90, 75), interpolation=cv2.INTER_AREA)

        # Save to output folder
        output_path = os.path.join(output_folder, filename)
        cv2.imwrite(output_path, resized_image)
        print(f"Cropped and saved: {filename}")

print("✅ All images cropped successfully.")
