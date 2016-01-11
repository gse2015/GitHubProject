# -*- coding: utf-8 -*-
from PIL import Image
from PIL import ImageFont
from PIL import ImageDraw


def compareImg(image_1, image_2):
    if image_1.size != image_2.size:
        raise 'image size not same!'
    w, h = image_1.size
    points = list()
    for y in range(h):
        for x in range(w):
            c1 = image_1.getpixel((x, y))
            c2 = image_2.getpixel((x, y))
            if c1 != c2:
                points.append((x, y))
    if len(points):
        r, b = points[0]
        l, t = points[0]
    for pt in points:
        r = max(r, pt[0])
        b = max(b, pt[1])
        l = min(l, pt[0])
        t = min(t, pt[1])

    return (l, t, r, b)

img = Image.open(r'C:\test.bmp')
img2 = Image.open(r'C:\test.bmp')

imgD = ImageDraw.Draw(img2)

imgD.point((100, 20), (255, 0, 0))
imgD.point((50, 200), (255, 0, 0))
imgD.point((328, 209), (255, 0, 0))
imgD.point((65, 86), (255, 0, 0))

img2.show()

result = compareImg(img, img2)

#abc = img.getdata()
#print abc
#img.show()
l = result[0] - 1
t = result[1] - 1
r = result[2] + 1
b = result[3] + 1

#imgD.line(((0, 10), (100, 50)), fill = (255, 0, 0, 100))

imgD.line(((l, t), (r, t), (r, b), (l, b), (l, t)), fill = (0, 0, 255))
#imgD.rectangle((r[0], r[1], r[2], r[3]))

#imgD.text((10, 400), u'abc')
#w, h = img.size
#a = img.getpixel((0,100))
#print img
#for x in range(w):
#    for y in range(10, 100, 2):
#        img.putpixel((x, 100+y), (0, 0, 100))
img2.show()
#img.save(r'C:\Test_save.bmp', 'bmp')

img2.save(r'C:\Test_save.jpg', 'jpeg')

print('Hello World')