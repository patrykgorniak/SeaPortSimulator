from PIL import Image
import sys

def writeMap():
    pass

def main(path):
    file = open("map.bin","bw+")
    im = Image.open(path)
    for y in range(0, im.size[1]):
        for x in range(0, im.size[0]):
            xy= (x, y)
            rgb = im.getpixel(xy)
            if rgb < 255:
                file.write(bytes("{} {};".format(x,y).encode('utf-8')))
                print(x,y,rgb)
    file.close()


if __name__=="__main__":
    main(sys.argv[1])
