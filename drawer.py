from linecache import getline
import cv2 as cv
import numpy as np
import sys
f = sys.argv[1]
file = open(f)
line = file.readline()
width = int(line.split()[0])
height = int(line.split()[1])
shape = (width, height, 3)
die1 = np.zeros(shape, np.uint8)
die1.fill(255)
die2 = np.zeros(shape, np.uint8)
die2.fill(255)
term = np.zeros(shape, np.uint8)
term.fill(255)

line = file.readline()
cv.namedWindow("die1", cv.WINDOW_NORMAL)
cv.namedWindow("die2", cv.WINDOW_NORMAL)
cv.namedWindow("term", cv.WINDOW_NORMAL)
for i in range(int(line.split()[0])):
    line = file.readline()
    topinst = line.split()
    cv.rectangle(die1, (int(topinst[0]), int(topinst[1])),
                 (int(topinst[0])+int(topinst[2]), int(topinst[1])+int(topinst[3])), (250, 200, 150), -1)
    cv.rectangle(die1, (int(topinst[0]), int(topinst[1])),
                 (int(topinst[0])+int(topinst[2]), int(topinst[1])+int(topinst[3])), (150, 120, 90), 2)
WW = int((width*1000)/(height+width))
WH = int((height*1000)/(height+width))
cv.resizeWindow('die1', WW, WH)
cv.imwrite('die1.jpg', die1, [cv.IMWRITE_JPEG_QUALITY, 90])
line = file.readline()
for i in range(int(line.split()[0])):
    line = file.readline()
    topinst = line.split()
    cv.rectangle(die2, (int(topinst[0]), int(topinst[1])),
                 (int(topinst[0])+int(topinst[2]), int(topinst[1])+int(topinst[3])), (255, 200, 150), -1)
    cv.rectangle(die2, (int(topinst[0]), int(topinst[1])),
                 (int(topinst[0])+int(topinst[2]), int(topinst[1])+int(topinst[3])), (150, 120, 90), 2)
cv.resizeWindow('die2', WW, WH)
cv.imwrite('die2.jpg', die2, [cv.IMWRITE_JPEG_QUALITY, 90])
line = file.readline()
TW = int(line.split()[0])
TH = int(line.split()[1])
line = file.readline()
print(line)
for i in range(int(line.split()[0])):
    line = file.readline()
    terminal = line.split()
    posx = int(terminal[0])
    posy = int(terminal[1])
    cv.rectangle(term, (int(posx-(TW/2)), int(posy-(TH/2))),
                 (int(posx+(TW/2)), int(posy+(TH/2))), (90, 120, 150), -1)

cv.resizeWindow('term', WW, WH)
cv.imwrite('term.jpg', term, [cv.IMWRITE_JPEG_QUALITY, 90])
cv.waitKey(0)
cv.destroyAllWindows()
cv.waitKey(1)
