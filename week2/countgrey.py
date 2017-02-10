import sys
import os
import numpy as np
import cv2

def split_into_rgb_channels(image):
  '''Split the target image into its red, green and blue channels.
  image - a numpy array of shape (rows, columns, 3).
  output - three numpy arrays of shape (rows, columns) and dtype same as
  image, containing the corresponding channels. 
  '''
  red = image[:,:,2]
  green = image[:,:,1]
  blue = image[:,:,0]
  return red, green, blue

def grey_color(red, green, blue):
  count_grey = 0
  for i in range(len(red)):
    for j in range(0, len(red[i])):
      if(red[i][j] > 128 and green[i][j] > 128 and blue[i][j] > 128):
        count_grey += 1
  return count_grey

def main():
  ''' This function searches for a folder images/knowpapa subfolder, and splits
  all images found in that folder into its equivalent rgb channel. 
  It saves each image appending the terms 'red', 'green' and
  'blue' to the orginal filename.
  '''
  count_grey = 0
  imagesdir = os.path.abspath(os.path.join(os.curdir, 'images'))
  print "Searching for images in {} Directory)".format(imagesdir)
  exts = ['.bmp', '.pbm', '.pgm', '.ppm', '.sr', '.ras', '.jpeg', '.jpg', 
          '.jpe', '.jp2', '.tiff', '.tif', '.png']
  for dirname, dirnames, filenames in os.walk(imagesdir):
    for filename in filenames:
      name, ext = os.path.splitext(filename)
      img = cv2.imread(os.path.join(dirname, filename))
      red, green, blue = split_into_rgb_channels(img)
      print "File name: " + filename
      this_img_grey = grey_color(red, green, blue)
      print "grey color: " + str(this_img_grey) + "\n"
      count_grey += this_img_grey

  print "Total grey color for all images: " + str(count_grey)

if __name__ == "__main__":
  main()
