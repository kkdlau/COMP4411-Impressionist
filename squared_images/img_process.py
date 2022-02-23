from PIL import Image
from os import listdir
from os.path import isfile, join

mypath = "/Users/dannylau/Program/COMP4411-Impressionist/squared_images"
onlyfiles = [f for f in listdir(mypath) if isfile(join(mypath, f))]

count = 0
for p in onlyfiles:
    if "bmp" in p:
        image = Image.open(p)
        image = image.resize(size=(5, 5))
        image.save('resized_{i}.bmp'.format(i=count))
        count = count + 1