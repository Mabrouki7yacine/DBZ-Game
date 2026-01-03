import cv2

# Load the original image
input_path = input("Enter your image name : ") # Change to your image path

image = cv2.imread(input_path)

# Check if image is loaded
if image is None:
    print("Error: Could not load image.")
    exit()

# Resize the image to 640x480
resized_image = cv2.resize(image, (640, 480), interpolation=cv2.INTER_AREA)

# Save the resized image
output_path =  input("Enter your image output name : ")
cv2.imwrite(output_path, resized_image)

print(f"Image resized and saved as {output_path}")
