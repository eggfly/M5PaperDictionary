from ctypes import *
from PIL import Image

grayScale = Image.open("go.png").convert('L')
pixels = grayScale.tobytes()
# print(pixels)
print(len(pixels))
fp = open("out.c", 'wb')
count = 0
value = 0
for i in pixels:
    _4bit = 15 - (ord(i) / 16)
    if count % 2 == 0:
        value += 16 * _4bit
    else:
        value += _4bit
        fp.write(hex(value))
        fp.write(", ")
        value = 0
    count += 1
    if count % 32 == 0:
        fp.write("\n")
fp.close()
